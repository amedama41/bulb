#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP

#include <canard/net/ofp/detail/basic_action.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  using basic_action = ofp::detail::basic_action<
      T
    , ofp::v13::protocol::ofp_action_header
    , ofp::v13::protocol::ofp_action_type
  >;

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP
