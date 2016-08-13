#ifndef CANARD_NET_OFP_V13_ACTION_SET_HPP
#define CANARD_NET_OFP_V13_ACTION_SET_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>
#include <boost/optional/optional.hpp>
#include <canard/network/openflow/detail/is_related.hpp>
#include <canard/network/openflow/v13/action_list.hpp>
#include <canard/network/openflow/v13/action_order.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    class action_set
        : private boost::equality_comparable<action_set>
    {
    public:
        using key_type = std::uint64_t;
        using value_type = action_list::value_type;
        using reference = action_list::const_reference;
        using const_reference = action_list::const_reference;
        using iterator = action_list::const_iterator;
        using const_iterator = action_list::const_iterator;
        using difference_type = action_list::difference_type;
        using size_type = action_list::size_type;
        using reverse_iterator = action_list::reverse_iterator;
        using const_reverse_iterator = action_list::const_reverse_iterator;

        action_set() = default;

        template <
              class... Actions
            , typename std::enable_if<
                !detail::is_related<action_set, Actions...>::value
              >::type* = nullptr
        >
        action_set(Actions&&... actions)
        {
            std::pair<const_iterator, bool> dummy[] = {
                insert(std::forward<Actions>(actions))...
            };
            static_cast<void>(dummy);
        }

        CANARD_NET_OFP_DECL auto begin() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto end() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto cbegin() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto cend() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto rbegin() const noexcept
            -> const_reverse_iterator;

        CANARD_NET_OFP_DECL auto rend() const noexcept
            -> const_reverse_iterator;

        CANARD_NET_OFP_DECL auto crbegin() const noexcept
            -> const_reverse_iterator;

        CANARD_NET_OFP_DECL auto crend() const noexcept
            -> const_reverse_iterator;

        CANARD_NET_OFP_DECL auto empty() const noexcept
            -> bool;

        CANARD_NET_OFP_DECL auto size() const noexcept
            -> size_type;

        CANARD_NET_OFP_DECL auto max_size() const noexcept
            -> size_type;

        CANARD_NET_OFP_DECL auto at(key_type const) const
            -> const_reference;

        template <class Action>
        auto at() const
            -> Action const&
        {
            return v13::any_cast<Action>(at(action_order<Action>::value));
        }

        template <class Action>
        auto get() const
            -> Action const&
        {
            auto const it = find(action_order<Action>::value);
            return v13::any_cast<Action>(*it);
        }

        template <class Action>
        auto insert(Action&& action)
            -> std::pair<const_iterator, bool>
        {
            auto const order = get_order(action);
            auto const pos = non_const_lower_bound(order);
            if (pos != actions_.end() && get_order(*pos) == order) {
                return std::make_pair(const_iterator{pos}, false);
            }
            else {
                auto const it = actions_.insert(
                        pos, value_type{std::forward<Action>(action)});
                return std::make_pair(const_iterator{it}, true);
            }
        }

        template <class Action>
        auto assign(Action&& action)
            -> std::pair<const_iterator, bool>
        {
            auto const order = get_order(action);
            auto const pos = non_const_lower_bound(order);
            if (pos != actions_.end() && get_order(*pos) == order) {
                *pos = std::forward<Action>(action);
                return std::make_pair(const_iterator{pos}, true);
            }
            else {
                return std::make_pair(const_iterator{actions_.end()}, false);
            }
        }

        template <class Action>
        auto insert_or_assign(Action&& action)
            -> std::pair<const_iterator, bool>
        {
            auto const order = get_order(action);
            auto const pos = non_const_lower_bound(order);
            if (pos != actions_.end() && get_order(*pos) == order) {
                *pos = std::forward<Action>(action);
                return std::make_pair(const_iterator{pos}, false);
            }
            else {
                auto const it = actions_.insert(
                        pos, value_type{std::forward<Action>(action)});
                return std::make_pair(const_iterator{it}, true);
            }
        }

        CANARD_NET_OFP_DECL auto erase(const_iterator)
            -> const_iterator;

        template <class Action>
        auto erase()
            -> size_type
        {
            auto const it = find(action_order<Action>::value);
            if (it == actions_.end()) {
                return 0;
            }
            actions_.erase(it);
            return 1;
        }

        CANARD_NET_OFP_DECL void swap(action_set&) noexcept;

        CANARD_NET_OFP_DECL void clear() noexcept;

        CANARD_NET_OFP_DECL auto find(key_type const) const
            -> const_iterator;

        template <class Action>
        auto find() const
            -> boost::optional<Action const&>
        {
            auto const it = find(action_order<Action>::value);
            if (it == actions_.end()) {
                return boost::none;
            }
            return v13::any_cast<Action>(*it);
        }

        CANARD_NET_OFP_DECL auto length() const
            -> std::uint16_t;

        CANARD_NET_OFP_DECL auto to_list() const&
            -> action_list const&;

        CANARD_NET_OFP_DECL auto to_list() &&
            -> action_list;

        friend auto operator==(action_set const& lhs, action_set const& rhs)
            -> bool
        {
            return lhs.equal_impl(rhs);
        }

        friend auto equivalent(
                action_set const& lhs, action_set const& rhs) noexcept
            -> bool
        {
            return lhs.equivalent_impl(rhs);
        }

        CANARD_NET_OFP_DECL static auto is_action_set(action_list const&)
            -> bool;

        CANARD_NET_OFP_DECL static auto equivalent_as_action_set(
                action_list const&, action_list const&) noexcept
            -> bool;

    private:
        CANARD_NET_OFP_DECL auto non_const_lower_bound(key_type const)
            -> action_list::iterator;

        CANARD_NET_OFP_DECL auto equal_impl(action_set const&) const
            -> bool;

        CANARD_NET_OFP_DECL auto equivalent_impl(
                action_set const&) const noexcept
            -> bool;

    private:
        action_list actions_;
    };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)
# include <canard/network/openflow/v13/impl/action_set.ipp>
#endif

#endif // CANARD_NET_OFP_V13_ACTION_SET_HPP
