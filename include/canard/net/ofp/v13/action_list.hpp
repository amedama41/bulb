#ifndef CANARD_NET_OFP_V13_ACTION_LIST_HPP
#define CANARD_NET_OFP_V13_ACTION_LIST_HPP

#include <canard/net/ofp/detail/config.hpp>

#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/any_action.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  using action_list = ofp::list<any_action>;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {

  extern template class list<ofp::v13::any_action>;

} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_ACTION_LIST_HPP
