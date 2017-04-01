#ifndef CANARD_NET_OFP_V10_BYTEORDER_CONVERSION_HPP
#define CANARD_NET_OFP_V10_BYTEORDER_CONVERSION_HPP

#include <boost/endian/conversion.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <canard/net/ofp/v10/detail/fusion_adaptor.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    namespace protocol {

        struct generic_endian_reverse_inplace
        {
            template <class Field>
            void operator()(Field& field) const
            {
                using boost::endian::endian_reverse_inplace;
                endian_reverse_inplace(field);
            }

            template <class T, std::size_t N>
            void operator()(T (&field)[N]) const
            {
                using boost::endian::endian_reverse_inplace;
                for (T& e : field) { endian_reverse_inplace(e); }
            }

            template <std::size_t N>
            void operator()(char (&)[N]) const {}

            template <std::size_t N>
            void operator()(std::uint8_t (&)[N]) const {}
        };

        inline void endian_reverse_inplace(ofp_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_header data) noexcept
            -> ofp_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_hello& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_hello data) noexcept
            -> ofp_hello
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_switch_config& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_switch_config data) noexcept
            -> ofp_switch_config
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_phy_port& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_phy_port data) noexcept
            -> ofp_phy_port
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_switch_features& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_switch_features data) noexcept
            -> ofp_switch_features
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port_status& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port_status data) noexcept
            -> ofp_port_status
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port_mod& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port_mod data) noexcept
            -> ofp_port_mod
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_packet_in& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_packet_in data) noexcept
            -> ofp_packet_in
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_output& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_output data) noexcept
            -> ofp_action_output
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_vlan_vid& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_vlan_vid data) noexcept
            -> ofp_action_vlan_vid
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_vlan_pcp& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_vlan_pcp data) noexcept
            -> ofp_action_vlan_pcp
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_dl_addr& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_dl_addr data) noexcept
            -> ofp_action_dl_addr
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_nw_addr& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_nw_addr data) noexcept
            -> ofp_action_nw_addr
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_tp_port& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_tp_port data) noexcept
            -> ofp_action_tp_port
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_nw_tos& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_nw_tos data) noexcept
            -> ofp_action_nw_tos
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_vendor_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_vendor_header data) noexcept
            -> ofp_action_vendor_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_header data) noexcept
            -> ofp_action_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_packet_out& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_packet_out data) noexcept
            -> ofp_packet_out
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_match& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_match data) noexcept
            -> ofp_match
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_flow_mod& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_flow_mod data) noexcept
            -> ofp_flow_mod
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_flow_removed& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_flow_removed data) noexcept
            -> ofp_flow_removed
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_error_msg& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_error_msg data) noexcept
            -> ofp_error_msg
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_stats_request data) noexcept
            -> ofp_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_stats_reply& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_stats_reply data) noexcept
            -> ofp_stats_reply
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_desc_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_desc_stats data) noexcept
            -> ofp_desc_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_flow_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_flow_stats_request data) noexcept
            -> ofp_flow_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_flow_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_flow_stats data) noexcept
            -> ofp_flow_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_aggregate_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_aggregate_stats_request data) noexcept
            -> ofp_aggregate_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_aggregate_stats_reply& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_aggregate_stats_reply data) noexcept
            -> ofp_aggregate_stats_reply
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_stats data) noexcept
            -> ofp_table_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port_stats_request data) noexcept
            -> ofp_port_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port_stats data) noexcept
            -> ofp_port_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_vendor_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_vendor_header data) noexcept
            -> ofp_vendor_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_prop_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_prop_header data) noexcept
            -> ofp_queue_prop_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_prop_min_rate& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_prop_min_rate data) noexcept
            -> ofp_queue_prop_min_rate
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_packet_queue& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_packet_queue data) noexcept
            -> ofp_packet_queue
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_get_config_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_get_config_request data) noexcept
            -> ofp_queue_get_config_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_get_config_reply& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_get_config_reply data) noexcept
            -> ofp_queue_get_config_reply
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_enqueue& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_enqueue data) noexcept
            -> ofp_action_enqueue
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_stats_request data) noexcept
            -> ofp_queue_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_stats data) noexcept
            -> ofp_queue_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

    } // namespace protocol

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_BYTEORDER_CONVERSION_HPP
