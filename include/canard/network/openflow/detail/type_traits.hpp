#ifndef CANARD_NET_OFP_TYPE_TRAITS_HPP
#define CANARD_NET_OFP_TYPE_TRAITS_HPP

#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

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

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_TYPE_TRAITS_HPP
