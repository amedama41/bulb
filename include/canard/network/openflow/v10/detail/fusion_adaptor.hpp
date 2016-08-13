#ifndef CANARD_NET_OFP_V10_FUSION_ADATOR_HPP
#define CANARD_NET_OFP_V10_FUSION_ADATOR_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_header,
    (std::uint8_t, version)
    (std::uint8_t, type)
    (std::uint16_t, length)
    (std::uint32_t, xid)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_hello,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_switch_config,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint16_t, flags)
    (std::uint16_t, miss_send_len)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_phy_port,
    (std::uint16_t, port_no)
    (std::uint8_t, hw_addr[0])
    (std::uint8_t, hw_addr[1])
    (std::uint8_t, hw_addr[2])
    (std::uint8_t, hw_addr[3])
    (std::uint8_t, hw_addr[4])
    (std::uint8_t, hw_addr[5])
    (std::uint32_t, config)
    (std::uint32_t, state)
    (std::uint32_t, curr)
    (std::uint32_t, advertised)
    (std::uint32_t, supported)
    (std::uint32_t, peer)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_switch_features,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint64_t, datapath_id)
    (std::uint32_t, n_buffers)
    (std::uint8_t, n_tables)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint32_t, capabilities)
    (std::uint32_t, actions)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_port_status,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint8_t, reason)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
    (std::uint8_t, pad[6])
    (canard::net::ofp::v10::v10_detail::ofp_phy_port, desc)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_port_mod,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint16_t, port_no)
    (std::uint8_t, hw_addr[0])
    (std::uint8_t, hw_addr[1])
    (std::uint8_t, hw_addr[2])
    (std::uint8_t, hw_addr[3])
    (std::uint8_t, hw_addr[4])
    (std::uint8_t, hw_addr[5])
    (std::uint32_t, config)
    (std::uint32_t, mask)
    (std::uint32_t, advertise)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_packet_in,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint32_t, buffer_id)
    (std::uint16_t, total_len)
    (std::uint16_t, in_port)
    (std::uint8_t, reason)
    (std::uint8_t, pad)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_output,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint16_t, port)
    (std::uint16_t, max_len)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_vlan_vid,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint16_t, vlan_vid)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_vlan_pcp,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, vlan_pcp)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_dl_addr,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, dl_addr[0])
    (std::uint8_t, dl_addr[1])
    (std::uint8_t, dl_addr[2])
    (std::uint8_t, dl_addr[3])
    (std::uint8_t, dl_addr[4])
    (std::uint8_t, dl_addr[5])
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_nw_addr,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, nw_addr)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_tp_port,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint16_t, tp_port)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_nw_tos,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, nw_tos)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_vendor_header,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, vendor)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_header,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_packet_out,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint32_t, buffer_id)
    (std::uint16_t, in_port)
    (std::uint16_t, actions_len)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_match,
    (std::uint32_t, wildcards)
    (std::uint16_t, in_port)
    (std::uint8_t, dl_src[0])
    (std::uint8_t, dl_src[1])
    (std::uint8_t, dl_src[2])
    (std::uint8_t, dl_src[3])
    (std::uint8_t, dl_src[4])
    (std::uint8_t, dl_src[5])
    (std::uint8_t, dl_dst[0])
    (std::uint8_t, dl_dst[1])
    (std::uint8_t, dl_dst[2])
    (std::uint8_t, dl_dst[3])
    (std::uint8_t, dl_dst[4])
    (std::uint8_t, dl_dst[5])
    (std::uint16_t, dl_vlan)
    (std::uint8_t, dl_vlan_pcp)
    (std::uint8_t, pad1[0])
    (std::uint16_t, dl_type)
    (std::uint8_t, nw_tos)
    (std::uint8_t, nw_proto)
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint32_t, nw_src)
    (std::uint32_t, nw_dst)
    (std::uint16_t, tp_src)
    (std::uint16_t, tp_dst)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_flow_mod,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (canard::net::ofp::v10::v10_detail::ofp_match, match)
    (std::uint64_t, cookie)
    (std::uint16_t, command)
    (std::uint16_t, idle_timeout)
    (std::uint16_t, hard_timeout)
    (std::uint16_t, priority)
    (std::uint32_t, buffer_id)
    (std::uint16_t, out_port)
    (std::uint16_t, flags)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_flow_removed,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (canard::net::ofp::v10::v10_detail::ofp_match, match)
    (std::uint64_t, cookie)
    (std::uint16_t, priority)
    (std::uint8_t, reason)
    (std::uint8_t, pad[0])
    (std::uint32_t, duration_sec)
    (std::uint32_t, duration_nsec)
    (std::uint16_t, idle_timeout)
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint64_t, packet_count)
    (std::uint64_t, byte_count)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_error_msg,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint16_t, type)
    (std::uint16_t, code)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_stats_request,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint16_t, type)
    (std::uint16_t, flags)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_stats_reply,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint16_t, type)
    (std::uint16_t, flags)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_desc_stats,

)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_flow_stats_request,
    (canard::net::ofp::v10::v10_detail::ofp_match, match)
    (std::uint8_t, table_id)
    (std::uint8_t, pad)
    (std::uint16_t, out_port)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_flow_stats,
    (std::uint16_t, length)
    (std::uint8_t, table_id)
    (std::uint8_t, pad)
    (canard::net::ofp::v10::v10_detail::ofp_match, match)
    (std::uint32_t, duration_sec)
    (std::uint32_t, duration_nsec)
    (std::uint16_t, priority)
    (std::uint16_t, idle_timeout)
    (std::uint16_t, hard_timeout)
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint8_t, pad2[2])
    (std::uint8_t, pad2[3])
    (std::uint8_t, pad2[4])
    (std::uint8_t, pad2[5])
    (std::uint64_t, cookie)
    (std::uint64_t, packet_count)
    (std::uint64_t, byte_count)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_aggregate_stats_request,
    (canard::net::ofp::v10::v10_detail::ofp_match, match)
    (std::uint8_t, table_id)
    (std::uint8_t, pad)
    (std::uint16_t, out_port)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_aggregate_stats_reply,
    (std::uint64_t, packet_count)
    (std::uint64_t, byte_count)
    (std::uint32_t, flow_count)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_table_stats,
    (std::uint8_t, table_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint32_t, wildcards)
    (std::uint32_t, max_entries)
    (std::uint32_t, active_count)
    (std::uint64_t, lookup_count)
    (std::uint64_t, matched_count)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_port_stats_request,
    (std::uint16_t, port_no)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_port_stats,
    (std::uint16_t, port_no)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
    (std::uint64_t, rx_packets)
    (std::uint64_t, tx_packets)
    (std::uint64_t, rx_bytes)
    (std::uint64_t, tx_bytes)
    (std::uint64_t, rx_dropped)
    (std::uint64_t, tx_dropped)
    (std::uint64_t, rx_errors)
    (std::uint64_t, tx_errors)
    (std::uint64_t, rx_frame_err)
    (std::uint64_t, rx_over_err)
    (std::uint64_t, rx_crc_err)
    (std::uint64_t, collisions)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_vendor_header,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint32_t, vendor)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_queue_prop_header,
    (std::uint16_t, property)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_queue_prop_min_rate,
    (canard::net::ofp::v10::v10_detail::ofp_queue_prop_header, prop_header)
    (std::uint16_t, rate)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_packet_queue,
    (std::uint32_t, queue_id)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_queue_get_config_request,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint16_t, port)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_queue_get_config_reply,
    (canard::net::ofp::v10::v10_detail::ofp_header, header)
    (std::uint16_t, port)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_action_enqueue,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint16_t, port)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
    (std::uint32_t, queue_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_queue_stats_request,
    (std::uint16_t, port_no)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint32_t, queue_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::v10_detail::ofp_queue_stats,
    (std::uint16_t, port_no)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint32_t, queue_id)
    (std::uint64_t, tx_bytes)
    (std::uint64_t, tx_packets)
    (std::uint64_t, tx_errors)
)

#endif // CANARD_NET_OFP_V10_FUSION_ADATOR_HPP
