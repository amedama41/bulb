#ifndef CANARD_NET_OFP_V10_ACTION_LIST_HPP
#define CANARD_NET_OFP_V10_ACTION_LIST_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/v10/any_action.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  using action_list = ofp::list<any_action>;

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {

  extern template class list<ofp::v10::any_action>;

} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V10_ACTION_LIST_HPP
