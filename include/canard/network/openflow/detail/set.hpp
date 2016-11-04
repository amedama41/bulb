#ifndef CANARD_NET_OFP_SET_HPP
#define CANARD_NET_OFP_SET_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <boost/container/flat_set.hpp>
#include <boost/operators.hpp>
#include <boost/optional/optional.hpp>
#include <boost/range/algorithm/adjacent_find.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/type_traits/is_all_constructible.hpp>

namespace canard {
namespace net {
namespace ofp {

  template <class KeyTraits>
  class set
    : private boost::equality_comparable<set<KeyTraits>>
  {
    struct dummy_type {};

  public:
    using list_type = ofp::list<typename KeyTraits::value_type>;
    using key_type = typename KeyTraits::key_type;
    using value_type = typename list_type::value_type;
    using reference = typename list_type::const_reference;
    using const_reference = typename list_type::const_reference;
    using iterator = typename list_type::const_iterator;
    using const_iterator = typename list_type::const_iterator;
    using difference_type = typename list_type::difference_type;
    using size_type = typename list_type::size_type;
    using reverse_iterator = typename list_type::reverse_iterator;
    using const_reverse_iterator = typename list_type::const_reverse_iterator;

  private:
    template <class... Types>
    using enable_if_is_all_constructible_t = typename std::enable_if<
      type_traits::is_all_constructible<value_type, Types...>::value
    >::type;

  public:
    set() = default;

    template <
        class... Types
      , class = enable_if_is_all_constructible_t<value_type, Types...>
    >
    set(Types&&... ts)
      : list_{}
    {
      dummy_type const unused[] = { init_impl(std::forward<Types>(ts))... };
      static_cast<void>(unused);
    }

    CANARD_NET_OFP_DECL auto begin() const noexcept
      -> const_iterator
    { return list_.begin(); }

    CANARD_NET_OFP_DECL auto end() const noexcept
      -> const_iterator
    { return list_.end(); }

    CANARD_NET_OFP_DECL auto cbegin() const noexcept
      -> const_iterator
    { return list_.cbegin(); }

    CANARD_NET_OFP_DECL auto cend() const noexcept
      -> const_iterator
    { return list_.cend(); }

    CANARD_NET_OFP_DECL auto rbegin() const noexcept
      -> const_reverse_iterator
    { return list_.rbegin(); }

    CANARD_NET_OFP_DECL auto rend() const noexcept
      -> const_reverse_iterator
    { return list_.rend(); }

    CANARD_NET_OFP_DECL auto crbegin() const noexcept
      -> const_reverse_iterator
    { return list_.crbegin(); }

    CANARD_NET_OFP_DECL auto crend() const noexcept
      -> const_reverse_iterator
    { return list_.crend(); }

    CANARD_NET_OFP_DECL auto empty() const noexcept
      -> bool
    { return list_.empty(); }

    CANARD_NET_OFP_DECL auto size() const noexcept
      -> size_type
    { return list_.size(); }

    CANARD_NET_OFP_DECL auto max_size() const noexcept
      -> size_type
    { return list_.max_size(); }

    CANARD_NET_OFP_DECL auto at(key_type const key) const
      -> const_reference
    {
      auto const it = find(key);
      if (it == list_.end()) {
        throw std::out_of_range{"not found specified value"};
      }
      return *it;
    }

    template <class T>
    auto at() const
      -> T const&
    { return any_cast<T>(at(KeyTraits::template key<T>::value)); }

    template <class T>
    auto get() const
      -> T const&
    { return any_cast<T>(*find(KeyTraits::template key<T>::value)); }

    template <class T>
    auto insert(T&& t)
      -> std::pair<const_iterator, bool>
    {
      auto const key = KeyTraits::get_key(t);
      auto const pos = non_const_lower_bound(key);
      if (pos != list_.end() && KeyTraits::get_key(*pos) == key) {
        return std::make_pair(const_iterator{pos}, false);
      }
      else {
        auto const it = list_.insert(pos, value_type{std::forward<T>(t)});
        return std::make_pair(const_iterator{it}, true);
      }
    }

    template <class T>
    auto assign(T&& t)
      -> std::pair<const_iterator, bool>
    {
      auto const key = KeyTraits::get_key(t);
      auto const pos = non_const_lower_bound(key);
      if (pos != list_.end() && KeyTraits::get_key(*pos) == key) {
        *pos = std::forward<T>(t);
        return std::make_pair(const_iterator{pos}, true);
      }
      else {
        return std::make_pair(const_iterator{list_.end()}, false);
      }
    }

    template <class T>
    auto insert_or_assign(T&& t)
      -> std::pair<const_iterator, bool>
    {
      auto const key = KeyTraits::get_key(t);
      auto const pos = non_const_lower_bound(key);
      if (pos != list_.end() && KeyTraits::get_key(*pos) == key) {
        *pos = std::forward<T>(t);
        return std::make_pair(const_iterator{pos}, false);
      }
      else {
        auto const it = list_.insert(pos, value_type{std::forward<T>(t)});
        return std::make_pair(const_iterator{it}, true);
      }
    }

    CANARD_NET_OFP_DECL auto erase(const_iterator pos)
      -> const_iterator
    { return list_.erase(pos); }

    template <class T>
    auto erase()
      -> size_type
    {
      auto const it = find(KeyTraits::template key<T>::value);
      if (it == list_.end()) {
        return 0;
      }
      list_.erase(it);
      return 1;
    }

    CANARD_NET_OFP_DECL void swap(set& other) noexcept
    { list_.swap(other.list_); }

    CANARD_NET_OFP_DECL void clear() noexcept
    { list_.clear(); }

    CANARD_NET_OFP_DECL auto find(key_type const key) const noexcept
      -> const_iterator
    {
      return boost::find_if(
            list_
          , [=](const_reference e) { return KeyTraits::get_key(e) == key; });
    }

    template <class T>
    auto find() const
      -> boost::optional<T const&>
    {
      auto const it = find(KeyTraits::template key<T>::value);
      if (it == list_.end()) {
        return boost::none;
      }
      return any_cast<T>(*it);
    }

    CANARD_NET_OFP_DECL auto length() const
      -> std::uint16_t
    { return list_.length(); }

    CANARD_NET_OFP_DECL auto to_list() const& noexcept
      -> list_type const&
    { return list_; }

    CANARD_NET_OFP_DECL auto to_list() && noexcept
      -> list_type&&
    { return std::move(list_); }

    friend auto operator==(set const& lhs, set const& rhs)
      -> bool
    { return lhs.equal_impl(rhs); }

    friend auto equivalent(set const& lhs, set const& rhs) noexcept
      -> bool
    { return lhs.equivalent_impl(rhs); }

    CANARD_NET_OFP_DECL static auto is_valid_set(list_type const& values)
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

    CANARD_NET_OFP_DECL static auto equivalent_as_set(
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

  private:
    template <class T>
    CANARD_NET_OFP_DECL auto init_impl(T&& t)
      -> dummy_type
    {
      insert(std::forward<T>(t));
      return {};
    }

    CANARD_NET_OFP_DECL auto non_const_lower_bound(key_type const key)
      -> typename list_type::iterator
    {
      return boost::lower_bound(
            list_
          , key
          , [](const_reference e, key_type const key)
            { return KeyTraits::get_key(e) < key; });
    }

    CANARD_NET_OFP_DECL auto equal_impl(set const& rhs) const noexcept
      -> bool
    { return list_ == rhs.list_; }

    CANARD_NET_OFP_DECL auto equivalent_impl(set const& rhs) const noexcept
      -> bool
    {
      return boost::equal(
            list_, rhs.list_
          , [](const_reference lhs_value, const_reference rhs_value)
            { return equivalent(lhs_value, rhs_value); });
    }

    template <class T>
    CANARD_NET_OFP_DECL static auto any_cast(const_reference value)
      -> T const&
    { return KeyTraits::template any_cast<T>(value); }

    class set_info
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
        { return std::addressof(v); }
      };

      static auto create_value_set(list_type const& list)
        -> set_type
      {
        auto const rng = list | boost::adaptors::transformed(to_pointer{});
        return set_type(rng.begin(), rng.end());
      }

      set_type value_set_;
    };

  private:
    list_type list_;
  };

} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_SET_HPP
