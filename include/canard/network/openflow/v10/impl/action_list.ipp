#ifndef CANARD_NET_OFP_V10_IMPL_ACTION_LIST_IPP
#define CANARD_NET_OFP_V10_IMPL_ACTION_LIST_IPP

#include <canard/network/openflow/detail/config.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/openflow/detail/impl/action_list.hpp>
#  include <canard/network/openflow/v10/action_list.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class action_list<
    ofp::v10::any_action, ofp::v10::v10_detail::ofp_action_header
  >;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V10_IMPL_ACTION_LIST_IPP