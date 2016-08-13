#ifndef CANARD_NET_OFP_DETAIL_ACTION_LIST_HPP
#define CANARD_NET_OFP_DETAIL_ACTION_LIST_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include <boost/operators.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/network/openflow/detail/is_related.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

    template <class T>
    auto is_input_iterator_impl(T&)
        -> std::is_base_of<
              std::input_iterator_tag
            , typename std::iterator_traits<T>::iterator_category
           >;

    auto is_input_iterator_impl(...)
        -> std::false_type;

    template <class... T>
    struct is_input_iterator : std::false_type
    {};

    template <class T>
    struct is_input_iterator<T>
        : decltype(is_input_iterator_impl(std::declval<T>()))
    {};


    template <class AnyAction, class ActionHeader>
    class action_list
        : private boost::equality_comparable<
            action_list<AnyAction, ActionHeader>
          >
    {
        using container = std::vector<AnyAction>;

    public:
        using value_type = typename container::value_type;
        using reference = typename container::reference;
        using const_reference = typename container::const_reference;
        using iterator = typename container::iterator;
        using const_iterator = typename container::const_iterator;
        using difference_type = typename container::difference_type;
        using size_type = typename container::size_type;

        using reverse_iterator = typename container::reverse_iterator;
        using const_reverse_iterator
            = typename container::const_reverse_iterator;

        template <
              class... Actions
            , typename std::enable_if<
                    !detail::is_related<action_list, Actions...>::value
              >::type* = nullptr
        >
        action_list(Actions&&... actions)
            : actions_{value_type(std::forward<Actions>(actions))...}
        {
        }

        CANARD_NET_OFP_DECL auto begin() noexcept
            -> iterator;

        CANARD_NET_OFP_DECL auto begin() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto end() noexcept
            -> iterator;

        CANARD_NET_OFP_DECL auto end() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto cbegin() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto cend() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto rbegin() noexcept
            -> reverse_iterator;

        CANARD_NET_OFP_DECL auto rbegin() const noexcept
            -> const_reverse_iterator;

        CANARD_NET_OFP_DECL auto rend() noexcept
            -> reverse_iterator;

        CANARD_NET_OFP_DECL auto rend() const noexcept
            -> const_reverse_iterator;

        CANARD_NET_OFP_DECL auto crbegin() const noexcept
            -> const_reverse_iterator;

        CANARD_NET_OFP_DECL auto crend() const noexcept
            -> const_reverse_iterator;

        CANARD_NET_OFP_DECL void swap(action_list&) noexcept;

        CANARD_NET_OFP_DECL auto size() const noexcept
            -> size_type;

        CANARD_NET_OFP_DECL auto max_size() const noexcept
            -> size_type;

        CANARD_NET_OFP_DECL auto empty() const noexcept
            -> bool;

        CANARD_NET_OFP_DECL void clear() noexcept;

        CANARD_NET_OFP_DECL auto operator[](size_type const)
            -> reference;

        CANARD_NET_OFP_DECL auto operator[](size_type const) const
            -> const_reference;

        CANARD_NET_OFP_DECL auto at(size_type const)
            -> reference;

        CANARD_NET_OFP_DECL auto at(size_type const) const
            -> const_reference;

        template <class Action>
        void push_back(Action&& action)
        {
            actions_.emplace_back(std::forward<Action>(action));
        }

        CANARD_NET_OFP_DECL void pop_back();

        template <class... Actions>
        auto insert(const_iterator pos, Actions&&... actions)
            -> typename std::enable_if<
                  !is_input_iterator<Actions...>::value, iterator
               >::type
        {
            return actions_.insert(
                    pos, { value_type(std::forward<Actions>(actions))... });
        }

        template <class InputIterator>
        auto insert(const_iterator pos, InputIterator first, InputIterator last)
            -> typename std::enable_if<
                  is_input_iterator<InputIterator>::value, iterator
               >::type
        {
            return actions_.insert(pos, first, last);
        }

        CANARD_NET_OFP_DECL auto erase(const_iterator)
            -> iterator;

        CANARD_NET_OFP_DECL auto erase(
                const_iterator first, const_iterator last)
            -> iterator;

        CANARD_NET_OFP_DECL auto length() const
            -> std::size_t;

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            boost::for_each(actions_, [&](value_type const& action) {
                action.encode(container);
            });
            return container;
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> action_list
        {
            auto actions = std::vector<value_type>{};
            actions.reserve(std::distance(first, last) / sizeof(ActionHeader));
            while (std::distance(first, last) >= sizeof(ActionHeader)) {
                actions.push_back(value_type::decode(first, last));
            }
            if (first != last) {
                throw std::runtime_error{"action_list length is invalid"};
            }
            return action_list{std::move(actions)};
        }

        friend auto operator==(action_list const& lhs, action_list const& rhs)
            -> bool
        {
            return lhs.equal_impl(rhs);
        }

        friend auto equivalent(
                action_list const& lhs, action_list const& rhs) noexcept
            -> bool
        {
            return lhs.equivalent_impl(rhs);
        }

    private:
        CANARD_NET_OFP_DECL explicit action_list(std::vector<value_type>&&);

        CANARD_NET_OFP_DECL auto equal_impl(action_list const&) const
            -> bool;

        CANARD_NET_OFP_DECL auto equivalent_impl(
                action_list const&) const noexcept
            -> bool;

    private:
        std::vector<value_type> actions_;
    };

    template <class AnyAction, class ActionHeader>
    void swap(action_list<AnyAction, ActionHeader>& lhs
            , action_list<AnyAction, ActionHeader>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/openflow/detail/impl/action_list.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ACTION_LIST_HPP
