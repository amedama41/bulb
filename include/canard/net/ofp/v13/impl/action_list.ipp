#ifndef CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP
#define CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP

#include <canard/net/ofp/detail/config.hpp>
#include <canard/net/ofp/v13/action_list.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/net/ofp/detail/impl/list.hpp>

namespace canard {
namespace net {
namespace ofp {

  template class list<ofp::v13::any_action>;

} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP
