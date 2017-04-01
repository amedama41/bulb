#ifndef CANARD_NET_OFP_TYPE_TRAITS_IS_ALL_CONSTRUCTIBLE_HPP
#define CANARD_NET_OFP_TYPE_TRAITS_IS_ALL_CONSTRUCTIBLE_HPP

#include <type_traits>
#include <canard/net/ofp/type_traits/conjuction.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace type_traits {

  template <class T, class... U>
  struct is_all_constructible
    : conjuction<std::is_constructible<T, U>...>
  {};

} // namespace type_traits
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_TYPE_TRAITS_IS_ALL_CONSTRUCTIBLE_HPP
