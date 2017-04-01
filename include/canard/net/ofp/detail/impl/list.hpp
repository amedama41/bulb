#ifndef CANARD_NET_OFP_DETAIL_IMPL_LIST_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_LIST_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/list.hpp>

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/numeric.hpp>

namespace canard {
namespace net {
namespace ofp {

  // construct:
  template <class ProtocolType>
  list<ProtocolType>::list() = default;

  template <class ProtocolType>
  list<ProtocolType>::list(size_type const n, const_reference value)
    : values_(n, value)
  {
  }

  template <class ProtocolType>
  auto list<ProtocolType>::operator=(
      std::initializer_list<value_type> const init_list)
    -> list&
  {
    values_ = init_list;
    return *this;
  }

  template <class ProtocolType>
  void list<ProtocolType>::assign(size_type const n, const_reference value)
  {
    values_.assign(n, value);
  }

  template <class ProtocolType>
  void list<ProtocolType>::assign(
      std::initializer_list<value_type> const init_list)
  {
    values_.assign(init_list);
  }

  // iterators:
  template <class ProtocolType>
  auto list<ProtocolType>::begin() noexcept
    -> iterator
  {
    return values_.begin();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::begin() const noexcept
    -> const_iterator
  {
    return values_.begin();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::end() noexcept
    -> iterator
  {
    return values_.end();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::end() const noexcept
    -> const_iterator
  {
    return values_.end();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::cbegin() const noexcept
    -> const_iterator
  {
    return values_.cbegin();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::cend() const noexcept
    -> const_iterator
  {
    return values_.cend();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::rbegin() noexcept
    -> reverse_iterator
  {
    return values_.rbegin();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::rbegin() const noexcept
    -> const_reverse_iterator
  {
    return values_.rbegin();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::rend() noexcept
    -> reverse_iterator
  {
    return values_.rend();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::rend() const noexcept
    -> const_reverse_iterator
  {
    return values_.rend();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::crbegin() const noexcept
    -> const_reverse_iterator
  {
    return values_.crbegin();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::crend() const noexcept
    -> const_reverse_iterator
  {
    return values_.crend();
  }

  // capacity:
  template <class ProtocolType>
  auto list<ProtocolType>::size() const noexcept
    -> size_type
  {
    return values_.size();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::max_size() const noexcept
    -> size_type
  {
    return values_.max_size();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::empty() const noexcept
    -> bool
  {
    return values_.empty();
  }

  template <class ProtocolType>
  void list<ProtocolType>::reserve(size_type const n)
  {
    values_.reserve(n);
  }

  template <class ProtocolType>
  void list<ProtocolType>::shrink_to_fit()
  {
    values_.shrink_to_fit();
  }

  // element access:
  template <class ProtocolType>
  auto list<ProtocolType>::operator[](size_type const n)
    -> reference
  {
    return values_[n];
  }

  template <class ProtocolType>
  auto list<ProtocolType>::operator[](size_type const n) const
    -> const_reference
  {
    return values_[n];
  }

  template <class ProtocolType>
  auto list<ProtocolType>::at(size_type const n)
    -> reference
  {
    return values_.at(n);
  }

  template <class ProtocolType>
  auto list<ProtocolType>::at(size_type const n) const
    -> const_reference
  {
    return values_.at(n);
  }

  template <class ProtocolType>
  auto list<ProtocolType>::front()
    -> reference
  {
    return values_.front();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::front() const
    -> const_reference
  {
    return values_.front();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::back()
    -> reference
  {
    return values_.back();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::back() const
    -> const_reference
  {
    return values_.back();
  }

  // modifiers:
  template <class ProtocolType>
  void list<ProtocolType>::push_back(value_type const& value)
  {
    values_.push_back(value);
  }

  template <class ProtocolType>
  void list<ProtocolType>::push_back(value_type&& value)
  {
    values_.push_back(std::move(value));
  }

  template <class ProtocolType>
  void list<ProtocolType>::pop_back()
  {
    values_.pop_back();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::insert(
      const_iterator const pos, size_type const n, const_reference value)
    -> iterator
  {
    return values_.insert(pos, n, value);
  }

  template <class ProtocolType>
  auto list<ProtocolType>::insert(
        const_iterator const pos
      , std::initializer_list<value_type> const init_list)
    -> iterator
  {
    return values_.insert(pos, init_list);
  }

  template <class ProtocolType>
  auto list<ProtocolType>::erase(const_iterator const pos)
    -> iterator
  {
    return values_.erase(pos);
  }

  template <class ProtocolType>
  auto list<ProtocolType>::erase(
      const_iterator const first, const_iterator const last)
    -> iterator
  {
    return values_.erase(first, last);
  }

  template <class ProtocolType>
  void list<ProtocolType>::swap(list& other) noexcept
  {
    values_.swap(other.values_);
  }

  template <class ProtocolType>
  void list<ProtocolType>::clear() noexcept
  {
    return values_.clear();
  }

  // OFP operations:
  template <class ProtocolType>
  auto list<ProtocolType>::calc_ofp_length(
      std::uint16_t const base_length) const
    -> std::uint16_t
  {
    auto const max_length
      = std::numeric_limits<std::uint16_t>::max() - base_length;
    if (size() > (max_length / value_type::min_byte_length())) {
      throw std::runtime_error{"too many elements"};
    }
    auto const total_length = length_impl();
    if (total_length > max_length) {
      throw std::runtime_error{"too large length"};
    }
    return total_length + base_length;
  }

  template <class ProtocolType>
  auto list<ProtocolType>::length() const noexcept
    -> std::size_t
  {
    return length_impl();
  }

  template <class ProtocolType>
  auto list<ProtocolType>::byte_length() const noexcept
    -> std::size_t
  {
    return length_impl();
  }

  // private operations:
  template <class ProtocolType>
  list<ProtocolType>::list(container_type&& values)
    : values_(std::move(values))
  {
  }

  template <class ProtocolType>
  auto list<ProtocolType>::init_impl(value_type&& value)
    -> list_detail::dummy_type
  {
    values_.push_back(std::move(value));
    return list_detail::dummy_type{};
  }

  template <class ProtocolType>
  void list<ProtocolType>::assign_impl(
      value_type* const first, value_type* const last)
  {
    values_.assign(
        std::make_move_iterator(first), std::make_move_iterator(last));
  }

  template <class ProtocolType>
  auto list<ProtocolType>::insert_impl(
      const_iterator const pos, value_type* const first, value_type* const last)
    -> iterator
  {
    return values_.insert(
        pos, std::make_move_iterator(first), std::make_move_iterator(last));
  }

  template <class ProtocolType>
  auto list<ProtocolType>::length_impl() const noexcept
    -> std::size_t
  {
    return boost::accumulate(
          values_, std::size_t{0}
        , [](std::size_t const sum, const_reference v)
          { return sum + v.byte_length(); });
  }

  template <class ProtocolType>
  auto list<ProtocolType>::equal_impl(list const& rhs) const noexcept
    -> bool
  {
    return values_ == rhs.values_;
  }

  template <class ProtocolType>
  auto list<ProtocolType>::equivalent_impl(list const& rhs) const noexcept
    -> bool
  {
    return boost::equal(
          values_, rhs.values_
        , [](const_reference lhs, const_reference rhs)
          { return equivalent(lhs, rhs); });
  }

} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_LIST_HPP
