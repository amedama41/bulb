#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_ACTIONS_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_ACTIONS_HPP

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
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/table_feature_property/id/action_id.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace table_feature_properties {

    template <class T>
    class basic_prop_actions
        : public detail::basic_protocol_type<basic_prop_actions<T>>
    {
    public:
        static constexpr protocol::ofp_table_feature_prop_type prop_type
            = T::prop_type;

        using raw_ofp_type = protocol::ofp_table_feature_prop_actions;
        using action_ids_type = ofp::list<action_id>;
        using iterator = action_ids_type::iterator;
        using const_iterator = action_ids_type::const_iterator;

    private:
        template <class... ActionIDs>
        using enable_if_is_all_constructible_t = typename std::enable_if<
            type_traits::is_all_constructible<
                action_ids_type::value_type, ActionIDs...
            >::value
        >::type;

    public:
        explicit basic_prop_actions(action_ids_type action_ids)
            : table_feature_prop_actions_{
                  prop_type
                , action_ids.calc_ofp_length(sizeof(raw_ofp_type))
              }
            , action_ids_(std::move(action_ids))
        {
        }

        template <
              class... ActionIDs
            , class = enable_if_is_all_constructible_t<ActionIDs...>
        >
        explicit basic_prop_actions(ActionIDs&&... action_ids)
            : basic_prop_actions{
                action_ids_type{std::forward<ActionIDs>(action_ids)...}
              }
        {
        }

        basic_prop_actions(basic_prop_actions const&) = default;

        basic_prop_actions(basic_prop_actions&& other)
            : table_feature_prop_actions_(other.table_feature_prop_actions_)
            , action_ids_(other.extract_action_ids())
        {
        }

        auto operator=(basic_prop_actions const& other)
            -> basic_prop_actions&
        {
            return operator=(basic_prop_actions{other});
        }

        auto operator=(basic_prop_actions&& other)
            -> basic_prop_actions&
        {
            auto tmp = std::move(other);
            std::swap(table_feature_prop_actions_
                    , tmp.table_feature_prop_actions_);
            action_ids_.swap(tmp.action_ids_);
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
            return table_feature_prop_actions_.length;
        }

        auto action_ids() const noexcept
            -> action_ids_type const&
        {
            return action_ids_;
        }

        auto extract_action_ids()
            -> action_ids_type
        {
            auto action_ids = action_ids_type{};
            action_ids.swap(action_ids_);
            table_feature_prop_actions_.length = sizeof(raw_ofp_type);
            return action_ids;
        }

        auto begin() noexcept
            -> iterator
        {
            return action_ids_.begin();
        }

        auto begin() const noexcept
            -> const_iterator
        {
            return action_ids_.begin();
        }

        auto end() noexcept
            -> iterator
        {
            return action_ids_.end();
        }

        auto end() const noexcept
            -> const_iterator
        {
            return action_ids_.end();
        }

    private:
        basic_prop_actions(
                  raw_ofp_type const& table_feature_prop_actions
                , action_ids_type&& action_ids)
            : table_feature_prop_actions_(table_feature_prop_actions)
            , action_ids_(std::move(action_ids))
        {
        }

        friend detail::basic_protocol_type<basic_prop_actions>;

        friend constexpr auto exclude_padding(
            detail::basic_protocol_type_tag<basic_prop_actions>) noexcept
          -> bool
        {
          return true;
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, table_feature_prop_actions_);
            boost::for_each(action_ids_, [&](action_id const& id) {
                id.encode(container);
            });
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> basic_prop_actions
        {
            auto const property = detail::decode<raw_ofp_type>(first, last);

            last = std::next(first, property.length - sizeof(raw_ofp_type));
            auto action_ids = action_ids_type::decode(first, last);

            return basic_prop_actions{property, std::move(action_ids)};
        }

        auto equal_impl(basic_prop_actions const& rhs) const noexcept
            -> bool
        {
            return action_ids_ == rhs.action_ids_;
        }

        auto equivalent_impl(basic_prop_actions const& rhs) const noexcept
            -> bool
        {
            if (action_ids_.size() != rhs.action_ids_.size()) {
                return false;
            }
            auto const rhs_ids_end = rhs.action_ids_.end();
            for (auto const& lhs_id : action_ids_) {
                using const_reference = action_ids_type::const_reference;
                if (boost::find_if(
                              rhs.action_ids_
                            , [&lhs_id](const_reference rhs_id)
                              { return equivalent(lhs_id, rhs_id); })
                        == rhs_ids_end) {
                    return false;
                }
            }
            return true;
        }

    private:
        raw_ofp_type table_feature_prop_actions_;
        action_ids_type action_ids_;
    };

    namespace actions_detail {

        struct write_actions
        {
            static protocol::ofp_table_feature_prop_type const prop_type
                = protocol::OFPTFPT_WRITE_ACTIONS;
        };

        struct write_actions_miss
        {
            static protocol::ofp_table_feature_prop_type const prop_type
                = protocol::OFPTFPT_WRITE_ACTIONS_MISS;
        };

        struct apply_actions
        {
            static protocol::ofp_table_feature_prop_type const prop_type
                = protocol::OFPTFPT_APPLY_ACTIONS;
        };

        struct apply_actions_miss
        {
            static protocol::ofp_table_feature_prop_type const prop_type
                = protocol::OFPTFPT_APPLY_ACTIONS_MISS;
        };

    } // namespace actions_detail

    using write_actions = basic_prop_actions<actions_detail::write_actions>;
    using write_actions_miss
        = basic_prop_actions<actions_detail::write_actions_miss>;
    using apply_actions = basic_prop_actions<actions_detail::apply_actions>;
    using apply_actions_miss
        = basic_prop_actions<actions_detail::apply_actions_miss>;

} // namespace table_feature_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_ACTIONS_HPP
