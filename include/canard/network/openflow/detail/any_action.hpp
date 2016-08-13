#ifndef CANARD_NET_OFP_DETAIL_ANY_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_ANY_ACTION_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/size.hpp>
#include <boost/operators.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <canard/mpl/adapted/std_tuple.hpp>
#include <canard/network/openflow/detail/is_related.hpp>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
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
        static constexpr std::size_t number_of_types
            = boost::mpl::size<action_type_list>::type::value;

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

        CANARD_NET_OFP_DECL auto type() const noexcept
            -> ofp_action_type;

        CANARD_NET_OFP_DECL auto length() const noexcept
            -> std::uint16_t;

        CANARD_NET_OFP_DECL auto index() const noexcept
            -> std::size_t;

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
            return lhs.equal_impl(rhs);
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto operator==(any_action const& lhs, Action const& rhs)
            -> bool
        {
            if (auto const action
                    = boost::get<Action>(std::addressof(lhs.variant_))) {
                return *action == rhs;
            }
            return false;
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto operator==(Action const& lhs, any_action const& rhs)
            -> bool
        {
            if (auto const action
                    = boost::get<Action>(std::addressof(rhs.variant_))) {
                return lhs == *action;
            }
            return false;
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto operator!=(any_action const& lhs, Action const& rhs)
            -> bool
        {
            return !(lhs == rhs);
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto operator!=(Action const& lhs, any_action const& rhs)
            -> bool
        {
            return !(lhs == rhs);
        }

        friend auto equivalent(
                any_action const& lhs, any_action const& rhs) noexcept
            -> bool
        {
            return lhs.equivalent_impl(rhs);
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto equivalent(
                any_action const& lhs, Action const& rhs) noexcept
            -> bool
        {
            if (auto const action
                    = boost::get<Action>(std::addressof(lhs.variant_))) {
                return equivalent(*action, rhs);
            }
            return false;
        }

        template <class Action, class = containable_if_t<Action>>
        friend auto equivalent(
                Action const& lhs, any_action const& rhs) noexcept
            -> bool
        {
            if (auto const action
                    = boost::get<Action>(std::addressof(rhs.variant_))) {
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
        CANARD_NET_OFP_DECL auto equal_impl(any_action const& rhs) const
            -> bool;

        CANARD_NET_OFP_DECL auto equivalent_impl(
                any_action const& rhs) const noexcept
            -> bool;

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
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/openflow/detail/impl/any_action.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_ACTION_HPP
