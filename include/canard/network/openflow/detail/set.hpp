#ifndef CANARD_NET_OFP_SET_HPP
#define CANARD_NET_OFP_SET_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>
#include <boost/optional/optional.hpp>
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
    CANARD_NET_OFP_DECL set();

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

    CANARD_NET_OFP_DECL auto at(key_type) const
      -> const_reference;

    template <class T>
    auto at() const
      -> T const&
    {
      return any_cast<T>(at(KeyTraits::template key<T>::value));
    }

    template <class T>
    auto get() const
      -> T const&
    {
      return any_cast<T>(*find(KeyTraits::template key<T>::value));
    }

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

    CANARD_NET_OFP_DECL auto erase(const_iterator)
      -> const_iterator;

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

    CANARD_NET_OFP_DECL void swap(set&) noexcept;

    CANARD_NET_OFP_DECL void clear() noexcept;

    CANARD_NET_OFP_DECL auto find(key_type) const noexcept
      -> const_iterator;

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
      -> std::uint16_t;

    CANARD_NET_OFP_DECL auto to_list() const& noexcept
      -> list_type const&;

    CANARD_NET_OFP_DECL auto to_list() && noexcept
      -> list_type&&;

    friend auto operator==(set const& lhs, set const& rhs)
      -> bool
    {
      return lhs.equal_impl(rhs);
    }

    friend auto equivalent(set const& lhs, set const& rhs) noexcept
      -> bool
    {
      return lhs.equivalent_impl(rhs);
    }

    CANARD_NET_OFP_DECL static auto is_valid_set(list_type const&)
      -> bool;

    CANARD_NET_OFP_DECL static auto equivalent_as_set(
        list_type const&, list_type const&)
      -> bool;

  private:
    class set_info;

    template <class T>
    auto init_impl(T&& t)
      -> dummy_type
    {
      insert(std::forward<T>(t));
      return {};
    }

    CANARD_NET_OFP_DECL auto non_const_lower_bound(key_type)
      -> typename list_type::iterator;

    CANARD_NET_OFP_DECL auto equal_impl(set const&) const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto equivalent_impl(set const&) const noexcept
      -> bool;

    template <class T>
    static auto any_cast(const_reference value)
      -> T const&
    {
      return KeyTraits::template any_cast<T>(value);
    }

  private:
    list_type list_;
  };

} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
# include <canard/network/openflow/detail/impl/set.hpp>
#endif

#endif // CANARD_NET_OFP_SET_HPP
