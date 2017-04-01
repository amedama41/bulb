#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_MESSAGE_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_MESSAGE_HPP

#include <canard/network/openflow/detail/basic_message.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  using basic_message_tag = ofp::detail::basic_message_tag<T>;

  template <class T>
  using basic_message = ofp::detail::basic_message<
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

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_MESSAGE_HPP
