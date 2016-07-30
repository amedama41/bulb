#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_ANY_ACTION_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_ANY_ACTION_HPP

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/mpl/contains.hpp>
#include <boost/operators.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <canard/mpl/adapted/std_tuple.hpp>
#include <canard/network/protocol/openflow/detail/is_related.hpp>
#include <canard/network/protocol/openflow/detail/visitors.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    template <class ActionDecoder>
    class any_action
        : private boost::equality_comparable<any_action<ActionDecoder>>
    {
        using ofp_action_type = typename ActionDecoder::ofp_action_type;
        using action_type_list = typename ActionDecoder::action_type_list;
        using action_variant
            = typename boost::make_variant_over<action_type_list>::type;

        template <class Action>
        using containable_if_t = typename std::enable_if<
            boost::mpl::contains<action_type_list, Action>::value
        >::type;

    public:
        template <
              class Action
            , typename std::enable_if<
                  !detail::is_related<any_action, Action>::value
              >::type* = nullptr
        >
        any_action(Action&& action)
            : variant_(std::forward<Action>(action))
        {
        }

        template <
              class Action
            , typename std::enable_if<
                  !detail::is_related<any_action, Action>::value
              >::type* = nullptr
        >
        auto operator=(Action&& action)
            -> any_action&
        {
            variant_ = std::forward<Action>(action);
            return *this;
        }

        auto type() const noexcept
            -> ofp_action_type
        {
            auto visitor = detail::type_visitor<ofp_action_type>{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            auto visitor = detail::length_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            auto visitor = detail::encoding_visitor<Container>{container};
            return boost::apply_visitor(visitor, variant_);
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> any_action
        {
            return ActionDecoder::template decode<any_action>(
                    first, last, to_any_action{});
        }

        template <class Visitor>
        auto visit(Visitor&& visitor)
            -> typename std::remove_reference<Visitor>::type::result_type
        {
            return boost::apply_visitor(
                    std::forward<Visitor>(visitor), variant_);
        }

        template <class Visitor>
        auto visit(Visitor&& visitor) const
            -> typename std::remove_reference<Visitor>::type::result_type
        {
            return boost::apply_visitor(
                    std::forward<Visitor>(visitor), variant_);
        }

        friend auto operator==(any_action const& lhs, any_action const& rhs)
            -> bool
        {
            return lhs.variant_ == rhs.variant_;
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto operator==(any_action const& lhs, Action const& rhs)
            -> bool
        {
            if (auto const action = boost::get<Action>(&lhs.variant_)) {
                return *action == rhs;
            }
            return false;
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto operator==(Action const& lhs, any_action const& rhs)
            -> bool
        {
            if (auto const action = boost::get<Action>(&rhs.variant_)) {
                return lhs == *action;
            }
            return false;
        }

        friend auto equivalent(
                any_action const& lhs, any_action const& rhs) noexcept
            -> bool
        {
            auto visitor = detail::equivalent_visitor{};
            return boost::apply_visitor(visitor, lhs.variant_, rhs.variant_);
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto equivalent(
                any_action const& lhs, Action const& rhs) noexcept
            -> bool
        {
            if (auto const action = boost::get<Action>(&lhs.variant_)) {
                return equivalent(*action, rhs);
            }
            return false;
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto equivalent(
                Action const& lhs, any_action const& rhs) noexcept
            -> bool
        {
            if (auto const action = boost::get<Action>(&rhs.variant_)) {
                return equivalent(lhs, *action);
            }
            return false;
        }

        template <class T, class Decoder>
        friend auto any_cast(any_action<Decoder> const&)
            -> T const&;

        template <class T, class Decoder>
        friend auto any_cast(any_action<Decoder> const*)
            -> T const*;

    private:
        struct to_any_action
        {
            template <class Action>
            auto operator()(Action&& action) const
                -> any_action
            {
                return any_action{std::forward<Action>(action)};
            }
        };

    private:
        action_variant variant_;
    };

    template <class T, class ActionDecoder>
    auto any_cast(any_action<ActionDecoder> const& action)
        -> T const&
    {
        return boost::get<T>(action.variant_);
    }

    template <class T, class ActionDecoder>
    auto any_cast(any_action<ActionDecoder> const* const action)
        -> T const*
    {
        return boost::get<T>(std::addressof(action->variant_));
    }

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_ANY_ACTION_HPP
