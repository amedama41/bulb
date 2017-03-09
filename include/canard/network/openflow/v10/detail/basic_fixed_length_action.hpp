#ifndef CANARD_NET_OFP_DETAIL_V10_BASIC_FIXED_LENGTH_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_V10_BASIC_FIXED_LENGTH_ACTION_HPP

#include <canard/network/openflow/detail/basic_fixed_length_action.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v10 {

  template <class T>
  using basic_fixed_length_action = ofp::detail::basic_fixed_length_action<
      T
    , ofp::v10::protocol::ofp_action_header
    , ofp::v10::protocol::ofp_action_type
  >;

} // namespace v10
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V10_BASIC_FIXED_LENGTH_ACTION_HPP
