#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_IMPL_ACTION_LIST_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_IMPL_ACTION_LIST_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>
#include <canard/network/protocol/openflow/detail/action_list.hpp>
#include <cstddef>
#include <utility>
#include <vector>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/numeric.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::begin() noexcept
    -> iterator
  {
    return actions_.begin();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::begin() const noexcept
    -> const_iterator
  {
    return actions_.begin();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::end() noexcept
    -> iterator
  {
    return actions_.end();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::end() const noexcept
    -> const_iterator
  {
    return actions_.end();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::cbegin() const noexcept
    -> const_iterator
  {
    return actions_.cbegin();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::cend() const noexcept
    -> const_iterator
  {
    return actions_.cend();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::rbegin() noexcept
    -> reverse_iterator
  {
    return actions_.rbegin();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::rbegin() const noexcept
    -> const_reverse_iterator
  {
    return actions_.rbegin();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::rend() noexcept
    -> reverse_iterator
  {
    return actions_.rend();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::rend() const noexcept
    -> const_reverse_iterator
  {
    return actions_.rend();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::crbegin() const noexcept
    -> const_reverse_iterator
  {
    return actions_.crbegin();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::crend() const noexcept
    -> const_reverse_iterator
  {
    return actions_.crend();
  }

  template <class AnyAction, class ActionHeader>
  void action_list<AnyAction, ActionHeader>::swap(action_list& other) noexcept
  {
    actions_.swap(other.actions_);
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::size() const noexcept
    -> size_type
  {
    return actions_.size();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::max_size() const noexcept
    -> size_type
  {
    return actions_.max_size();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::empty() const noexcept
    -> bool
  {
    return actions_.empty();
  }

  template <class AnyAction, class ActionHeader>
  void action_list<AnyAction, ActionHeader>::clear() noexcept
  {
    return actions_.clear();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::operator[](size_type const n)
    -> reference
  {
    return actions_[n];
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::operator[](size_type const n) const
    -> const_reference
  {
    return actions_[n];
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::at(size_type const n)
    -> reference
  {
    return actions_.at(n);
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::at(size_type const n) const
    -> const_reference
  {
    return actions_.at(n);
  }

  template <class AnyAction, class ActionHeader>
  void action_list<AnyAction, ActionHeader>::pop_back()
  {
    actions_.pop_back();
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::erase(const_iterator pos)
    -> iterator
  {
    return actions_.erase(pos);
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::erase(
      const_iterator first, const_iterator last)
    -> iterator
  {
    return actions_.erase(first, last);
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::length() const
    -> std::size_t
  {
    return boost::accumulate(
          actions_ | boost::adaptors::transformed(
          [](value_type const& action) { return action.length(); })
        , std::size_t{0});
  }

  template <class AnyAction, class ActionHeader>
  action_list<AnyAction, ActionHeader>::action_list(
      std::vector<value_type>&& actions)
    : actions_(std::move(actions))
  {
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::equal_impl(
      action_list const& rhs) const
    -> bool
  {
    return actions_ == rhs.actions_;
  }

  template <class AnyAction, class ActionHeader>
  auto action_list<AnyAction, ActionHeader>::equivalent_impl(
      action_list const& rhs) const noexcept
    -> bool
  {
    return boost::equal(
          actions_, rhs.actions_
        , [](const_reference lhs, const_reference rhs)
          { return equivalent(lhs, rhs); });
  }

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_IMPL_ACTION_LIST_HPP
