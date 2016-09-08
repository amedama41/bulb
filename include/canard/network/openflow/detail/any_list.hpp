#ifndef CANARD_NET_OFP_DETAIL_ANY_LIST_HPP
#define CANARD_NET_OFP_DETAIL_ANY_LIST_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include <boost/operators.hpp>
#include <boost/range/algorithm/for_each.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  namespace any_list_detail {

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

    template <class...>
    struct conjuction : std::true_type {};

    template <class B1, class... Bn>
    struct conjuction<B1, Bn...>
      : std::conditional<B1::value, conjuction<Bn...>, B1>::type
    {};

    template <class T, class... Args>
    using is_all_constructible_t = typename std::enable_if<
      conjuction<std::is_constructible<T, Args>...>::value
    >::type;

  } // namespace any_list_detail


  template <class AnyType>
  class any_list
    : private boost::equality_comparable<any_list<AnyType>>
  {
    using container = std::vector<AnyType>;

  public:
    using value_type = typename container::value_type;
    using reference = typename container::reference;
    using const_reference = typename container::const_reference;
    using iterator = typename container::iterator;
    using const_iterator = typename container::const_iterator;
    using difference_type = typename container::difference_type;
    using size_type = typename container::size_type;

    using reverse_iterator = typename container::reverse_iterator;
    using const_reverse_iterator = typename container::const_reverse_iterator;

    template <
        class... Args
      , class = any_list_detail::is_all_constructible_t<value_type, Args...>
    >
    any_list(Args&&... args)
      : values_{value_type(std::forward<Args>(args))...}
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

    CANARD_NET_OFP_DECL void swap(any_list&) noexcept;

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

    template <class T>
    void push_back(T&& t)
    {
      values_.emplace_back(std::forward<T>(t));
    }

    CANARD_NET_OFP_DECL void pop_back();

    template <class... Args>
    auto insert(const_iterator pos, Args&&... args)
      -> typename std::enable_if<
          !any_list_detail::is_input_iterator<Args...>::value, iterator
         >::type
    {
      return values_.insert(
          pos, { value_type(std::forward<Args>(args))... });
    }

    template <class InputIterator>
    auto insert(const_iterator pos, InputIterator first, InputIterator last)
      -> typename std::enable_if<
          any_list_detail::is_input_iterator<InputIterator>::value, iterator
         >::type
    {
      return values_.insert(pos, first, last);
    }

    CANARD_NET_OFP_DECL auto erase(const_iterator)
      -> iterator;

    CANARD_NET_OFP_DECL auto erase(const_iterator first, const_iterator last)
      -> iterator;

    CANARD_NET_OFP_DECL auto length() const
      -> std::size_t;

    template <class Container>
    auto encode(Container& container) const
      -> Container&
    {
      boost::for_each(
          values_, [&](value_type const& v) { v.encode(container); });
      return container;
    }

    template <class Iterator>
    static auto decode(Iterator& first, Iterator last)
      -> any_list
    {
      auto values = container{};
      using header_type = typename value_type::header_type;
      values.reserve(std::distance(first, last) / sizeof(header_type));
      while (std::distance(first, last) >= sizeof(header_type)) {
        values.push_back(value_type::decode(first, last));
      }
      if (first != last) {
        throw std::runtime_error{"list binary length is invalid"};
      }
      return any_list{std::move(values)};
    }

    template <class Validator>
    void validate(Validator validator_for_children) const
    {
      boost::for_each(values_, validator_for_children);
    }

    friend auto operator==(any_list const& lhs, any_list const& rhs) noexcept
      -> bool
    {
      return lhs.equal_impl(rhs);
    }

    friend auto equivalent(any_list const& lhs, any_list const& rhs) noexcept
      -> bool
    {
      return lhs.equivalent_impl(rhs);
    }

  private:
    CANARD_NET_OFP_DECL explicit any_list(container&&);

    CANARD_NET_OFP_DECL auto equal_impl(any_list const&) const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto equivalent_impl(any_list const&) const noexcept
      -> bool;

  private:
    container values_;
  };

  template <class AnyType>
  void swap(any_list<AnyType>& lhs, any_list<AnyType>& rhs) noexcept
  {
    lhs.swap(rhs);
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/openflow/detail/impl/any_list.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_LIST_HPP
