#ifndef CANARD_NET_OFP_TYPE_TRAITS_CONJUCTION_HPP
#define CANARD_NET_OFP_TYPE_TRAITS_CONJUCTION_HPP

#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
namespace type_traits {

  template <class...>
  struct conjuction : std::true_type {};

  template <class B1, class... Bn>
  struct conjuction<B1, Bn...>
    : std::conditional<B1::value, conjuction<Bn...>, B1>::type
  {};

} // namespace type_traits
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_TYPE_TRAITS_CONJUCTION_HPP
