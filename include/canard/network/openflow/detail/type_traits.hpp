#ifndef CANARD_NET_OFP_TYPE_TRAITS_HPP
#define CANARD_NET_OFP_TYPE_TRAITS_HPP

#include <iterator>
#include <type_traits>
#include <utility>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  namespace type_traits_detail {

    template <class T>
    auto is_input_iterator_impl(T const&)
      -> std::is_base_of<
            std::input_iterator_tag
          , typename std::iterator_traits<T>::iterator_category
         >;

    auto is_input_iterator_impl(...)
      -> std::false_type;

  } // namespace type_traits_detail

  template <class...>
  struct is_input_iterator : std::false_type {};

  template <class T>
  struct is_input_iterator<T>
    : decltype(type_traits_detail::is_input_iterator_impl(std::declval<T>()))
  {};

  template <class Iterator, class T = void>
  using enable_if_is_input_iterator_t
    = typename std::enable_if<is_input_iterator<Iterator>::value, T>::type;

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

  template <class T, class... Args>
  using enable_if_is_all_constructible_t = typename std::enable_if<
    sizeof...(Args) && conjuction<std::is_constructible<T, Args>...>::value
  >::type;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_TYPE_TRAITS_HPP
