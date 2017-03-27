#ifndef CANARD_NET_OFP_V13_OPENFLOW_HPP
#define CANARD_NET_OFP_V13_OPENFLOW_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace protocol {

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

  namespace ofp_port_no_detail {

    enum ofp_port_no // : std::uint32_t
    {
      OFPP_MAX = 4294967040,
      max = OFPP_MAX,
      OFPP_IN_PORT = 4294967288,
      in_port = OFPP_IN_PORT,
      OFPP_TABLE = 4294967289,
      table = OFPP_TABLE,
      OFPP_NORMAL = 4294967290,
      normal = OFPP_NORMAL,
      OFPP_FLOOD = 4294967291,
      flood = OFPP_FLOOD,
      OFPP_ALL = 4294967292,
      all = OFPP_ALL,
      OFPP_CONTROLLER = 4294967293,
      controller = OFPP_CONTROLLER,
      OFPP_LOCAL = 4294967294,
      local = OFPP_LOCAL,
      OFPP_ANY = 4294967295,
      any = OFPP_ANY,
    };

  } // namespace ofp_port_no_detail

  using ofp_port_no_detail::ofp_port_no;
  using ofp_port_no_detail::ofp_port_no::OFPP_MAX;
  using ofp_port_no_detail::ofp_port_no::OFPP_IN_PORT;
  using ofp_port_no_detail::ofp_port_no::OFPP_TABLE;
  using ofp_port_no_detail::ofp_port_no::OFPP_NORMAL;
  using ofp_port_no_detail::ofp_port_no::OFPP_FLOOD;
  using ofp_port_no_detail::ofp_port_no::OFPP_ALL;
  using ofp_port_no_detail::ofp_port_no::OFPP_CONTROLLER;
  using ofp_port_no_detail::ofp_port_no::OFPP_LOCAL;
  using ofp_port_no_detail::ofp_port_no::OFPP_ANY;

  using port_no = ofp_port_no_detail::ofp_port_no;

  namespace ofp_type_detail {

    enum ofp_type // : std::uint8_t
    {
      OFPT_HELLO = 0,
      hello = OFPT_HELLO,
      OFPT_ERROR = 1,
      error = OFPT_ERROR,
      OFPT_ECHO_REQUEST = 2,
      echo_request = OFPT_ECHO_REQUEST,
      OFPT_ECHO_REPLY = 3,
      echo_reply = OFPT_ECHO_REPLY,
      OFPT_EXPERIMENTER = 4,
      experimenter = OFPT_EXPERIMENTER,
      OFPT_FEATURES_REQUEST = 5,
      features_request = OFPT_FEATURES_REQUEST,
      OFPT_FEATURES_REPLY = 6,
      features_reply = OFPT_FEATURES_REPLY,
      OFPT_GET_CONFIG_REQUEST = 7,
      get_config_request = OFPT_GET_CONFIG_REQUEST,
      OFPT_GET_CONFIG_REPLY = 8,
      get_config_reply = OFPT_GET_CONFIG_REPLY,
      OFPT_SET_CONFIG = 9,
      set_config = OFPT_SET_CONFIG,
      OFPT_PACKET_IN = 10,
      packet_in = OFPT_PACKET_IN,
      OFPT_FLOW_REMOVED = 11,
      flow_removed = OFPT_FLOW_REMOVED,
      OFPT_PORT_STATUS = 12,
      port_status = OFPT_PORT_STATUS,
      OFPT_PACKET_OUT = 13,
      packet_out = OFPT_PACKET_OUT,
      OFPT_FLOW_MOD = 14,
      flow_mod = OFPT_FLOW_MOD,
      OFPT_GROUP_MOD = 15,
      group_mod = OFPT_GROUP_MOD,
      OFPT_PORT_MOD = 16,
      port_mod = OFPT_PORT_MOD,
      OFPT_TABLE_MOD = 17,
      table_mod = OFPT_TABLE_MOD,
      OFPT_MULTIPART_REQUEST = 18,
      multipart_request = OFPT_MULTIPART_REQUEST,
      OFPT_MULTIPART_REPLY = 19,
      multipart_reply = OFPT_MULTIPART_REPLY,
      OFPT_BARRIER_REQUEST = 20,
      barrier_request = OFPT_BARRIER_REQUEST,
      OFPT_BARRIER_REPLY = 21,
      barrier_reply = OFPT_BARRIER_REPLY,
      OFPT_QUEUE_GET_CONFIG_REQUEST = 22,
      queue_get_config_request = OFPT_QUEUE_GET_CONFIG_REQUEST,
      OFPT_QUEUE_GET_CONFIG_REPLY = 23,
      queue_get_config_reply = OFPT_QUEUE_GET_CONFIG_REPLY,
      OFPT_ROLE_REQUEST = 24,
      role_request = OFPT_ROLE_REQUEST,
      OFPT_ROLE_REPLY = 25,
      role_reply = OFPT_ROLE_REPLY,
      OFPT_GET_ASYNC_REQUEST = 26,
      get_async_request = OFPT_GET_ASYNC_REQUEST,
      OFPT_GET_ASYNC_REPLY = 27,
      get_async_reply = OFPT_GET_ASYNC_REPLY,
      OFPT_SET_ASYNC = 28,
      set_async = OFPT_SET_ASYNC,
      OFPT_METER_MOD = 29,
      meter_mod = OFPT_METER_MOD,
    };

  } // namespace ofp_type_detail

  using ofp_type_detail::ofp_type;
  using ofp_type_detail::ofp_type::OFPT_HELLO;
  using ofp_type_detail::ofp_type::OFPT_ERROR;
  using ofp_type_detail::ofp_type::OFPT_ECHO_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_ECHO_REPLY;
  using ofp_type_detail::ofp_type::OFPT_EXPERIMENTER;
  using ofp_type_detail::ofp_type::OFPT_FEATURES_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_FEATURES_REPLY;
  using ofp_type_detail::ofp_type::OFPT_GET_CONFIG_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_GET_CONFIG_REPLY;
  using ofp_type_detail::ofp_type::OFPT_SET_CONFIG;
  using ofp_type_detail::ofp_type::OFPT_PACKET_IN;
  using ofp_type_detail::ofp_type::OFPT_FLOW_REMOVED;
  using ofp_type_detail::ofp_type::OFPT_PORT_STATUS;
  using ofp_type_detail::ofp_type::OFPT_PACKET_OUT;
  using ofp_type_detail::ofp_type::OFPT_FLOW_MOD;
  using ofp_type_detail::ofp_type::OFPT_GROUP_MOD;
  using ofp_type_detail::ofp_type::OFPT_PORT_MOD;
  using ofp_type_detail::ofp_type::OFPT_TABLE_MOD;
  using ofp_type_detail::ofp_type::OFPT_MULTIPART_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_MULTIPART_REPLY;
  using ofp_type_detail::ofp_type::OFPT_BARRIER_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_BARRIER_REPLY;
  using ofp_type_detail::ofp_type::OFPT_QUEUE_GET_CONFIG_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_QUEUE_GET_CONFIG_REPLY;
  using ofp_type_detail::ofp_type::OFPT_ROLE_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_ROLE_REPLY;
  using ofp_type_detail::ofp_type::OFPT_GET_ASYNC_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_GET_ASYNC_REPLY;
  using ofp_type_detail::ofp_type::OFPT_SET_ASYNC;
  using ofp_type_detail::ofp_type::OFPT_METER_MOD;

  using msg_type = ofp_type_detail::ofp_type;

  namespace ofp_hello_elem_type_detail {

    enum ofp_hello_elem_type // : std::uint16_t
    {
      OFPHET_VERSIONBITMAP = 1,
      versionbitmap = OFPHET_VERSIONBITMAP,
    };

  } // namespace ofp_hello_elem_type_detail

  using ofp_hello_elem_type_detail::ofp_hello_elem_type;
  using ofp_hello_elem_type_detail::ofp_hello_elem_type::OFPHET_VERSIONBITMAP;

  using hello_elem_type = ofp_hello_elem_type_detail::ofp_hello_elem_type;

  namespace ofp_config_flags_detail {

    enum ofp_config_flags // : std::uint16_t
    {
      OFPC_FRAG_NORMAL = 0,
      normal = OFPC_FRAG_NORMAL,
      OFPC_FRAG_DROP = 1,
      drop = OFPC_FRAG_DROP,
      OFPC_FRAG_REASM = 2,
      reasm = OFPC_FRAG_REASM,
      OFPC_FRAG_MASK = 3,
      mask = OFPC_FRAG_MASK,
    };

  } // namespace ofp_config_flags_detail

  using ofp_config_flags_detail::ofp_config_flags;
  using ofp_config_flags_detail::ofp_config_flags::OFPC_FRAG_NORMAL;
  using ofp_config_flags_detail::ofp_config_flags::OFPC_FRAG_DROP;
  using ofp_config_flags_detail::ofp_config_flags::OFPC_FRAG_REASM;
  using ofp_config_flags_detail::ofp_config_flags::OFPC_FRAG_MASK;

  using config_flags = ofp_config_flags_detail::ofp_config_flags;

  namespace ofp_table_config_detail {

    enum ofp_table_config // : std::uint32_t
    {
      OFPTC_DEPRECATED_MASK = 3,
      deprecated_mask = OFPTC_DEPRECATED_MASK,
    };

  } // namespace ofp_table_config_detail

  using ofp_table_config_detail::ofp_table_config;
  using ofp_table_config_detail::ofp_table_config::OFPTC_DEPRECATED_MASK;

  using table_config = ofp_table_config_detail::ofp_table_config;

  namespace ofp_table_detail {

    enum ofp_table // : std::uint8_t
    {
      OFPTT_MAX = 254,
      max = OFPTT_MAX,
      OFPTT_ALL = 255,
      all = OFPTT_ALL,
    };

  } // namespace ofp_table_detail

  using ofp_table_detail::ofp_table;
  using ofp_table_detail::ofp_table::OFPTT_MAX;
  using ofp_table_detail::ofp_table::OFPTT_ALL;

  using table = ofp_table_detail::ofp_table;

  namespace ofp_capabilities_detail {

    enum ofp_capabilities // : std::uint32_t
    {
      OFPC_FLOW_STATS = 1,
      flow_stats = OFPC_FLOW_STATS,
      OFPC_TABLE_STATS = 2,
      table_stats = OFPC_TABLE_STATS,
      OFPC_PORT_STATS = 4,
      port_stats = OFPC_PORT_STATS,
      OFPC_GROUP_STATS = 8,
      group_stats = OFPC_GROUP_STATS,
      OFPC_IP_REASM = 32,
      ip_reasm = OFPC_IP_REASM,
      OFPC_QUEUE_STATS = 64,
      queue_stats = OFPC_QUEUE_STATS,
      OFPC_PORT_BLOCKED = 256,
      port_blocked = OFPC_PORT_BLOCKED,
    };

  } // namespace ofp_capabilities_detail

  using ofp_capabilities_detail::ofp_capabilities;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_FLOW_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_TABLE_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_PORT_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_GROUP_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_IP_REASM;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_QUEUE_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_PORT_BLOCKED;

  using capabilities = ofp_capabilities_detail::ofp_capabilities;

  namespace ofp_port_config_detail {

    enum ofp_port_config // : std::uint32_t
    {
      OFPPC_PORT_DOWN = 1,
      port_down = OFPPC_PORT_DOWN,
      OFPPC_NO_RECV = 4,
      no_recv = OFPPC_NO_RECV,
      OFPPC_NO_FWD = 32,
      no_fwd = OFPPC_NO_FWD,
      OFPPC_NO_PACKET_IN = 64,
      no_packet_in = OFPPC_NO_PACKET_IN,
    };

  } // namespace ofp_port_config_detail

  using ofp_port_config_detail::ofp_port_config;
  using ofp_port_config_detail::ofp_port_config::OFPPC_PORT_DOWN;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_RECV;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_FWD;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_PACKET_IN;

  using port_config = ofp_port_config_detail::ofp_port_config;

  namespace ofp_port_state_detail {

    enum ofp_port_state // : std::uint32_t
    {
      OFPPS_LINK_DOWN = 1,
      link_down = OFPPS_LINK_DOWN,
      OFPPS_BLOCKED = 2,
      blocked = OFPPS_BLOCKED,
      OFPPS_LIVE = 4,
      live = OFPPS_LIVE,
    };

  } // namespace ofp_port_state_detail

  using ofp_port_state_detail::ofp_port_state;
  using ofp_port_state_detail::ofp_port_state::OFPPS_LINK_DOWN;
  using ofp_port_state_detail::ofp_port_state::OFPPS_BLOCKED;
  using ofp_port_state_detail::ofp_port_state::OFPPS_LIVE;

  using port_state = ofp_port_state_detail::ofp_port_state;

  namespace ofp_port_features_detail {

    enum ofp_port_features // : std::uint32_t
    {
      OFPPF_10MB_HD = 1,
      mode_10mb_hd = OFPPF_10MB_HD,
      OFPPF_10MB_FD = 2,
      mode_10mb_fd = OFPPF_10MB_FD,
      OFPPF_100MB_HD = 4,
      mode_100mb_hd = OFPPF_100MB_HD,
      OFPPF_100MB_FD = 8,
      mode_100mb_fd = OFPPF_100MB_FD,
      OFPPF_1GB_HD = 16,
      mode_1gb_hd = OFPPF_1GB_HD,
      OFPPF_1GB_FD = 32,
      mode_1gb_fd = OFPPF_1GB_FD,
      OFPPF_10GB_FD = 64,
      mode_10gb_fd = OFPPF_10GB_FD,
      OFPPF_40GB_FD = 128,
      mode_40gb_fd = OFPPF_40GB_FD,
      OFPPF_100GB_FD = 256,
      mode_100gb_fd = OFPPF_100GB_FD,
      OFPPF_1TB_FD = 512,
      mode_1tb_fd = OFPPF_1TB_FD,
      OFPPF_OTHER = 1024,
      mode_other = OFPPF_OTHER,
      OFPPF_COPPER = 2048,
      copper = OFPPF_COPPER,
      OFPPF_FIBER = 4096,
      fiber = OFPPF_FIBER,
      OFPPF_AUTONEG = 8192,
      autoneg = OFPPF_AUTONEG,
      OFPPF_PAUSE = 16384,
      pause = OFPPF_PAUSE,
      OFPPF_PAUSE_ASYM = 32768,
      pause_asym = OFPPF_PAUSE_ASYM,
    };

  } // namespace ofp_port_features_detail

  using ofp_port_features_detail::ofp_port_features;
  using ofp_port_features_detail::ofp_port_features::OFPPF_10MB_HD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_10MB_FD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_100MB_HD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_100MB_FD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_1GB_HD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_1GB_FD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_10GB_FD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_40GB_FD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_100GB_FD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_1TB_FD;
  using ofp_port_features_detail::ofp_port_features::OFPPF_OTHER;
  using ofp_port_features_detail::ofp_port_features::OFPPF_COPPER;
  using ofp_port_features_detail::ofp_port_features::OFPPF_FIBER;
  using ofp_port_features_detail::ofp_port_features::OFPPF_AUTONEG;
  using ofp_port_features_detail::ofp_port_features::OFPPF_PAUSE;
  using ofp_port_features_detail::ofp_port_features::OFPPF_PAUSE_ASYM;

  using port_features = ofp_port_features_detail::ofp_port_features;

  namespace ofp_port_reason_detail {

    enum ofp_port_reason // : std::uint8_t
    {
      OFPPR_ADD = 0,
      by_add = OFPPR_ADD,
      OFPPR_DELETE = 1,
      by_delete = OFPPR_DELETE,
      OFPPR_MODIFY = 2,
      by_modify = OFPPR_MODIFY,
    };

  } // namespace ofp_port_reason_detail

  using ofp_port_reason_detail::ofp_port_reason;
  using ofp_port_reason_detail::ofp_port_reason::OFPPR_ADD;
  using ofp_port_reason_detail::ofp_port_reason::OFPPR_DELETE;
  using ofp_port_reason_detail::ofp_port_reason::OFPPR_MODIFY;

  using port_reason = ofp_port_reason_detail::ofp_port_reason;

  namespace ofp_match_type_detail {

    enum ofp_match_type // : std::uint16_t
    {
      OFPMT_STANDARD = 0,
      standard = OFPMT_STANDARD,
      OFPMT_OXM = 1,
      oxm = OFPMT_OXM,
    };

  } // namespace ofp_match_type_detail

  using ofp_match_type_detail::ofp_match_type;
  using ofp_match_type_detail::ofp_match_type::OFPMT_STANDARD;
  using ofp_match_type_detail::ofp_match_type::OFPMT_OXM;

  using match_type = ofp_match_type_detail::ofp_match_type;

  namespace ofp_oxm_class_detail {

    enum ofp_oxm_class // : std::uint16_t
    {
      OFPXMC_NXM_0 = 0,
      nxm_0 = OFPXMC_NXM_0,
      OFPXMC_NXM_1 = 1,
      nxm_1 = OFPXMC_NXM_1,
      OFPXMC_OPENFLOW_BASIC = 32768,
      openflow_basic = OFPXMC_OPENFLOW_BASIC,
      OFPXMC_EXPERIMENTER = 65535,
      experimenter = OFPXMC_EXPERIMENTER,
    };

  } // namespace ofp_oxm_class_detail

  using ofp_oxm_class_detail::ofp_oxm_class;
  using ofp_oxm_class_detail::ofp_oxm_class::OFPXMC_NXM_0;
  using ofp_oxm_class_detail::ofp_oxm_class::OFPXMC_NXM_1;
  using ofp_oxm_class_detail::ofp_oxm_class::OFPXMC_OPENFLOW_BASIC;
  using ofp_oxm_class_detail::ofp_oxm_class::OFPXMC_EXPERIMENTER;

  using oxm_class = ofp_oxm_class_detail::ofp_oxm_class;

  namespace oxm_ofb_match_fields_detail {

    enum oxm_ofb_match_fields // : std::uint8_t
    {
      OFPXMT_OFB_IN_PORT = 0,
      in_port = OFPXMT_OFB_IN_PORT,
      OFPXMT_OFB_IN_PHY_PORT = 1,
      in_phy_port = OFPXMT_OFB_IN_PHY_PORT,
      OFPXMT_OFB_METADATA = 2,
      metadata = OFPXMT_OFB_METADATA,
      OFPXMT_OFB_ETH_DST = 3,
      eth_dst = OFPXMT_OFB_ETH_DST,
      OFPXMT_OFB_ETH_SRC = 4,
      eth_src = OFPXMT_OFB_ETH_SRC,
      OFPXMT_OFB_ETH_TYPE = 5,
      eth_type = OFPXMT_OFB_ETH_TYPE,
      OFPXMT_OFB_VLAN_VID = 6,
      vlan_vid = OFPXMT_OFB_VLAN_VID,
      OFPXMT_OFB_VLAN_PCP = 7,
      vlan_pcp = OFPXMT_OFB_VLAN_PCP,
      OFPXMT_OFB_IP_DSCP = 8,
      ip_dscp = OFPXMT_OFB_IP_DSCP,
      OFPXMT_OFB_IP_ECN = 9,
      ip_ecn = OFPXMT_OFB_IP_ECN,
      OFPXMT_OFB_IP_PROTO = 10,
      ip_proto = OFPXMT_OFB_IP_PROTO,
      OFPXMT_OFB_IPV4_SRC = 11,
      ipv4_src = OFPXMT_OFB_IPV4_SRC,
      OFPXMT_OFB_IPV4_DST = 12,
      ipv4_dst = OFPXMT_OFB_IPV4_DST,
      OFPXMT_OFB_TCP_SRC = 13,
      tcp_src = OFPXMT_OFB_TCP_SRC,
      OFPXMT_OFB_TCP_DST = 14,
      tcp_dst = OFPXMT_OFB_TCP_DST,
      OFPXMT_OFB_UDP_SRC = 15,
      udp_src = OFPXMT_OFB_UDP_SRC,
      OFPXMT_OFB_UDP_DST = 16,
      udp_dst = OFPXMT_OFB_UDP_DST,
      OFPXMT_OFB_SCTP_SRC = 17,
      sctp_src = OFPXMT_OFB_SCTP_SRC,
      OFPXMT_OFB_SCTP_DST = 18,
      sctp_dst = OFPXMT_OFB_SCTP_DST,
      OFPXMT_OFB_ICMPV4_TYPE = 19,
      icmpv4_type = OFPXMT_OFB_ICMPV4_TYPE,
      OFPXMT_OFB_ICMPV4_CODE = 20,
      icmpv4_code = OFPXMT_OFB_ICMPV4_CODE,
      OFPXMT_OFB_ARP_OP = 21,
      arp_op = OFPXMT_OFB_ARP_OP,
      OFPXMT_OFB_ARP_SPA = 22,
      arp_spa = OFPXMT_OFB_ARP_SPA,
      OFPXMT_OFB_ARP_TPA = 23,
      arp_tpa = OFPXMT_OFB_ARP_TPA,
      OFPXMT_OFB_ARP_SHA = 24,
      arp_sha = OFPXMT_OFB_ARP_SHA,
      OFPXMT_OFB_ARP_THA = 25,
      arp_tha = OFPXMT_OFB_ARP_THA,
      OFPXMT_OFB_IPV6_SRC = 26,
      ipv6_src = OFPXMT_OFB_IPV6_SRC,
      OFPXMT_OFB_IPV6_DST = 27,
      ipv6_dst = OFPXMT_OFB_IPV6_DST,
      OFPXMT_OFB_IPV6_FLABEL = 28,
      ipv6_flabel = OFPXMT_OFB_IPV6_FLABEL,
      OFPXMT_OFB_ICMPV6_TYPE = 29,
      icmpv6_type = OFPXMT_OFB_ICMPV6_TYPE,
      OFPXMT_OFB_ICMPV6_CODE = 30,
      icmpv6_code = OFPXMT_OFB_ICMPV6_CODE,
      OFPXMT_OFB_IPV6_ND_TARGET = 31,
      ipv6_nd_target = OFPXMT_OFB_IPV6_ND_TARGET,
      OFPXMT_OFB_IPV6_ND_SLL = 32,
      ipv6_nd_sll = OFPXMT_OFB_IPV6_ND_SLL,
      OFPXMT_OFB_IPV6_ND_TLL = 33,
      ipv6_nd_tll = OFPXMT_OFB_IPV6_ND_TLL,
      OFPXMT_OFB_MPLS_LABEL = 34,
      mpls_label = OFPXMT_OFB_MPLS_LABEL,
      OFPXMT_OFB_MPLS_TC = 35,
      mpls_tc = OFPXMT_OFB_MPLS_TC,
      OFPXMT_OFB_MPLS_BOS = 36,
      mpls_bos = OFPXMT_OFB_MPLS_BOS,
      OFPXMT_OFB_PBB_ISID = 37,
      pbb_isid = OFPXMT_OFB_PBB_ISID,
      OFPXMT_OFB_TUNNEL_ID = 38,
      tunnel_id = OFPXMT_OFB_TUNNEL_ID,
      OFPXMT_OFB_IPV6_EXTHDR = 39,
      ipv6_exthdr = OFPXMT_OFB_IPV6_EXTHDR,
    };

  } // namespace oxm_ofb_match_fields_detail

  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IN_PORT;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IN_PHY_PORT;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_METADATA;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ETH_DST;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ETH_SRC;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ETH_TYPE;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_VLAN_VID;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_VLAN_PCP;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IP_DSCP;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IP_ECN;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IP_PROTO;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV4_SRC;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV4_DST;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_TCP_SRC;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_TCP_DST;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_UDP_SRC;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_UDP_DST;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_SCTP_SRC;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_SCTP_DST;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ICMPV4_TYPE;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ICMPV4_CODE;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ARP_OP;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ARP_SPA;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ARP_TPA;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ARP_SHA;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ARP_THA;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV6_SRC;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV6_DST;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV6_FLABEL;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ICMPV6_TYPE;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_ICMPV6_CODE;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV6_ND_TARGET;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV6_ND_SLL;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV6_ND_TLL;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_MPLS_LABEL;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_MPLS_TC;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_MPLS_BOS;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_PBB_ISID;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_TUNNEL_ID;
  using oxm_ofb_match_fields_detail::oxm_ofb_match_fields::OFPXMT_OFB_IPV6_EXTHDR;

  using ofb_match_fields = oxm_ofb_match_fields_detail::oxm_ofb_match_fields;

  namespace ofp_vlan_id_detail {

    enum ofp_vlan_id // : std::uint16_t
    {
      OFPVID_PRESENT = 4096,
      present = OFPVID_PRESENT,
      OFPVID_NONE = 0,
      none = OFPVID_NONE,
    };

  } // namespace ofp_vlan_id_detail

  using ofp_vlan_id_detail::ofp_vlan_id;
  using ofp_vlan_id_detail::ofp_vlan_id::OFPVID_PRESENT;
  using ofp_vlan_id_detail::ofp_vlan_id::OFPVID_NONE;

  using vlan_id = ofp_vlan_id_detail::ofp_vlan_id;

  namespace ofp_ipv6exthdr_flags_detail {

    enum ofp_ipv6exthdr_flags // : std::uint16_t
    {
      OFPIEH_NONEXT = 1,
      nonext = OFPIEH_NONEXT,
      OFPIEH_ESP = 2,
      esp = OFPIEH_ESP,
      OFPIEH_AUTH = 4,
      auth = OFPIEH_AUTH,
      OFPIEH_DEST = 8,
      dest = OFPIEH_DEST,
      OFPIEH_FRAG = 16,
      frag = OFPIEH_FRAG,
      OFPIEH_ROUTER = 32,
      router = OFPIEH_ROUTER,
      OFPIEH_HOP = 64,
      hop = OFPIEH_HOP,
      OFPIEH_UNREP = 128,
      unrep = OFPIEH_UNREP,
      OFPIEH_UNSEQ = 256,
      unseq = OFPIEH_UNSEQ,
    };

  } // namespace ofp_ipv6exthdr_flags_detail

  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_NONEXT;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_ESP;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_AUTH;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_DEST;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_FRAG;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_ROUTER;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_HOP;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_UNREP;
  using ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags::OFPIEH_UNSEQ;

  using ipv6exthdr_flags = ofp_ipv6exthdr_flags_detail::ofp_ipv6exthdr_flags;

  namespace ofp_action_type_detail {

    enum ofp_action_type // : std::uint16_t
    {
      OFPAT_OUTPUT = 0,
      output = OFPAT_OUTPUT,
      OFPAT_COPY_TTL_OUT = 11,
      copy_ttl_out = OFPAT_COPY_TTL_OUT,
      OFPAT_COPY_TTL_IN = 12,
      copy_ttl_in = OFPAT_COPY_TTL_IN,
      OFPAT_SET_MPLS_TTL = 15,
      set_mpls_ttl = OFPAT_SET_MPLS_TTL,
      OFPAT_DEC_MPLS_TTL = 16,
      dec_mpls_ttl = OFPAT_DEC_MPLS_TTL,
      OFPAT_PUSH_VLAN = 17,
      push_vlan = OFPAT_PUSH_VLAN,
      OFPAT_POP_VLAN = 18,
      pop_vlan = OFPAT_POP_VLAN,
      OFPAT_PUSH_MPLS = 19,
      push_mpls = OFPAT_PUSH_MPLS,
      OFPAT_POP_MPLS = 20,
      pop_mpls = OFPAT_POP_MPLS,
      OFPAT_SET_QUEUE = 21,
      set_queue = OFPAT_SET_QUEUE,
      OFPAT_GROUP = 22,
      group = OFPAT_GROUP,
      OFPAT_SET_NW_TTL = 23,
      set_nw_ttl = OFPAT_SET_NW_TTL,
      OFPAT_DEC_NW_TTL = 24,
      dec_nw_ttl = OFPAT_DEC_NW_TTL,
      OFPAT_SET_FIELD = 25,
      set_field = OFPAT_SET_FIELD,
      OFPAT_PUSH_PBB = 26,
      push_pbb = OFPAT_PUSH_PBB,
      OFPAT_POP_PBB = 27,
      pop_pbb = OFPAT_POP_PBB,
      OFPAT_EXPERIMENTER = 65535,
      experimenter = OFPAT_EXPERIMENTER,
    };

  } // namespace ofp_action_type_detail

  using ofp_action_type_detail::ofp_action_type;
  using ofp_action_type_detail::ofp_action_type::OFPAT_OUTPUT;
  using ofp_action_type_detail::ofp_action_type::OFPAT_COPY_TTL_OUT;
  using ofp_action_type_detail::ofp_action_type::OFPAT_COPY_TTL_IN;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_MPLS_TTL;
  using ofp_action_type_detail::ofp_action_type::OFPAT_DEC_MPLS_TTL;
  using ofp_action_type_detail::ofp_action_type::OFPAT_PUSH_VLAN;
  using ofp_action_type_detail::ofp_action_type::OFPAT_POP_VLAN;
  using ofp_action_type_detail::ofp_action_type::OFPAT_PUSH_MPLS;
  using ofp_action_type_detail::ofp_action_type::OFPAT_POP_MPLS;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_QUEUE;
  using ofp_action_type_detail::ofp_action_type::OFPAT_GROUP;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_NW_TTL;
  using ofp_action_type_detail::ofp_action_type::OFPAT_DEC_NW_TTL;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_FIELD;
  using ofp_action_type_detail::ofp_action_type::OFPAT_PUSH_PBB;
  using ofp_action_type_detail::ofp_action_type::OFPAT_POP_PBB;
  using ofp_action_type_detail::ofp_action_type::OFPAT_EXPERIMENTER;

  using action_type = ofp_action_type_detail::ofp_action_type;

  namespace ofp_controller_max_len_detail {

    enum ofp_controller_max_len // : std::uint16_t
    {
      OFPCML_MAX = 65509,
      max = OFPCML_MAX,
      OFPCML_NO_BUFFER = 65535,
      no_buffer = OFPCML_NO_BUFFER,
    };

  } // namespace ofp_controller_max_len_detail

  using ofp_controller_max_len_detail::ofp_controller_max_len;
  using ofp_controller_max_len_detail::ofp_controller_max_len::OFPCML_MAX;
  using ofp_controller_max_len_detail::ofp_controller_max_len::OFPCML_NO_BUFFER;

  using controller_max_len = ofp_controller_max_len_detail::ofp_controller_max_len;

  namespace ofp_instruction_type_detail {

    enum ofp_instruction_type // : std::uint16_t
    {
      OFPIT_GOTO_TABLE = 1,
      goto_table = OFPIT_GOTO_TABLE,
      OFPIT_WRITE_METADATA = 2,
      write_metadata = OFPIT_WRITE_METADATA,
      OFPIT_WRITE_ACTIONS = 3,
      write_actions = OFPIT_WRITE_ACTIONS,
      OFPIT_APPLY_ACTIONS = 4,
      apply_actions = OFPIT_APPLY_ACTIONS,
      OFPIT_CLEAR_ACTIONS = 5,
      clear_actions = OFPIT_CLEAR_ACTIONS,
      OFPIT_METER = 6,
      meter = OFPIT_METER,
      OFPIT_EXPERIMENTER = 65535,
      experimenter = OFPIT_EXPERIMENTER,
    };

  } // namespace ofp_instruction_type_detail

  using ofp_instruction_type_detail::ofp_instruction_type;
  using ofp_instruction_type_detail::ofp_instruction_type::OFPIT_GOTO_TABLE;
  using ofp_instruction_type_detail::ofp_instruction_type::OFPIT_WRITE_METADATA;
  using ofp_instruction_type_detail::ofp_instruction_type::OFPIT_WRITE_ACTIONS;
  using ofp_instruction_type_detail::ofp_instruction_type::OFPIT_APPLY_ACTIONS;
  using ofp_instruction_type_detail::ofp_instruction_type::OFPIT_CLEAR_ACTIONS;
  using ofp_instruction_type_detail::ofp_instruction_type::OFPIT_METER;
  using ofp_instruction_type_detail::ofp_instruction_type::OFPIT_EXPERIMENTER;

  using instruction_type = ofp_instruction_type_detail::ofp_instruction_type;

  namespace ofp_flow_mod_command_detail {

    enum ofp_flow_mod_command // : std::uint16_t
    {
      OFPFC_ADD = 0,
      add_cmd = OFPFC_ADD,
      OFPFC_MODIFY = 1,
      modify_cmd = OFPFC_MODIFY,
      OFPFC_MODIFY_STRICT = 2,
      modify_strict_cmd = OFPFC_MODIFY_STRICT,
      OFPFC_DELETE = 3,
      delete_cmd = OFPFC_DELETE,
      OFPFC_DELETE_STRICT = 4,
      delete_strict_cmd = OFPFC_DELETE_STRICT,
    };

  } // namespace ofp_flow_mod_command_detail

  using ofp_flow_mod_command_detail::ofp_flow_mod_command;
  using ofp_flow_mod_command_detail::ofp_flow_mod_command::OFPFC_ADD;
  using ofp_flow_mod_command_detail::ofp_flow_mod_command::OFPFC_MODIFY;
  using ofp_flow_mod_command_detail::ofp_flow_mod_command::OFPFC_MODIFY_STRICT;
  using ofp_flow_mod_command_detail::ofp_flow_mod_command::OFPFC_DELETE;
  using ofp_flow_mod_command_detail::ofp_flow_mod_command::OFPFC_DELETE_STRICT;

  using flow_mod_command = ofp_flow_mod_command_detail::ofp_flow_mod_command;

  namespace ofp_flow_mod_flags_detail {

    enum ofp_flow_mod_flags // : std::uint16_t
    {
      OFPFF_SEND_FLOW_REM = 1,
      send_flow_rem = OFPFF_SEND_FLOW_REM,
      OFPFF_CHECK_OVERLAP = 2,
      check_overlap = OFPFF_CHECK_OVERLAP,
      OFPFF_RESET_COUNTS = 4,
      reset_counts = OFPFF_RESET_COUNTS,
      OFPFF_NO_PKT_COUNTS = 8,
      no_pkt_counts = OFPFF_NO_PKT_COUNTS,
      OFPFF_NO_BYT_COUNTS = 16,
      no_byt_counts = OFPFF_NO_BYT_COUNTS,
    };

  } // namespace ofp_flow_mod_flags_detail

  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags;
  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags::OFPFF_SEND_FLOW_REM;
  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags::OFPFF_CHECK_OVERLAP;
  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags::OFPFF_RESET_COUNTS;
  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags::OFPFF_NO_PKT_COUNTS;
  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags::OFPFF_NO_BYT_COUNTS;

  using flow_mod_flags = ofp_flow_mod_flags_detail::ofp_flow_mod_flags;

  namespace ofp_group_detail {

    enum ofp_group // : std::uint32_t
    {
      OFPG_MAX = 4294967040,
      max = OFPG_MAX,
      OFPG_ALL = 4294967292,
      all = OFPG_ALL,
      OFPG_ANY = 4294967295,
      any = OFPG_ANY,
    };

  } // namespace ofp_group_detail

  using ofp_group_detail::ofp_group;
  using ofp_group_detail::ofp_group::OFPG_MAX;
  using ofp_group_detail::ofp_group::OFPG_ALL;
  using ofp_group_detail::ofp_group::OFPG_ANY;

  using group = ofp_group_detail::ofp_group;

  namespace ofp_group_mod_command_detail {

    enum ofp_group_mod_command // : std::uint8_t
    {
      OFPGC_ADD = 0,
      add_cmd = OFPGC_ADD,
      OFPGC_MODIFY = 1,
      modify_cmd = OFPGC_MODIFY,
      OFPGC_DELETE = 2,
      delete_cmd = OFPGC_DELETE,
    };

  } // namespace ofp_group_mod_command_detail

  using ofp_group_mod_command_detail::ofp_group_mod_command;
  using ofp_group_mod_command_detail::ofp_group_mod_command::OFPGC_ADD;
  using ofp_group_mod_command_detail::ofp_group_mod_command::OFPGC_MODIFY;
  using ofp_group_mod_command_detail::ofp_group_mod_command::OFPGC_DELETE;

  using group_mod_command = ofp_group_mod_command_detail::ofp_group_mod_command;

  namespace ofp_group_type_detail {

    enum ofp_group_type // : std::uint8_t
    {
      OFPGT_ALL = 0,
      all = OFPGT_ALL,
      OFPGT_SELECT = 1,
      select = OFPGT_SELECT,
      OFPGT_INDIRECT = 2,
      indirect = OFPGT_INDIRECT,
      OFPGT_FF = 3,
      ff = OFPGT_FF,
    };

  } // namespace ofp_group_type_detail

  using ofp_group_type_detail::ofp_group_type;
  using ofp_group_type_detail::ofp_group_type::OFPGT_ALL;
  using ofp_group_type_detail::ofp_group_type::OFPGT_SELECT;
  using ofp_group_type_detail::ofp_group_type::OFPGT_INDIRECT;
  using ofp_group_type_detail::ofp_group_type::OFPGT_FF;

  using group_type = ofp_group_type_detail::ofp_group_type;

  namespace ofp_packet_in_reason_detail {

    enum ofp_packet_in_reason // : std::uint8_t
    {
      OFPR_NO_MATCH = 0,
      no_match = OFPR_NO_MATCH,
      OFPR_ACTION = 1,
      action = OFPR_ACTION,
      OFPR_INVALID_TTL = 2,
      invalid_ttl = OFPR_INVALID_TTL,
    };

  } // namespace ofp_packet_in_reason_detail

  using ofp_packet_in_reason_detail::ofp_packet_in_reason;
  using ofp_packet_in_reason_detail::ofp_packet_in_reason::OFPR_NO_MATCH;
  using ofp_packet_in_reason_detail::ofp_packet_in_reason::OFPR_ACTION;
  using ofp_packet_in_reason_detail::ofp_packet_in_reason::OFPR_INVALID_TTL;

  using packet_in_reason = ofp_packet_in_reason_detail::ofp_packet_in_reason;

  namespace ofp_flow_removed_reason_detail {

    enum ofp_flow_removed_reason // : std::uint8_t
    {
      OFPRR_IDLE_TIMEOUT = 0,
      by_idle_timeout = OFPRR_IDLE_TIMEOUT,
      OFPRR_HARD_TIMEOUT = 1,
      by_hard_timeout = OFPRR_HARD_TIMEOUT,
      OFPRR_DELETE = 2,
      by_delete = OFPRR_DELETE,
      OFPRR_GROUP_DELETE = 3,
      by_group_delete = OFPRR_GROUP_DELETE,
    };

  } // namespace ofp_flow_removed_reason_detail

  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason;
  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason::OFPRR_IDLE_TIMEOUT;
  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason::OFPRR_HARD_TIMEOUT;
  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason::OFPRR_DELETE;
  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason::OFPRR_GROUP_DELETE;

  using flow_removed_reason = ofp_flow_removed_reason_detail::ofp_flow_removed_reason;

  namespace ofp_meter_detail {

    enum ofp_meter // : std::uint32_t
    {
      OFPM_MAX = 4294901760,
      max = OFPM_MAX,
      OFPM_SLOWPATH = 4294967293,
      slowpath = OFPM_SLOWPATH,
      OFPM_CONTROLLER = 4294967294,
      controller = OFPM_CONTROLLER,
      OFPM_ALL = 4294967295,
      all = OFPM_ALL,
    };

  } // namespace ofp_meter_detail

  using ofp_meter_detail::ofp_meter;
  using ofp_meter_detail::ofp_meter::OFPM_MAX;
  using ofp_meter_detail::ofp_meter::OFPM_SLOWPATH;
  using ofp_meter_detail::ofp_meter::OFPM_CONTROLLER;
  using ofp_meter_detail::ofp_meter::OFPM_ALL;

  using meter = ofp_meter_detail::ofp_meter;

  namespace ofp_meter_band_type_detail {

    enum ofp_meter_band_type // : std::uint16_t
    {
      OFPMBT_DROP = 1,
      drop = OFPMBT_DROP,
      OFPMBT_DSCP_REMARK = 2,
      dscp_remark = OFPMBT_DSCP_REMARK,
      OFPMBT_EXPERIMENTER = 65535,
      experimenter = OFPMBT_EXPERIMENTER,
    };

  } // namespace ofp_meter_band_type_detail

  using ofp_meter_band_type_detail::ofp_meter_band_type;
  using ofp_meter_band_type_detail::ofp_meter_band_type::OFPMBT_DROP;
  using ofp_meter_band_type_detail::ofp_meter_band_type::OFPMBT_DSCP_REMARK;
  using ofp_meter_band_type_detail::ofp_meter_band_type::OFPMBT_EXPERIMENTER;

  using meter_band_type = ofp_meter_band_type_detail::ofp_meter_band_type;

  namespace ofp_meter_mod_command_detail {

    enum ofp_meter_mod_command // : std::uint16_t
    {
      OFPMC_ADD = 0,
      add_cmd = OFPMC_ADD,
      OFPMC_MODIFY = 1,
      modify_cmd = OFPMC_MODIFY,
      OFPMC_DELETE = 2,
      delete_cmd = OFPMC_DELETE,
    };

  } // namespace ofp_meter_mod_command_detail

  using ofp_meter_mod_command_detail::ofp_meter_mod_command;
  using ofp_meter_mod_command_detail::ofp_meter_mod_command::OFPMC_ADD;
  using ofp_meter_mod_command_detail::ofp_meter_mod_command::OFPMC_MODIFY;
  using ofp_meter_mod_command_detail::ofp_meter_mod_command::OFPMC_DELETE;

  using meter_mod_command = ofp_meter_mod_command_detail::ofp_meter_mod_command;

  namespace ofp_meter_flags_detail {

    enum ofp_meter_flags // : std::uint16_t
    {
      OFPMF_KBPS = 1,
      kbps = OFPMF_KBPS,
      OFPMF_PKTPS = 2,
      pktps = OFPMF_PKTPS,
      OFPMF_BURST = 4,
      burst = OFPMF_BURST,
      OFPMF_STATS = 8,
      stats = OFPMF_STATS,
    };

  } // namespace ofp_meter_flags_detail

  using ofp_meter_flags_detail::ofp_meter_flags;
  using ofp_meter_flags_detail::ofp_meter_flags::OFPMF_KBPS;
  using ofp_meter_flags_detail::ofp_meter_flags::OFPMF_PKTPS;
  using ofp_meter_flags_detail::ofp_meter_flags::OFPMF_BURST;
  using ofp_meter_flags_detail::ofp_meter_flags::OFPMF_STATS;

  using meter_flags = ofp_meter_flags_detail::ofp_meter_flags;

  namespace ofp_error_type_detail {

    enum ofp_error_type // : std::uint16_t
    {
      OFPET_HELLO_FAILED = 0,
      hello_failed = OFPET_HELLO_FAILED,
      OFPET_BAD_REQUEST = 1,
      bad_request = OFPET_BAD_REQUEST,
      OFPET_BAD_ACTION = 2,
      bad_action = OFPET_BAD_ACTION,
      OFPET_BAD_INSTRUCTION = 3,
      bad_instruction = OFPET_BAD_INSTRUCTION,
      OFPET_BAD_MATCH = 4,
      bad_match = OFPET_BAD_MATCH,
      OFPET_FLOW_MOD_FAILED = 5,
      flow_mod_failed = OFPET_FLOW_MOD_FAILED,
      OFPET_GROUP_MOD_FAILED = 6,
      group_mod_failed = OFPET_GROUP_MOD_FAILED,
      OFPET_PORT_MOD_FAILED = 7,
      port_mod_failed = OFPET_PORT_MOD_FAILED,
      OFPET_TABLE_MOD_FAILED = 8,
      table_mod_failed = OFPET_TABLE_MOD_FAILED,
      OFPET_QUEUE_OP_FAILED = 9,
      queue_op_failed = OFPET_QUEUE_OP_FAILED,
      OFPET_SWITCH_CONFIG_FAILED = 10,
      switch_config_failed = OFPET_SWITCH_CONFIG_FAILED,
      OFPET_ROLE_REQUEST_FAILED = 11,
      role_request_failed = OFPET_ROLE_REQUEST_FAILED,
      OFPET_METER_MOD_FAILED = 12,
      meter_mod_failed = OFPET_METER_MOD_FAILED,
      OFPET_TABLE_FEATURES_FAILED = 13,
      table_features_failed = OFPET_TABLE_FEATURES_FAILED,
      OFPET_EXPERIMENTER = 65535,
      experimenter = OFPET_EXPERIMENTER,
    };

  } // namespace ofp_error_type_detail

  using ofp_error_type_detail::ofp_error_type;
  using ofp_error_type_detail::ofp_error_type::OFPET_HELLO_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_BAD_REQUEST;
  using ofp_error_type_detail::ofp_error_type::OFPET_BAD_ACTION;
  using ofp_error_type_detail::ofp_error_type::OFPET_BAD_INSTRUCTION;
  using ofp_error_type_detail::ofp_error_type::OFPET_BAD_MATCH;
  using ofp_error_type_detail::ofp_error_type::OFPET_FLOW_MOD_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_GROUP_MOD_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_PORT_MOD_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_TABLE_MOD_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_QUEUE_OP_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_SWITCH_CONFIG_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_ROLE_REQUEST_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_METER_MOD_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_TABLE_FEATURES_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_EXPERIMENTER;

  using error_type = ofp_error_type_detail::ofp_error_type;

  namespace ofp_hello_failed_code_detail {

    enum ofp_hello_failed_code // : std::uint16_t
    {
      OFPHFC_INCOMPATIBLE = 0,
      incompatible = OFPHFC_INCOMPATIBLE,
      OFPHFC_EPERM = 1,
      eperm = OFPHFC_EPERM,
    };

  } // namespace ofp_hello_failed_code_detail

  using ofp_hello_failed_code_detail::ofp_hello_failed_code;
  using ofp_hello_failed_code_detail::ofp_hello_failed_code::OFPHFC_INCOMPATIBLE;
  using ofp_hello_failed_code_detail::ofp_hello_failed_code::OFPHFC_EPERM;

  using hello_failed_code = ofp_hello_failed_code_detail::ofp_hello_failed_code;

  namespace ofp_bad_request_code_detail {

    enum ofp_bad_request_code // : std::uint16_t
    {
      OFPBRC_BAD_VERSION = 0,
      bad_version = OFPBRC_BAD_VERSION,
      OFPBRC_BAD_TYPE = 1,
      bad_type = OFPBRC_BAD_TYPE,
      OFPBRC_BAD_MULTIPART = 2,
      bad_multipart = OFPBRC_BAD_MULTIPART,
      OFPBRC_BAD_EXPERIMENTER = 3,
      bad_experimenter = OFPBRC_BAD_EXPERIMENTER,
      OFPBRC_BAD_EXP_TYPE = 4,
      bad_exp_type = OFPBRC_BAD_EXP_TYPE,
      OFPBRC_EPERM = 5,
      eperm = OFPBRC_EPERM,
      OFPBRC_BAD_LEN = 6,
      bad_len = OFPBRC_BAD_LEN,
      OFPBRC_BUFFER_EMPTY = 7,
      buffer_empty = OFPBRC_BUFFER_EMPTY,
      OFPBRC_BUFFER_UNKNOWN = 8,
      buffer_unknown = OFPBRC_BUFFER_UNKNOWN,
      OFPBRC_BAD_TABLE_ID = 9,
      bad_table_id = OFPBRC_BAD_TABLE_ID,
      OFPBRC_IS_SLAVE = 10,
      is_slave = OFPBRC_IS_SLAVE,
      OFPBRC_BAD_PORT = 11,
      bad_port = OFPBRC_BAD_PORT,
      OFPBRC_BAD_PACKET = 12,
      bad_packet = OFPBRC_BAD_PACKET,
      OFPBRC_MULTIPART_BUFFER_OVERFLOW = 13,
      multipart_buffer_overflow = OFPBRC_MULTIPART_BUFFER_OVERFLOW,
    };

  } // namespace ofp_bad_request_code_detail

  using ofp_bad_request_code_detail::ofp_bad_request_code;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_VERSION;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_TYPE;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_MULTIPART;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_EXPERIMENTER;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_EXP_TYPE;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_EPERM;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_LEN;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BUFFER_EMPTY;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BUFFER_UNKNOWN;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_TABLE_ID;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_IS_SLAVE;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_PORT;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_PACKET;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_MULTIPART_BUFFER_OVERFLOW;

  using bad_request_code = ofp_bad_request_code_detail::ofp_bad_request_code;

  namespace ofp_bad_action_code_detail {

    enum ofp_bad_action_code // : std::uint16_t
    {
      OFPBAC_BAD_TYPE = 0,
      bad_type = OFPBAC_BAD_TYPE,
      OFPBAC_BAD_LEN = 1,
      bad_len = OFPBAC_BAD_LEN,
      OFPBAC_BAD_EXPERIMENTER = 2,
      bad_experimenter = OFPBAC_BAD_EXPERIMENTER,
      OFPBAC_BAD_EXP_TYPE = 3,
      bad_exp_type = OFPBAC_BAD_EXP_TYPE,
      OFPBAC_BAD_OUT_PORT = 4,
      bad_out_port = OFPBAC_BAD_OUT_PORT,
      OFPBAC_BAD_ARGUMENT = 5,
      bad_argument = OFPBAC_BAD_ARGUMENT,
      OFPBAC_EPERM = 6,
      eperm = OFPBAC_EPERM,
      OFPBAC_TOO_MANY = 7,
      too_many = OFPBAC_TOO_MANY,
      OFPBAC_BAD_QUEUE = 8,
      bad_queue = OFPBAC_BAD_QUEUE,
      OFPBAC_BAD_OUT_GROUP = 9,
      bad_out_group = OFPBAC_BAD_OUT_GROUP,
      OFPBAC_MATCH_INCONSISTENT = 10,
      match_inconsistent = OFPBAC_MATCH_INCONSISTENT,
      OFPBAC_UNSUPPORTED_ORDER = 11,
      unsupported_order = OFPBAC_UNSUPPORTED_ORDER,
      OFPBAC_BAD_TAG = 12,
      bad_tag = OFPBAC_BAD_TAG,
      OFPBAC_BAD_SET_TYPE = 13,
      bad_set_type = OFPBAC_BAD_SET_TYPE,
      OFPBAC_BAD_SET_LEN = 14,
      bad_set_len = OFPBAC_BAD_SET_LEN,
      OFPBAC_BAD_SET_ARGUMENT = 15,
      bad_set_argument = OFPBAC_BAD_SET_ARGUMENT,
    };

  } // namespace ofp_bad_action_code_detail

  using ofp_bad_action_code_detail::ofp_bad_action_code;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_TYPE;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_LEN;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_EXPERIMENTER;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_EXP_TYPE;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_OUT_PORT;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_ARGUMENT;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_EPERM;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_TOO_MANY;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_QUEUE;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_OUT_GROUP;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_MATCH_INCONSISTENT;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_UNSUPPORTED_ORDER;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_TAG;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_SET_TYPE;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_SET_LEN;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_SET_ARGUMENT;

  using bad_action_code = ofp_bad_action_code_detail::ofp_bad_action_code;

  namespace ofp_bad_instruction_code_detail {

    enum ofp_bad_instruction_code // : std::uint16_t
    {
      OFPBIC_UNKNOWN_INST = 0,
      unknown_inst = OFPBIC_UNKNOWN_INST,
      OFPBIC_UNSUP_INST = 1,
      unsup_inst = OFPBIC_UNSUP_INST,
      OFPBIC_BAD_TABLE_ID = 2,
      bad_table_id = OFPBIC_BAD_TABLE_ID,
      OFPBIC_UNSUP_METADATA = 3,
      unsup_metadata = OFPBIC_UNSUP_METADATA,
      OFPBIC_UNSUP_METADATA_MASK = 4,
      unsup_metadata_mask = OFPBIC_UNSUP_METADATA_MASK,
      OFPBIC_BAD_EXPERIMENTER = 5,
      bad_experimenter = OFPBIC_BAD_EXPERIMENTER,
      OFPBIC_BAD_EXP_TYPE = 6,
      bad_exp_type = OFPBIC_BAD_EXP_TYPE,
      OFPBIC_BAD_LEN = 7,
      bad_len = OFPBIC_BAD_LEN,
      OFPBIC_EPERM = 8,
      eperm = OFPBIC_EPERM,
    };

  } // namespace ofp_bad_instruction_code_detail

  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_UNKNOWN_INST;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_UNSUP_INST;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_BAD_TABLE_ID;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_UNSUP_METADATA;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_UNSUP_METADATA_MASK;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_BAD_EXPERIMENTER;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_BAD_EXP_TYPE;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_BAD_LEN;
  using ofp_bad_instruction_code_detail::ofp_bad_instruction_code::OFPBIC_EPERM;

  using bad_instruction_code = ofp_bad_instruction_code_detail::ofp_bad_instruction_code;

  namespace ofp_bad_match_code_detail {

    enum ofp_bad_match_code // : std::uint16_t
    {
      OFPBMC_BAD_TYPE = 0,
      bad_type = OFPBMC_BAD_TYPE,
      OFPBMC_BAD_LEN = 1,
      bad_len = OFPBMC_BAD_LEN,
      OFPBMC_BAD_TAG = 2,
      bad_tag = OFPBMC_BAD_TAG,
      OFPBMC_BAD_DL_ADDR_MASK = 3,
      bad_dl_addr_mask = OFPBMC_BAD_DL_ADDR_MASK,
      OFPBMC_BAD_NW_ADDR_MASK = 4,
      bad_nw_addr_mask = OFPBMC_BAD_NW_ADDR_MASK,
      OFPBMC_BAD_WILDCARDS = 5,
      bad_wildcards = OFPBMC_BAD_WILDCARDS,
      OFPBMC_BAD_FIELD = 6,
      bad_field = OFPBMC_BAD_FIELD,
      OFPBMC_BAD_VALUE = 7,
      bad_value = OFPBMC_BAD_VALUE,
      OFPBMC_BAD_MASK = 8,
      bad_mask = OFPBMC_BAD_MASK,
      OFPBMC_BAD_PREREQ = 9,
      bad_prereq = OFPBMC_BAD_PREREQ,
      OFPBMC_DUP_FIELD = 10,
      dup_field = OFPBMC_DUP_FIELD,
      OFPBMC_EPERM = 11,
      eperm = OFPBMC_EPERM,
    };

  } // namespace ofp_bad_match_code_detail

  using ofp_bad_match_code_detail::ofp_bad_match_code;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_TYPE;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_LEN;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_TAG;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_DL_ADDR_MASK;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_NW_ADDR_MASK;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_WILDCARDS;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_FIELD;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_VALUE;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_MASK;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_BAD_PREREQ;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_DUP_FIELD;
  using ofp_bad_match_code_detail::ofp_bad_match_code::OFPBMC_EPERM;

  using bad_match_code = ofp_bad_match_code_detail::ofp_bad_match_code;

  namespace ofp_flow_mod_failed_code_detail {

    enum ofp_flow_mod_failed_code // : std::uint16_t
    {
      OFPFMFC_UNKNOWN = 0,
      unknown = OFPFMFC_UNKNOWN,
      OFPFMFC_TABLE_FULL = 1,
      table_full = OFPFMFC_TABLE_FULL,
      OFPFMFC_BAD_TABLE_ID = 2,
      bad_table_id = OFPFMFC_BAD_TABLE_ID,
      OFPFMFC_OVERLAP = 3,
      overlap = OFPFMFC_OVERLAP,
      OFPFMFC_EPERM = 4,
      eperm = OFPFMFC_EPERM,
      OFPFMFC_BAD_TIMEOUT = 5,
      bad_timeout = OFPFMFC_BAD_TIMEOUT,
      OFPFMFC_BAD_COMMAND = 6,
      bad_command = OFPFMFC_BAD_COMMAND,
      OFPFMFC_BAD_FLAGS = 7,
      bad_flags = OFPFMFC_BAD_FLAGS,
    };

  } // namespace ofp_flow_mod_failed_code_detail

  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_UNKNOWN;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_TABLE_FULL;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_BAD_TABLE_ID;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_OVERLAP;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_EPERM;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_BAD_TIMEOUT;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_BAD_COMMAND;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_BAD_FLAGS;

  using flow_mod_failed_code = ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code;

  namespace ofp_group_mod_failed_code_detail {

    enum ofp_group_mod_failed_code // : std::uint16_t
    {
      OFPGMFC_GROUP_EXISTS = 0,
      group_exists = OFPGMFC_GROUP_EXISTS,
      OFPGMFC_INVALID_GROUP = 1,
      invalid_group = OFPGMFC_INVALID_GROUP,
      OFPGMFC_WEIGHT_UNSUPPORTED = 2,
      weight_unsupported = OFPGMFC_WEIGHT_UNSUPPORTED,
      OFPGMFC_OUT_OF_GROUPS = 3,
      out_of_groups = OFPGMFC_OUT_OF_GROUPS,
      OFPGMFC_OUT_OF_BUCKETS = 4,
      out_of_buckets = OFPGMFC_OUT_OF_BUCKETS,
      OFPGMFC_CHAINING_UNSUPPORTED = 5,
      chaining_unsupported = OFPGMFC_CHAINING_UNSUPPORTED,
      OFPGMFC_WATCH_UNSUPPORTED = 6,
      watch_unsupported = OFPGMFC_WATCH_UNSUPPORTED,
      OFPGMFC_LOOP = 7,
      loop = OFPGMFC_LOOP,
      OFPGMFC_UNKNOWN_GROUP = 8,
      unknown_group = OFPGMFC_UNKNOWN_GROUP,
      OFPGMFC_CHAINED_GROUP = 9,
      chained_group = OFPGMFC_CHAINED_GROUP,
      OFPGMFC_BAD_TYPE = 10,
      bad_type = OFPGMFC_BAD_TYPE,
      OFPGMFC_BAD_COMMAND = 11,
      bad_command = OFPGMFC_BAD_COMMAND,
      OFPGMFC_BAD_BUCKET = 12,
      bad_bucket = OFPGMFC_BAD_BUCKET,
      OFPGMFC_BAD_WATCH = 13,
      bad_watch = OFPGMFC_BAD_WATCH,
      OFPGMFC_EPERM = 14,
      eperm = OFPGMFC_EPERM,
    };

  } // namespace ofp_group_mod_failed_code_detail

  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_GROUP_EXISTS;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_INVALID_GROUP;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_WEIGHT_UNSUPPORTED;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_OUT_OF_GROUPS;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_OUT_OF_BUCKETS;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_CHAINING_UNSUPPORTED;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_WATCH_UNSUPPORTED;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_LOOP;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_UNKNOWN_GROUP;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_CHAINED_GROUP;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_BAD_TYPE;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_BAD_COMMAND;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_BAD_BUCKET;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_BAD_WATCH;
  using ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code::OFPGMFC_EPERM;

  using group_mod_failed_code = ofp_group_mod_failed_code_detail::ofp_group_mod_failed_code;

  namespace ofp_port_mod_failed_code_detail {

    enum ofp_port_mod_failed_code // : std::uint16_t
    {
      OFPPMFC_BAD_PORT = 0,
      bad_port = OFPPMFC_BAD_PORT,
      OFPPMFC_BAD_HW_ADDR = 1,
      bad_hw_addr = OFPPMFC_BAD_HW_ADDR,
      OFPPMFC_BAD_CONFIG = 2,
      bad_config = OFPPMFC_BAD_CONFIG,
      OFPPMFC_BAD_ADVERTISE = 3,
      bad_advertise = OFPPMFC_BAD_ADVERTISE,
      OFPPMFC_EPERM = 4,
      eperm = OFPPMFC_EPERM,
    };

  } // namespace ofp_port_mod_failed_code_detail

  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code;
  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code::OFPPMFC_BAD_PORT;
  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code::OFPPMFC_BAD_HW_ADDR;
  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code::OFPPMFC_BAD_CONFIG;
  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code::OFPPMFC_BAD_ADVERTISE;
  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code::OFPPMFC_EPERM;

  using port_mod_failed_code = ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code;

  namespace ofp_table_mod_failed_code_detail {

    enum ofp_table_mod_failed_code // : std::uint16_t
    {
      OFPTMFC_BAD_TABLE = 0,
      bad_table = OFPTMFC_BAD_TABLE,
      OFPTMFC_BAD_CONFIG = 1,
      bad_config = OFPTMFC_BAD_CONFIG,
      OFPTMFC_EPERM = 2,
      eperm = OFPTMFC_EPERM,
    };

  } // namespace ofp_table_mod_failed_code_detail

  using ofp_table_mod_failed_code_detail::ofp_table_mod_failed_code;
  using ofp_table_mod_failed_code_detail::ofp_table_mod_failed_code::OFPTMFC_BAD_TABLE;
  using ofp_table_mod_failed_code_detail::ofp_table_mod_failed_code::OFPTMFC_BAD_CONFIG;
  using ofp_table_mod_failed_code_detail::ofp_table_mod_failed_code::OFPTMFC_EPERM;

  using table_mod_failed_code = ofp_table_mod_failed_code_detail::ofp_table_mod_failed_code;

  namespace ofp_queue_op_failed_code_detail {

    enum ofp_queue_op_failed_code // : std::uint16_t
    {
      OFPQOFC_BAD_PORT = 0,
      bad_port = OFPQOFC_BAD_PORT,
      OFPQOFC_BAD_QUEUE = 1,
      bad_queue = OFPQOFC_BAD_QUEUE,
      OFPQOFC_EPERM = 2,
      eperm = OFPQOFC_EPERM,
    };

  } // namespace ofp_queue_op_failed_code_detail

  using ofp_queue_op_failed_code_detail::ofp_queue_op_failed_code;
  using ofp_queue_op_failed_code_detail::ofp_queue_op_failed_code::OFPQOFC_BAD_PORT;
  using ofp_queue_op_failed_code_detail::ofp_queue_op_failed_code::OFPQOFC_BAD_QUEUE;
  using ofp_queue_op_failed_code_detail::ofp_queue_op_failed_code::OFPQOFC_EPERM;

  using queue_op_failed_code = ofp_queue_op_failed_code_detail::ofp_queue_op_failed_code;

  namespace ofp_switch_config_failed_code_detail {

    enum ofp_switch_config_failed_code // : std::uint16_t
    {
      OFPSCFC_BAD_FLAGS = 0,
      bad_flags = OFPSCFC_BAD_FLAGS,
      OFPSCFC_BAD_LEN = 1,
      bad_len = OFPSCFC_BAD_LEN,
      OFPSCFC_EPERM = 2,
      eperm = OFPSCFC_EPERM,
    };

  } // namespace ofp_switch_config_failed_code_detail

  using ofp_switch_config_failed_code_detail::ofp_switch_config_failed_code;
  using ofp_switch_config_failed_code_detail::ofp_switch_config_failed_code::OFPSCFC_BAD_FLAGS;
  using ofp_switch_config_failed_code_detail::ofp_switch_config_failed_code::OFPSCFC_BAD_LEN;
  using ofp_switch_config_failed_code_detail::ofp_switch_config_failed_code::OFPSCFC_EPERM;

  using switch_config_failed_code = ofp_switch_config_failed_code_detail::ofp_switch_config_failed_code;

  namespace ofp_role_request_failed_code_detail {

    enum ofp_role_request_failed_code // : std::uint16_t
    {
      OFPRRFC_STALE = 0,
      stale = OFPRRFC_STALE,
      OFPRRFC_UNSUP = 1,
      unsup = OFPRRFC_UNSUP,
      OFPRRFC_BAD_ROLE = 2,
      bad_role = OFPRRFC_BAD_ROLE,
    };

  } // namespace ofp_role_request_failed_code_detail

  using ofp_role_request_failed_code_detail::ofp_role_request_failed_code;
  using ofp_role_request_failed_code_detail::ofp_role_request_failed_code::OFPRRFC_STALE;
  using ofp_role_request_failed_code_detail::ofp_role_request_failed_code::OFPRRFC_UNSUP;
  using ofp_role_request_failed_code_detail::ofp_role_request_failed_code::OFPRRFC_BAD_ROLE;

  using role_request_failed_code = ofp_role_request_failed_code_detail::ofp_role_request_failed_code;

  namespace ofp_meter_mod_failed_code_detail {

    enum ofp_meter_mod_failed_code // : std::uint16_t
    {
      OFPMMFC_UNKNOWN = 0,
      unknown = OFPMMFC_UNKNOWN,
      OFPMMFC_METER_EXISTS = 1,
      meter_exists = OFPMMFC_METER_EXISTS,
      OFPMMFC_INVALID_METER = 2,
      invalid_meter = OFPMMFC_INVALID_METER,
      OFPMMFC_UNKNOWN_METER = 3,
      unknown_meter = OFPMMFC_UNKNOWN_METER,
      OFPMMFC_BAD_COMMAND = 4,
      bad_command = OFPMMFC_BAD_COMMAND,
      OFPMMFC_BAD_FLAGS = 5,
      bad_flags = OFPMMFC_BAD_FLAGS,
      OFPMMFC_BAD_RATE = 6,
      bad_rate = OFPMMFC_BAD_RATE,
      OFPMMFC_BAD_BURST = 7,
      bad_burst = OFPMMFC_BAD_BURST,
      OFPMMFC_BAD_BAND = 8,
      bad_band = OFPMMFC_BAD_BAND,
      OFPMMFC_BAD_BAND_VALUE = 9,
      bad_band_value = OFPMMFC_BAD_BAND_VALUE,
      OFPMMFC_OUT_OF_METERS = 10,
      out_of_meters = OFPMMFC_OUT_OF_METERS,
      OFPMMFC_OUT_OF_BANDS = 11,
      out_of_bands = OFPMMFC_OUT_OF_BANDS,
    };

  } // namespace ofp_meter_mod_failed_code_detail

  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_UNKNOWN;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_METER_EXISTS;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_INVALID_METER;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_UNKNOWN_METER;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_BAD_COMMAND;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_BAD_FLAGS;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_BAD_RATE;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_BAD_BURST;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_BAD_BAND;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_BAD_BAND_VALUE;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_OUT_OF_METERS;
  using ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code::OFPMMFC_OUT_OF_BANDS;

  using meter_mod_failed_code = ofp_meter_mod_failed_code_detail::ofp_meter_mod_failed_code;

  namespace ofp_table_features_failed_code_detail {

    enum ofp_table_features_failed_code // : std::uint16_t
    {
      OFPTFFC_BAD_TABLE = 0,
      bad_table = OFPTFFC_BAD_TABLE,
      OFPTFFC_BAD_METADATA = 1,
      bad_metadata = OFPTFFC_BAD_METADATA,
      OFPTFFC_BAD_TYPE = 2,
      bad_type = OFPTFFC_BAD_TYPE,
      OFPTFFC_BAD_LEN = 3,
      bad_len = OFPTFFC_BAD_LEN,
      OFPTFFC_BAD_ARGUMENT = 4,
      bad_argument = OFPTFFC_BAD_ARGUMENT,
      OFPTFFC_EPERM = 5,
      eperm = OFPTFFC_EPERM,
    };

  } // namespace ofp_table_features_failed_code_detail

  using ofp_table_features_failed_code_detail::ofp_table_features_failed_code;
  using ofp_table_features_failed_code_detail::ofp_table_features_failed_code::OFPTFFC_BAD_TABLE;
  using ofp_table_features_failed_code_detail::ofp_table_features_failed_code::OFPTFFC_BAD_METADATA;
  using ofp_table_features_failed_code_detail::ofp_table_features_failed_code::OFPTFFC_BAD_TYPE;
  using ofp_table_features_failed_code_detail::ofp_table_features_failed_code::OFPTFFC_BAD_LEN;
  using ofp_table_features_failed_code_detail::ofp_table_features_failed_code::OFPTFFC_BAD_ARGUMENT;
  using ofp_table_features_failed_code_detail::ofp_table_features_failed_code::OFPTFFC_EPERM;

  using table_features_failed_code = ofp_table_features_failed_code_detail::ofp_table_features_failed_code;

  namespace ofp_multipart_type_detail {

    enum ofp_multipart_type // : std::uint16_t
    {
      OFPMP_DESC = 0,
      desc = OFPMP_DESC,
      OFPMP_FLOW = 1,
      flow = OFPMP_FLOW,
      OFPMP_AGGREGATE = 2,
      aggregate = OFPMP_AGGREGATE,
      OFPMP_TABLE = 3,
      table = OFPMP_TABLE,
      OFPMP_PORT_STATS = 4,
      port_stats = OFPMP_PORT_STATS,
      OFPMP_QUEUE = 5,
      queue = OFPMP_QUEUE,
      OFPMP_GROUP = 6,
      group = OFPMP_GROUP,
      OFPMP_GROUP_DESC = 7,
      group_desc = OFPMP_GROUP_DESC,
      OFPMP_GROUP_FEATURES = 8,
      group_features = OFPMP_GROUP_FEATURES,
      OFPMP_METER = 9,
      meter = OFPMP_METER,
      OFPMP_METER_CONFIG = 10,
      meter_config = OFPMP_METER_CONFIG,
      OFPMP_METER_FEATURES = 11,
      meter_features = OFPMP_METER_FEATURES,
      OFPMP_TABLE_FEATURES = 12,
      table_features = OFPMP_TABLE_FEATURES,
      OFPMP_PORT_DESC = 13,
      port_desc = OFPMP_PORT_DESC,
      OFPMP_EXPERIMENTER = 65535,
      experimenter = OFPMP_EXPERIMENTER,
    };

  } // namespace ofp_multipart_type_detail

  using ofp_multipart_type_detail::ofp_multipart_type;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_DESC;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_FLOW;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_AGGREGATE;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_TABLE;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_PORT_STATS;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_QUEUE;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_GROUP;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_GROUP_DESC;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_GROUP_FEATURES;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_METER;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_METER_CONFIG;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_METER_FEATURES;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_TABLE_FEATURES;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_PORT_DESC;
  using ofp_multipart_type_detail::ofp_multipart_type::OFPMP_EXPERIMENTER;

  using multipart_type = ofp_multipart_type_detail::ofp_multipart_type;

  namespace ofp_multipart_request_flags_detail {

    enum ofp_multipart_request_flags // : std::uint16_t
    {
      OFPMPF_REQ_MORE = 1,
      req_more = OFPMPF_REQ_MORE,
    };

  } // namespace ofp_multipart_request_flags_detail

  using ofp_multipart_request_flags_detail::ofp_multipart_request_flags;
  using ofp_multipart_request_flags_detail::ofp_multipart_request_flags::OFPMPF_REQ_MORE;

  using multipart_request_flags = ofp_multipart_request_flags_detail::ofp_multipart_request_flags;

  namespace ofp_multipart_reply_flags_detail {

    enum ofp_multipart_reply_flags // : std::uint16_t
    {
      OFPMPF_REPLY_MORE = 1,
      reply_more = OFPMPF_REPLY_MORE,
    };

  } // namespace ofp_multipart_reply_flags_detail

  using ofp_multipart_reply_flags_detail::ofp_multipart_reply_flags;
  using ofp_multipart_reply_flags_detail::ofp_multipart_reply_flags::OFPMPF_REPLY_MORE;

  using multipart_reply_flags = ofp_multipart_reply_flags_detail::ofp_multipart_reply_flags;

  namespace ofp_table_feature_prop_type_detail {

    enum ofp_table_feature_prop_type // : std::uint16_t
    {
      OFPTFPT_INSTRUCTIONS = 0,
      instructions = OFPTFPT_INSTRUCTIONS,
      OFPTFPT_INSTRUCTIONS_MISS = 1,
      instructions_miss = OFPTFPT_INSTRUCTIONS_MISS,
      OFPTFPT_NEXT_TABLES = 2,
      next_tables = OFPTFPT_NEXT_TABLES,
      OFPTFPT_NEXT_TABLES_MISS = 3,
      next_tables_miss = OFPTFPT_NEXT_TABLES_MISS,
      OFPTFPT_WRITE_ACTIONS = 4,
      write_actions = OFPTFPT_WRITE_ACTIONS,
      OFPTFPT_WRITE_ACTIONS_MISS = 5,
      write_actions_miss = OFPTFPT_WRITE_ACTIONS_MISS,
      OFPTFPT_APPLY_ACTIONS = 6,
      apply_actions = OFPTFPT_APPLY_ACTIONS,
      OFPTFPT_APPLY_ACTIONS_MISS = 7,
      apply_actions_miss = OFPTFPT_APPLY_ACTIONS_MISS,
      OFPTFPT_MATCH = 8,
      match = OFPTFPT_MATCH,
      OFPTFPT_WILDCARDS = 10,
      wildcards = OFPTFPT_WILDCARDS,
      OFPTFPT_WRITE_SETFIELD = 12,
      write_setfield = OFPTFPT_WRITE_SETFIELD,
      OFPTFPT_WRITE_SETFIELD_MISS = 13,
      write_setfield_miss = OFPTFPT_WRITE_SETFIELD_MISS,
      OFPTFPT_APPLY_SETFIELD = 14,
      apply_setfield = OFPTFPT_APPLY_SETFIELD,
      OFPTFPT_APPLY_SETFIELD_MISS = 15,
      apply_setfield_miss = OFPTFPT_APPLY_SETFIELD_MISS,
      OFPTFPT_EXPERIMENTER = 65534,
      experimenter = OFPTFPT_EXPERIMENTER,
      OFPTFPT_EXPERIMENTER_MISS = 65535,
      experimenter_miss = OFPTFPT_EXPERIMENTER_MISS,
    };

  } // namespace ofp_table_feature_prop_type_detail

  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_INSTRUCTIONS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_INSTRUCTIONS_MISS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_NEXT_TABLES;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_NEXT_TABLES_MISS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_WRITE_ACTIONS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_WRITE_ACTIONS_MISS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_APPLY_ACTIONS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_APPLY_ACTIONS_MISS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_MATCH;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_WILDCARDS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_WRITE_SETFIELD;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_WRITE_SETFIELD_MISS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_APPLY_SETFIELD;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_APPLY_SETFIELD_MISS;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_EXPERIMENTER;
  using ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type::OFPTFPT_EXPERIMENTER_MISS;

  using table_feature_prop_type = ofp_table_feature_prop_type_detail::ofp_table_feature_prop_type;

  namespace ofp_group_capabilities_detail {

    enum ofp_group_capabilities // : std::uint32_t
    {
      OFPGFC_SELECT_WEIGHT = 1,
      select_weight = OFPGFC_SELECT_WEIGHT,
      OFPGFC_SELECT_LIVENESS = 2,
      select_liveness = OFPGFC_SELECT_LIVENESS,
      OFPGFC_CHAINING = 4,
      chaining = OFPGFC_CHAINING,
      OFPGFC_CHAINING_CHECKS = 8,
      chaining_checks = OFPGFC_CHAINING_CHECKS,
    };

  } // namespace ofp_group_capabilities_detail

  using ofp_group_capabilities_detail::ofp_group_capabilities;
  using ofp_group_capabilities_detail::ofp_group_capabilities::OFPGFC_SELECT_WEIGHT;
  using ofp_group_capabilities_detail::ofp_group_capabilities::OFPGFC_SELECT_LIVENESS;
  using ofp_group_capabilities_detail::ofp_group_capabilities::OFPGFC_CHAINING;
  using ofp_group_capabilities_detail::ofp_group_capabilities::OFPGFC_CHAINING_CHECKS;

  using group_capabilities = ofp_group_capabilities_detail::ofp_group_capabilities;

  namespace ofp_queue_properties_detail {

    enum ofp_queue_properties // : std::uint16_t
    {
      OFPQT_MIN_RATE = 1,
      min_rate = OFPQT_MIN_RATE,
      OFPQT_MAX_RATE = 2,
      max_rate = OFPQT_MAX_RATE,
      OFPQT_EXPERIMENTER = 65535,
      experimenter = OFPQT_EXPERIMENTER,
    };

  } // namespace ofp_queue_properties_detail

  using ofp_queue_properties_detail::ofp_queue_properties;
  using ofp_queue_properties_detail::ofp_queue_properties::OFPQT_MIN_RATE;
  using ofp_queue_properties_detail::ofp_queue_properties::OFPQT_MAX_RATE;
  using ofp_queue_properties_detail::ofp_queue_properties::OFPQT_EXPERIMENTER;

  using queue_prop_type = ofp_queue_properties_detail::ofp_queue_properties;

  namespace ofp_controller_role_detail {

    enum ofp_controller_role // : std::uint32_t
    {
      OFPCR_ROLE_NOCHANGE = 0,
      nochange = OFPCR_ROLE_NOCHANGE,
      OFPCR_ROLE_EQUAL = 1,
      equal = OFPCR_ROLE_EQUAL,
      OFPCR_ROLE_MASTER = 2,
      master = OFPCR_ROLE_MASTER,
      OFPCR_ROLE_SLAVE = 3,
      slave = OFPCR_ROLE_SLAVE,
    };

  } // namespace ofp_controller_role_detail

  using ofp_controller_role_detail::ofp_controller_role;
  using ofp_controller_role_detail::ofp_controller_role::OFPCR_ROLE_NOCHANGE;
  using ofp_controller_role_detail::ofp_controller_role::OFPCR_ROLE_EQUAL;
  using ofp_controller_role_detail::ofp_controller_role::OFPCR_ROLE_MASTER;
  using ofp_controller_role_detail::ofp_controller_role::OFPCR_ROLE_SLAVE;

  using controller_role = ofp_controller_role_detail::ofp_controller_role;

} // namespace protocol
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OPENFLOW_HPP
