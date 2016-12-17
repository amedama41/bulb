#ifndef CANARD_NET_OFP_V10_MATCH_SET_HPP
#define CANARD_NET_OFP_V10_MATCH_SET_HPP

#include <tuple>
#include <type_traits>
#include <utility>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/query/all.hpp>
#include <boost/optional/optional.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/is_related.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v10/common/match_fields.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    class match_set
        : public detail::basic_protocol_type<match_set>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_match;

        template <
              class... MatchFields
            , typename std::enable_if<
                       !detail::is_related<match_set, MatchFields...>::value
                    && !detail::is_related<raw_ofp_type, MatchFields...>::value
              >::type* = nullptr
        >
        explicit match_set(MatchFields&&... fields) noexcept
            : match_{protocol::OFPFW_ALL, 0}
        {
            set_impl(std::forward<MatchFields>(fields)...);
        }

        explicit match_set(raw_ofp_type const& match) noexcept
            : match_(match)
        {
        }

        auto empty() const noexcept
            -> bool
        {
            constexpr std::uint32_t nw_addr_mask
                = protocol::OFPFW_NW_SRC_ALL | protocol::OFPFW_NW_DST_ALL;
            constexpr std::uint32_t all_mask_wo_nw_addr
                = protocol::OFPFW_ALL
                & ~(protocol::OFPFW_NW_SRC_MASK | protocol::OFPFW_NW_DST_MASK);

            return ((wildcards() & nw_addr_mask) == nw_addr_mask)
                && ((wildcards() & all_mask_wo_nw_addr) == all_mask_wo_nw_addr);
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
        }

        auto wildcards() const noexcept
            -> std::uint32_t
        {
            return match_.wildcards;
        }

        template <class MatchField>
        auto get() const noexcept
            -> boost::optional<MatchField>
        {
            if (MatchField::is_wildcard(ofp_match())) {
                return boost::none;
            }
            return MatchField::create_from_match(ofp_match());
        }

        template <class MatchField>
        void add(MatchField&& field) noexcept
        {
            set(std::forward<MatchField>(field));
        }

        template <class MatchField>
        void set(MatchField&& field) noexcept
        {
            std::forward<MatchField>(field).set_value(match_);
        }

        template <class MatchField>
        void erase() noexcept
        {
            MatchField::erase_from_match(match_);
        }

        auto ofp_match() const noexcept
            -> raw_ofp_type const&
        {
            return match_;
        }

    private:
        void set_impl() noexcept
        {
        }

        template <class MatchField, class... MatchFields>
        void set_impl(MatchField&& field, MatchFields&&... fields) noexcept
        {
            set(std::forward<MatchField>(field));
            set_impl(std::forward<MatchFields>(fields)...);
        }

        friend basic_protocol_type;

        template <class Validator>
        void validate_impl(Validator)
        {
            // TODO
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, match_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> match_set
        {
            return match_set{detail::decode<raw_ofp_type>(first, last)};
        }

        auto equal_impl(match_set const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(match_, rhs.match_);
        }

        auto equivalent_impl(match_set const& rhs) const noexcept
            -> bool
        {
            return equivalent_for_each_field{this, &rhs};
        }

        struct equivalent_for_each_field
        {
            template <class FieldType>
            auto operator()(FieldType*) const noexcept
                -> bool
            {
                if (auto const lhs_field = lhs->get<FieldType>()) {
                    return equivalent(*lhs_field, *rhs->get<FieldType>());
                }
                return true;
            }

            operator bool() const noexcept
            {
                if ((lhs->wildcards() & protocol::OFPFW_ALL)
                        != (rhs->wildcards() & protocol::OFPFW_ALL)) {
                    return false;
                }

                using match_field_pointer_list = std::tuple<
                      match_fields::in_port*
                    , match_fields::eth_src*, match_fields::eth_dst*
                    , match_fields::vlan_vid*, match_fields::vlan_pcp*
                    , match_fields::eth_type*
                    , match_fields::ip_dscp*, match_fields::ip_proto*
                    , match_fields::ipv4_src*, match_fields::ipv4_dst*
                    , match_fields::tcp_src*, match_fields::tcp_dst*
                >;
                return boost::fusion::all(match_field_pointer_list{}, *this);
            }

            match_set const* lhs;
            match_set const* rhs;
        };

    private:
        raw_ofp_type match_;
    };

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MATCH_SET_HPP
