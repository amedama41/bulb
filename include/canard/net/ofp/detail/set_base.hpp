#ifndef CANARD_NET_OFP_SET_BASE_HPP
#define CANARD_NET_OFP_SET_BASE_HPP

#include <canard/net/ofp/detail/config.hpp>

#include <cstdint>
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>
#include <boost/optional/optional.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/type_traits/is_all_constructible.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Derived, class Key, class T>
  class set_base
    : private boost::equality_comparable<Derived>
  {
  public:
    using list_type = ofp::list<T>;
    using key_type = Key;
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
    CANARD_NET_OFP_DECL set_base();

    template <
        class... Types
      , class = enable_if_is_all_constructible_t<value_type, Types...>
    >
    set_base(Types&&... ts)
      : list_{}
    {
      struct dummy_type {};
      dummy_type const unused[] = {
        (insert(std::forward<Types>(ts)), dummy_type{})...
      };
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

    template <class U>
    auto at() const
      -> U const&
    {
      return cast<U>(at(key<U>()));
    }

    template <class U>
    auto get() const
      -> U const&
    {
      return cast<U>(*find(key<U>()));
    }

    template <class U>
    auto insert(U&& u)
      -> std::pair<const_iterator, bool>
    {
      auto const key = get_key(u);
      auto const pos = non_const_lower_bound(key);
      if (pos != list_.end() && get_key(*pos) == key) {
        return std::make_pair(const_iterator{pos}, false);
      }
      else {
        auto const it = list_.insert(pos, value_type{std::forward<U>(u)});
        return std::make_pair(const_iterator{it}, true);
      }
    }

    template <class U>
    auto assign(U&& u)
      -> std::pair<const_iterator, bool>
    {
      auto const key = get_key(u);
      auto const pos = non_const_lower_bound(key);
      if (pos != list_.end() && get_key(*pos) == key) {
        *pos = std::forward<U>(u);
        return std::make_pair(const_iterator{pos}, true);
      }
      else {
        return std::make_pair(const_iterator{list_.end()}, false);
      }
    }

    template <class U>
    auto insert_or_assign(U&& u)
      -> std::pair<const_iterator, bool>
    {
      auto const key = get_key(u);
      auto const pos = non_const_lower_bound(key);
      if (pos != list_.end() && get_key(*pos) == key) {
        *pos = std::forward<U>(u);
        return std::make_pair(const_iterator{pos}, false);
      }
      else {
        auto const it = list_.insert(pos, value_type{std::forward<U>(u)});
        return std::make_pair(const_iterator{it}, true);
      }
    }

    CANARD_NET_OFP_DECL auto erase(const_iterator)
      -> const_iterator;

    template <class U>
    auto erase()
      -> size_type
    {
      auto const it = find(key<U>());
      if (it == list_.end()) {
        return 0;
      }
      list_.erase(it);
      return 1;
    }

    CANARD_NET_OFP_DECL void swap(set_base&) noexcept;

    CANARD_NET_OFP_DECL void clear() noexcept;

    CANARD_NET_OFP_DECL auto find(key_type) const noexcept
      -> const_iterator;

    template <class U>
    auto find() const
      -> boost::optional<U const&>
    {
      auto const it = find(key<U>());
      if (it == list_.end()) {
        return boost::none;
      }
      return cast<U>(*it);
    }

    CANARD_NET_OFP_DECL auto length() const noexcept
      -> std::uint16_t;

    CANARD_NET_OFP_DECL auto to_list() const& noexcept
      -> list_type const&;

    CANARD_NET_OFP_DECL auto to_list() && noexcept
      -> list_type&&;

    friend auto operator==(Derived const& lhs, Derived const& rhs)
      -> bool
    {
      return lhs.equal_impl(rhs);
    }

    friend auto equivalent(Derived const& lhs, Derived const& rhs) noexcept
      -> bool
    {
      return lhs.equivalent_impl(rhs);
    }

    CANARD_NET_OFP_DECL static auto is_valid_set(list_type const&)
      -> bool;

    CANARD_NET_OFP_DECL static auto equivalent_as_set(
        list_type const&, list_type const&)
      -> bool;

  protected:
    class default_set_info;

  private:
    CANARD_NET_OFP_DECL auto non_const_lower_bound(key_type)
      -> typename list_type::iterator;

    CANARD_NET_OFP_DECL auto equal_impl(set_base const&) const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto equivalent_impl(set_base const&) const noexcept
      -> bool;

    template <class U>
    static constexpr auto key() noexcept
      -> key_type
    {
      return Derived::template key_impl<U>();
    }

    template <class U>
    static auto get_key(U const& u) noexcept
      -> key_type
    {
      return Derived::get_key_impl(u);
    }

    template <class U>
    static auto cast(const_reference value)
      -> U const&
    {
      return Derived::template cast_impl<U>(value);
    }

  private:
    list_type list_;
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
# include <canard/net/ofp/detail/impl/set_base.hpp>
#endif

#endif // CANARD_NET_OFP_SET_BASE_HPP
