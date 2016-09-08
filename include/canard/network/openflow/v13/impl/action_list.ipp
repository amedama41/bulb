#ifndef CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP
#define CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP

#include <canard/network/openflow/detail/config.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/openflow/detail/impl/any_list.hpp>
#  include <canard/network/openflow/v13/action_list.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class any_list<ofp::v13::any_action>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP
