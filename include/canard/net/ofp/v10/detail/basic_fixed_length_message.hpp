#ifndef CANARD_NET_OFP_V10_BASIC_FIXED_LENGTH_MESSAGE_HPP
#define CANARD_NET_OFP_V10_BASIC_FIXED_LENGTH_MESSAGE_HPP

#include <canard/net/ofp/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace v10_detail {

  template <class T>
  using basic_fixed_length_message = ofp::detail::basic_fixed_length_message<
      T
    , ofp::v10::protocol::ofp_header
    , ofp::v10::protocol::ofp_type
    , ofp::v10::protocol::OFP_VERSION
  >;

} // namespace v10_detail
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_BASIC_FIXED_LENGTH_MESSAGE_HPP
