#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_MESSAGE_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_MESSAGE_HPP

#include <canard/net/ofp/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  using basic_fixed_length_message = ofp::detail::basic_fixed_length_message<
      T
    , ofp::v13::protocol::ofp_header
    , ofp::v13::protocol::ofp_type
    , ofp::v13::protocol::OFP_VERSION
  >;

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_MESSAGE_HPP
