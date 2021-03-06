#ifndef CANARD_NET_OFP_V13_MESSAGES_HPP
#define CANARD_NET_OFP_V13_MESSAGES_HPP

#include <tuple>
#include <canard/net/ofp/v13/message/hello.hpp>
#include <canard/net/ofp/v13/message/error.hpp>
#include <canard/net/ofp/v13/message/echo.hpp>
#include <canard/net/ofp/v13/message/table_mod.hpp>
#include <canard/net/ofp/v13/message/flow_mod.hpp>
#include <canard/net/ofp/v13/message/group_mod.hpp>
#include <canard/net/ofp/v13/message/port_mod.hpp>
#include <canard/net/ofp/v13/message/meter_mod.hpp>
#include <canard/net/ofp/v13/message/switch_features.hpp>
#include <canard/net/ofp/v13/message/switch_config.hpp>
#include <canard/net/ofp/v13/message/multipart_messages.hpp>
#include <canard/net/ofp/v13/message/packet_out.hpp>
#include <canard/net/ofp/v13/message/barrier.hpp>
#include <canard/net/ofp/v13/message/queue_config.hpp>
#include <canard/net/ofp/v13/message/role.hpp>
#include <canard/net/ofp/v13/message/async_config.hpp>
#include <canard/net/ofp/v13/message/packet_in.hpp>
#include <canard/net/ofp/v13/message/flow_removed.hpp>
#include <canard/net/ofp/v13/message/port_status.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  using default_switch_message_list = std::tuple<
      messages::error
    , messages::echo_request
    , messages::echo_reply
    , messages::features_reply
    , messages::get_config_reply
    , messages::packet_in
    , messages::flow_removed
    , messages::port_status
    , messages::barrier_reply
    , messages::queue_get_config_reply
    , messages::role_reply
    , messages::get_async_reply
  >;

  using default_multipart_reply_list = std::tuple<
      messages::multipart::description_reply
    , messages::multipart::flow_stats_reply
    , messages::multipart::aggregate_stats_reply
    , messages::multipart::table_stats_reply
    , messages::multipart::port_stats_reply
    , messages::multipart::queue_stats_reply
    , messages::multipart::group_stats_reply
    , messages::multipart::group_description_reply
    , messages::multipart::group_features_reply
    , messages::multipart::meter_stats_reply
    , messages::multipart::meter_config_reply
    , messages::multipart::meter_features_reply
    , messages::multipart::table_features_reply
    , messages::multipart::port_description_reply
  >;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_HPP
