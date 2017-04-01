#ifndef CANARD_NET_OFP_V10_FUSION_ADATOR_HPP
#define CANARD_NET_OFP_V10_FUSION_ADATOR_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_header,
    version,
    type,
    length,
    xid
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_hello,
    header
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_switch_config,
    header,
    flags,
    miss_send_len
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_phy_port,
    port_no,
    hw_addr,
    name,
    config,
    state,
    curr,
    advertised,
    supported,
    peer
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_switch_features,
    header,
    datapath_id,
    n_buffers,
    n_tables,
    pad,
    capabilities,
    actions
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_port_status,
    header,
    reason,
    pad,
    desc
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_port_mod,
    header,
    port_no,
    hw_addr,
    config,
    mask,
    advertise,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_packet_in,
    header,
    buffer_id,
    total_len,
    in_port,
    reason,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_output,
    type,
    len,
    port,
    max_len
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_vlan_vid,
    type,
    len,
    vlan_vid,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_vlan_pcp,
    type,
    len,
    vlan_pcp,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_dl_addr,
    type,
    len,
    dl_addr,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_nw_addr,
    type,
    len,
    nw_addr
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_tp_port,
    type,
    len,
    tp_port,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_nw_tos,
    type,
    len,
    nw_tos,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_vendor_header,
    type,
    len,
    vendor
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_header,
    type,
    len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_packet_out,
    header,
    buffer_id,
    in_port,
    actions_len
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_match,
    wildcards,
    in_port,
    dl_src,
    dl_dst,
    dl_vlan,
    dl_vlan_pcp,
    pad1,
    dl_type,
    nw_tos,
    nw_proto,
    pad2,
    nw_src,
    nw_dst,
    tp_src,
    tp_dst
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_flow_mod,
    header,
    match,
    cookie,
    command,
    idle_timeout,
    hard_timeout,
    priority,
    buffer_id,
    out_port,
    flags
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_flow_removed,
    header,
    match,
    cookie,
    priority,
    reason,
    pad,
    duration_sec,
    duration_nsec,
    idle_timeout,
    pad2,
    packet_count,
    byte_count
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_error_msg,
    header,
    type,
    code
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_stats_request,
    header,
    type,
    flags
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_stats_reply,
    header,
    type,
    flags
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_desc_stats,
    mfr_desc,
    hw_desc,
    sw_desc,
    serial_num,
    dp_desc
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_flow_stats_request,
    match,
    table_id,
    pad,
    out_port
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_flow_stats,
    length,
    table_id,
    pad,
    match,
    duration_sec,
    duration_nsec,
    priority,
    idle_timeout,
    hard_timeout,
    pad2,
    cookie,
    packet_count,
    byte_count
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_aggregate_stats_request,
    match,
    table_id,
    pad,
    out_port
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_aggregate_stats_reply,
    packet_count,
    byte_count,
    flow_count,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_table_stats,
    table_id,
    pad,
    name,
    wildcards,
    max_entries,
    active_count,
    lookup_count,
    matched_count
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_port_stats_request,
    port_no,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_port_stats,
    port_no,
    pad,
    rx_packets,
    tx_packets,
    rx_bytes,
    tx_bytes,
    rx_dropped,
    tx_dropped,
    rx_errors,
    tx_errors,
    rx_frame_err,
    rx_over_err,
    rx_crc_err,
    collisions
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_vendor_header,
    header,
    vendor
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_queue_prop_header,
    property,
    len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_queue_prop_min_rate,
    prop_header,
    rate,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_packet_queue,
    queue_id,
    len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_queue_get_config_request,
    header,
    port,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_queue_get_config_reply,
    header,
    port,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_action_enqueue,
    type,
    len,
    port,
    pad,
    queue_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_queue_stats_request,
    port_no,
    pad,
    queue_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v10::protocol::ofp_queue_stats,
    port_no,
    pad,
    queue_id,
    tx_bytes,
    tx_packets,
    tx_errors
)

#endif // CANARD_NET_OFP_V10_FUSION_ADATOR_HPP
