#ifndef CANARD_NET_OFP_V10_ERROR_CODE_HPP
#define CANARD_NET_OFP_V10_ERROR_CODE_HPP

#include <tuple>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>
#include <boost/fusion/support/pair.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  namespace error_code_detail {

    template <class ErrorCode, protocol::ofp_error_type ErrorType>
    using pair = boost::fusion::pair<
      ErrorCode, std::integral_constant<protocol::ofp_error_type, ErrorType>
    >;

    using error_code_map = boost::fusion::map<
        pair<protocol::ofp_hello_failed_code, protocol::OFPET_HELLO_FAILED>
      , pair<protocol::ofp_bad_request_code, protocol::OFPET_BAD_REQUEST>
      , pair<protocol::ofp_bad_action_code, protocol::OFPET_BAD_ACTION>
      , pair<protocol::ofp_flow_mod_failed_code, protocol::OFPET_FLOW_MOD_FAILED>
      , pair<protocol::ofp_port_mod_failed_code, protocol::OFPET_PORT_MOD_FAILED>
      , pair<protocol::ofp_queue_op_failed_code, protocol::OFPET_QUEUE_OP_FAILED>
    >;

    template <class ErrorCode>
    using error_type = typename boost::fusion::result_of::value_at_key<
      error_code_map, ErrorCode
    >::type;

  } // namespace error_code_detail


  template <class ErrorCodeType>
  struct error_code
  {
    using type = ErrorCodeType;
    static constexpr protocol::ofp_error_type error_type
      = error_code_detail::error_type<type>::value;
  };

  using default_error_code_list = std::tuple<
      error_code<protocol::ofp_hello_failed_code>
    , error_code<protocol::ofp_bad_request_code>
    , error_code<protocol::ofp_bad_action_code>
    , error_code<protocol::ofp_flow_mod_failed_code>
    , error_code<protocol::ofp_port_mod_failed_code>
    , error_code<protocol::ofp_queue_op_failed_code>
  >;

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ERROR_CODE_HPP
