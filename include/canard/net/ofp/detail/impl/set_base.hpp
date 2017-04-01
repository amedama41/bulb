#ifndef CANARD_NET_OFP_IMPL_SET_BASE_HPP
#define CANARD_NET_OFP_IMPL_SET_BASE_HPP

#include <canard/net/ofp/detail/config.hpp>
#include <canard/net/ofp/detail/set_base.hpp>

#include <memory>
#include <stdexcept>
#include <utility>
#include <boost/container/flat_map.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/range/adaptor/map.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Derived, class Key, class T>
  set_base<Derived, Key, T>::set_base() = default;

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::begin() const noexcept
    -> const_iterator
  {
    return list_.begin();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::end() const noexcept
    -> const_iterator
  {
    return list_.end();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::cbegin() const noexcept
    -> const_iterator
  {
    return list_.cbegin();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::cend() const noexcept
    -> const_iterator
  {
    return list_.cend();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::rbegin() const noexcept
    -> const_reverse_iterator
  {
    return list_.rbegin();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::rend() const noexcept
    -> const_reverse_iterator
  {
    return list_.rend();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::crbegin() const noexcept
    -> const_reverse_iterator
  {
    return list_.crbegin();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::crend() const noexcept
    -> const_reverse_iterator
  {
    return list_.crend();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::empty() const noexcept
    -> bool
  {
    return list_.empty();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::size() const noexcept
    -> size_type
  {
    return list_.size();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::max_size() const noexcept
    -> size_type
  {
    return list_.max_size();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::at(key_type const key) const
    -> const_reference
  {
    auto const it = find(key);
    if (it == list_.end()) {
      throw std::out_of_range{"not found specified value"};
    }
    return *it;
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::erase(const_iterator const pos)
    -> const_iterator
  {
    return list_.erase(pos);
  }

  template <class Derived, class Key, class T>
  void set_base<Derived, Key, T>::swap(set_base& other) noexcept
  {
    list_.swap(other.list_);
  }

  template <class Derived, class Key, class T>
  void set_base<Derived, Key, T>::clear() noexcept
  {
    list_.clear();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::find(key_type const key) const noexcept
    -> const_iterator
  {
    return boost::find_if(
          list_
        , [=](const_reference e) { return get_key(e) == key; });
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::length() const noexcept
    -> std::uint16_t
  {
    return list_.length();
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::to_list() const& noexcept
    -> list_type const&
  {
    return list_;
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::to_list() && noexcept
    -> list_type&&
  {
    return std::move(list_);
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::is_valid_set(list_type const& values)
    -> bool
  {
    return bool(typename Derived::set_info{values});
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::equivalent_as_set(
      list_type const& lhs, list_type const& rhs)
    -> bool
  {
    if (auto const lhs_info = typename Derived::set_info{lhs}) {
      if (auto const rhs_info = typename Derived::set_info{rhs}) {
        return lhs_info == rhs_info;
      }
    }
    return false;
  }

  template <class Derived, class Key, class T>
  class set_base<Derived, Key, T>::default_set_info
  {
    using set_type = boost::container::flat_map<key_type, const_iterator>;

  public:
    explicit default_set_info(list_type const& list)
      : value_set_{}
      , is_valid_set_{}
    {
      value_set_.reserve(list.size());
      auto const it_end = list.cend();
      for (auto it = list.cbegin(); it != it_end; ++it) {
        if (!value_set_.emplace(get_key(*it), it).second) {
          is_valid_set_ = false;
          return;
        }
      }
      is_valid_set_ = true;
    }

    explicit operator bool() const noexcept
    {
      return is_valid_set_;
    }

    friend auto operator==(
        default_set_info const& lhs, default_set_info const& rhs) noexcept
      -> bool
    {
      return boost::equal(
            lhs.value_set_ | boost::adaptors::map_values
          , rhs.value_set_ | boost::adaptors::map_values
          , [](const_iterator lhs_it, const_iterator rhs_it)
            { return equivalent(*lhs_it, *rhs_it); });
    }

  private:
    set_type value_set_;
    bool is_valid_set_;
  };

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::non_const_lower_bound(key_type const key)
    -> typename list_type::iterator
  {
    return boost::lower_bound(
          list_
        , key
        , [](const_reference e, key_type const key)
          { return Derived::get_key(e) < key; });
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::equal_impl(set_base const& rhs) const noexcept
    -> bool
  {
    return list_ == rhs.list_;
  }

  template <class Derived, class Key, class T>
  auto set_base<Derived, Key, T>::equivalent_impl(
      set_base const& rhs) const noexcept
    -> bool
  {
    return boost::equal(
          list_, rhs.list_
        , [](const_reference lhs_value, const_reference rhs_value)
          { return equivalent(lhs_value, rhs_value); });
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_IMPL_SET_BASE_HPP
