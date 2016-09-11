#ifndef CANARD_NET_OFP_V13_IMPL_ANY_HELLO_ELEMENT_IPP
#define CANARD_NET_OFP_V13_IMPL_ANY_HELLO_ELEMENT_IPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/any_hello_element.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/openflow/detail/impl/any_hello_element.hpp>
#  include <canard/network/openflow/detail/impl/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class any_hello_element<ofp::v13::hello_element_decoder>;
  template class any_type<any_hello_element<ofp::v13::hello_element_decoder>>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_IMPL_ANY_HELLO_ELEMENT_IPP
