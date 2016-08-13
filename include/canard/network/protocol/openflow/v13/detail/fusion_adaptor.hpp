#ifndef CANARD_NET_OFP_V13_FUSION_ADATOR_HPP
#define CANARD_NET_OFP_V13_FUSION_ADATOR_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <canard/network/protocol/openflow/v13/openflow.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_header,
    (std::uint8_t, version)
    (std::uint8_t, type)
    (std::uint16_t, length)
    (std::uint32_t, xid)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_hello_elem_header,
    (std::uint16_t, type)
    (std::uint16_t, length)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_hello_elem_versionbitmap,
    (std::uint16_t, type)
    (std::uint16_t, length)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_hello,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_switch_config,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint16_t, flags)
    (std::uint16_t, miss_send_len)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_mod,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint8_t, table_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint32_t, config)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_port,
    (std::uint32_t, port_no)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, hw_addr[0])
    (std::uint8_t, hw_addr[1])
    (std::uint8_t, hw_addr[2])
    (std::uint8_t, hw_addr[3])
    (std::uint8_t, hw_addr[4])
    (std::uint8_t, hw_addr[5])
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint32_t, config)
    (std::uint32_t, state)
    (std::uint32_t, curr)
    (std::uint32_t, advertised)
    (std::uint32_t, supported)
    (std::uint32_t, peer)
    (std::uint32_t, curr_speed)
    (std::uint32_t, max_speed)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_switch_features,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint64_t, datapath_id)
    (std::uint32_t, n_buffers)
    (std::uint8_t, n_tables)
    (std::uint8_t, auxiliary_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint32_t, capabilities)
    (std::uint32_t, reserved)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_port_status,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint8_t, reason)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
    (std::uint8_t, pad[6])
    (canard::net::ofp::v13::v13_detail::ofp_port, desc)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_port_mod,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint32_t, port_no)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, hw_addr[0])
    (std::uint8_t, hw_addr[1])
    (std::uint8_t, hw_addr[2])
    (std::uint8_t, hw_addr[3])
    (std::uint8_t, hw_addr[4])
    (std::uint8_t, hw_addr[5])
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint32_t, config)
    (std::uint32_t, mask)
    (std::uint32_t, advertise)
    (std::uint8_t, pad3[0])
    (std::uint8_t, pad3[1])
    (std::uint8_t, pad3[2])
    (std::uint8_t, pad3[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_match,
    (std::uint16_t, type)
    (std::uint16_t, length)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_oxm_experimenter_header,
    (std::uint32_t, oxm_header)
    (std::uint32_t, experimenter)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_header,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_output,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, port)
    (std::uint16_t, max_len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_mpls_ttl,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, mpls_ttl)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_push,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint16_t, ethertype)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_pop_mpls,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint16_t, ethertype)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_group,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, group_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_nw_ttl,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, nw_ttl)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_set_field,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, field[0])
    (std::uint8_t, field[1])
    (std::uint8_t, field[2])
    (std::uint8_t, field[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_experimenter_header,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, experimenter)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_instruction,
    (std::uint16_t, type)
    (std::uint16_t, len)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_instruction_goto_table,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, table_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_instruction_write_metadata,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint64_t, metadata)
    (std::uint64_t, metadata_mask)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_instruction_actions,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_instruction_meter,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, meter_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_instruction_experimenter,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, experimenter)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_flow_mod,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint64_t, cookie)
    (std::uint64_t, cookie_mask)
    (std::uint8_t, table_id)
    (std::uint8_t, command)
    (std::uint16_t, idle_timeout)
    (std::uint16_t, hard_timeout)
    (std::uint16_t, priority)
    (std::uint32_t, buffer_id)
    (std::uint32_t, out_port)
    (std::uint32_t, out_group)
    (std::uint16_t, flags)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_bucket,
    (std::uint16_t, len)
    (std::uint16_t, weight)
    (std::uint32_t, watch_port)
    (std::uint32_t, watch_group)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_group_mod,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint16_t, command)
    (std::uint8_t, type)
    (std::uint8_t, pad)
    (std::uint32_t, group_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_packet_out,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint32_t, buffer_id)
    (std::uint32_t, in_port)
    (std::uint16_t, actions_len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_packet_in,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint32_t, buffer_id)
    (std::uint16_t, total_len)
    (std::uint8_t, reason)
    (std::uint8_t, table_id)
    (std::uint64_t, cookie)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_flow_removed,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint64_t, cookie)
    (std::uint16_t, priority)
    (std::uint8_t, reason)
    (std::uint8_t, table_id)
    (std::uint32_t, duration_sec)
    (std::uint32_t, duration_nsec)
    (std::uint16_t, idle_timeout)
    (std::uint16_t, hard_timeout)
    (std::uint64_t, packet_count)
    (std::uint64_t, byte_count)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_band_header,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, rate)
    (std::uint32_t, burst_size)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_band_drop,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, rate)
    (std::uint32_t, burst_size)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_band_dscp_remark,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, rate)
    (std::uint32_t, burst_size)
    (std::uint8_t, prec_level)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_band_experimenter,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, rate)
    (std::uint32_t, burst_size)
    (std::uint32_t, experimenter)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_mod,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint16_t, command)
    (std::uint16_t, flags)
    (std::uint32_t, meter_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_error_msg,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint16_t, type)
    (std::uint16_t, code)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_error_experimenter_msg,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint16_t, type)
    (std::uint16_t, exp_type)
    (std::uint32_t, experimenter)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_multipart_request,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint16_t, type)
    (std::uint16_t, flags)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_multipart_reply,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint16_t, type)
    (std::uint16_t, flags)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_desc,

)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_flow_stats_request,
    (std::uint8_t, table_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint32_t, out_port)
    (std::uint32_t, out_group)
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint8_t, pad2[2])
    (std::uint8_t, pad2[3])
    (std::uint64_t, cookie)
    (std::uint64_t, cookie_mask)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_flow_stats,
    (std::uint16_t, length)
    (std::uint8_t, table_id)
    (std::uint8_t, pad)
    (std::uint32_t, duration_sec)
    (std::uint32_t, duration_nsec)
    (std::uint16_t, priority)
    (std::uint16_t, idle_timeout)
    (std::uint16_t, hard_timeout)
    (std::uint16_t, flags)
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint8_t, pad2[2])
    (std::uint8_t, pad2[3])
    (std::uint64_t, cookie)
    (std::uint64_t, packet_count)
    (std::uint64_t, byte_count)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_aggregate_stats_request,
    (std::uint8_t, table_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint32_t, out_port)
    (std::uint32_t, out_group)
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint8_t, pad2[2])
    (std::uint8_t, pad2[3])
    (std::uint64_t, cookie)
    (std::uint64_t, cookie_mask)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_aggregate_stats_reply,
    (std::uint64_t, packet_count)
    (std::uint64_t, byte_count)
    (std::uint32_t, flow_count)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_feature_prop_header,
    (std::uint16_t, type)
    (std::uint16_t, length)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_feature_prop_instructions,
    (std::uint16_t, type)
    (std::uint16_t, length)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_feature_prop_next_tables,
    (std::uint16_t, type)
    (std::uint16_t, length)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_feature_prop_actions,
    (std::uint16_t, type)
    (std::uint16_t, length)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_feature_prop_oxm,
    (std::uint16_t, type)
    (std::uint16_t, length)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_feature_prop_experimenter,
    (std::uint16_t, type)
    (std::uint16_t, length)
    (std::uint32_t, experimenter)
    (std::uint32_t, exp_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_features,
    (std::uint16_t, length)
    (std::uint8_t, table_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint64_t, metadata_match)
    (std::uint64_t, metadata_write)
    (std::uint32_t, config)
    (std::uint32_t, max_entries)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_table_stats,
    (std::uint8_t, table_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint32_t, active_count)
    (std::uint64_t, lookup_count)
    (std::uint64_t, matched_count)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_port_stats_request,
    (std::uint32_t, port_no)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_port_stats,
    (std::uint32_t, port_no)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
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
    (std::uint32_t, duration_sec)
    (std::uint32_t, duration_nsec)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_group_stats_request,
    (std::uint32_t, group_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_bucket_counter,
    (std::uint64_t, packet_count)
    (std::uint64_t, byte_count)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_group_stats,
    (std::uint16_t, length)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint32_t, group_id)
    (std::uint32_t, ref_count)
    (std::uint8_t, pad2[0])
    (std::uint8_t, pad2[1])
    (std::uint8_t, pad2[2])
    (std::uint8_t, pad2[3])
    (std::uint64_t, packet_count)
    (std::uint64_t, byte_count)
    (std::uint32_t, duration_sec)
    (std::uint32_t, duration_nsec)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_group_desc,
    (std::uint16_t, length)
    (std::uint8_t, type)
    (std::uint8_t, pad)
    (std::uint32_t, group_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_group_features,
    (std::uint32_t, types)
    (std::uint32_t, capabilities)
    (std::uint32_t, max_groups[0])
    (std::uint32_t, max_groups[1])
    (std::uint32_t, max_groups[2])
    (std::uint32_t, max_groups[3])
    (std::uint32_t, actions[0])
    (std::uint32_t, actions[1])
    (std::uint32_t, actions[2])
    (std::uint32_t, actions[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_multipart_request,
    (std::uint32_t, meter_id)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_band_stats,
    (std::uint64_t, packet_band_count)
    (std::uint64_t, byte_band_count)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_stats,
    (std::uint32_t, meter_id)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
    (std::uint32_t, flow_count)
    (std::uint64_t, packet_in_count)
    (std::uint64_t, byte_in_count)
    (std::uint32_t, duration_sec)
    (std::uint32_t, duration_nsec)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_config,
    (std::uint16_t, length)
    (std::uint16_t, flags)
    (std::uint32_t, meter_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_meter_features,
    (std::uint32_t, max_meter)
    (std::uint32_t, band_types)
    (std::uint32_t, capabilities)
    (std::uint8_t, max_bands)
    (std::uint8_t, max_color)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_experimenter_multipart_header,
    (std::uint32_t, experimenter)
    (std::uint32_t, exp_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_experimenter_header,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint32_t, experimenter)
    (std::uint32_t, exp_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_queue_prop_header,
    (std::uint16_t, property)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_queue_prop_min_rate,
    (canard::net::ofp::v13::v13_detail::ofp_queue_prop_header, prop_header)
    (std::uint16_t, rate)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_queue_prop_max_rate,
    (canard::net::ofp::v13::v13_detail::ofp_queue_prop_header, prop_header)
    (std::uint16_t, rate)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_queue_prop_experimenter,
    (canard::net::ofp::v13::v13_detail::ofp_queue_prop_header, prop_header)
    (std::uint32_t, experimenter)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_packet_queue,
    (std::uint32_t, queue_id)
    (std::uint32_t, port)
    (std::uint16_t, len)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint8_t, pad[4])
    (std::uint8_t, pad[5])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_queue_get_config_request,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint32_t, port)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_queue_get_config_reply,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint32_t, port)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_action_set_queue,
    (std::uint16_t, type)
    (std::uint16_t, len)
    (std::uint32_t, queue_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_queue_stats_request,
    (std::uint32_t, port_no)
    (std::uint32_t, queue_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_queue_stats,
    (std::uint32_t, port_no)
    (std::uint32_t, queue_id)
    (std::uint64_t, tx_bytes)
    (std::uint64_t, tx_packets)
    (std::uint64_t, tx_errors)
    (std::uint32_t, duration_sec)
    (std::uint32_t, duration_nsec)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_role_request,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint32_t, role)
    (std::uint8_t, pad[0])
    (std::uint8_t, pad[1])
    (std::uint8_t, pad[2])
    (std::uint8_t, pad[3])
    (std::uint64_t, generation_id)
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::v13_detail::ofp_async_config,
    (canard::net::ofp::v13::v13_detail::ofp_header, header)
    (std::uint32_t, packet_in_mask[0])
    (std::uint32_t, packet_in_mask[1])
    (std::uint32_t, port_status_mask[0])
    (std::uint32_t, port_status_mask[1])
    (std::uint32_t, flow_removed_mask[0])
    (std::uint32_t, flow_removed_mask[1])
)

#endif // CANARD_NET_OFP_V13_FUSION_ADATOR_HPP
