#ifndef CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP
#define CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/protocol/openflow/detail/impl/action_list.hpp>
#  include <canard/network/protocol/openflow/v13/action_list.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class action_list<
    ofp::v13::any_action, ofp::v13::v13_detail::ofp_action_header
  >;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_IMPL_ACTION_LIST_IPP
