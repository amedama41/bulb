#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_LIST_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_LIST_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/any_list.hpp>

#include <cstddef>
#include <utility>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/numeric.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class AnyType>
  auto any_list<AnyType>::begin() noexcept
    -> iterator
  {
    return values_.begin();
  }

  template <class AnyType>
  auto any_list<AnyType>::begin() const noexcept
    -> const_iterator
  {
    return values_.begin();
  }

  template <class AnyType>
  auto any_list<AnyType>::end() noexcept
    -> iterator
  {
    return values_.end();
  }

  template <class AnyType>
  auto any_list<AnyType>::end() const noexcept
    -> const_iterator
  {
    return values_.end();
  }

  template <class AnyType>
  auto any_list<AnyType>::cbegin() const noexcept
    -> const_iterator
  {
    return values_.cbegin();
  }

  template <class AnyType>
  auto any_list<AnyType>::cend() const noexcept
    -> const_iterator
  {
    return values_.cend();
  }

  template <class AnyType>
  auto any_list<AnyType>::rbegin() noexcept
    -> reverse_iterator
  {
    return values_.rbegin();
  }

  template <class AnyType>
  auto any_list<AnyType>::rbegin() const noexcept
    -> const_reverse_iterator
  {
    return values_.rbegin();
  }

  template <class AnyType>
  auto any_list<AnyType>::rend() noexcept
    -> reverse_iterator
  {
    return values_.rend();
  }

  template <class AnyType>
  auto any_list<AnyType>::rend() const noexcept
    -> const_reverse_iterator
  {
    return values_.rend();
  }

  template <class AnyType>
  auto any_list<AnyType>::crbegin() const noexcept
    -> const_reverse_iterator
  {
    return values_.crbegin();
  }

  template <class AnyType>
  auto any_list<AnyType>::crend() const noexcept
    -> const_reverse_iterator
  {
    return values_.crend();
  }

  template <class AnyType>
  void any_list<AnyType>::swap(any_list& other) noexcept
  {
    values_.swap(other.values_);
  }

  template <class AnyType>
  auto any_list<AnyType>::size() const noexcept
    -> size_type
  {
    return values_.size();
  }

  template <class AnyType>
  auto any_list<AnyType>::max_size() const noexcept
    -> size_type
  {
    return values_.max_size();
  }

  template <class AnyType>
  auto any_list<AnyType>::empty() const noexcept
    -> bool
  {
    return values_.empty();
  }

  template <class AnyType>
  void any_list<AnyType>::clear() noexcept
  {
    return values_.clear();
  }

  template <class AnyType>
  auto any_list<AnyType>::operator[](size_type const n)
    -> reference
  {
    return values_[n];
  }

  template <class AnyType>
  auto any_list<AnyType>::operator[](size_type const n) const
    -> const_reference
  {
    return values_[n];
  }

  template <class AnyType>
  auto any_list<AnyType>::at(size_type const n)
    -> reference
  {
    return values_.at(n);
  }

  template <class AnyType>
  auto any_list<AnyType>::at(size_type const n) const
    -> const_reference
  {
    return values_.at(n);
  }

  template <class AnyType>
  void any_list<AnyType>::pop_back()
  {
    values_.pop_back();
  }

  template <class AnyType>
  auto any_list<AnyType>::erase(const_iterator pos)
    -> iterator
  {
    return values_.erase(pos);
  }

  template <class AnyType>
  auto any_list<AnyType>::erase(const_iterator first, const_iterator last)
    -> iterator
  {
    return values_.erase(first, last);
  }

  template <class AnyType>
  auto any_list<AnyType>::length() const
    -> std::size_t
  {
    return boost::accumulate(
          values_
        | boost::adaptors::transformed(
            [](const_reference v) { return v.length(); })
        , std::size_t{0});
  }

  template <class AnyType>
  any_list<AnyType>::any_list(container&& values)
    : values_(std::move(values))
  {
  }

  template <class AnyType>
  auto any_list<AnyType>::equal_impl(any_list const& rhs) const noexcept
    -> bool
  {
    return values_ == rhs.values_;
  }

  template <class AnyType>
  auto any_list<AnyType>::equivalent_impl(any_list const& rhs) const noexcept
    -> bool
  {
    return boost::equal(
          values_, rhs.values_
        , [](const_reference lhs, const_reference rhs)
          { return equivalent(lhs, rhs); });
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_LIST_HPP
