#ifndef CANARD_NET_OFP_LIST_HPP
#define CANARD_NET_OFP_LIST_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include <boost/operators.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/network/openflow/type_traits/is_all_constructible.hpp>
#include <canard/network/openflow/type_traits/is_iterator.hpp>

namespace canard {
namespace net {
namespace ofp {

  namespace list_detail {

    struct dummy_type {};

    template <class T, class U>
    using enable_if_is_initializer_list_t = typename std::enable_if<
      std::is_same<T, std::initializer_list<U>>::value
    >::type;

    template <class T, class... Us>
    using enable_if_is_all_constructible_t = typename std::enable_if<
      sizeof...(Us) && type_traits::is_all_constructible<T, Us...>::value
    >::type;

    template <class Iterator, class T = void>
    using enable_if_is_input_iterator_t = typename std::enable_if<
      type_traits::is_input_iterator<Iterator>::value, T
    >::type;

    template <class T>
    auto has_header_size_impl(T const&)
      -> decltype(T::header_size, std::true_type{});

    auto has_header_size_impl(...)
      -> std::false_type;

    template <class T>
    struct has_header_size
        : decltype(has_header_size_impl(std::declval<T>()))
    {};

    template <class ProtocolType, class = void>
    struct header_size_impl
    {
      static constexpr std::uint16_t value = ProtocolType::min_byte_length();
    };

    template <class ProtocolType>
    struct header_size_impl<
        ProtocolType
      , typename std::enable_if<has_header_size<ProtocolType>::value>::type
    >
    {
      static constexpr std::uint16_t value = ProtocolType::header_size;
    };

    template <class ProtocolType>
    constexpr auto header_size() noexcept
      -> std::uint16_t
    {
      return header_size_impl<ProtocolType>::value;
    }

  } // namespace list_detail

  template <class ProtocolType>
  class list
    : private boost::equality_comparable<list<ProtocolType>>
  {
    using container_type = std::vector<ProtocolType>;

  public:
    // types:
    using value_type = typename container_type::value_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using difference_type = typename container_type::difference_type;
    using size_type = typename container_type::size_type;

    using reverse_iterator = typename container_type::reverse_iterator;
    using const_reverse_iterator
        = typename container_type::const_reverse_iterator;

    // construct:
    list();

    CANARD_NET_OFP_DECL list(size_type, const_reference);

    template <
        class InputIterator
      , class = list_detail::enable_if_is_input_iterator_t<InputIterator>
    >
    list(InputIterator first, InputIterator last)
      : values_(first, last)
    {
    }

    template <
        class... Ts
      , class = list_detail::enable_if_is_all_constructible_t<value_type, Ts...>
    >
    list(Ts&&... ts)
      : values_{}
    {
      values_.reserve(sizeof...(ts));
      list_detail::dummy_type unused[]
        = { init_impl(value_type(std::forward<Ts>(ts)))... };
      static_cast<void>(unused);
    }

    template <
        class List
      , class = list_detail::enable_if_is_initializer_list_t<List, value_type>
    >
    list(List const init_list)
      : values_(init_list)
    {
    }

    CANARD_NET_OFP_DECL auto operator=(std::initializer_list<value_type>)
      -> list&;

    template <
        class InputIterator
      , class = list_detail::enable_if_is_input_iterator_t<InputIterator>
    >
    void assign(InputIterator first, InputIterator last)
    {
      values_.assign(first, last);
    }

    CANARD_NET_OFP_DECL void assign(size_type, const_reference);

    CANARD_NET_OFP_DECL void assign(std::initializer_list<value_type>);

    template <
        class... Ts
      , class = list_detail::enable_if_is_all_constructible_t<value_type, Ts...>
    >
    void assign(Ts&&... ts)
    {
      value_type init_list[] = { value_type(std::forward<Ts>(ts))... };
      return assign_impl(std::begin(init_list), std::end(init_list));
    }

    // iterators:
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

    // capacity:
    CANARD_NET_OFP_DECL auto size() const noexcept
      -> size_type;

    CANARD_NET_OFP_DECL auto max_size() const noexcept
      -> size_type;

    CANARD_NET_OFP_DECL auto empty() const noexcept
      -> bool;

    CANARD_NET_OFP_DECL void reserve(size_type);

    CANARD_NET_OFP_DECL void shrink_to_fit();

    // element access:
    CANARD_NET_OFP_DECL auto operator[](size_type)
      -> reference;

    CANARD_NET_OFP_DECL auto operator[](size_type) const
      -> const_reference;

    CANARD_NET_OFP_DECL auto at(size_type)
      -> reference;

    CANARD_NET_OFP_DECL auto at(size_type) const
      -> const_reference;

    CANARD_NET_OFP_DECL auto front()
      -> reference;

    CANARD_NET_OFP_DECL auto front() const
      -> const_reference;

    CANARD_NET_OFP_DECL auto back()
      -> reference;

    CANARD_NET_OFP_DECL auto back() const
      -> const_reference;

    // modifiers:
    template <class... Args>
    void emplace_back(Args&&... args)
    {
      values_.emplace_back(std::forward<Args>(args)...);
    }

    CANARD_NET_OFP_DECL void push_back(value_type const&);

    CANARD_NET_OFP_DECL void push_back(value_type&&);

    CANARD_NET_OFP_DECL void pop_back();

    template <class... Args>
    auto emplace(const_iterator const pos, Args&&... args)
      -> iterator
    {
      return values_.emplace(pos, std::forward<Args>(args)...);
    }

    template <
        class... Ts
      , class = list_detail::enable_if_is_all_constructible_t<value_type, Ts...>
    >
    auto insert(const_iterator const pos, Ts&&... ts)
      -> iterator
    {
      value_type init_list[] = { value_type(std::forward<Ts>(ts))... };
      return insert_impl(pos, std::begin(init_list), std::end(init_list));
    }

    CANARD_NET_OFP_DECL auto insert(const_iterator, size_type, const_reference)
      -> iterator;

    template <class InputIterator>
    auto insert(
          const_iterator const pos, InputIterator first, InputIterator last)
      -> list_detail::enable_if_is_input_iterator_t<InputIterator, iterator>
    {
      return values_.insert(pos, first, last);
    }

    CANARD_NET_OFP_DECL auto insert(
        const_iterator, std::initializer_list<value_type>)
      -> iterator;

    CANARD_NET_OFP_DECL auto erase(const_iterator)
      -> iterator;

    CANARD_NET_OFP_DECL auto erase(const_iterator, const_iterator)
      -> iterator;

    CANARD_NET_OFP_DECL void swap(list&) noexcept;

    CANARD_NET_OFP_DECL void clear() noexcept;

    // OFP operations:
    CANARD_NET_OFP_DECL auto calc_ofp_length(std::uint16_t const) const
      -> std::uint16_t;

    CANARD_NET_OFP_DECL auto length() const noexcept
      -> std::size_t;

    CANARD_NET_OFP_DECL auto byte_length() const noexcept
      -> std::size_t;

    template <class Validator>
    void validate(Validator validator_for_children) const
    {
      boost::for_each(values_, validator_for_children);
    }

    template <class Container>
    auto encode(Container& container) const
      -> Container&
    {
      boost::for_each(values_, [&](const_reference v) { v.encode(container); });
      return container;
    }

    template <class Iterator>
    static auto decode(Iterator& first, Iterator last)
      -> list
    {
      auto values = container_type{};
      values.reserve(
          std::distance(first, last) / value_type::min_byte_length());
      while (std::distance(first, last)
          >= list_detail::header_size<value_type>()) {
        values.push_back(value_type::decode(first, last));
      }
      if (first != last) {
        throw std::runtime_error{"list byte length is invalid"};
      }
      return list{std::move(values)};
    }

    friend auto operator==(list const& lhs, list const& rhs) noexcept
      -> bool
    {
      return lhs.equal_impl(rhs);
    }

    friend auto equivalent(list const& lhs, list const& rhs) noexcept
      -> bool
    {
      return lhs.equivalent_impl(rhs);
    }

  private:
    CANARD_NET_OFP_DECL explicit list(container_type&&);

    CANARD_NET_OFP_DECL auto init_impl(value_type&&)
      -> list_detail::dummy_type;

    CANARD_NET_OFP_DECL void assign_impl(value_type*, value_type*);

    CANARD_NET_OFP_DECL auto insert_impl(
        const_iterator, value_type*, value_type*)
      -> iterator;

    CANARD_NET_OFP_DECL auto length_impl() const noexcept
      -> std::size_t;

    CANARD_NET_OFP_DECL auto equal_impl(list const&) const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto equivalent_impl(list const&) const noexcept
      -> bool;

  private:
    container_type values_;
  };

  template <class ProtocolType>
  void swap(list<ProtocolType>& lhs, list<ProtocolType>& rhs) noexcept
  {
    lhs.swap(rhs);
  }

} // namespace ofp
} // namespace net
} // namespace canard

#include <canard/network/openflow/detail/impl/list.hpp>

#endif // CANARD_NET_OFP_LIST_HPP
