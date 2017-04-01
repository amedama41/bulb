#ifndef CANARD_NET_OFP_V13_FUSION_ADATOR_HPP
#define CANARD_NET_OFP_V13_FUSION_ADATOR_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_header,
    version,
    type,
    length,
    xid
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_hello_elem_header,
    type,
    length
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_hello_elem_versionbitmap,
    type,
    length
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_hello,
    header
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_switch_config,
    header,
    flags,
    miss_send_len
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_mod,
    header,
    table_id,
    pad,
    config
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_port,
    port_no,
    pad,
    hw_addr,
    pad2,
    name,
    config,
    state,
    curr,
    advertised,
    supported,
    peer,
    curr_speed,
    max_speed
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_switch_features,
    header,
    datapath_id,
    n_buffers,
    n_tables,
    auxiliary_id,
    pad,
    capabilities,
    reserved
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_port_status,
    header,
    reason,
    pad,
    desc
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_port_mod,
    header,
    port_no,
    pad,
    hw_addr,
    pad2,
    config,
    mask,
    advertise,
    pad3
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_match,
    type,
    length,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_oxm_experimenter_header,
    oxm_header,
    experimenter
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_header,
    type,
    len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_output,
    type,
    len,
    port,
    max_len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_mpls_ttl,
    type,
    len,
    mpls_ttl,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_push,
    type,
    len,
    ethertype,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_pop_mpls,
    type,
    len,
    ethertype,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_group,
    type,
    len,
    group_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_nw_ttl,
    type,
    len,
    nw_ttl,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_set_field,
    type,
    len,
    field
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_experimenter_header,
    type,
    len,
    experimenter
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_instruction,
    type,
    len
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_instruction_goto_table,
    type,
    len,
    table_id,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_instruction_write_metadata,
    type,
    len,
    pad,
    metadata,
    metadata_mask
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_instruction_actions,
    type,
    len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_instruction_meter,
    type,
    len,
    meter_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_instruction_experimenter,
    type,
    len,
    experimenter
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_flow_mod,
    header,
    cookie,
    cookie_mask,
    table_id,
    command,
    idle_timeout,
    hard_timeout,
    priority,
    buffer_id,
    out_port,
    out_group,
    flags,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_bucket,
    len,
    weight,
    watch_port,
    watch_group,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_group_mod,
    header,
    command,
    type,
    pad,
    group_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_packet_out,
    header,
    buffer_id,
    in_port,
    actions_len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_packet_in,
    header,
    buffer_id,
    total_len,
    reason,
    table_id,
    cookie
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_flow_removed,
    header,
    cookie,
    priority,
    reason,
    table_id,
    duration_sec,
    duration_nsec,
    idle_timeout,
    hard_timeout,
    packet_count,
    byte_count
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_band_header,
    type,
    len,
    rate,
    burst_size
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_band_drop,
    type,
    len,
    rate,
    burst_size,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_band_dscp_remark,
    type,
    len,
    rate,
    burst_size,
    prec_level,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_band_experimenter,
    type,
    len,
    rate,
    burst_size,
    experimenter
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_mod,
    header,
    command,
    flags,
    meter_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_error_msg,
    header,
    type,
    code
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_error_experimenter_msg,
    header,
    type,
    exp_type,
    experimenter
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_multipart_request,
    header,
    type,
    flags,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_multipart_reply,
    header,
    type,
    flags,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_desc,
    mfr_desc,
    hw_desc,
    sw_desc,
    serial_num,
    dp_desc
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_flow_stats_request,
    table_id,
    pad,
    out_port,
    out_group,
    pad2,
    cookie,
    cookie_mask
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_flow_stats,
    length,
    table_id,
    pad,
    duration_sec,
    duration_nsec,
    priority,
    idle_timeout,
    hard_timeout,
    flags,
    pad2,
    cookie,
    packet_count,
    byte_count
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_aggregate_stats_request,
    table_id,
    pad,
    out_port,
    out_group,
    pad2,
    cookie,
    cookie_mask
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_aggregate_stats_reply,
    packet_count,
    byte_count,
    flow_count,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_feature_prop_header,
    type,
    length
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_feature_prop_instructions,
    type,
    length
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_feature_prop_next_tables,
    type,
    length
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_feature_prop_actions,
    type,
    length
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_feature_prop_oxm,
    type,
    length
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_feature_prop_experimenter,
    type,
    length,
    experimenter,
    exp_type
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_features,
    length,
    table_id,
    pad,
    name,
    metadata_match,
    metadata_write,
    config,
    max_entries
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_table_stats,
    table_id,
    pad,
    active_count,
    lookup_count,
    matched_count
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_port_stats_request,
    port_no,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_port_stats,
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
    collisions,
    duration_sec,
    duration_nsec
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_group_stats_request,
    group_id,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_bucket_counter,
    packet_count,
    byte_count
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_group_stats,
    length,
    pad,
    group_id,
    ref_count,
    pad2,
    packet_count,
    byte_count,
    duration_sec,
    duration_nsec
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_group_desc,
    length,
    type,
    pad,
    group_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_group_features,
    types,
    capabilities,
    max_groups,
    actions
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_multipart_request,
    meter_id,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_band_stats,
    packet_band_count,
    byte_band_count
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_stats,
    meter_id,
    len,
    pad,
    flow_count,
    packet_in_count,
    byte_in_count,
    duration_sec,
    duration_nsec
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_config,
    length,
    flags,
    meter_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_meter_features,
    max_meter,
    band_types,
    capabilities,
    max_bands,
    max_color,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_experimenter_multipart_header,
    experimenter,
    exp_type
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_experimenter_header,
    header,
    experimenter,
    exp_type
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_queue_prop_header,
    property,
    len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_queue_prop_min_rate,
    prop_header,
    rate,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_queue_prop_max_rate,
    prop_header,
    rate,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_queue_prop_experimenter,
    prop_header,
    experimenter,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_packet_queue,
    queue_id,
    port,
    len,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_queue_get_config_request,
    header,
    port,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_queue_get_config_reply,
    header,
    port,
    pad
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_action_set_queue,
    type,
    len,
    queue_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_queue_stats_request,
    port_no,
    queue_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_queue_stats,
    port_no,
    queue_id,
    tx_bytes,
    tx_packets,
    tx_errors,
    duration_sec,
    duration_nsec
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_role_request,
    header,
    role,
    pad,
    generation_id
)

BOOST_FUSION_ADAPT_STRUCT(
    canard::net::ofp::v13::protocol::ofp_async_config,
    header,
    packet_in_mask,
    port_status_mask,
    flow_removed_mask
)

#endif // CANARD_NET_OFP_V13_FUSION_ADATOR_HPP
