#ifndef CANARD_NET_OFP_TYPE_TRAITS_IS_ITERATOR_HPP
#define CANARD_NET_OFP_TYPE_TRAITS_IS_ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include <utility>

namespace canard {
namespace net {
namespace ofp {
namespace type_traits {

  namespace is_iterator_detail {

    template <class T>
    auto is_input_iterator_impl(T const&)
      -> std::is_base_of<
            std::input_iterator_tag
          , typename std::iterator_traits<T>::iterator_category
         >;

    auto is_input_iterator_impl(...)
      -> std::false_type;

  } // namespace is_iterator_detail

  template <class T>
  struct is_input_iterator
    : decltype(is_iterator_detail::is_input_iterator_impl(std::declval<T>()))
  {};

} // namespace type_traits
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_TYPE_TRAITS_IS_ITERATOR_HPP
