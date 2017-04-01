#ifndef CANARD_NET_OFP_V10_MESSAGES_HPP
#define CANARD_NET_OFP_V10_MESSAGES_HPP

#include <tuple>
#include <canard/network/openflow/v10/message/barrier.hpp>
#include <canard/network/openflow/v10/message/echo.hpp>
#include <canard/network/openflow/v10/message/error.hpp>
#include <canard/network/openflow/v10/message/flow_add.hpp>
#include <canard/network/openflow/v10/message/flow_modify.hpp>
#include <canard/network/openflow/v10/message/flow_delete.hpp>
#include <canard/network/openflow/v10/message/flow_removed.hpp>
#include <canard/network/openflow/v10/message/packet_in.hpp>
#include <canard/network/openflow/v10/message/packet_out.hpp>
#include <canard/network/openflow/v10/message/port_mod.hpp>
#include <canard/network/openflow/v10/message/port_status.hpp>
#include <canard/network/openflow/v10/message/queue_config.hpp>
#include <canard/network/openflow/v10/message/stats/aggregate_stats.hpp>
#include <canard/network/openflow/v10/message/stats/description.hpp>
#include <canard/network/openflow/v10/message/stats/flow_stats.hpp>
#include <canard/network/openflow/v10/message/stats/port_stats.hpp>
#include <canard/network/openflow/v10/message/stats/queue_stats.hpp>
#include <canard/network/openflow/v10/message/stats/table_stats.hpp>
#include <canard/network/openflow/v10/message/switch_config.hpp>
#include <canard/network/openflow/v10/message/switch_features.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    using default_reply_list = std::tuple<
          messages::echo_reply
        , messages::features_reply
        , messages::get_config_reply
        , messages::barrier_reply
        , messages::queue_get_config_reply
    >;

    using default_async_list = std::tuple<
          messages::error
        , messages::packet_in
        , messages::flow_removed
        , messages::port_status
    >;

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
    >;

    using default_stats_reply_list = std::tuple<
          messages::statistics::description_reply
        , messages::statistics::flow_stats_reply
        , messages::statistics::aggregate_stats_reply
        , messages::statistics::table_stats_reply
        , messages::statistics::port_stats_reply
        , messages::statistics::queue_stats_reply
    >;

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_HPP
