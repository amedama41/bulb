#ifndef CANARD_NET_OFP_V13_OPENFLOW_HPP
#define CANARD_NET_OFP_V13_OPENFLOW_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  namespace v13_detail {

    struct ofp_header
    {
      std::uint8_t version;
      std::uint8_t type;
      std::uint16_t length;
      std::uint32_t xid;
    };
    static_assert(sizeof(ofp_header) == 8, "");

    struct ofp_hello_elem_header
    {
      std::uint16_t type;
      std::uint16_t length;
    };
    static_assert(sizeof(ofp_hello_elem_header) == 4, "");

    struct ofp_hello_elem_versionbitmap
    {
      std::uint16_t type;
      std::uint16_t length;
    };
    static_assert(sizeof(ofp_hello_elem_versionbitmap) == 4, "");

    struct ofp_hello
    {
      ofp_header header;
    };
    static_assert(sizeof(ofp_hello) == 8, "");

    struct ofp_switch_config
    {
      ofp_header header;
      std::uint16_t flags;
      std::uint16_t miss_send_len;
    };
    static_assert(sizeof(ofp_switch_config) == 12, "");

    struct ofp_table_mod
    {
      ofp_header header;
      std::uint8_t table_id;
      std::uint8_t pad[3];
      std::uint32_t config;
    };
    static_assert(sizeof(ofp_table_mod) == 16, "");

    struct ofp_port
    {
      std::uint32_t port_no;
      std::uint8_t pad[4];
      std::uint8_t hw_addr[6];
      std::uint8_t pad2[2];
      char name[16];
      std::uint32_t config;
      std::uint32_t state;
      std::uint32_t curr;
      std::uint32_t advertised;
      std::uint32_t supported;
      std::uint32_t peer;
      std::uint32_t curr_speed;
      std::uint32_t max_speed;
    };
    static_assert(sizeof(ofp_port) == 64, "");

    struct ofp_switch_features
    {
      ofp_header header;
      std::uint64_t datapath_id;
      std::uint32_t n_buffers;
      std::uint8_t n_tables;
      std::uint8_t auxiliary_id;
      std::uint8_t pad[2];
      std::uint32_t capabilities;
      std::uint32_t reserved;
    };
    static_assert(sizeof(ofp_switch_features) == 32, "");

    struct ofp_port_status
    {
      ofp_header header;
      std::uint8_t reason;
      std::uint8_t pad[7];
      ofp_port desc;
    };
    static_assert(sizeof(ofp_port_status) == 80, "");

    struct ofp_port_mod
    {
      ofp_header header;
      std::uint32_t port_no;
      std::uint8_t pad[4];
      std::uint8_t hw_addr[6];
      std::uint8_t pad2[2];
      std::uint32_t config;
      std::uint32_t mask;
      std::uint32_t advertise;
      std::uint8_t pad3[4];
    };
    static_assert(sizeof(ofp_port_mod) == 40, "");

    struct ofp_match
    {
      std::uint16_t type;
      std::uint16_t length;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_match) == 8, "");

    struct ofp_oxm_experimenter_header
    {
      std::uint32_t oxm_header;
      std::uint32_t experimenter;
    };
    static_assert(sizeof(ofp_oxm_experimenter_header) == 8, "");

    struct ofp_action_header
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_action_header) == 8, "");

    struct ofp_action_output
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t port;
      std::uint16_t max_len;
      std::uint8_t pad[6];
    };
    static_assert(sizeof(ofp_action_output) == 16, "");

    struct ofp_action_mpls_ttl
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint8_t mpls_ttl;
      std::uint8_t pad[3];
    };
    static_assert(sizeof(ofp_action_mpls_ttl) == 8, "");

    struct ofp_action_push
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint16_t ethertype;
      std::uint8_t pad[2];
    };
    static_assert(sizeof(ofp_action_push) == 8, "");

    struct ofp_action_pop_mpls
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint16_t ethertype;
      std::uint8_t pad[2];
    };
    static_assert(sizeof(ofp_action_pop_mpls) == 8, "");

    struct ofp_action_group
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t group_id;
    };
    static_assert(sizeof(ofp_action_group) == 8, "");

    struct ofp_action_nw_ttl
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint8_t nw_ttl;
      std::uint8_t pad[3];
    };
    static_assert(sizeof(ofp_action_nw_ttl) == 8, "");

    struct ofp_action_set_field
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint8_t field[4];
    };
    static_assert(sizeof(ofp_action_set_field) == 8, "");

    struct ofp_action_experimenter_header
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t experimenter;
    };
    static_assert(sizeof(ofp_action_experimenter_header) == 8, "");

    struct ofp_instruction
    {
      std::uint16_t type;
      std::uint16_t len;
    };
    static_assert(sizeof(ofp_instruction) == 4, "");

    struct ofp_instruction_goto_table
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint8_t table_id;
      std::uint8_t pad[3];
    };
    static_assert(sizeof(ofp_instruction_goto_table) == 8, "");

    struct ofp_instruction_write_metadata
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint8_t pad[4];
      std::uint64_t metadata;
      std::uint64_t metadata_mask;
    };
    static_assert(sizeof(ofp_instruction_write_metadata) == 24, "");

    struct ofp_instruction_actions
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_instruction_actions) == 8, "");

    struct ofp_instruction_meter
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t meter_id;
    };
    static_assert(sizeof(ofp_instruction_meter) == 8, "");

    struct ofp_instruction_experimenter
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t experimenter;
    };
    static_assert(sizeof(ofp_instruction_experimenter) == 8, "");

    struct ofp_flow_mod
    {
      ofp_header header;
      std::uint64_t cookie;
      std::uint64_t cookie_mask;
      std::uint8_t table_id;
      std::uint8_t command;
      std::uint16_t idle_timeout;
      std::uint16_t hard_timeout;
      std::uint16_t priority;
      std::uint32_t buffer_id;
      std::uint32_t out_port;
      std::uint32_t out_group;
      std::uint16_t flags;
      std::uint8_t pad[2];
    };
    static_assert(sizeof(ofp_flow_mod) == 48, "");

    struct ofp_bucket
    {
      std::uint16_t len;
      std::uint16_t weight;
      std::uint32_t watch_port;
      std::uint32_t watch_group;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_bucket) == 16, "");

    struct ofp_group_mod
    {
      ofp_header header;
      std::uint16_t command;
      std::uint8_t type;
      std::uint8_t pad;
      std::uint32_t group_id;
    };
    static_assert(sizeof(ofp_group_mod) == 16, "");

    struct ofp_packet_out
    {
      ofp_header header;
      std::uint32_t buffer_id;
      std::uint32_t in_port;
      std::uint16_t actions_len;
      std::uint8_t pad[6];
    };
    static_assert(sizeof(ofp_packet_out) == 24, "");

    struct ofp_packet_in
    {
      ofp_header header;
      std::uint32_t buffer_id;
      std::uint16_t total_len;
      std::uint8_t reason;
      std::uint8_t table_id;
      std::uint64_t cookie;
    };
    static_assert(sizeof(ofp_packet_in) == 24, "");

    struct ofp_flow_removed
    {
      ofp_header header;
      std::uint64_t cookie;
      std::uint16_t priority;
      std::uint8_t reason;
      std::uint8_t table_id;
      std::uint32_t duration_sec;
      std::uint32_t duration_nsec;
      std::uint16_t idle_timeout;
      std::uint16_t hard_timeout;
      std::uint64_t packet_count;
      std::uint64_t byte_count;
    };
    static_assert(sizeof(ofp_flow_removed) == 48, "");

    struct ofp_meter_band_header
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t rate;
      std::uint32_t burst_size;
    };
    static_assert(sizeof(ofp_meter_band_header) == 12, "");

    struct ofp_meter_band_drop
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t rate;
      std::uint32_t burst_size;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_meter_band_drop) == 16, "");

    struct ofp_meter_band_dscp_remark
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t rate;
      std::uint32_t burst_size;
      std::uint8_t prec_level;
      std::uint8_t pad[3];
    };
    static_assert(sizeof(ofp_meter_band_dscp_remark) == 16, "");

    struct ofp_meter_band_experimenter
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t rate;
      std::uint32_t burst_size;
      std::uint32_t experimenter;
    };
    static_assert(sizeof(ofp_meter_band_experimenter) == 16, "");

    struct ofp_meter_mod
    {
      ofp_header header;
      std::uint16_t command;
      std::uint16_t flags;
      std::uint32_t meter_id;
    };
    static_assert(sizeof(ofp_meter_mod) == 16, "");

    struct ofp_error_msg
    {
      ofp_header header;
      std::uint16_t type;
      std::uint16_t code;
    };
    static_assert(sizeof(ofp_error_msg) == 12, "");

    struct ofp_error_experimenter_msg
    {
      ofp_header header;
      std::uint16_t type;
      std::uint16_t exp_type;
      std::uint32_t experimenter;
    };
    static_assert(sizeof(ofp_error_experimenter_msg) == 16, "");

    struct ofp_multipart_request
    {
      ofp_header header;
      std::uint16_t type;
      std::uint16_t flags;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_multipart_request) == 16, "");

    struct ofp_multipart_reply
    {
      ofp_header header;
      std::uint16_t type;
      std::uint16_t flags;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_multipart_reply) == 16, "");

    struct ofp_desc
    {
      char mfr_desc[256];
      char hw_desc[256];
      char sw_desc[256];
      char serial_num[32];
      char dp_desc[256];
    };
    static_assert(sizeof(ofp_desc) == 1056, "");

    struct ofp_flow_stats_request
    {
      std::uint8_t table_id;
      std::uint8_t pad[3];
      std::uint32_t out_port;
      std::uint32_t out_group;
      std::uint8_t pad2[4];
      std::uint64_t cookie;
      std::uint64_t cookie_mask;
    };
    static_assert(sizeof(ofp_flow_stats_request) == 32, "");

    struct ofp_flow_stats
    {
      std::uint16_t length;
      std::uint8_t table_id;
      std::uint8_t pad;
      std::uint32_t duration_sec;
      std::uint32_t duration_nsec;
      std::uint16_t priority;
      std::uint16_t idle_timeout;
      std::uint16_t hard_timeout;
      std::uint16_t flags;
      std::uint8_t pad2[4];
      std::uint64_t cookie;
      std::uint64_t packet_count;
      std::uint64_t byte_count;
    };
    static_assert(sizeof(ofp_flow_stats) == 48, "");

    struct ofp_aggregate_stats_request
    {
      std::uint8_t table_id;
      std::uint8_t pad[3];
      std::uint32_t out_port;
      std::uint32_t out_group;
      std::uint8_t pad2[4];
      std::uint64_t cookie;
      std::uint64_t cookie_mask;
    };
    static_assert(sizeof(ofp_aggregate_stats_request) == 32, "");

    struct ofp_aggregate_stats_reply
    {
      std::uint64_t packet_count;
      std::uint64_t byte_count;
      std::uint32_t flow_count;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_aggregate_stats_reply) == 24, "");

    struct ofp_table_feature_prop_header
    {
      std::uint16_t type;
      std::uint16_t length;
    };
    static_assert(sizeof(ofp_table_feature_prop_header) == 4, "");

    struct ofp_table_feature_prop_instructions
    {
      std::uint16_t type;
      std::uint16_t length;
    };
    static_assert(sizeof(ofp_table_feature_prop_instructions) == 4, "");

    struct ofp_table_feature_prop_next_tables
    {
      std::uint16_t type;
      std::uint16_t length;
    };
    static_assert(sizeof(ofp_table_feature_prop_next_tables) == 4, "");

    struct ofp_table_feature_prop_actions
    {
      std::uint16_t type;
      std::uint16_t length;
    };
    static_assert(sizeof(ofp_table_feature_prop_actions) == 4, "");

    struct ofp_table_feature_prop_oxm
    {
      std::uint16_t type;
      std::uint16_t length;
    };
    static_assert(sizeof(ofp_table_feature_prop_oxm) == 4, "");

    struct ofp_table_feature_prop_experimenter
    {
      std::uint16_t type;
      std::uint16_t length;
      std::uint32_t experimenter;
      std::uint32_t exp_type;
    };
    static_assert(sizeof(ofp_table_feature_prop_experimenter) == 12, "");

    struct ofp_table_features
    {
      std::uint16_t length;
      std::uint8_t table_id;
      std::uint8_t pad[5];
      char name[32];
      std::uint64_t metadata_match;
      std::uint64_t metadata_write;
      std::uint32_t config;
      std::uint32_t max_entries;
    };
    static_assert(sizeof(ofp_table_features) == 64, "");

    struct ofp_table_stats
    {
      std::uint8_t table_id;
      std::uint8_t pad[3];
      std::uint32_t active_count;
      std::uint64_t lookup_count;
      std::uint64_t matched_count;
    };
    static_assert(sizeof(ofp_table_stats) == 24, "");

    struct ofp_port_stats_request
    {
      std::uint32_t port_no;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_port_stats_request) == 8, "");

    struct ofp_port_stats
    {
      std::uint32_t port_no;
      std::uint8_t pad[4];
      std::uint64_t rx_packets;
      std::uint64_t tx_packets;
      std::uint64_t rx_bytes;
      std::uint64_t tx_bytes;
      std::uint64_t rx_dropped;
      std::uint64_t tx_dropped;
      std::uint64_t rx_errors;
      std::uint64_t tx_errors;
      std::uint64_t rx_frame_err;
      std::uint64_t rx_over_err;
      std::uint64_t rx_crc_err;
      std::uint64_t collisions;
      std::uint32_t duration_sec;
      std::uint32_t duration_nsec;
    };
    static_assert(sizeof(ofp_port_stats) == 112, "");

    struct ofp_group_stats_request
    {
      std::uint32_t group_id;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_group_stats_request) == 8, "");

    struct ofp_bucket_counter
    {
      std::uint64_t packet_count;
      std::uint64_t byte_count;
    };
    static_assert(sizeof(ofp_bucket_counter) == 16, "");

    struct ofp_group_stats
    {
      std::uint16_t length;
      std::uint8_t pad[2];
      std::uint32_t group_id;
      std::uint32_t ref_count;
      std::uint8_t pad2[4];
      std::uint64_t packet_count;
      std::uint64_t byte_count;
      std::uint32_t duration_sec;
      std::uint32_t duration_nsec;
    };
    static_assert(sizeof(ofp_group_stats) == 40, "");

    struct ofp_group_desc
    {
      std::uint16_t length;
      std::uint8_t type;
      std::uint8_t pad;
      std::uint32_t group_id;
    };
    static_assert(sizeof(ofp_group_desc) == 8, "");

    struct ofp_group_features
    {
      std::uint32_t types;
      std::uint32_t capabilities;
      std::uint32_t max_groups[4];
      std::uint32_t actions[4];
    };
    static_assert(sizeof(ofp_group_features) == 40, "");

    struct ofp_meter_multipart_request
    {
      std::uint32_t meter_id;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_meter_multipart_request) == 8, "");

    struct ofp_meter_band_stats
    {
      std::uint64_t packet_band_count;
      std::uint64_t byte_band_count;
    };
    static_assert(sizeof(ofp_meter_band_stats) == 16, "");

    struct ofp_meter_stats
    {
      std::uint32_t meter_id;
      std::uint16_t len;
      std::uint8_t pad[6];
      std::uint32_t flow_count;
      std::uint64_t packet_in_count;
      std::uint64_t byte_in_count;
      std::uint32_t duration_sec;
      std::uint32_t duration_nsec;
    };
    static_assert(sizeof(ofp_meter_stats) == 40, "");

    struct ofp_meter_config
    {
      std::uint16_t length;
      std::uint16_t flags;
      std::uint32_t meter_id;
    };
    static_assert(sizeof(ofp_meter_config) == 8, "");

    struct ofp_meter_features
    {
      std::uint32_t max_meter;
      std::uint32_t band_types;
      std::uint32_t capabilities;
      std::uint8_t max_bands;
      std::uint8_t max_color;
      std::uint8_t pad[2];
    };
    static_assert(sizeof(ofp_meter_features) == 16, "");

    struct ofp_experimenter_multipart_header
    {
      std::uint32_t experimenter;
      std::uint32_t exp_type;
    };
    static_assert(sizeof(ofp_experimenter_multipart_header) == 8, "");

    struct ofp_experimenter_header
    {
      ofp_header header;
      std::uint32_t experimenter;
      std::uint32_t exp_type;
    };
    static_assert(sizeof(ofp_experimenter_header) == 16, "");

    struct ofp_queue_prop_header
    {
      std::uint16_t property;
      std::uint16_t len;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_queue_prop_header) == 8, "");

    struct ofp_queue_prop_min_rate
    {
      ofp_queue_prop_header prop_header;
      std::uint16_t rate;
      std::uint8_t pad[6];
    };
    static_assert(sizeof(ofp_queue_prop_min_rate) == 16, "");

    struct ofp_queue_prop_max_rate
    {
      ofp_queue_prop_header prop_header;
      std::uint16_t rate;
      std::uint8_t pad[6];
    };
    static_assert(sizeof(ofp_queue_prop_max_rate) == 16, "");

    struct ofp_queue_prop_experimenter
    {
      ofp_queue_prop_header prop_header;
      std::uint32_t experimenter;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_queue_prop_experimenter) == 16, "");

    struct ofp_packet_queue
    {
      std::uint32_t queue_id;
      std::uint32_t port;
      std::uint16_t len;
      std::uint8_t pad[6];
    };
    static_assert(sizeof(ofp_packet_queue) == 16, "");

    struct ofp_queue_get_config_request
    {
      ofp_header header;
      std::uint32_t port;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_queue_get_config_request) == 16, "");

    struct ofp_queue_get_config_reply
    {
      ofp_header header;
      std::uint32_t port;
      std::uint8_t pad[4];
    };
    static_assert(sizeof(ofp_queue_get_config_reply) == 16, "");

    struct ofp_action_set_queue
    {
      std::uint16_t type;
      std::uint16_t len;
      std::uint32_t queue_id;
    };
    static_assert(sizeof(ofp_action_set_queue) == 8, "");

    struct ofp_queue_stats_request
    {
      std::uint32_t port_no;
      std::uint32_t queue_id;
    };
    static_assert(sizeof(ofp_queue_stats_request) == 8, "");

    struct ofp_queue_stats
    {
      std::uint32_t port_no;
      std::uint32_t queue_id;
      std::uint64_t tx_bytes;
      std::uint64_t tx_packets;
      std::uint64_t tx_errors;
      std::uint32_t duration_sec;
      std::uint32_t duration_nsec;
    };
    static_assert(sizeof(ofp_queue_stats) == 40, "");

    struct ofp_role_request
    {
      ofp_header header;
      std::uint32_t role;
      std::uint8_t pad[4];
      std::uint64_t generation_id;
    };
    static_assert(sizeof(ofp_role_request) == 24, "");

    struct ofp_async_config
    {
      ofp_header header;
      std::uint32_t packet_in_mask[2];
      std::uint32_t port_status_mask[2];
      std::uint32_t flow_removed_mask[2];
    };
    static_assert(sizeof(ofp_async_config) == 32, "");

  } // namespace v13_detail

  namespace protocol {

    enum uint8_t_constants : std::uint8_t
    {
      OFP_VERSION = 0x04,
    };

    enum uint16_t_constants : std::uint16_t
    {
      OFP_TCP_PORT = 6653,
      OFP_SSL_PORT = 6653,
      OFP_DEFAULT_MISS_SEND_LEN = 128,
      OFP_FLOW_PERMANENT = 0,
      OFP_DEFAULT_PRIORITY = 0x8000,
      OFPQ_MIN_RATE_UNCFG = 0xffff,
      OFPQ_MAX_RATE_UNCFG = 0xffff,
    };

    enum uint32_t_constants : std::uint32_t
    {
      OFP_NO_BUFFER = 0xffffffff,
      OFPQ_ALL = 0xffffffff,
    };

    enum ofp_port_no
    {
      OFPP_MAX = 4294967040,
      OFPP_IN_PORT = 4294967288,
      OFPP_TABLE = 4294967289,
      OFPP_NORMAL = 4294967290,
      OFPP_FLOOD = 4294967291,
      OFPP_ALL = 4294967292,
      OFPP_CONTROLLER = 4294967293,
      OFPP_LOCAL = 4294967294,
      OFPP_ANY = 4294967295,
    };

    enum ofp_type
    {
      OFPT_HELLO = 0,
      OFPT_ERROR = 1,
      OFPT_ECHO_REQUEST = 2,
      OFPT_ECHO_REPLY = 3,
      OFPT_EXPERIMENTER = 4,
      OFPT_FEATURES_REQUEST = 5,
      OFPT_FEATURES_REPLY = 6,
      OFPT_GET_CONFIG_REQUEST = 7,
      OFPT_GET_CONFIG_REPLY = 8,
      OFPT_SET_CONFIG = 9,
      OFPT_PACKET_IN = 10,
      OFPT_FLOW_REMOVED = 11,
      OFPT_PORT_STATUS = 12,
      OFPT_PACKET_OUT = 13,
      OFPT_FLOW_MOD = 14,
      OFPT_GROUP_MOD = 15,
      OFPT_PORT_MOD = 16,
      OFPT_TABLE_MOD = 17,
      OFPT_MULTIPART_REQUEST = 18,
      OFPT_MULTIPART_REPLY = 19,
      OFPT_BARRIER_REQUEST = 20,
      OFPT_BARRIER_REPLY = 21,
      OFPT_QUEUE_GET_CONFIG_REQUEST = 22,
      OFPT_QUEUE_GET_CONFIG_REPLY = 23,
      OFPT_ROLE_REQUEST = 24,
      OFPT_ROLE_REPLY = 25,
      OFPT_GET_ASYNC_REQUEST = 26,
      OFPT_GET_ASYNC_REPLY = 27,
      OFPT_SET_ASYNC = 28,
      OFPT_METER_MOD = 29,
    };

    enum ofp_hello_elem_type
    {
      OFPHET_VERSIONBITMAP = 1,
    };

    enum ofp_config_flags
    {
      OFPC_FRAG_NORMAL = 0,
      OFPC_FRAG_DROP = 1,
      OFPC_FRAG_REASM = 2,
      OFPC_FRAG_MASK = 3,
    };

    enum ofp_table_config
    {
      OFPTC_DEPRECATED_MASK = 3,
    };

    enum ofp_table
    {
      OFPTT_MAX = 254,
      OFPTT_ALL = 255,
    };

    enum ofp_capabilities
    {
      OFPC_FLOW_STATS = 1,
      OFPC_TABLE_STATS = 2,
      OFPC_PORT_STATS = 4,
      OFPC_GROUP_STATS = 8,
      OFPC_IP_REASM = 32,
      OFPC_QUEUE_STATS = 64,
      OFPC_PORT_BLOCKED = 256,
    };

    enum ofp_port_config
    {
      OFPPC_PORT_DOWN = 1,
      OFPPC_NO_RECV = 4,
      OFPPC_NO_FWD = 32,
      OFPPC_NO_PACKET_IN = 64,
    };

    enum ofp_port_state
    {
      OFPPS_LINK_DOWN = 1,
      OFPPS_BLOCKED = 2,
      OFPPS_LIVE = 4,
    };

    enum ofp_port_features
    {
      OFPPF_10MB_HD = 1,
      OFPPF_10MB_FD = 2,
      OFPPF_100MB_HD = 4,
      OFPPF_100MB_FD = 8,
      OFPPF_1GB_HD = 16,
      OFPPF_1GB_FD = 32,
      OFPPF_10GB_FD = 64,
      OFPPF_40GB_FD = 128,
      OFPPF_100GB_FD = 256,
      OFPPF_1TB_FD = 512,
      OFPPF_OTHER = 1024,
      OFPPF_COPPER = 2048,
      OFPPF_FIBER = 4096,
      OFPPF_AUTONEG = 8192,
      OFPPF_PAUSE = 16384,
      OFPPF_PAUSE_ASYM = 32768,
    };

    enum ofp_port_reason
    {
      OFPPR_ADD = 0,
      OFPPR_DELETE = 1,
      OFPPR_MODIFY = 2,
    };

    enum ofp_match_type
    {
      OFPMT_STANDARD = 0,
      OFPMT_OXM = 1,
    };

    enum ofp_oxm_class
    {
      OFPXMC_NXM_0 = 0,
      OFPXMC_NXM_1 = 1,
      OFPXMC_OPENFLOW_BASIC = 32768,
      OFPXMC_EXPERIMENTER = 65535,
    };

    enum oxm_ofb_match_fields
    {
      OFPXMT_OFB_IN_PORT = 0,
      OFPXMT_OFB_IN_PHY_PORT = 1,
      OFPXMT_OFB_METADATA = 2,
      OFPXMT_OFB_ETH_DST = 3,
      OFPXMT_OFB_ETH_SRC = 4,
      OFPXMT_OFB_ETH_TYPE = 5,
      OFPXMT_OFB_VLAN_VID = 6,
      OFPXMT_OFB_VLAN_PCP = 7,
      OFPXMT_OFB_IP_DSCP = 8,
      OFPXMT_OFB_IP_ECN = 9,
      OFPXMT_OFB_IP_PROTO = 10,
      OFPXMT_OFB_IPV4_SRC = 11,
      OFPXMT_OFB_IPV4_DST = 12,
      OFPXMT_OFB_TCP_SRC = 13,
      OFPXMT_OFB_TCP_DST = 14,
      OFPXMT_OFB_UDP_SRC = 15,
      OFPXMT_OFB_UDP_DST = 16,
      OFPXMT_OFB_SCTP_SRC = 17,
      OFPXMT_OFB_SCTP_DST = 18,
      OFPXMT_OFB_ICMPV4_TYPE = 19,
      OFPXMT_OFB_ICMPV4_CODE = 20,
      OFPXMT_OFB_ARP_OP = 21,
      OFPXMT_OFB_ARP_SPA = 22,
      OFPXMT_OFB_ARP_TPA = 23,
      OFPXMT_OFB_ARP_SHA = 24,
      OFPXMT_OFB_ARP_THA = 25,
      OFPXMT_OFB_IPV6_SRC = 26,
      OFPXMT_OFB_IPV6_DST = 27,
      OFPXMT_OFB_IPV6_FLABEL = 28,
      OFPXMT_OFB_ICMPV6_TYPE = 29,
      OFPXMT_OFB_ICMPV6_CODE = 30,
      OFPXMT_OFB_IPV6_ND_TARGET = 31,
      OFPXMT_OFB_IPV6_ND_SLL = 32,
      OFPXMT_OFB_IPV6_ND_TLL = 33,
      OFPXMT_OFB_MPLS_LABEL = 34,
      OFPXMT_OFB_MPLS_TC = 35,
      OFPXMT_OFB_MPLS_BOS = 36,
      OFPXMT_OFB_PBB_ISID = 37,
      OFPXMT_OFB_TUNNEL_ID = 38,
      OFPXMT_OFB_IPV6_EXTHDR = 39,
    };

    enum ofp_vlan_id
    {
      OFPVID_PRESENT = 4096,
      OFPVID_NONE = 0,
    };

    enum ofp_ipv6exthdr_flags
    {
      OFPIEH_NONEXT = 1,
      OFPIEH_ESP = 2,
      OFPIEH_AUTH = 4,
      OFPIEH_DEST = 8,
      OFPIEH_FRAG = 16,
      OFPIEH_ROUTER = 32,
      OFPIEH_HOP = 64,
      OFPIEH_UNREP = 128,
      OFPIEH_UNSEQ = 256,
    };

    enum ofp_action_type
    {
      OFPAT_OUTPUT = 0,
      OFPAT_COPY_TTL_OUT = 11,
      OFPAT_COPY_TTL_IN = 12,
      OFPAT_SET_MPLS_TTL = 15,
      OFPAT_DEC_MPLS_TTL = 16,
      OFPAT_PUSH_VLAN = 17,
      OFPAT_POP_VLAN = 18,
      OFPAT_PUSH_MPLS = 19,
      OFPAT_POP_MPLS = 20,
      OFPAT_SET_QUEUE = 21,
      OFPAT_GROUP = 22,
      OFPAT_SET_NW_TTL = 23,
      OFPAT_DEC_NW_TTL = 24,
      OFPAT_SET_FIELD = 25,
      OFPAT_PUSH_PBB = 26,
      OFPAT_POP_PBB = 27,
      OFPAT_EXPERIMENTER = 65535,
    };

    enum ofp_controller_max_len
    {
      OFPCML_MAX = 65509,
      OFPCML_NO_BUFFER = 65535,
    };

    enum ofp_instruction_type
    {
      OFPIT_GOTO_TABLE = 1,
      OFPIT_WRITE_METADATA = 2,
      OFPIT_WRITE_ACTIONS = 3,
      OFPIT_APPLY_ACTIONS = 4,
      OFPIT_CLEAR_ACTIONS = 5,
      OFPIT_METER = 6,
      OFPIT_EXPERIMENTER = 65535,
    };

    enum ofp_flow_mod_command
    {
      OFPFC_ADD = 0,
      OFPFC_MODIFY = 1,
      OFPFC_MODIFY_STRICT = 2,
      OFPFC_DELETE = 3,
      OFPFC_DELETE_STRICT = 4,
    };

    enum ofp_flow_mod_flags
    {
      OFPFF_SEND_FLOW_REM = 1,
      OFPFF_CHECK_OVERLAP = 2,
      OFPFF_RESET_COUNTS = 4,
      OFPFF_NO_PKT_COUNTS = 8,
      OFPFF_NO_BYT_COUNTS = 16,
    };

    enum ofp_group
    {
      OFPG_MAX = 4294967040,
      OFPG_ALL = 4294967292,
      OFPG_ANY = 4294967295,
    };

    enum ofp_group_mod_command
    {
      OFPGC_ADD = 0,
      OFPGC_MODIFY = 1,
      OFPGC_DELETE = 2,
    };

    enum ofp_group_type
    {
      OFPGT_ALL = 0,
      OFPGT_SELECT = 1,
      OFPGT_INDIRECT = 2,
      OFPGT_FF = 3,
    };

    enum ofp_packet_in_reason
    {
      OFPR_NO_MATCH = 0,
      OFPR_ACTION = 1,
      OFPR_INVALID_TTL = 2,
    };

    enum ofp_flow_removed_reason
    {
      OFPRR_IDLE_TIMEOUT = 0,
      OFPRR_HARD_TIMEOUT = 1,
      OFPRR_DELETE = 2,
      OFPRR_GROUP_DELETE = 3,
    };

    enum ofp_meter
    {
      OFPM_MAX = 4294901760,
      OFPM_SLOWPATH = 4294967293,
      OFPM_CONTROLLER = 4294967294,
      OFPM_ALL = 4294967295,
    };

    enum ofp_meter_band_type
    {
      OFPMBT_DROP = 1,
      OFPMBT_DSCP_REMARK = 2,
      OFPMBT_EXPERIMENTER = 65535,
    };

    enum ofp_meter_mod_command
    {
      OFPMC_ADD = 0,
      OFPMC_MODIFY = 1,
      OFPMC_DELETE = 2,
    };

    enum ofp_meter_flags
    {
      OFPMF_KBPS = 1,
      OFPMF_PKTPS = 2,
      OFPMF_BURST = 4,
      OFPMF_STATS = 8,
    };

    enum ofp_error_type
    {
      OFPET_HELLO_FAILED = 0,
      OFPET_BAD_REQUEST = 1,
      OFPET_BAD_ACTION = 2,
      OFPET_BAD_INSTRUCTION = 3,
      OFPET_BAD_MATCH = 4,
      OFPET_FLOW_MOD_FAILED = 5,
      OFPET_GROUP_MOD_FAILED = 6,
      OFPET_PORT_MOD_FAILED = 7,
      OFPET_TABLE_MOD_FAILED = 8,
      OFPET_QUEUE_OP_FAILED = 9,
      OFPET_SWITCH_CONFIG_FAILED = 10,
      OFPET_ROLE_REQUEST_FAILED = 11,
      OFPET_METER_MOD_FAILED = 12,
      OFPET_TABLE_FEATURES_FAILED = 13,
      OFPET_EXPERIMENTER = 65535,
    };

    enum ofp_hello_failed_code
    {
      OFPHFC_INCOMPATIBLE = 0,
      OFPHFC_EPERM = 1,
    };

    enum ofp_bad_request_code
    {
      OFPBRC_BAD_VERSION = 0,
      OFPBRC_BAD_TYPE = 1,
      OFPBRC_BAD_MULTIPART = 2,
      OFPBRC_BAD_EXPERIMENTER = 3,
      OFPBRC_BAD_EXP_TYPE = 4,
      OFPBRC_EPERM = 5,
      OFPBRC_BAD_LEN = 6,
      OFPBRC_BUFFER_EMPTY = 7,
      OFPBRC_BUFFER_UNKNOWN = 8,
      OFPBRC_BAD_TABLE_ID = 9,
      OFPBRC_IS_SLAVE = 10,
      OFPBRC_BAD_PORT = 11,
      OFPBRC_BAD_PACKET = 12,
      OFPBRC_MULTIPART_BUFFER_OVERFLOW = 13,
    };

    enum ofp_bad_action_code
    {
      OFPBAC_BAD_TYPE = 0,
      OFPBAC_BAD_LEN = 1,
      OFPBAC_BAD_EXPERIMENTER = 2,
      OFPBAC_BAD_EXP_TYPE = 3,
      OFPBAC_BAD_OUT_PORT = 4,
      OFPBAC_BAD_ARGUMENT = 5,
      OFPBAC_EPERM = 6,
      OFPBAC_TOO_MANY = 7,
      OFPBAC_BAD_QUEUE = 8,
      OFPBAC_BAD_OUT_GROUP = 9,
      OFPBAC_MATCH_INCONSISTENT = 10,
      OFPBAC_UNSUPPORTED_ORDER = 11,
      OFPBAC_BAD_TAG = 12,
      OFPBAC_BAD_SET_TYPE = 13,
      OFPBAC_BAD_SET_LEN = 14,
      OFPBAC_BAD_SET_ARGUMENT = 15,
    };

    enum ofp_bad_instruction_code
    {
      OFPBIC_UNKNOWN_INST = 0,
      OFPBIC_UNSUP_INST = 1,
      OFPBIC_BAD_TABLE_ID = 2,
      OFPBIC_UNSUP_METADATA = 3,
      OFPBIC_UNSUP_METADATA_MASK = 4,
      OFPBIC_BAD_EXPERIMENTER = 5,
      OFPBIC_BAD_EXP_TYPE = 6,
      OFPBIC_BAD_LEN = 7,
      OFPBIC_EPERM = 8,
    };

    enum ofp_bad_match_code
    {
      OFPBMC_BAD_TYPE = 0,
      OFPBMC_BAD_LEN = 1,
      OFPBMC_BAD_TAG = 2,
      OFPBMC_BAD_DL_ADDR_MASK = 3,
      OFPBMC_BAD_NW_ADDR_MASK = 4,
      OFPBMC_BAD_WILDCARDS = 5,
      OFPBMC_BAD_FIELD = 6,
      OFPBMC_BAD_VALUE = 7,
      OFPBMC_BAD_MASK = 8,
      OFPBMC_BAD_PREREQ = 9,
      OFPBMC_DUP_FIELD = 10,
      OFPBMC_EPERM = 11,
    };

    enum ofp_flow_mod_failed_code
    {
      OFPFMFC_UNKNOWN = 0,
      OFPFMFC_TABLE_FULL = 1,
      OFPFMFC_BAD_TABLE_ID = 2,
      OFPFMFC_OVERLAP = 3,
      OFPFMFC_EPERM = 4,
      OFPFMFC_BAD_TIMEOUT = 5,
      OFPFMFC_BAD_COMMAND = 6,
      OFPFMFC_BAD_FLAGS = 7,
    };

    enum ofp_group_mod_failed_code
    {
      OFPGMFC_GROUP_EXISTS = 0,
      OFPGMFC_INVALID_GROUP = 1,
      OFPGMFC_WEIGHT_UNSUPPORTED = 2,
      OFPGMFC_OUT_OF_GROUPS = 3,
      OFPGMFC_OUT_OF_BUCKETS = 4,
      OFPGMFC_CHAINING_UNSUPPORTED = 5,
      OFPGMFC_WATCH_UNSUPPORTED = 6,
      OFPGMFC_LOOP = 7,
      OFPGMFC_UNKNOWN_GROUP = 8,
      OFPGMFC_CHAINED_GROUP = 9,
      OFPGMFC_BAD_TYPE = 10,
      OFPGMFC_BAD_COMMAND = 11,
      OFPGMFC_BAD_BUCKET = 12,
      OFPGMFC_BAD_WATCH = 13,
      OFPGMFC_EPERM = 14,
    };

    enum ofp_port_mod_failed_code
    {
      OFPPMFC_BAD_PORT = 0,
      OFPPMFC_BAD_HW_ADDR = 1,
      OFPPMFC_BAD_CONFIG = 2,
      OFPPMFC_BAD_ADVERTISE = 3,
      OFPPMFC_EPERM = 4,
    };

    enum ofp_table_mod_failed_code
    {
      OFPTMFC_BAD_TABLE = 0,
      OFPTMFC_BAD_CONFIG = 1,
      OFPTMFC_EPERM = 2,
    };

    enum ofp_queue_op_failed_code
    {
      OFPQOFC_BAD_PORT = 0,
      OFPQOFC_BAD_QUEUE = 1,
      OFPQOFC_EPERM = 2,
    };

    enum ofp_switch_config_failed_code
    {
      OFPSCFC_BAD_FLAGS = 0,
      OFPSCFC_BAD_LEN = 1,
      OFPSCFC_EPERM = 2,
    };

    enum ofp_role_request_failed_code
    {
      OFPRRFC_STALE = 0,
      OFPRRFC_UNSUP = 1,
      OFPRRFC_BAD_ROLE = 2,
    };

    enum ofp_meter_mod_failed_code
    {
      OFPMMFC_UNKNOWN = 0,
      OFPMMFC_METER_EXISTS = 1,
      OFPMMFC_INVALID_METER = 2,
      OFPMMFC_UNKNOWN_METER = 3,
      OFPMMFC_BAD_COMMAND = 4,
      OFPMMFC_BAD_FLAGS = 5,
      OFPMMFC_BAD_RATE = 6,
      OFPMMFC_BAD_BURST = 7,
      OFPMMFC_BAD_BAND = 8,
      OFPMMFC_BAD_BAND_VALUE = 9,
      OFPMMFC_OUT_OF_METERS = 10,
      OFPMMFC_OUT_OF_BANDS = 11,
    };

    enum ofp_table_features_failed_code
    {
      OFPTFFC_BAD_TABLE = 0,
      OFPTFFC_BAD_METADATA = 1,
      OFPTFFC_BAD_TYPE = 2,
      OFPTFFC_BAD_LEN = 3,
      OFPTFFC_BAD_ARGUMENT = 4,
      OFPTFFC_EPERM = 5,
    };

    enum ofp_multipart_type
    {
      OFPMP_DESC = 0,
      OFPMP_FLOW = 1,
      OFPMP_AGGREGATE = 2,
      OFPMP_TABLE = 3,
      OFPMP_PORT_STATS = 4,
      OFPMP_QUEUE = 5,
      OFPMP_GROUP = 6,
      OFPMP_GROUP_DESC = 7,
      OFPMP_GROUP_FEATURES = 8,
      OFPMP_METER = 9,
      OFPMP_METER_CONFIG = 10,
      OFPMP_METER_FEATURES = 11,
      OFPMP_TABLE_FEATURES = 12,
      OFPMP_PORT_DESC = 13,
      OFPMP_EXPERIMENTER = 65535,
    };

    enum ofp_multipart_request_flags
    {
      OFPMPF_REQ_MORE = 1,
    };

    enum ofp_multipart_reply_flags
    {
      OFPMPF_REPLY_MORE = 1,
    };

    enum ofp_table_feature_prop_type
    {
      OFPTFPT_INSTRUCTIONS = 0,
      OFPTFPT_INSTRUCTIONS_MISS = 1,
      OFPTFPT_NEXT_TABLES = 2,
      OFPTFPT_NEXT_TABLES_MISS = 3,
      OFPTFPT_WRITE_ACTIONS = 4,
      OFPTFPT_WRITE_ACTIONS_MISS = 5,
      OFPTFPT_APPLY_ACTIONS = 6,
      OFPTFPT_APPLY_ACTIONS_MISS = 7,
      OFPTFPT_MATCH = 8,
      OFPTFPT_WILDCARDS = 10,
      OFPTFPT_WRITE_SETFIELD = 12,
      OFPTFPT_WRITE_SETFIELD_MISS = 13,
      OFPTFPT_APPLY_SETFIELD = 14,
      OFPTFPT_APPLY_SETFIELD_MISS = 15,
      OFPTFPT_EXPERIMENTER = 65534,
      OFPTFPT_EXPERIMENTER_MISS = 65535,
    };

    enum ofp_group_capabilities
    {
      OFPGFC_SELECT_WEIGHT = 1,
      OFPGFC_SELECT_LIVENESS = 2,
      OFPGFC_CHAINING = 4,
      OFPGFC_CHAINING_CHECKS = 8,
    };

    enum ofp_queue_properties
    {
      OFPQT_MIN_RATE = 1,
      OFPQT_MAX_RATE = 2,
      OFPQT_EXPERIMENTER = 65535,
    };

    enum ofp_controller_role
    {
      OFPCR_ROLE_NOCHANGE = 0,
      OFPCR_ROLE_EQUAL = 1,
      OFPCR_ROLE_MASTER = 2,
      OFPCR_ROLE_SLAVE = 3,
    };

    namespace protocol_detail {

      struct port_no_def
      {
        using ofp_type = protocol::ofp_port_no;

        enum type : std::uint32_t
        {
          max = protocol::OFPP_MAX,
          in_port = protocol::OFPP_IN_PORT,
          table = protocol::OFPP_TABLE,
          normal = protocol::OFPP_NORMAL,
          flood = protocol::OFPP_FLOOD,
          all = protocol::OFPP_ALL,
          controller = protocol::OFPP_CONTROLLER,
          local = protocol::OFPP_LOCAL,
          any = protocol::OFPP_ANY,
        };
      };

      struct msg_type_def
      {
        using ofp_type = protocol::ofp_type;

        enum type : std::uint8_t
        {
          hello = protocol::OFPT_HELLO,
          error = protocol::OFPT_ERROR,
          echo_request = protocol::OFPT_ECHO_REQUEST,
          echo_reply = protocol::OFPT_ECHO_REPLY,
          experimenter = protocol::OFPT_EXPERIMENTER,
          features_request = protocol::OFPT_FEATURES_REQUEST,
          features_reply = protocol::OFPT_FEATURES_REPLY,
          get_config_request = protocol::OFPT_GET_CONFIG_REQUEST,
          get_config_reply = protocol::OFPT_GET_CONFIG_REPLY,
          set_config = protocol::OFPT_SET_CONFIG,
          packet_in = protocol::OFPT_PACKET_IN,
          flow_removed = protocol::OFPT_FLOW_REMOVED,
          port_status = protocol::OFPT_PORT_STATUS,
          packet_out = protocol::OFPT_PACKET_OUT,
          flow_mod = protocol::OFPT_FLOW_MOD,
          group_mod = protocol::OFPT_GROUP_MOD,
          port_mod = protocol::OFPT_PORT_MOD,
          table_mod = protocol::OFPT_TABLE_MOD,
          multipart_request = protocol::OFPT_MULTIPART_REQUEST,
          multipart_reply = protocol::OFPT_MULTIPART_REPLY,
          barrier_request = protocol::OFPT_BARRIER_REQUEST,
          barrier_reply = protocol::OFPT_BARRIER_REPLY,
          queue_get_config_request = protocol::OFPT_QUEUE_GET_CONFIG_REQUEST,
          queue_get_config_reply = protocol::OFPT_QUEUE_GET_CONFIG_REPLY,
          role_request = protocol::OFPT_ROLE_REQUEST,
          role_reply = protocol::OFPT_ROLE_REPLY,
          get_async_request = protocol::OFPT_GET_ASYNC_REQUEST,
          get_async_reply = protocol::OFPT_GET_ASYNC_REPLY,
          set_async = protocol::OFPT_SET_ASYNC,
          meter_mod = protocol::OFPT_METER_MOD,
        };
      };

      struct hello_elem_type_def
      {
        using ofp_type = protocol::ofp_hello_elem_type;

        enum type : std::uint16_t
        {
          versionbitmap = protocol::OFPHET_VERSIONBITMAP,
        };
      };

      struct config_flags_def
      {
        using ofp_type = protocol::ofp_config_flags;

        enum type : std::uint16_t
        {
          normal = protocol::OFPC_FRAG_NORMAL,
          drop = protocol::OFPC_FRAG_DROP,
          reasm = protocol::OFPC_FRAG_REASM,
          mask = protocol::OFPC_FRAG_MASK,
        };
      };

      struct table_config_def
      {
        using ofp_type = protocol::ofp_table_config;

        enum type : std::uint32_t
        {
          deprecated_mask = protocol::OFPTC_DEPRECATED_MASK,
        };
      };

      struct table_def
      {
        using ofp_type = protocol::ofp_table;

        enum type : std::uint8_t
        {
          max = protocol::OFPTT_MAX,
          all = protocol::OFPTT_ALL,
        };
      };

      struct capabilities_def
      {
        using ofp_type = protocol::ofp_capabilities;

        enum type : std::uint32_t
        {
          flow_stats = protocol::OFPC_FLOW_STATS,
          table_stats = protocol::OFPC_TABLE_STATS,
          port_stats = protocol::OFPC_PORT_STATS,
          group_stats = protocol::OFPC_GROUP_STATS,
          ip_reasm = protocol::OFPC_IP_REASM,
          queue_stats = protocol::OFPC_QUEUE_STATS,
          port_blocked = protocol::OFPC_PORT_BLOCKED,
        };
      };

      struct port_config_def
      {
        using ofp_type = protocol::ofp_port_config;

        enum type : std::uint32_t
        {
          port_down = protocol::OFPPC_PORT_DOWN,
          no_recv = protocol::OFPPC_NO_RECV,
          no_fwd = protocol::OFPPC_NO_FWD,
          no_packet_in = protocol::OFPPC_NO_PACKET_IN,
        };
      };

      struct port_state_def
      {
        using ofp_type = protocol::ofp_port_state;

        enum type : std::uint32_t
        {
          link_down = protocol::OFPPS_LINK_DOWN,
          blocked = protocol::OFPPS_BLOCKED,
          live = protocol::OFPPS_LIVE,
        };
      };

      struct port_features_def
      {
        using ofp_type = protocol::ofp_port_features;

        enum type : std::uint32_t
        {
          mode_10mb_hd = protocol::OFPPF_10MB_HD,
          mode_10mb_fd = protocol::OFPPF_10MB_FD,
          mode_100mb_hd = protocol::OFPPF_100MB_HD,
          mode_100mb_fd = protocol::OFPPF_100MB_FD,
          mode_1gb_hd = protocol::OFPPF_1GB_HD,
          mode_1gb_fd = protocol::OFPPF_1GB_FD,
          mode_10gb_fd = protocol::OFPPF_10GB_FD,
          mode_40gb_fd = protocol::OFPPF_40GB_FD,
          mode_100gb_fd = protocol::OFPPF_100GB_FD,
          mode_1tb_fd = protocol::OFPPF_1TB_FD,
          mode_other = protocol::OFPPF_OTHER,
          copper = protocol::OFPPF_COPPER,
          fiber = protocol::OFPPF_FIBER,
          autoneg = protocol::OFPPF_AUTONEG,
          pause = protocol::OFPPF_PAUSE,
          pause_asym = protocol::OFPPF_PAUSE_ASYM,
        };
      };

      struct port_reason_def
      {
        using ofp_type = protocol::ofp_port_reason;

        enum type : std::uint8_t
        {
          by_add = protocol::OFPPR_ADD,
          by_delete = protocol::OFPPR_DELETE,
          by_modify = protocol::OFPPR_MODIFY,
        };
      };

      struct match_type_def
      {
        using ofp_type = protocol::ofp_match_type;

        enum type : std::uint16_t
        {
          standard = protocol::OFPMT_STANDARD,
          oxm = protocol::OFPMT_OXM,
        };
      };

      struct oxm_class_def
      {
        using ofp_type = protocol::ofp_oxm_class;

        enum type : std::uint16_t
        {
          nxm_0 = protocol::OFPXMC_NXM_0,
          nxm_1 = protocol::OFPXMC_NXM_1,
          openflow_basic = protocol::OFPXMC_OPENFLOW_BASIC,
          experimenter = protocol::OFPXMC_EXPERIMENTER,
        };
      };

      struct ofb_match_fields_def
      {
        using ofp_type = protocol::oxm_ofb_match_fields;

        enum type : std::uint8_t
        {
          in_port = protocol::OFPXMT_OFB_IN_PORT,
          in_phy_port = protocol::OFPXMT_OFB_IN_PHY_PORT,
          metadata = protocol::OFPXMT_OFB_METADATA,
          eth_dst = protocol::OFPXMT_OFB_ETH_DST,
          eth_src = protocol::OFPXMT_OFB_ETH_SRC,
          eth_type = protocol::OFPXMT_OFB_ETH_TYPE,
          vlan_vid = protocol::OFPXMT_OFB_VLAN_VID,
          vlan_pcp = protocol::OFPXMT_OFB_VLAN_PCP,
          ip_dscp = protocol::OFPXMT_OFB_IP_DSCP,
          ip_ecn = protocol::OFPXMT_OFB_IP_ECN,
          ip_proto = protocol::OFPXMT_OFB_IP_PROTO,
          ipv4_src = protocol::OFPXMT_OFB_IPV4_SRC,
          ipv4_dst = protocol::OFPXMT_OFB_IPV4_DST,
          tcp_src = protocol::OFPXMT_OFB_TCP_SRC,
          tcp_dst = protocol::OFPXMT_OFB_TCP_DST,
          udp_src = protocol::OFPXMT_OFB_UDP_SRC,
          udp_dst = protocol::OFPXMT_OFB_UDP_DST,
          sctp_src = protocol::OFPXMT_OFB_SCTP_SRC,
          sctp_dst = protocol::OFPXMT_OFB_SCTP_DST,
          icmpv4_type = protocol::OFPXMT_OFB_ICMPV4_TYPE,
          icmpv4_code = protocol::OFPXMT_OFB_ICMPV4_CODE,
          arp_op = protocol::OFPXMT_OFB_ARP_OP,
          arp_spa = protocol::OFPXMT_OFB_ARP_SPA,
          arp_tpa = protocol::OFPXMT_OFB_ARP_TPA,
          arp_sha = protocol::OFPXMT_OFB_ARP_SHA,
          arp_tha = protocol::OFPXMT_OFB_ARP_THA,
          ipv6_src = protocol::OFPXMT_OFB_IPV6_SRC,
          ipv6_dst = protocol::OFPXMT_OFB_IPV6_DST,
          ipv6_flabel = protocol::OFPXMT_OFB_IPV6_FLABEL,
          icmpv6_type = protocol::OFPXMT_OFB_ICMPV6_TYPE,
          icmpv6_code = protocol::OFPXMT_OFB_ICMPV6_CODE,
          ipv6_nd_target = protocol::OFPXMT_OFB_IPV6_ND_TARGET,
          ipv6_nd_sll = protocol::OFPXMT_OFB_IPV6_ND_SLL,
          ipv6_nd_tll = protocol::OFPXMT_OFB_IPV6_ND_TLL,
          mpls_label = protocol::OFPXMT_OFB_MPLS_LABEL,
          mpls_tc = protocol::OFPXMT_OFB_MPLS_TC,
          mpls_bos = protocol::OFPXMT_OFB_MPLS_BOS,
          pbb_isid = protocol::OFPXMT_OFB_PBB_ISID,
          tunnel_id = protocol::OFPXMT_OFB_TUNNEL_ID,
          ipv6_exthdr = protocol::OFPXMT_OFB_IPV6_EXTHDR,
        };
      };

      struct vlan_id_def
      {
        using ofp_type = protocol::ofp_vlan_id;

        enum type : std::uint16_t
        {
          present = protocol::OFPVID_PRESENT,
          none = protocol::OFPVID_NONE,
        };
      };

      struct ipv6exthdr_flags_def
      {
        using ofp_type = protocol::ofp_ipv6exthdr_flags;

        enum type : std::uint16_t
        {
          nonext = protocol::OFPIEH_NONEXT,
          esp = protocol::OFPIEH_ESP,
          auth = protocol::OFPIEH_AUTH,
          dest = protocol::OFPIEH_DEST,
          frag = protocol::OFPIEH_FRAG,
          router = protocol::OFPIEH_ROUTER,
          hop = protocol::OFPIEH_HOP,
          unrep = protocol::OFPIEH_UNREP,
          unseq = protocol::OFPIEH_UNSEQ,
        };
      };

      struct action_type_def
      {
        using ofp_type = protocol::ofp_action_type;

        enum type : std::uint16_t
        {
          output = protocol::OFPAT_OUTPUT,
          copy_ttl_out = protocol::OFPAT_COPY_TTL_OUT,
          copy_ttl_in = protocol::OFPAT_COPY_TTL_IN,
          set_mpls_ttl = protocol::OFPAT_SET_MPLS_TTL,
          dec_mpls_ttl = protocol::OFPAT_DEC_MPLS_TTL,
          push_vlan = protocol::OFPAT_PUSH_VLAN,
          pop_vlan = protocol::OFPAT_POP_VLAN,
          push_mpls = protocol::OFPAT_PUSH_MPLS,
          pop_mpls = protocol::OFPAT_POP_MPLS,
          set_queue = protocol::OFPAT_SET_QUEUE,
          group = protocol::OFPAT_GROUP,
          set_nw_ttl = protocol::OFPAT_SET_NW_TTL,
          dec_nw_ttl = protocol::OFPAT_DEC_NW_TTL,
          set_field = protocol::OFPAT_SET_FIELD,
          push_pbb = protocol::OFPAT_PUSH_PBB,
          pop_pbb = protocol::OFPAT_POP_PBB,
          experimenter = protocol::OFPAT_EXPERIMENTER,
        };
      };

      struct controller_max_len_def
      {
        using ofp_type = protocol::ofp_controller_max_len;

        enum type : std::uint16_t
        {
          max = protocol::OFPCML_MAX,
          no_buffer = protocol::OFPCML_NO_BUFFER,
        };
      };

      struct instruction_type_def
      {
        using ofp_type = protocol::ofp_instruction_type;

        enum type : std::uint16_t
        {
          goto_table = protocol::OFPIT_GOTO_TABLE,
          write_metadata = protocol::OFPIT_WRITE_METADATA,
          write_actions = protocol::OFPIT_WRITE_ACTIONS,
          apply_actions = protocol::OFPIT_APPLY_ACTIONS,
          clear_actions = protocol::OFPIT_CLEAR_ACTIONS,
          meter = protocol::OFPIT_METER,
          experimenter = protocol::OFPIT_EXPERIMENTER,
        };
      };

      struct flow_mod_command_def
      {
        using ofp_type = protocol::ofp_flow_mod_command;

        enum type : std::uint16_t
        {
          add_cmd = protocol::OFPFC_ADD,
          modify_cmd = protocol::OFPFC_MODIFY,
          modify_strict_cmd = protocol::OFPFC_MODIFY_STRICT,
          delete_cmd = protocol::OFPFC_DELETE,
          delete_strict_cmd = protocol::OFPFC_DELETE_STRICT,
        };
      };

      struct flow_mod_flags_def
      {
        using ofp_type = protocol::ofp_flow_mod_flags;

        enum type : std::uint16_t
        {
          send_flow_rem = protocol::OFPFF_SEND_FLOW_REM,
          check_overlap = protocol::OFPFF_CHECK_OVERLAP,
          reset_counts = protocol::OFPFF_RESET_COUNTS,
          no_pkt_counts = protocol::OFPFF_NO_PKT_COUNTS,
          no_byt_counts = protocol::OFPFF_NO_BYT_COUNTS,
        };
      };

      struct group_def
      {
        using ofp_type = protocol::ofp_group;

        enum type : std::uint32_t
        {
          max = protocol::OFPG_MAX,
          all = protocol::OFPG_ALL,
          any = protocol::OFPG_ANY,
        };
      };

      struct group_mod_command_def
      {
        using ofp_type = protocol::ofp_group_mod_command;

        enum type : std::uint8_t
        {
          add_cmd = protocol::OFPGC_ADD,
          modify_cmd = protocol::OFPGC_MODIFY,
          delete_cmd = protocol::OFPGC_DELETE,
        };
      };

      struct group_type_def
      {
        using ofp_type = protocol::ofp_group_type;

        enum type : std::uint8_t
        {
          all = protocol::OFPGT_ALL,
          select = protocol::OFPGT_SELECT,
          indirect = protocol::OFPGT_INDIRECT,
          ff = protocol::OFPGT_FF,
        };
      };

      struct packet_in_reason_def
      {
        using ofp_type = protocol::ofp_packet_in_reason;

        enum type : std::uint8_t
        {
          no_match = protocol::OFPR_NO_MATCH,
          action = protocol::OFPR_ACTION,
          invalid_ttl = protocol::OFPR_INVALID_TTL,
        };
      };

      struct flow_removed_reason_def
      {
        using ofp_type = protocol::ofp_flow_removed_reason;

        enum type : std::uint8_t
        {
          by_idle_timeout = protocol::OFPRR_IDLE_TIMEOUT,
          by_hard_timeout = protocol::OFPRR_HARD_TIMEOUT,
          by_delete = protocol::OFPRR_DELETE,
          by_group_delete = protocol::OFPRR_GROUP_DELETE,
        };
      };

      struct meter_def
      {
        using ofp_type = protocol::ofp_meter;

        enum type : std::uint32_t
        {
          max = protocol::OFPM_MAX,
          slowpath = protocol::OFPM_SLOWPATH,
          controller = protocol::OFPM_CONTROLLER,
          all = protocol::OFPM_ALL,
        };
      };

      struct meter_band_type_def
      {
        using ofp_type = protocol::ofp_meter_band_type;

        enum type : std::uint16_t
        {
          drop = protocol::OFPMBT_DROP,
          dscp_remark = protocol::OFPMBT_DSCP_REMARK,
          experimenter = protocol::OFPMBT_EXPERIMENTER,
        };
      };

      struct meter_mod_command_def
      {
        using ofp_type = protocol::ofp_meter_mod_command;

        enum type : std::uint16_t
        {
          add_cmd = protocol::OFPMC_ADD,
          modify_cmd = protocol::OFPMC_MODIFY,
          delete_cmd = protocol::OFPMC_DELETE,
        };
      };

      struct meter_flags_def
      {
        using ofp_type = protocol::ofp_meter_flags;

        enum type : std::uint16_t
        {
          kbps = protocol::OFPMF_KBPS,
          pktps = protocol::OFPMF_PKTPS,
          burst = protocol::OFPMF_BURST,
          stats = protocol::OFPMF_STATS,
        };
      };

      struct error_type_def
      {
        using ofp_type = protocol::ofp_error_type;

        enum type : std::uint16_t
        {
          hello_failed = protocol::OFPET_HELLO_FAILED,
          bad_request = protocol::OFPET_BAD_REQUEST,
          bad_action = protocol::OFPET_BAD_ACTION,
          bad_instruction = protocol::OFPET_BAD_INSTRUCTION,
          bad_match = protocol::OFPET_BAD_MATCH,
          flow_mod_failed = protocol::OFPET_FLOW_MOD_FAILED,
          group_mod_failed = protocol::OFPET_GROUP_MOD_FAILED,
          port_mod_failed = protocol::OFPET_PORT_MOD_FAILED,
          table_mod_failed = protocol::OFPET_TABLE_MOD_FAILED,
          queue_op_failed = protocol::OFPET_QUEUE_OP_FAILED,
          switch_config_failed = protocol::OFPET_SWITCH_CONFIG_FAILED,
          role_request_failed = protocol::OFPET_ROLE_REQUEST_FAILED,
          meter_mod_failed = protocol::OFPET_METER_MOD_FAILED,
          table_features_failed = protocol::OFPET_TABLE_FEATURES_FAILED,
          experimenter = protocol::OFPET_EXPERIMENTER,
        };
      };

      struct hello_failed_code_def
      {
        using ofp_type = protocol::ofp_hello_failed_code;

        enum type : std::uint16_t
        {
          incompatible = protocol::OFPHFC_INCOMPATIBLE,
          eperm = protocol::OFPHFC_EPERM,
        };
      };

      struct bad_request_code_def
      {
        using ofp_type = protocol::ofp_bad_request_code;

        enum type : std::uint16_t
        {
          bad_version = protocol::OFPBRC_BAD_VERSION,
          bad_type = protocol::OFPBRC_BAD_TYPE,
          bad_multipart = protocol::OFPBRC_BAD_MULTIPART,
          bad_experimenter = protocol::OFPBRC_BAD_EXPERIMENTER,
          bad_exp_type = protocol::OFPBRC_BAD_EXP_TYPE,
          eperm = protocol::OFPBRC_EPERM,
          bad_len = protocol::OFPBRC_BAD_LEN,
          buffer_empty = protocol::OFPBRC_BUFFER_EMPTY,
          buffer_unknown = protocol::OFPBRC_BUFFER_UNKNOWN,
          bad_table_id = protocol::OFPBRC_BAD_TABLE_ID,
          is_slave = protocol::OFPBRC_IS_SLAVE,
          bad_port = protocol::OFPBRC_BAD_PORT,
          bad_packet = protocol::OFPBRC_BAD_PACKET,
          multipart_buffer_overflow = protocol::OFPBRC_MULTIPART_BUFFER_OVERFLOW,
        };
      };

      struct bad_action_code_def
      {
        using ofp_type = protocol::ofp_bad_action_code;

        enum type : std::uint16_t
        {
          bad_type = protocol::OFPBAC_BAD_TYPE,
          bad_len = protocol::OFPBAC_BAD_LEN,
          bad_experimenter = protocol::OFPBAC_BAD_EXPERIMENTER,
          bad_exp_type = protocol::OFPBAC_BAD_EXP_TYPE,
          bad_out_port = protocol::OFPBAC_BAD_OUT_PORT,
          bad_argument = protocol::OFPBAC_BAD_ARGUMENT,
          eperm = protocol::OFPBAC_EPERM,
          too_many = protocol::OFPBAC_TOO_MANY,
          bad_queue = protocol::OFPBAC_BAD_QUEUE,
          bad_out_group = protocol::OFPBAC_BAD_OUT_GROUP,
          match_inconsistent = protocol::OFPBAC_MATCH_INCONSISTENT,
          unsupported_order = protocol::OFPBAC_UNSUPPORTED_ORDER,
          bad_tag = protocol::OFPBAC_BAD_TAG,
          bad_set_type = protocol::OFPBAC_BAD_SET_TYPE,
          bad_set_len = protocol::OFPBAC_BAD_SET_LEN,
          bad_set_argument = protocol::OFPBAC_BAD_SET_ARGUMENT,
        };
      };

      struct bad_instruction_code_def
      {
        using ofp_type = protocol::ofp_bad_instruction_code;

        enum type : std::uint16_t
        {
          unknown_inst = protocol::OFPBIC_UNKNOWN_INST,
          unsup_inst = protocol::OFPBIC_UNSUP_INST,
          bad_table_id = protocol::OFPBIC_BAD_TABLE_ID,
          unsup_metadata = protocol::OFPBIC_UNSUP_METADATA,
          unsup_metadata_mask = protocol::OFPBIC_UNSUP_METADATA_MASK,
          bad_experimenter = protocol::OFPBIC_BAD_EXPERIMENTER,
          bad_exp_type = protocol::OFPBIC_BAD_EXP_TYPE,
          bad_len = protocol::OFPBIC_BAD_LEN,
          eperm = protocol::OFPBIC_EPERM,
        };
      };

      struct bad_match_code_def
      {
        using ofp_type = protocol::ofp_bad_match_code;

        enum type : std::uint16_t
        {
          bad_type = protocol::OFPBMC_BAD_TYPE,
          bad_len = protocol::OFPBMC_BAD_LEN,
          bad_tag = protocol::OFPBMC_BAD_TAG,
          bad_dl_addr_mask = protocol::OFPBMC_BAD_DL_ADDR_MASK,
          bad_nw_addr_mask = protocol::OFPBMC_BAD_NW_ADDR_MASK,
          bad_wildcards = protocol::OFPBMC_BAD_WILDCARDS,
          bad_field = protocol::OFPBMC_BAD_FIELD,
          bad_value = protocol::OFPBMC_BAD_VALUE,
          bad_mask = protocol::OFPBMC_BAD_MASK,
          bad_prereq = protocol::OFPBMC_BAD_PREREQ,
          dup_field = protocol::OFPBMC_DUP_FIELD,
          eperm = protocol::OFPBMC_EPERM,
        };
      };

      struct flow_mod_failed_code_def
      {
        using ofp_type = protocol::ofp_flow_mod_failed_code;

        enum type : std::uint16_t
        {
          unknown = protocol::OFPFMFC_UNKNOWN,
          table_full = protocol::OFPFMFC_TABLE_FULL,
          bad_table_id = protocol::OFPFMFC_BAD_TABLE_ID,
          overlap = protocol::OFPFMFC_OVERLAP,
          eperm = protocol::OFPFMFC_EPERM,
          bad_timeout = protocol::OFPFMFC_BAD_TIMEOUT,
          bad_command = protocol::OFPFMFC_BAD_COMMAND,
          bad_flags = protocol::OFPFMFC_BAD_FLAGS,
        };
      };

      struct group_mod_failed_code_def
      {
        using ofp_type = protocol::ofp_group_mod_failed_code;

        enum type : std::uint16_t
        {
          group_exists = protocol::OFPGMFC_GROUP_EXISTS,
          invalid_group = protocol::OFPGMFC_INVALID_GROUP,
          weight_unsupported = protocol::OFPGMFC_WEIGHT_UNSUPPORTED,
          out_of_groups = protocol::OFPGMFC_OUT_OF_GROUPS,
          out_of_buckets = protocol::OFPGMFC_OUT_OF_BUCKETS,
          chaining_unsupported = protocol::OFPGMFC_CHAINING_UNSUPPORTED,
          watch_unsupported = protocol::OFPGMFC_WATCH_UNSUPPORTED,
          loop = protocol::OFPGMFC_LOOP,
          unknown_group = protocol::OFPGMFC_UNKNOWN_GROUP,
          chained_group = protocol::OFPGMFC_CHAINED_GROUP,
          bad_type = protocol::OFPGMFC_BAD_TYPE,
          bad_command = protocol::OFPGMFC_BAD_COMMAND,
          bad_bucket = protocol::OFPGMFC_BAD_BUCKET,
          bad_watch = protocol::OFPGMFC_BAD_WATCH,
          eperm = protocol::OFPGMFC_EPERM,
        };
      };

      struct port_mod_failed_code_def
      {
        using ofp_type = protocol::ofp_port_mod_failed_code;

        enum type : std::uint16_t
        {
          bad_port = protocol::OFPPMFC_BAD_PORT,
          bad_hw_addr = protocol::OFPPMFC_BAD_HW_ADDR,
          bad_config = protocol::OFPPMFC_BAD_CONFIG,
          bad_advertise = protocol::OFPPMFC_BAD_ADVERTISE,
          eperm = protocol::OFPPMFC_EPERM,
        };
      };

      struct table_mod_failed_code_def
      {
        using ofp_type = protocol::ofp_table_mod_failed_code;

        enum type : std::uint16_t
        {
          bad_table = protocol::OFPTMFC_BAD_TABLE,
          bad_config = protocol::OFPTMFC_BAD_CONFIG,
          eperm = protocol::OFPTMFC_EPERM,
        };
      };

      struct queue_op_failed_code_def
      {
        using ofp_type = protocol::ofp_queue_op_failed_code;

        enum type : std::uint16_t
        {
          bad_port = protocol::OFPQOFC_BAD_PORT,
          bad_queue = protocol::OFPQOFC_BAD_QUEUE,
          eperm = protocol::OFPQOFC_EPERM,
        };
      };

      struct switch_config_failed_code_def
      {
        using ofp_type = protocol::ofp_switch_config_failed_code;

        enum type : std::uint16_t
        {
          bad_flags = protocol::OFPSCFC_BAD_FLAGS,
          bad_len = protocol::OFPSCFC_BAD_LEN,
          eperm = protocol::OFPSCFC_EPERM,
        };
      };

      struct role_request_failed_code_def
      {
        using ofp_type = protocol::ofp_role_request_failed_code;

        enum type : std::uint16_t
        {
          stale = protocol::OFPRRFC_STALE,
          unsup = protocol::OFPRRFC_UNSUP,
          bad_role = protocol::OFPRRFC_BAD_ROLE,
        };
      };

      struct meter_mod_failed_code_def
      {
        using ofp_type = protocol::ofp_meter_mod_failed_code;

        enum type : std::uint16_t
        {
          unknown = protocol::OFPMMFC_UNKNOWN,
          meter_exists = protocol::OFPMMFC_METER_EXISTS,
          invalid_meter = protocol::OFPMMFC_INVALID_METER,
          unknown_meter = protocol::OFPMMFC_UNKNOWN_METER,
          bad_command = protocol::OFPMMFC_BAD_COMMAND,
          bad_flags = protocol::OFPMMFC_BAD_FLAGS,
          bad_rate = protocol::OFPMMFC_BAD_RATE,
          bad_burst = protocol::OFPMMFC_BAD_BURST,
          bad_band = protocol::OFPMMFC_BAD_BAND,
          bad_band_value = protocol::OFPMMFC_BAD_BAND_VALUE,
          out_of_meters = protocol::OFPMMFC_OUT_OF_METERS,
          out_of_bands = protocol::OFPMMFC_OUT_OF_BANDS,
        };
      };

      struct table_features_failed_code_def
      {
        using ofp_type = protocol::ofp_table_features_failed_code;

        enum type : std::uint16_t
        {
          bad_table = protocol::OFPTFFC_BAD_TABLE,
          bad_metadata = protocol::OFPTFFC_BAD_METADATA,
          bad_type = protocol::OFPTFFC_BAD_TYPE,
          bad_len = protocol::OFPTFFC_BAD_LEN,
          bad_argument = protocol::OFPTFFC_BAD_ARGUMENT,
          eperm = protocol::OFPTFFC_EPERM,
        };
      };

      struct multipart_type_def
      {
        using ofp_type = protocol::ofp_multipart_type;

        enum type : std::uint16_t
        {
          desc = protocol::OFPMP_DESC,
          flow = protocol::OFPMP_FLOW,
          aggregate = protocol::OFPMP_AGGREGATE,
          table = protocol::OFPMP_TABLE,
          port_stats = protocol::OFPMP_PORT_STATS,
          queue = protocol::OFPMP_QUEUE,
          group = protocol::OFPMP_GROUP,
          group_desc = protocol::OFPMP_GROUP_DESC,
          group_features = protocol::OFPMP_GROUP_FEATURES,
          meter = protocol::OFPMP_METER,
          meter_config = protocol::OFPMP_METER_CONFIG,
          meter_features = protocol::OFPMP_METER_FEATURES,
          table_features = protocol::OFPMP_TABLE_FEATURES,
          port_desc = protocol::OFPMP_PORT_DESC,
          experimenter = protocol::OFPMP_EXPERIMENTER,
        };
      };

      struct multipart_request_flags_def
      {
        using ofp_type = protocol::ofp_multipart_request_flags;

        enum type : std::uint16_t
        {
          req_more = protocol::OFPMPF_REQ_MORE,
        };
      };

      struct multipart_reply_flags_def
      {
        using ofp_type = protocol::ofp_multipart_reply_flags;

        enum type : std::uint16_t
        {
          reply_more = protocol::OFPMPF_REPLY_MORE,
        };
      };

      struct table_feature_prop_type_def
      {
        using ofp_type = protocol::ofp_table_feature_prop_type;

        enum type : std::uint16_t
        {
          instructions = protocol::OFPTFPT_INSTRUCTIONS,
          instructions_miss = protocol::OFPTFPT_INSTRUCTIONS_MISS,
          next_tables = protocol::OFPTFPT_NEXT_TABLES,
          next_tables_miss = protocol::OFPTFPT_NEXT_TABLES_MISS,
          write_actions = protocol::OFPTFPT_WRITE_ACTIONS,
          write_actions_miss = protocol::OFPTFPT_WRITE_ACTIONS_MISS,
          apply_actions = protocol::OFPTFPT_APPLY_ACTIONS,
          apply_actions_miss = protocol::OFPTFPT_APPLY_ACTIONS_MISS,
          match = protocol::OFPTFPT_MATCH,
          wildcards = protocol::OFPTFPT_WILDCARDS,
          write_setfield = protocol::OFPTFPT_WRITE_SETFIELD,
          write_setfield_miss = protocol::OFPTFPT_WRITE_SETFIELD_MISS,
          apply_setfield = protocol::OFPTFPT_APPLY_SETFIELD,
          apply_setfield_miss = protocol::OFPTFPT_APPLY_SETFIELD_MISS,
          experimenter = protocol::OFPTFPT_EXPERIMENTER,
          experimenter_miss = protocol::OFPTFPT_EXPERIMENTER_MISS,
        };
      };

      struct group_capabilities_def
      {
        using ofp_type = protocol::ofp_group_capabilities;

        enum type : std::uint32_t
        {
          select_weight = protocol::OFPGFC_SELECT_WEIGHT,
          select_liveness = protocol::OFPGFC_SELECT_LIVENESS,
          chaining = protocol::OFPGFC_CHAINING,
          chaining_checks = protocol::OFPGFC_CHAINING_CHECKS,
        };
      };

      struct queue_prop_type_def
      {
        using ofp_type = protocol::ofp_queue_properties;

        enum type : std::uint16_t
        {
          min_rate = protocol::OFPQT_MIN_RATE,
          max_rate = protocol::OFPQT_MAX_RATE,
          experimenter = protocol::OFPQT_EXPERIMENTER,
        };
      };

      struct controller_role_def
      {
        using ofp_type = protocol::ofp_controller_role;

        enum type : std::uint32_t
        {
          nochange = protocol::OFPCR_ROLE_NOCHANGE,
          equal = protocol::OFPCR_ROLE_EQUAL,
          master = protocol::OFPCR_ROLE_MASTER,
          slave = protocol::OFPCR_ROLE_SLAVE,
        };
      };

    } // namespace protocol_detail

    template <class T>
    class scoped_enum : public T
    {
      using type = typename T::type;
      type value_;
    public:
      constexpr scoped_enum() noexcept : value_{} {}
      constexpr scoped_enum(type v) noexcept : value_(v) {}
      explicit constexpr scoped_enum(
        typename std::underlying_type<type>::type v) noexcept
        : value_(type(v)) {}
      constexpr operator type() const noexcept { return value_; }
    };

    using port_no = scoped_enum<protocol_detail::port_no_def>;
    using msg_type = scoped_enum<protocol_detail::msg_type_def>;
    using hello_elem_type = scoped_enum<protocol_detail::hello_elem_type_def>;
    using config_flags = scoped_enum<protocol_detail::config_flags_def>;
    using table_config = scoped_enum<protocol_detail::table_config_def>;
    using table = scoped_enum<protocol_detail::table_def>;
    using capabilities = scoped_enum<protocol_detail::capabilities_def>;
    using port_config = scoped_enum<protocol_detail::port_config_def>;
    using port_state = scoped_enum<protocol_detail::port_state_def>;
    using port_features = scoped_enum<protocol_detail::port_features_def>;
    using port_reason = scoped_enum<protocol_detail::port_reason_def>;
    using match_type = scoped_enum<protocol_detail::match_type_def>;
    using oxm_class = scoped_enum<protocol_detail::oxm_class_def>;
    using ofb_match_fields = scoped_enum<protocol_detail::ofb_match_fields_def>;
    using vlan_id = scoped_enum<protocol_detail::vlan_id_def>;
    using ipv6exthdr_flags = scoped_enum<protocol_detail::ipv6exthdr_flags_def>;
    using action_type = scoped_enum<protocol_detail::action_type_def>;
    using controller_max_len = scoped_enum<protocol_detail::controller_max_len_def>;
    using instruction_type = scoped_enum<protocol_detail::instruction_type_def>;
    using flow_mod_command = scoped_enum<protocol_detail::flow_mod_command_def>;
    using flow_mod_flags = scoped_enum<protocol_detail::flow_mod_flags_def>;
    using group = scoped_enum<protocol_detail::group_def>;
    using group_mod_command = scoped_enum<protocol_detail::group_mod_command_def>;
    using group_type = scoped_enum<protocol_detail::group_type_def>;
    using packet_in_reason = scoped_enum<protocol_detail::packet_in_reason_def>;
    using flow_removed_reason = scoped_enum<protocol_detail::flow_removed_reason_def>;
    using meter = scoped_enum<protocol_detail::meter_def>;
    using meter_band_type = scoped_enum<protocol_detail::meter_band_type_def>;
    using meter_mod_command = scoped_enum<protocol_detail::meter_mod_command_def>;
    using meter_flags = scoped_enum<protocol_detail::meter_flags_def>;
    using error_type = scoped_enum<protocol_detail::error_type_def>;
    using hello_failed_code = scoped_enum<protocol_detail::hello_failed_code_def>;
    using bad_request_code = scoped_enum<protocol_detail::bad_request_code_def>;
    using bad_action_code = scoped_enum<protocol_detail::bad_action_code_def>;
    using bad_instruction_code = scoped_enum<protocol_detail::bad_instruction_code_def>;
    using bad_match_code = scoped_enum<protocol_detail::bad_match_code_def>;
    using flow_mod_failed_code = scoped_enum<protocol_detail::flow_mod_failed_code_def>;
    using group_mod_failed_code = scoped_enum<protocol_detail::group_mod_failed_code_def>;
    using port_mod_failed_code = scoped_enum<protocol_detail::port_mod_failed_code_def>;
    using table_mod_failed_code = scoped_enum<protocol_detail::table_mod_failed_code_def>;
    using queue_op_failed_code = scoped_enum<protocol_detail::queue_op_failed_code_def>;
    using switch_config_failed_code = scoped_enum<protocol_detail::switch_config_failed_code_def>;
    using role_request_failed_code = scoped_enum<protocol_detail::role_request_failed_code_def>;
    using meter_mod_failed_code = scoped_enum<protocol_detail::meter_mod_failed_code_def>;
    using table_features_failed_code = scoped_enum<protocol_detail::table_features_failed_code_def>;
    using multipart_type = scoped_enum<protocol_detail::multipart_type_def>;
    using multipart_request_flags = scoped_enum<protocol_detail::multipart_request_flags_def>;
    using multipart_reply_flags = scoped_enum<protocol_detail::multipart_reply_flags_def>;
    using table_feature_prop_type = scoped_enum<protocol_detail::table_feature_prop_type_def>;
    using group_capabilities = scoped_enum<protocol_detail::group_capabilities_def>;
    using queue_prop_type = scoped_enum<protocol_detail::queue_prop_type_def>;
    using controller_role = scoped_enum<protocol_detail::controller_role_def>;

  } // namespace protocol

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OPENFLOW_HPP
