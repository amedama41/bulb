#ifndef CANARD_NET_OFP_V13_ERROR_CODE_HPP
#define CANARD_NET_OFP_V13_ERROR_CODE_HPP

#include <tuple>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>
#include <boost/fusion/support/pair.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  namespace error_code_detail {

    template <class ErrorCode, protocol::error_type ErrorType>
    using pair = boost::fusion::pair<
      ErrorCode, std::integral_constant<protocol::error_type, ErrorType>
    >;

    using error_code_map = boost::fusion::map<
        pair<protocol::hello_failed_code, protocol::error_type::hello_failed>
      , pair<protocol::bad_request_code, protocol::error_type::bad_request>
      , pair<protocol::bad_action_code, protocol::error_type::bad_action>
      , pair<protocol::bad_instruction_code, protocol::error_type::bad_instruction>
      , pair<protocol::bad_match_code, protocol::error_type::bad_match>
      , pair<protocol::flow_mod_failed_code, protocol::error_type::flow_mod_failed>
      , pair<protocol::group_mod_failed_code, protocol::error_type::group_mod_failed>
      , pair<protocol::port_mod_failed_code, protocol::error_type::port_mod_failed>
      , pair<protocol::table_mod_failed_code, protocol::error_type::table_mod_failed>
      , pair<protocol::queue_op_failed_code, protocol::error_type::queue_op_failed>
      , pair<protocol::switch_config_failed_code, protocol::error_type::switch_config_failed>
      , pair<protocol::role_request_failed_code, protocol::error_type::role_request_failed>
      , pair<protocol::meter_mod_failed_code, protocol::error_type::meter_mod_failed>
      , pair<protocol::table_features_failed_code, protocol::error_type::table_features_failed>
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
    static constexpr protocol::error_type error_type
      = error_code_detail::error_type<type>::value;
  };

  using default_error_code_list = std::tuple<
      error_code<protocol::hello_failed_code>
    , error_code<protocol::bad_request_code>
    , error_code<protocol::bad_action_code>
    , error_code<protocol::bad_instruction_code>
    , error_code<protocol::bad_match_code>
    , error_code<protocol::flow_mod_failed_code>
    , error_code<protocol::group_mod_failed_code>
    , error_code<protocol::port_mod_failed_code>
    , error_code<protocol::table_mod_failed_code>
    , error_code<protocol::queue_op_failed_code>
    , error_code<protocol::switch_config_failed_code>
    , error_code<protocol::role_request_failed_code>
    , error_code<protocol::meter_mod_failed_code>
    , error_code<protocol::table_features_failed_code>
  >;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ERROR_CODE_HPP
