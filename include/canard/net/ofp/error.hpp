#ifndef CANARD_NET_OFP_ERROR_HPP
#define CANARD_NET_OFP_ERROR_HPP

#include <canard/net/ofp/v13/openflow.hpp>
#include <canard/net/ofp/v13/message/error.hpp>

namespace canard {
namespace net {
namespace ofp {

  using error = v13::messages::error;

  constexpr auto OFPET_HELLO_FAILED = v13::protocol::OFPET_HELLO_FAILED;

  constexpr auto OFPHFC_INCOMPATIBLE = v13::protocol::OFPHFC_INCOMPATIBLE;
  constexpr auto OFPHFC_EPERM = v13::protocol::OFPHFC_EPERM;

  using hello_failed_code = v13::protocol::hello_failed_code;

} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_ERROR_HPP
