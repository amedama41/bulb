#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_OXM_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_OXM_HPP

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/type_traits/is_all_constructible.hpp>
#include <canard/network/openflow/v13/any_oxm_id.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace table_feature_properties {

    template <class T>
    class basic_prop_oxm
        : public detail::basic_protocol_type<basic_prop_oxm<T>>
    {
    public:
        static constexpr protocol::ofp_table_feature_prop_type prop_type
            = T::prop_type;

        using raw_ofp_type = protocol::ofp_table_feature_prop_oxm;
        using oxm_ids_type = ofp::list<any_oxm_id>;
        using iterator = oxm_ids_type::iterator;
        using const_iterator = oxm_ids_type::const_iterator;

    private:
        template <class... OXMIDs>
        using enable_if_is_all_constructible_t = typename std::enable_if<
            type_traits::is_all_constructible<
                oxm_ids_type::value_type, OXMIDs...
            >::value
        >::type;

    public:
        explicit basic_prop_oxm(oxm_ids_type oxm_ids)
            : table_feature_prop_oxm_{
                  prop_type
                , oxm_ids.calc_ofp_length(sizeof(raw_ofp_type))
              }
            , oxm_ids_(std::move(oxm_ids))
        {
        }

        template <
            class... OXMIDs, class = enable_if_is_all_constructible_t<OXMIDs...>
        >
        explicit basic_prop_oxm(OXMIDs&&... oxm_ids)
            : basic_prop_oxm{oxm_ids_type{std::forward<OXMIDs>(oxm_ids)...}}
        {
        }

        basic_prop_oxm(basic_prop_oxm const&) = default;

        basic_prop_oxm(basic_prop_oxm&& other)
            : table_feature_prop_oxm_(other.table_feature_prop_oxm_)
            , oxm_ids_(other.extract_oxm_ids())
        {
        }

        auto operator=(basic_prop_oxm const& other)
            -> basic_prop_oxm&
        {
            return operator=(basic_prop_oxm{other});
        }

        auto operator=(basic_prop_oxm&& other)
            -> basic_prop_oxm&
        {
            auto tmp = std::move(other);
            std::swap(table_feature_prop_oxm_, tmp.table_feature_prop_oxm_);
            oxm_ids_.swap(tmp.oxm_ids_);
            return *this;
        }

        static constexpr auto type() noexcept
            -> protocol::ofp_table_feature_prop_type
        {
            return prop_type;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return table_feature_prop_oxm_.length;
        }

        auto oxm_ids() const noexcept
            -> oxm_ids_type const&
        {
            return oxm_ids_;
        }

        auto extract_oxm_ids()
            -> oxm_ids_type
        {
            auto oxm_ids = oxm_ids_type{};
            oxm_ids.swap(oxm_ids_);
            table_feature_prop_oxm_.length = sizeof(raw_ofp_type);
            return oxm_ids;
        }

        auto begin() noexcept
            -> iterator
        {
            return oxm_ids_.begin();
        }

        auto begin() const noexcept
            -> const_iterator
        {
            return oxm_ids_.begin();
        }

        auto end() noexcept
            -> iterator
        {
            return oxm_ids_.end();
        }

        auto end() const noexcept
            -> const_iterator
        {
            return oxm_ids_.end();
        }

    private:
        basic_prop_oxm(
                  raw_ofp_type const& table_feature_prop_oxm
                , oxm_ids_type&& oxm_ids)
            : table_feature_prop_oxm_(table_feature_prop_oxm)
            , oxm_ids_(std::move(oxm_ids))
        {
        }

        friend detail::basic_protocol_type<basic_prop_oxm>;

        friend constexpr auto exclude_padding(
            detail::basic_protocol_type_tag<basic_prop_oxm>) noexcept
          -> bool
        {
          return true;
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, table_feature_prop_oxm_);
            boost::for_each(oxm_ids_, [&](any_oxm_id const& id) {
                id.encode(container);
            });
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> basic_prop_oxm
        {
            auto const property = detail::decode<raw_ofp_type>(first, last);

            last = std::next(first, property.length - sizeof(raw_ofp_type));
            auto oxm_ids = oxm_ids_type::decode(first, last);

            return basic_prop_oxm{property, std::move(oxm_ids)};
        }

        auto equal_impl(basic_prop_oxm const& rhs) const noexcept
            -> bool
        {
            return oxm_ids_ == rhs.oxm_ids_;
        }

        auto equivalent_impl(basic_prop_oxm const& rhs) const noexcept
            -> bool
        {
            if (oxm_ids_.size() != rhs.oxm_ids_.size()) {
                return false;
            }
            auto const rhs_ids_end = rhs.oxm_ids_.end();
            for (auto const& lhs_id : oxm_ids_) {
                using const_reference = oxm_ids_type::const_reference;
                if (boost::find_if(
                              rhs.oxm_ids_
                            , [&lhs_id](const_reference rhs_id)
                              { return equivalent(lhs_id, rhs_id); })
                        == rhs_ids_end) {
                    return false;
                }
            }
            return true;
        }

    private:
        raw_ofp_type table_feature_prop_oxm_;
        oxm_ids_type oxm_ids_;
    };

    namespace oxm_detail {

        struct match
        {
            static constexpr protocol::ofp_table_feature_prop_type prop_type
                = protocol::OFPTFPT_MATCH;
        };

        struct wildcards
        {
            static constexpr protocol::ofp_table_feature_prop_type prop_type
                = protocol::OFPTFPT_WILDCARDS;
        };

        struct write_setfield
        {
            static constexpr protocol::ofp_table_feature_prop_type prop_type
                = protocol::OFPTFPT_WRITE_SETFIELD;
        };

        struct write_setfield_miss
        {
            static constexpr protocol::ofp_table_feature_prop_type prop_type
                = protocol::OFPTFPT_WRITE_SETFIELD_MISS;
        };


        struct apply_setfield
        {
            static constexpr protocol::ofp_table_feature_prop_type prop_type
                = protocol::OFPTFPT_APPLY_SETFIELD;
        };


        struct apply_setfield_miss
        {
            static constexpr protocol::ofp_table_feature_prop_type prop_type
                = protocol::OFPTFPT_APPLY_SETFIELD_MISS;
        };

    } // namespace oxm_detail

    using match = basic_prop_oxm<oxm_detail::match>;
    using wildcards = basic_prop_oxm<oxm_detail::wildcards>;
    using write_setfield = basic_prop_oxm<oxm_detail::write_setfield>;
    using write_setfield_miss = basic_prop_oxm<oxm_detail::write_setfield_miss>;
    using apply_setfield = basic_prop_oxm<oxm_detail::apply_setfield>;
    using apply_setfield_miss = basic_prop_oxm<oxm_detail::apply_setfield_miss>;

} // namespace table_feature_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_OXM_HPP
