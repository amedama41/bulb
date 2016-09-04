#ifndef CANARD_NET_OFP_V13_OXM_MATCH_SET_HPP
#define CANARD_NET_OFP_V13_OXM_MATCH_SET_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/iterator.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/is_related.hpp>
#include <canard/network/openflow/detail/padding.hpp>
#include <canard/network/openflow/v13/any_oxm_match_field.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    class oxm_match_set
        : private boost::equality_comparable<oxm_match_set>
    {
        using container_type = std::map<std::uint32_t, any_oxm_match_field>;

    public:
        static constexpr protocol::ofp_match_type match_type
            = protocol::OFPMT_OXM;
        static constexpr std::size_t base_length
            = offsetof(v13_detail::ofp_match, pad);

        using key_type = std::uint32_t;
        using value_type = any_oxm_match_field;
        using reference = value_type const&;
        using const_reference = value_type const&;
        using iterator = boost::range_iterator<
            boost::select_second_const_range<container_type>
        >::type;
        using const_iterator = iterator;
        using difference_type = container_type::difference_type;
        using size_type = container_type::size_type;

        template <
              class... OXMMatchFields
            , typename std::enable_if<
                    !detail::is_related<oxm_match_set, OXMMatchFields...>::value
              >::type* = nullptr
        >
        oxm_match_set(OXMMatchFields&&... oxm_fields)
            : oxm_tlvs_{to_pair(std::forward<OXMMatchFields>(oxm_fields))...}
        {
        }

        static constexpr auto type()
            -> protocol::ofp_match_type
        {
            return match_type;
        }

        CANARD_NET_OFP_DECL auto length() const noexcept
            -> std::uint16_t;

        CANARD_NET_OFP_DECL auto begin() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto end() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto cbegin() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto cend() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto empty() const noexcept
            -> bool;

        CANARD_NET_OFP_DECL auto size() const noexcept
            -> size_type;

        CANARD_NET_OFP_DECL auto max_size() const noexcept
            -> size_type;

        template <class OXMMatchField>
        auto get() const
            -> OXMMatchField const&
        {
            auto const it = oxm_tlvs_.find(OXMMatchField::oxm_type());
            return v13::any_cast<OXMMatchField>(it->second);
        }

        CANARD_NET_OFP_DECL auto at(key_type const oxm_type) const
            -> const_reference;

        template <class OXMMatchField>
        auto insert(OXMMatchField const& field)
            -> std::pair<const_iterator, bool>
        {
            auto const result = oxm_tlvs_.emplace(field.oxm_type(), field);
            return make_result(result.first, result.second);
        }

        template <class OXMMatchField>
        auto assign(OXMMatchField const& field)
            -> std::pair<const_iterator, bool>
        {
            auto const it = oxm_tlvs_.find(field.oxm_type());
            if (it == oxm_tlvs_.end()) {
                return make_result(oxm_tlvs_.end(), false);
            }
            it->second = field;
            return make_result(it, true);
        }

        template <class OXMMatchField>
        auto insert_or_assign(OXMMatchField const& field)
            -> std::pair<const_iterator, bool>
        {
            auto const oxm_type = field.oxm_type();
            auto const it = oxm_tlvs_.lower_bound(oxm_type);
            if (it != oxm_tlvs_.end()
                    && !oxm_tlvs_.key_comp()(oxm_type, it->first)) {
                it->second = field;
                return make_result(it, false);
            }
            else {
                auto const result = oxm_tlvs_.emplace_hint(it, oxm_type, field);
                return make_result(result, true);
            }
        }

        template <class OXMMatchField>
        auto erase()
            -> size_type
        {
            return oxm_tlvs_.erase(OXMMatchField::oxm_type());
        }

        CANARD_NET_OFP_DECL auto erase(const_iterator)
            -> const_iterator;

        CANARD_NET_OFP_DECL void swap(oxm_match_set&);

        CANARD_NET_OFP_DECL void clear() noexcept;

        CANARD_NET_OFP_DECL auto find(key_type const) const
            -> const_iterator;

        template <class OXMMatchField>
        auto find() const
            -> boost::optional<OXMMatchField const&>
        {
            auto const it = oxm_tlvs_.find(OXMMatchField::oxm_type());
            if (it == oxm_tlvs_.end()) {
                return boost::none;
            }
            return v13::any_cast<OXMMatchField>(it->second);
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            detail::encode(container, std::uint16_t{match_type});
            detail::encode(container, length());
            boost::for_each(*this, [&](any_oxm_match_field const& field) {
                field.encode(container);
            });
            return detail::encode_byte_array(
                      container
                    , detail::padding
                    , v13_detail::padding_length(length()));
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> oxm_match_set
        {
            std::advance(first, sizeof(std::uint16_t));
            auto const length = detail::decode<std::uint16_t>(first, last);
            last = std::next(first, length - base_length);

            auto oxm_tlvs = container_type{};
            using oxm_header = std::uint32_t;
            while (std::distance(first, last) >= sizeof(oxm_header)) {
                oxm_match_field_decoder::decode<void>(
                        first, last, oxm_tlv_inserter{oxm_tlvs});
            }
            if (first != last) {
                throw std::runtime_error{"invalid oxm_match length"};
            }

            std::advance(first, v13_detail::padding_length(length));

            return oxm_match_set{std::move(oxm_tlvs)};
        }

        CANARD_NET_OFP_DECL static void validate_ofp_match(
                v13_detail::ofp_match const&);

        friend auto operator==(
                oxm_match_set const& lhs, oxm_match_set const& rhs)
            -> bool
        {
            return lhs.equal_impl(rhs);
        }

        friend auto equivalent(
                oxm_match_set const& lhs, oxm_match_set const& rhs) noexcept
            -> bool
        {
            return lhs.equivalent_impl(rhs);
        }

    private:
        CANARD_NET_OFP_DECL oxm_match_set(container_type&&);

        CANARD_NET_OFP_DECL auto equal_impl(oxm_match_set const&) const
            -> bool;

        CANARD_NET_OFP_DECL auto equivalent_impl(
                oxm_match_set const&) const noexcept
            -> bool;

        template <class OXMMatchField>
        static auto to_pair(OXMMatchField&& oxm_field)
            -> container_type::value_type
        {
            auto const oxm_type = oxm_field.oxm_type();
            return container_type::value_type{
                oxm_type, std::forward<OXMMatchField>(oxm_field)
            };
        }

        CANARD_NET_OFP_DECL static auto make_result(
                container_type::iterator it, bool const result)
            -> std::pair<const_iterator, bool>;

        struct oxm_tlv_inserter
        {
            template <class OXMMatchField>
            void operator()(OXMMatchField&& field) const
            {
                if (!oxm_tlvs.emplace(field.oxm_type(), field).second) {
                    throw std::runtime_error{"duplicated oxm field type"};
                }
            }

            container_type& oxm_tlvs;
        };

    private:
        container_type oxm_tlvs_;
    };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)
# include <canard/network/openflow/v13/impl/oxm_match_set.ipp>
#endif

#endif // CANARD_NET_OFP_V13_OXM_MATCH_SET_HPP
