#ifndef CANARD_NET_OFP_IMPL_SET_HPP
#define CANARD_NET_OFP_IMPL_SET_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/set.hpp>

#include <memory>
#include <stdexcept>
#include <utility>
#include <boost/container/flat_set.hpp>
#include <boost/range/algorithm/adjacent_find.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace canard {
namespace net {
namespace ofp {

  template <class KeyTraits>
  set<KeyTraits>::set() = default;

  template <class KeyTraits>
  auto set<KeyTraits>::begin() const noexcept
    -> const_iterator
  {
    return list_.begin();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::end() const noexcept
    -> const_iterator
  {
    return list_.end();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::cbegin() const noexcept
    -> const_iterator
  {
    return list_.cbegin();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::cend() const noexcept
    -> const_iterator
  {
    return list_.cend();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::rbegin() const noexcept
    -> const_reverse_iterator
  {
    return list_.rbegin();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::rend() const noexcept
    -> const_reverse_iterator
  {
    return list_.rend();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::crbegin() const noexcept
    -> const_reverse_iterator
  {
    return list_.crbegin();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::crend() const noexcept
    -> const_reverse_iterator
  {
    return list_.crend();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::empty() const noexcept
    -> bool
  {
    return list_.empty();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::size() const noexcept
    -> size_type
  {
    return list_.size();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::max_size() const noexcept
    -> size_type
  {
    return list_.max_size();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::at(key_type const key) const
    -> const_reference
  {
    auto const it = find(key);
    if (it == list_.end()) {
      throw std::out_of_range{"not found specified value"};
    }
    return *it;
  }

  template <class KeyTraits>
  auto set<KeyTraits>::erase(const_iterator const pos)
    -> const_iterator
  {
    return list_.erase(pos);
  }

  template <class KeyTraits>
  void set<KeyTraits>::swap(set& other) noexcept
  {
    list_.swap(other.list_);
  }

  template <class KeyTraits>
  void set<KeyTraits>::clear() noexcept
  {
    list_.clear();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::find(key_type const key) const noexcept
    -> const_iterator
  {
    return boost::find_if(
          list_
        , [=](const_reference e) { return KeyTraits::get_key(e) == key; });
  }

  template <class KeyTraits>
  auto set<KeyTraits>::length() const
    -> std::uint16_t
  {
    return list_.length();
  }

  template <class KeyTraits>
  auto set<KeyTraits>::to_list() const& noexcept
    -> list_type const&
  {
    return list_;
  }

  template <class KeyTraits>
  auto set<KeyTraits>::to_list() && noexcept
    -> list_type&&
  {
    return std::move(list_);
  }

  template <class KeyTraits>
  auto set<KeyTraits>::is_valid_set(list_type const& values)
    -> bool
  {
    auto key_set = boost::container::flat_set<key_type>{};
    key_set.reserve(values.size());
    for (auto const& v : values) {
      auto const result = key_set.insert(KeyTraits::get_key(v));
      if (!result.second) {
        return false;
      }
    }
    return true;
  }

  template <class KeyTraits>
  auto set<KeyTraits>::equivalent_as_set(
      list_type const& lhs, list_type const& rhs)
    -> bool
  {
    if (auto const lhs_info = set_info{lhs}) {
      if (auto const rhs_info = set_info{rhs}) {
        return lhs_info == rhs_info;
      }
    }
    return false;
  }

  template <class KeyTraits>
  class set<KeyTraits>::set_info
  {
    struct compare
    {
      auto operator()(
          value_type const* lhs, value_type const* rhs) const noexcept
        -> bool
      { return KeyTraits::get_key(*lhs) < KeyTraits::get_key(*rhs); }
    };

    using set_type
      = boost::container::flat_multiset<value_type const*, compare>;

  public:
    explicit set_info(list_type const& list)
      : value_set_(create_value_set(list))
    {
    }

    explicit operator bool() const noexcept
    {
      auto const it = boost::adjacent_find(
            value_set_
          , [](value_type const* lhs, value_type const* rhs)
            { return KeyTraits::get_key(*lhs) == KeyTraits::get_key(*rhs); });
      return it == value_set_.end();
    }

    friend auto operator==(set_info const& lhs, set_info const& rhs) noexcept
      -> bool
    {
      return boost::equal(
            lhs.value_set_, rhs.value_set_
          , [](value_type const* lhs_it, value_type const* rhs_it)
            { return equivalent(*lhs_it, *rhs_it); });
    }

  private:
    struct to_pointer
    {
      auto operator()(value_type const& v) const noexcept
        -> value_type const*
      {
        return std::addressof(v);
      }
    };

    static auto create_value_set(list_type const& list)
      -> set_type
    {
      auto const rng = list | boost::adaptors::transformed(to_pointer{});
      return set_type(rng.begin(), rng.end());
    }

    set_type value_set_;
  };

  template <class KeyTraits>
  auto set<KeyTraits>::non_const_lower_bound(key_type const key)
    -> typename list_type::iterator
  {
    return boost::lower_bound(
          list_
        , key
        , [](const_reference e, key_type const key)
          { return KeyTraits::get_key(e) < key; });
  }

  template <class KeyTraits>
  auto set<KeyTraits>::equal_impl(set const& rhs) const noexcept
    -> bool
  {
    return list_ == rhs.list_;
  }

  template <class KeyTraits>
  auto set<KeyTraits>::equivalent_impl(set const& rhs) const noexcept
    -> bool
  {
    return boost::equal(
          list_, rhs.list_
        , [](const_reference lhs_value, const_reference rhs_value)
          { return equivalent(lhs_value, rhs_value); });
  }

} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_IMPL_SET_HPP
