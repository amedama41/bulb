#ifndef CANARD_NET_OFP_V10_IMPL_ANY_ACTION_IPP
#define CANARD_NET_OFP_V10_IMPL_ANY_ACTION_IPP

#include <canard/net/ofp/detail/config.hpp>
#include <canard/net/ofp/v10/any_action.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#include <canard/net/ofp/detail/impl/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class any_type<ofp::v10::action_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V10_IMPL_ANY_ACTION_IPP
