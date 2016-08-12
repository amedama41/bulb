#ifndef CANARD_NETWORK_OPENFLOW_V13_IMPL_ACTION_LIST_IPP
#define CANARD_NETWORK_OPENFLOW_V13_IMPL_ACTION_LIST_IPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/protocol/openflow/detail/impl/action_list.hpp>
#  include <canard/network/protocol/openflow/v13/action_list.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

  template class action_list<
    openflow::v13::any_action, openflow::v13::v13_detail::ofp_action_header
  >;

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

# endif
#endif

#endif // CANARD_NETWORK_OPENFLOW_V13_IMPL_ACTION_LIST_IPP
