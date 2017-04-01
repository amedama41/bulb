#ifndef CANARD_NET_OFP_V10_OPENFLOW_HPP
#define CANARD_NET_OFP_V10_OPENFLOW_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace protocol {

  struct ofp_header
  {
    std::uint8_t version;
    std::uint8_t type;
    std::uint16_t length;
    std::uint32_t xid;
  };
  static_assert(sizeof(ofp_header) == 8, "");

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

  struct ofp_phy_port
  {
    std::uint16_t port_no;
    std::uint8_t hw_addr[6];
    char name[16];
    std::uint32_t config;
    std::uint32_t state;
    std::uint32_t curr;
    std::uint32_t advertised;
    std::uint32_t supported;
    std::uint32_t peer;
  };
  static_assert(sizeof(ofp_phy_port) == 48, "");

  struct ofp_switch_features
  {
    ofp_header header;
    std::uint64_t datapath_id;
    std::uint32_t n_buffers;
    std::uint8_t n_tables;
    std::uint8_t pad[3];
    std::uint32_t capabilities;
    std::uint32_t actions;
  };
  static_assert(sizeof(ofp_switch_features) == 32, "");

  struct ofp_port_status
  {
    ofp_header header;
    std::uint8_t reason;
    std::uint8_t pad[7];
    ofp_phy_port desc;
  };
  static_assert(sizeof(ofp_port_status) == 64, "");

  struct ofp_port_mod
  {
    ofp_header header;
    std::uint16_t port_no;
    std::uint8_t hw_addr[6];
    std::uint32_t config;
    std::uint32_t mask;
    std::uint32_t advertise;
    std::uint8_t pad[4];
  };
  static_assert(sizeof(ofp_port_mod) == 32, "");

  struct ofp_packet_in
  {
    ofp_header header;
    std::uint32_t buffer_id;
    std::uint16_t total_len;
    std::uint16_t in_port;
    std::uint8_t reason;
    std::uint8_t pad;
  };
  static_assert(sizeof(ofp_packet_in) == 20, "");

  struct ofp_action_output
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint16_t port;
    std::uint16_t max_len;
  };
  static_assert(sizeof(ofp_action_output) == 8, "");

  struct ofp_action_vlan_vid
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint16_t vlan_vid;
    std::uint8_t pad[2];
  };
  static_assert(sizeof(ofp_action_vlan_vid) == 8, "");

  struct ofp_action_vlan_pcp
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint8_t vlan_pcp;
    std::uint8_t pad[3];
  };
  static_assert(sizeof(ofp_action_vlan_pcp) == 8, "");

  struct ofp_action_dl_addr
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint8_t dl_addr[6];
    std::uint8_t pad[6];
  };
  static_assert(sizeof(ofp_action_dl_addr) == 16, "");

  struct ofp_action_nw_addr
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint32_t nw_addr;
  };
  static_assert(sizeof(ofp_action_nw_addr) == 8, "");

  struct ofp_action_tp_port
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint16_t tp_port;
    std::uint8_t pad[2];
  };
  static_assert(sizeof(ofp_action_tp_port) == 8, "");

  struct ofp_action_nw_tos
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint8_t nw_tos;
    std::uint8_t pad[3];
  };
  static_assert(sizeof(ofp_action_nw_tos) == 8, "");

  struct ofp_action_vendor_header
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint32_t vendor;
  };
  static_assert(sizeof(ofp_action_vendor_header) == 8, "");

  struct ofp_action_header
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint8_t pad[4];
  };
  static_assert(sizeof(ofp_action_header) == 8, "");

  struct ofp_packet_out
  {
    ofp_header header;
    std::uint32_t buffer_id;
    std::uint16_t in_port;
    std::uint16_t actions_len;
  };
  static_assert(sizeof(ofp_packet_out) == 16, "");

  struct ofp_match
  {
    std::uint32_t wildcards;
    std::uint16_t in_port;
    std::uint8_t dl_src[6];
    std::uint8_t dl_dst[6];
    std::uint16_t dl_vlan;
    std::uint8_t dl_vlan_pcp;
    std::uint8_t pad1[1];
    std::uint16_t dl_type;
    std::uint8_t nw_tos;
    std::uint8_t nw_proto;
    std::uint8_t pad2[2];
    std::uint32_t nw_src;
    std::uint32_t nw_dst;
    std::uint16_t tp_src;
    std::uint16_t tp_dst;
  };
  static_assert(sizeof(ofp_match) == 40, "");

  struct ofp_flow_mod
  {
    ofp_header header;
    ofp_match match;
    std::uint64_t cookie;
    std::uint16_t command;
    std::uint16_t idle_timeout;
    std::uint16_t hard_timeout;
    std::uint16_t priority;
    std::uint32_t buffer_id;
    std::uint16_t out_port;
    std::uint16_t flags;
  };
  static_assert(sizeof(ofp_flow_mod) == 72, "");

  struct ofp_flow_removed
  {
    ofp_header header;
    ofp_match match;
    std::uint64_t cookie;
    std::uint16_t priority;
    std::uint8_t reason;
    std::uint8_t pad[1];
    std::uint32_t duration_sec;
    std::uint32_t duration_nsec;
    std::uint16_t idle_timeout;
    std::uint8_t pad2[2];
    std::uint64_t packet_count;
    std::uint64_t byte_count;
  };
  static_assert(sizeof(ofp_flow_removed) == 88, "");

  struct ofp_error_msg
  {
    ofp_header header;
    std::uint16_t type;
    std::uint16_t code;
  };
  static_assert(sizeof(ofp_error_msg) == 12, "");

  struct ofp_stats_request
  {
    ofp_header header;
    std::uint16_t type;
    std::uint16_t flags;
  };
  static_assert(sizeof(ofp_stats_request) == 12, "");

  struct ofp_stats_reply
  {
    ofp_header header;
    std::uint16_t type;
    std::uint16_t flags;
  };
  static_assert(sizeof(ofp_stats_reply) == 12, "");

  struct ofp_desc_stats
  {
    char mfr_desc[256];
    char hw_desc[256];
    char sw_desc[256];
    char serial_num[32];
    char dp_desc[256];
  };
  static_assert(sizeof(ofp_desc_stats) == 1056, "");

  struct ofp_flow_stats_request
  {
    ofp_match match;
    std::uint8_t table_id;
    std::uint8_t pad;
    std::uint16_t out_port;
  };
  static_assert(sizeof(ofp_flow_stats_request) == 44, "");

  struct ofp_flow_stats
  {
    std::uint16_t length;
    std::uint8_t table_id;
    std::uint8_t pad;
    ofp_match match;
    std::uint32_t duration_sec;
    std::uint32_t duration_nsec;
    std::uint16_t priority;
    std::uint16_t idle_timeout;
    std::uint16_t hard_timeout;
    std::uint8_t pad2[6];
    std::uint64_t cookie;
    std::uint64_t packet_count;
    std::uint64_t byte_count;
  };
  static_assert(sizeof(ofp_flow_stats) == 88, "");

  struct ofp_aggregate_stats_request
  {
    ofp_match match;
    std::uint8_t table_id;
    std::uint8_t pad;
    std::uint16_t out_port;
  };
  static_assert(sizeof(ofp_aggregate_stats_request) == 44, "");

  struct ofp_aggregate_stats_reply
  {
    std::uint64_t packet_count;
    std::uint64_t byte_count;
    std::uint32_t flow_count;
    std::uint8_t pad[4];
  };
  static_assert(sizeof(ofp_aggregate_stats_reply) == 24, "");

  struct ofp_table_stats
  {
    std::uint8_t table_id;
    std::uint8_t pad[3];
    char name[32];
    std::uint32_t wildcards;
    std::uint32_t max_entries;
    std::uint32_t active_count;
    std::uint64_t lookup_count;
    std::uint64_t matched_count;
  };
  static_assert(sizeof(ofp_table_stats) == 64, "");

  struct ofp_port_stats_request
  {
    std::uint16_t port_no;
    std::uint8_t pad[6];
  };
  static_assert(sizeof(ofp_port_stats_request) == 8, "");

  struct ofp_port_stats
  {
    std::uint16_t port_no;
    std::uint8_t pad[6];
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
  };
  static_assert(sizeof(ofp_port_stats) == 104, "");

  struct ofp_vendor_header
  {
    ofp_header header;
    std::uint32_t vendor;
  };
  static_assert(sizeof(ofp_vendor_header) == 12, "");

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

  struct ofp_packet_queue
  {
    std::uint32_t queue_id;
    std::uint16_t len;
    std::uint8_t pad[2];
  };
  static_assert(sizeof(ofp_packet_queue) == 8, "");

  struct ofp_queue_get_config_request
  {
    ofp_header header;
    std::uint16_t port;
    std::uint8_t pad[2];
  };
  static_assert(sizeof(ofp_queue_get_config_request) == 12, "");

  struct ofp_queue_get_config_reply
  {
    ofp_header header;
    std::uint16_t port;
    std::uint8_t pad[6];
  };
  static_assert(sizeof(ofp_queue_get_config_reply) == 16, "");

  struct ofp_action_enqueue
  {
    std::uint16_t type;
    std::uint16_t len;
    std::uint16_t port;
    std::uint8_t pad[6];
    std::uint32_t queue_id;
  };
  static_assert(sizeof(ofp_action_enqueue) == 16, "");

  struct ofp_queue_stats_request
  {
    std::uint16_t port_no;
    std::uint8_t pad[2];
    std::uint32_t queue_id;
  };
  static_assert(sizeof(ofp_queue_stats_request) == 8, "");

  struct ofp_queue_stats
  {
    std::uint16_t port_no;
    std::uint8_t pad[2];
    std::uint32_t queue_id;
    std::uint64_t tx_bytes;
    std::uint64_t tx_packets;
    std::uint64_t tx_errors;
  };
  static_assert(sizeof(ofp_queue_stats) == 32, "");

  enum uint8_t_constants : std::uint8_t
  {
      OFP_VERSION = 0x01,
  };

  enum uint16_t_constants : std::uint16_t
  {
      OFP_TCP_PORT = 6633,
      OFP_SSL_PORT = 6633,
      OFP_DEFAULT_MISS_SEND_LEN = 128,
      OFP_VLAN_NONE = 0xffff,
      OFP_DL_TYPE_ETH2_CUTOFF = 0x0600,
      OFP_DL_TYPE_NOT_ETH_TYPE = 0x05ff,
      OFP_FLOW_PERMANENT = 0,
      OFP_DEFAULT_PRIORITY = 0x8000,
      OFPQ_MIN_RATE_UNCFG = 0xffff,
  };

  enum uint32_t_constants : std::uint32_t
  {
      OFP_NO_BUFFER = 0xffffffff,
      OFPQ_ALL = 0xffffffff,
  };

  enum size_t_constants : std::size_t
  {
      OFP_MAX_TABLE_NAME_LEN = 32,
      OFP_MAX_PORT_NAME_LEN = 16,
      OFP_ETH_ALEN = 6,
      DESC_STR_LEN = 256,
      SERIAL_NUM_LEN = 32,
  };

  namespace ofp_port_detail {

    enum ofp_port : std::uint16_t
    {
      OFPP_MAX = 65280,
      max = OFPP_MAX,
      OFPP_IN_PORT = 65528,
      in_port = OFPP_IN_PORT,
      OFPP_TABLE = 65529,
      table = OFPP_TABLE,
      OFPP_NORMAL = 65530,
      normal = OFPP_NORMAL,
      OFPP_FLOOD = 65531,
      flood = OFPP_FLOOD,
      OFPP_ALL = 65532,
      all = OFPP_ALL,
      OFPP_CONTROLLER = 65533,
      controller = OFPP_CONTROLLER,
      OFPP_LOCAL = 65534,
      local = OFPP_LOCAL,
      OFPP_NONE = 65535,
      OFPP_ANY = OFPP_NONE,
      none = OFPP_NONE,
      any = none,
    };

  } // namespace ofp_port_detail

  using ofp_port_detail::ofp_port;
  using ofp_port_detail::ofp_port::OFPP_MAX;
  using ofp_port_detail::ofp_port::OFPP_IN_PORT;
  using ofp_port_detail::ofp_port::OFPP_TABLE;
  using ofp_port_detail::ofp_port::OFPP_NORMAL;
  using ofp_port_detail::ofp_port::OFPP_FLOOD;
  using ofp_port_detail::ofp_port::OFPP_ALL;
  using ofp_port_detail::ofp_port::OFPP_CONTROLLER;
  using ofp_port_detail::ofp_port::OFPP_LOCAL;
  using ofp_port_detail::ofp_port::OFPP_NONE;
  using ofp_port_detail::ofp_port::OFPP_ANY;

  using port_no = ofp_port_detail::ofp_port;

  namespace ofp_type_detail {

    enum ofp_type : std::uint8_t
    {
      OFPT_HELLO = 0,
      hello = OFPT_HELLO,
      OFPT_ERROR = 1,
      error = OFPT_ERROR,
      OFPT_ECHO_REQUEST = 2,
      echo_request = OFPT_ECHO_REQUEST,
      OFPT_ECHO_REPLY = 3,
      echo_reply = OFPT_ECHO_REPLY,
      OFPT_VENDOR = 4,
      OFPT_EXPERIMENTER = OFPT_VENDOR,
      vendor = OFPT_VENDOR,
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
      OFPT_PORT_MOD = 15,
      port_mod = OFPT_PORT_MOD,
      OFPT_STATS_REQUEST = 16,
      stats_request = OFPT_STATS_REQUEST,
      OFPT_STATS_REPLY = 17,
      stats_reply = OFPT_STATS_REPLY,
      OFPT_BARRIER_REQUEST = 18,
      barrier_request = OFPT_BARRIER_REQUEST,
      OFPT_BARRIER_REPLY = 19,
      barrier_reply = OFPT_BARRIER_REPLY,
      OFPT_QUEUE_GET_CONFIG_REQUEST = 20,
      queue_get_config_request = OFPT_QUEUE_GET_CONFIG_REQUEST,
      OFPT_QUEUE_GET_CONFIG_REPLY = 21,
      queue_get_config_reply = OFPT_QUEUE_GET_CONFIG_REPLY,
    };

  } // namespace ofp_type_detail

  using ofp_type_detail::ofp_type;
  using ofp_type_detail::ofp_type::OFPT_HELLO;
  using ofp_type_detail::ofp_type::OFPT_ERROR;
  using ofp_type_detail::ofp_type::OFPT_ECHO_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_ECHO_REPLY;
  using ofp_type_detail::ofp_type::OFPT_VENDOR;
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
  using ofp_type_detail::ofp_type::OFPT_PORT_MOD;
  using ofp_type_detail::ofp_type::OFPT_STATS_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_STATS_REPLY;
  using ofp_type_detail::ofp_type::OFPT_BARRIER_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_BARRIER_REPLY;
  using ofp_type_detail::ofp_type::OFPT_QUEUE_GET_CONFIG_REQUEST;
  using ofp_type_detail::ofp_type::OFPT_QUEUE_GET_CONFIG_REPLY;

  using msg_type = ofp_type_detail::ofp_type;

  namespace ofp_config_flags_detail {

    enum ofp_config_flags : std::uint16_t
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

  namespace ofp_capabilities_detail {

    enum ofp_capabilities : std::uint32_t
    {
      OFPC_FLOW_STATS = 1,
      flow_stats = OFPC_FLOW_STATS,
      OFPC_TABLE_STATS = 2,
      table_stats = OFPC_TABLE_STATS,
      OFPC_PORT_STATS = 4,
      port_stats = OFPC_PORT_STATS,
      OFPC_STP = 8,
      stp = OFPC_STP,
      OFPC_RESERVED = 16,
      reserved = OFPC_RESERVED,
      OFPC_IP_REASM = 32,
      ip_reasm = OFPC_IP_REASM,
      OFPC_QUEUE_STATS = 64,
      queue_stats = OFPC_QUEUE_STATS,
      OFPC_ARP_MATCH_IP = 128,
      arp_match_ip = OFPC_ARP_MATCH_IP,
    };

  } // namespace ofp_capabilities_detail

  using ofp_capabilities_detail::ofp_capabilities;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_FLOW_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_TABLE_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_PORT_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_STP;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_RESERVED;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_IP_REASM;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_QUEUE_STATS;
  using ofp_capabilities_detail::ofp_capabilities::OFPC_ARP_MATCH_IP;

  using capabilities = ofp_capabilities_detail::ofp_capabilities;

  namespace ofp_port_config_detail {

    enum ofp_port_config : std::uint32_t
    {
      OFPPC_PORT_DOWN = 1,
      port_down = OFPPC_PORT_DOWN,
      OFPPC_NO_STP = 2,
      no_stp = OFPPC_NO_STP,
      OFPPC_NO_RECV = 4,
      no_recv = OFPPC_NO_RECV,
      OFPPC_NO_RECV_STP = 8,
      no_recv_stp = OFPPC_NO_RECV_STP,
      OFPPC_NO_FLOOD = 16,
      no_flood = OFPPC_NO_FLOOD,
      OFPPC_NO_FWD = 32,
      no_fwd = OFPPC_NO_FWD,
      OFPPC_NO_PACKET_IN = 64,
      no_packet_in = OFPPC_NO_PACKET_IN,
    };

  } // namespace ofp_port_config_detail

  using ofp_port_config_detail::ofp_port_config;
  using ofp_port_config_detail::ofp_port_config::OFPPC_PORT_DOWN;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_STP;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_RECV;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_RECV_STP;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_FLOOD;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_FWD;
  using ofp_port_config_detail::ofp_port_config::OFPPC_NO_PACKET_IN;

  using port_config = ofp_port_config_detail::ofp_port_config;

  namespace ofp_port_state_detail {

    enum ofp_port_state : std::uint32_t
    {
      OFPPS_LINK_DOWN = 1,
      link_down = OFPPS_LINK_DOWN,
      OFPPS_STP_LISTEN = 0,
      stp_listen = OFPPS_STP_LISTEN,
      OFPPS_STP_LEARN = 256,
      stp_learn = OFPPS_STP_LEARN,
      OFPPS_STP_FORWARD = 512,
      stp_forward = OFPPS_STP_FORWARD,
      OFPPS_STP_BLOCK = 768,
      stp_block = OFPPS_STP_BLOCK,
      OFPPS_STP_MASK = 768,
      stp_mask = OFPPS_STP_MASK,
    };

  } // namespace ofp_port_state_detail

  using ofp_port_state_detail::ofp_port_state;
  using ofp_port_state_detail::ofp_port_state::OFPPS_LINK_DOWN;
  using ofp_port_state_detail::ofp_port_state::OFPPS_STP_LISTEN;
  using ofp_port_state_detail::ofp_port_state::OFPPS_STP_LEARN;
  using ofp_port_state_detail::ofp_port_state::OFPPS_STP_FORWARD;
  using ofp_port_state_detail::ofp_port_state::OFPPS_STP_BLOCK;
  using ofp_port_state_detail::ofp_port_state::OFPPS_STP_MASK;

  using port_state = ofp_port_state_detail::ofp_port_state;

  namespace ofp_port_features_detail {

    enum ofp_port_features : std::uint32_t
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
      OFPPF_COPPER = 128,
      copper = OFPPF_COPPER,
      OFPPF_FIBER = 256,
      fiber = OFPPF_FIBER,
      OFPPF_AUTONEG = 512,
      autoneg = OFPPF_AUTONEG,
      OFPPF_PAUSE = 1024,
      pause = OFPPF_PAUSE,
      OFPPF_PAUSE_ASYM = 2048,
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
  using ofp_port_features_detail::ofp_port_features::OFPPF_COPPER;
  using ofp_port_features_detail::ofp_port_features::OFPPF_FIBER;
  using ofp_port_features_detail::ofp_port_features::OFPPF_AUTONEG;
  using ofp_port_features_detail::ofp_port_features::OFPPF_PAUSE;
  using ofp_port_features_detail::ofp_port_features::OFPPF_PAUSE_ASYM;

  using port_features = ofp_port_features_detail::ofp_port_features;

  namespace ofp_port_reason_detail {

    enum ofp_port_reason : std::uint8_t
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

  namespace ofp_packet_in_reason_detail {

    enum ofp_packet_in_reason : std::uint8_t
    {
      OFPR_NO_MATCH = 0,
      no_match = OFPR_NO_MATCH,
      OFPR_ACTION = 1,
      action = OFPR_ACTION,
    };

  } // namespace ofp_packet_in_reason_detail

  using ofp_packet_in_reason_detail::ofp_packet_in_reason;
  using ofp_packet_in_reason_detail::ofp_packet_in_reason::OFPR_NO_MATCH;
  using ofp_packet_in_reason_detail::ofp_packet_in_reason::OFPR_ACTION;

  using packet_in_reason = ofp_packet_in_reason_detail::ofp_packet_in_reason;

  namespace ofp_action_type_detail {

    enum ofp_action_type : std::uint16_t
    {
      OFPAT_OUTPUT = 0,
      output = OFPAT_OUTPUT,
      OFPAT_SET_VLAN_VID = 1,
      set_vlan_vid = OFPAT_SET_VLAN_VID,
      OFPAT_SET_VLAN_PCP = 2,
      set_vlan_pcp = OFPAT_SET_VLAN_PCP,
      OFPAT_STRIP_VLAN = 3,
      OFPAT_POP_VLAN = OFPAT_STRIP_VLAN,
      strip_vlan = OFPAT_STRIP_VLAN,
      pop_vlan = strip_vlan,
      OFPAT_SET_DL_SRC = 4,
      set_dl_src = OFPAT_SET_DL_SRC,
      OFPAT_SET_DL_DST = 5,
      set_dl_dst = OFPAT_SET_DL_DST,
      OFPAT_SET_NW_SRC = 6,
      set_nw_src = OFPAT_SET_NW_SRC,
      OFPAT_SET_NW_DST = 7,
      set_nw_dst = OFPAT_SET_NW_DST,
      OFPAT_SET_NW_TOS = 8,
      set_nw_tos = OFPAT_SET_NW_TOS,
      OFPAT_SET_TP_SRC = 9,
      set_tp_src = OFPAT_SET_TP_SRC,
      OFPAT_SET_TP_DST = 10,
      set_tp_dst = OFPAT_SET_TP_DST,
      OFPAT_ENQUEUE = 11,
      OFPAT_SET_QUEUE = OFPAT_ENQUEUE,
      enqueue = OFPAT_ENQUEUE,
      set_queue = enqueue,
      OFPAT_VENDOR = 65535,
      OFPAT_EXPERIMENTER = OFPAT_VENDOR,
      vendor = OFPAT_VENDOR,
      experimenter = vendor,
    };

  } // namespace ofp_action_type_detail

  using ofp_action_type_detail::ofp_action_type;
  using ofp_action_type_detail::ofp_action_type::OFPAT_OUTPUT;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_VLAN_VID;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_VLAN_PCP;
  using ofp_action_type_detail::ofp_action_type::OFPAT_STRIP_VLAN;
  using ofp_action_type_detail::ofp_action_type::OFPAT_POP_VLAN;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_DL_SRC;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_DL_DST;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_NW_SRC;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_NW_DST;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_NW_TOS;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_TP_SRC;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_TP_DST;
  using ofp_action_type_detail::ofp_action_type::OFPAT_ENQUEUE;
  using ofp_action_type_detail::ofp_action_type::OFPAT_SET_QUEUE;
  using ofp_action_type_detail::ofp_action_type::OFPAT_VENDOR;
  using ofp_action_type_detail::ofp_action_type::OFPAT_EXPERIMENTER;

  using action_type = ofp_action_type_detail::ofp_action_type;

  namespace ofp_flow_mod_command_detail {

    enum ofp_flow_mod_command : std::uint16_t
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

  namespace ofp_flow_wildcards_detail {

    enum ofp_flow_wildcards : std::uint32_t
    {
      OFPFW_IN_PORT = 1,
      OFPXMT_OFB_IN_PORT = OFPFW_IN_PORT,
      in_port = OFPFW_IN_PORT,
      OFPFW_DL_VLAN = 2,
      OFPXMT_OFB_VLAN_VID = OFPFW_DL_VLAN,
      dl_vlan = OFPFW_DL_VLAN,
      OFPFW_DL_SRC = 4,
      OFPXMT_OFB_ETH_SRC = OFPFW_DL_SRC,
      dl_src = OFPFW_DL_SRC,
      OFPFW_DL_DST = 8,
      OFPXMT_OFB_ETH_DST = OFPFW_DL_DST,
      dl_dst = OFPFW_DL_DST,
      OFPFW_DL_TYPE = 16,
      OFPXMT_OFB_ETH_TYPE = OFPFW_DL_TYPE,
      dl_type = OFPFW_DL_TYPE,
      OFPFW_NW_PROTO = 32,
      OFPXMT_OFB_IP_PROTO = OFPFW_NW_PROTO,
      OFPXMT_OFB_ARP_OP = OFPFW_NW_PROTO,
      nw_proto = OFPFW_NW_PROTO,
      OFPFW_TP_SRC = 64,
      OFPXMT_OFB_TCP_SRC = OFPFW_TP_SRC,
      OFPXMT_OFB_UDP_SRC = OFPFW_TP_SRC,
      OFPXMT_OFB_ICMPV4_TYPE = OFPFW_TP_SRC,
      tp_src = OFPFW_TP_SRC,
      OFPFW_TP_DST = 128,
      OFPXMT_OFB_TCP_DST = OFPFW_TP_DST,
      OFPXMT_OFB_UDP_DST = OFPFW_TP_DST,
      OFPXMT_OFB_ICMPV4_CODE = OFPFW_TP_DST,
      tp_dst = OFPFW_TP_DST,
      OFPFW_NW_SRC_SHIFT = 8,
      nw_src_shift = OFPFW_NW_SRC_SHIFT,
      OFPFW_NW_SRC_BITS = 6,
      nw_src_bits = OFPFW_NW_SRC_BITS,
      OFPFW_NW_SRC_MASK = 16128,
      nw_src_mask = OFPFW_NW_SRC_MASK,
      OFPFW_NW_SRC_ALL = 8192,
      OFPXMT_OFB_IPV4_SRC = OFPFW_NW_SRC_ALL,
      OFPXMT_OFB_ARP_SPA = OFPFW_NW_SRC_ALL,
      nw_src_all = OFPFW_NW_SRC_ALL,
      OFPFW_NW_DST_SHIFT = 14,
      nw_dst_shift = OFPFW_NW_DST_SHIFT,
      OFPFW_NW_DST_BITS = 6,
      nw_dst_bits = OFPFW_NW_DST_BITS,
      OFPFW_NW_DST_MASK = 1032192,
      nw_dst_mask = OFPFW_NW_DST_MASK,
      OFPFW_NW_DST_ALL = 524288,
      OFPXMT_OFB_IPV4_DST = OFPFW_NW_DST_ALL,
      OFPXMT_OFB_ARP_TPA = OFPFW_NW_DST_ALL,
      nw_dst_all = OFPFW_NW_DST_ALL,
      OFPFW_DL_VLAN_PCP = 1048576,
      OFPXMT_OFB_VLAN_PCP = OFPFW_DL_VLAN_PCP,
      dl_vlan_pcp = OFPFW_DL_VLAN_PCP,
      OFPFW_NW_TOS = 2097152,
      OFPXMT_OFB_IP_DSCP = OFPFW_NW_TOS,
      nw_tos = OFPFW_NW_TOS,
      OFPFW_ALL = 4194303,
      all = OFPFW_ALL,
    };

  } // namespace ofp_flow_wildcards_detail

  using ofp_flow_wildcards_detail::ofp_flow_wildcards;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_IN_PORT;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_IN_PORT;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_DL_VLAN;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_VLAN_VID;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_DL_SRC;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_ETH_SRC;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_DL_DST;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_ETH_DST;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_DL_TYPE;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_ETH_TYPE;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_PROTO;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_IP_PROTO;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_ARP_OP;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_TP_SRC;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_TCP_SRC;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_UDP_SRC;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_ICMPV4_TYPE;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_TP_DST;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_TCP_DST;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_UDP_DST;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_ICMPV4_CODE;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_SRC_SHIFT;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_SRC_BITS;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_SRC_MASK;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_SRC_ALL;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_IPV4_SRC;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_ARP_SPA;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_DST_SHIFT;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_DST_BITS;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_DST_MASK;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_DST_ALL;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_IPV4_DST;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_ARP_TPA;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_DL_VLAN_PCP;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_VLAN_PCP;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_NW_TOS;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPXMT_OFB_IP_DSCP;
  using ofp_flow_wildcards_detail::ofp_flow_wildcards::OFPFW_ALL;

  using flow_wildcards = ofp_flow_wildcards_detail::ofp_flow_wildcards;

  namespace ofp_flow_mod_flags_detail {

    enum ofp_flow_mod_flags : std::uint16_t
    {
      OFPFF_SEND_FLOW_REM = 1,
      send_flow_rem = OFPFF_SEND_FLOW_REM,
      OFPFF_CHECK_OVERLAP = 2,
      check_overlap = OFPFF_CHECK_OVERLAP,
      OFPFF_EMERG = 4,
      emerg = OFPFF_EMERG,
    };

  } // namespace ofp_flow_mod_flags_detail

  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags;
  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags::OFPFF_SEND_FLOW_REM;
  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags::OFPFF_CHECK_OVERLAP;
  using ofp_flow_mod_flags_detail::ofp_flow_mod_flags::OFPFF_EMERG;

  using flow_mod_flags = ofp_flow_mod_flags_detail::ofp_flow_mod_flags;

  namespace ofp_flow_removed_reason_detail {

    enum ofp_flow_removed_reason : std::uint8_t
    {
      OFPRR_IDLE_TIMEOUT = 0,
      by_idle_timeout = OFPRR_IDLE_TIMEOUT,
      OFPRR_HARD_TIMEOUT = 1,
      by_hard_timeout = OFPRR_HARD_TIMEOUT,
      OFPRR_DELETE = 2,
      by_delete = OFPRR_DELETE,
    };

  } // namespace ofp_flow_removed_reason_detail

  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason;
  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason::OFPRR_IDLE_TIMEOUT;
  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason::OFPRR_HARD_TIMEOUT;
  using ofp_flow_removed_reason_detail::ofp_flow_removed_reason::OFPRR_DELETE;

  using flow_removed_reason = ofp_flow_removed_reason_detail::ofp_flow_removed_reason;

  namespace ofp_error_type_detail {

    enum ofp_error_type : std::uint16_t
    {
      OFPET_HELLO_FAILED = 0,
      hello_failed = OFPET_HELLO_FAILED,
      OFPET_BAD_REQUEST = 1,
      bad_request = OFPET_BAD_REQUEST,
      OFPET_BAD_ACTION = 2,
      bad_action = OFPET_BAD_ACTION,
      OFPET_FLOW_MOD_FAILED = 3,
      flow_mod_failed = OFPET_FLOW_MOD_FAILED,
      OFPET_PORT_MOD_FAILED = 4,
      port_mod_failed = OFPET_PORT_MOD_FAILED,
      OFPET_QUEUE_OP_FAILED = 5,
      queue_op_failed = OFPET_QUEUE_OP_FAILED,
    };

  } // namespace ofp_error_type_detail

  using ofp_error_type_detail::ofp_error_type;
  using ofp_error_type_detail::ofp_error_type::OFPET_HELLO_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_BAD_REQUEST;
  using ofp_error_type_detail::ofp_error_type::OFPET_BAD_ACTION;
  using ofp_error_type_detail::ofp_error_type::OFPET_FLOW_MOD_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_PORT_MOD_FAILED;
  using ofp_error_type_detail::ofp_error_type::OFPET_QUEUE_OP_FAILED;

  using error_type = ofp_error_type_detail::ofp_error_type;

  namespace ofp_hello_failed_code_detail {

    enum ofp_hello_failed_code : std::uint16_t
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

    enum ofp_bad_request_code : std::uint16_t
    {
      OFPBRC_BAD_VERSION = 0,
      bad_version = OFPBRC_BAD_VERSION,
      OFPBRC_BAD_TYPE = 1,
      bad_type = OFPBRC_BAD_TYPE,
      OFPBRC_BAD_STAT = 2,
      bad_stat = OFPBRC_BAD_STAT,
      OFPBRC_BAD_VENDOR = 3,
      bad_vendor = OFPBRC_BAD_VENDOR,
      OFPBRC_BAD_SUBTYPE = 4,
      bad_subtype = OFPBRC_BAD_SUBTYPE,
      OFPBRC_EPERM = 5,
      eperm = OFPBRC_EPERM,
      OFPBRC_BAD_LEN = 6,
      bad_len = OFPBRC_BAD_LEN,
      OFPBRC_BUFFER_EMPTY = 7,
      buffer_empty = OFPBRC_BUFFER_EMPTY,
      OFPBRC_BUFFER_UNKNOWN = 8,
      buffer_unknown = OFPBRC_BUFFER_UNKNOWN,
    };

  } // namespace ofp_bad_request_code_detail

  using ofp_bad_request_code_detail::ofp_bad_request_code;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_VERSION;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_TYPE;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_STAT;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_VENDOR;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_SUBTYPE;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_EPERM;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BAD_LEN;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BUFFER_EMPTY;
  using ofp_bad_request_code_detail::ofp_bad_request_code::OFPBRC_BUFFER_UNKNOWN;

  using bad_request_code = ofp_bad_request_code_detail::ofp_bad_request_code;

  namespace ofp_bad_action_code_detail {

    enum ofp_bad_action_code : std::uint16_t
    {
      OFPBAC_BAD_TYPE = 0,
      bad_type = OFPBAC_BAD_TYPE,
      OFPBAC_BAD_LEN = 1,
      bad_len = OFPBAC_BAD_LEN,
      OFPBAC_BAD_VENDOR = 2,
      bad_vendor = OFPBAC_BAD_VENDOR,
      OFPBAC_BAD_VENDOR_TYPE = 3,
      bad_vendor_type = OFPBAC_BAD_VENDOR_TYPE,
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
    };

  } // namespace ofp_bad_action_code_detail

  using ofp_bad_action_code_detail::ofp_bad_action_code;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_TYPE;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_LEN;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_VENDOR;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_VENDOR_TYPE;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_OUT_PORT;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_ARGUMENT;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_EPERM;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_TOO_MANY;
  using ofp_bad_action_code_detail::ofp_bad_action_code::OFPBAC_BAD_QUEUE;

  using bad_action_code = ofp_bad_action_code_detail::ofp_bad_action_code;

  namespace ofp_flow_mod_failed_code_detail {

    enum ofp_flow_mod_failed_code : std::uint16_t
    {
      OFPFMFC_ALL_TABLES_FULL = 0,
      all_tables_full = OFPFMFC_ALL_TABLES_FULL,
      OFPFMFC_OVERLAP = 1,
      overlap = OFPFMFC_OVERLAP,
      OFPFMFC_EPERM = 2,
      eperm = OFPFMFC_EPERM,
      OFPFMFC_BAD_EMERG_TIMEOUT = 3,
      bad_emerg_timeout = OFPFMFC_BAD_EMERG_TIMEOUT,
      OFPFMFC_BAD_COMMAND = 4,
      bad_command = OFPFMFC_BAD_COMMAND,
      OFPFMFC_UNSUPPORTED = 5,
      unsupported = OFPFMFC_UNSUPPORTED,
    };

  } // namespace ofp_flow_mod_failed_code_detail

  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_ALL_TABLES_FULL;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_OVERLAP;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_EPERM;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_BAD_EMERG_TIMEOUT;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_BAD_COMMAND;
  using ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code::OFPFMFC_UNSUPPORTED;

  using flow_mod_failed_code = ofp_flow_mod_failed_code_detail::ofp_flow_mod_failed_code;

  namespace ofp_port_mod_failed_code_detail {

    enum ofp_port_mod_failed_code : std::uint16_t
    {
      OFPPMFC_BAD_PORT = 0,
      bad_port = OFPPMFC_BAD_PORT,
      OFPPMFC_BAD_HW_ADDR = 1,
      bad_hw_addr = OFPPMFC_BAD_HW_ADDR,
    };

  } // namespace ofp_port_mod_failed_code_detail

  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code;
  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code::OFPPMFC_BAD_PORT;
  using ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code::OFPPMFC_BAD_HW_ADDR;

  using port_mod_failed_code = ofp_port_mod_failed_code_detail::ofp_port_mod_failed_code;

  namespace ofp_queue_op_failed_code_detail {

    enum ofp_queue_op_failed_code : std::uint16_t
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

  namespace ofp_stats_types_detail {

    enum ofp_stats_types : std::uint16_t
    {
      OFPST_DESC = 0,
      OFPMP_DESC = OFPST_DESC,
      desc = OFPST_DESC,
      OFPST_FLOW = 1,
      OFPMP_FLOW = OFPST_FLOW,
      flow = OFPST_FLOW,
      OFPST_AGGREGATE = 2,
      OFPMP_AGGREGATE = OFPST_AGGREGATE,
      aggregate = OFPST_AGGREGATE,
      OFPST_TABLE = 3,
      OFPMP_TABLE = OFPST_TABLE,
      table = OFPST_TABLE,
      OFPST_PORT = 4,
      OFPMP_PORT_STATS = OFPST_PORT,
      port = OFPST_PORT,
      port_stats = port,
      OFPST_QUEUE = 5,
      OFPMP_QUEUE = OFPST_QUEUE,
      queue = OFPST_QUEUE,
      OFPST_VENDOR = 65535,
      OFPMP_EXPERIMENTER = OFPST_VENDOR,
      vendor = OFPST_VENDOR,
      experimenter = vendor,
    };

  } // namespace ofp_stats_types_detail

  using ofp_stats_types_detail::ofp_stats_types;
  using ofp_stats_types_detail::ofp_stats_types::OFPST_DESC;
  using ofp_stats_types_detail::ofp_stats_types::OFPMP_DESC;
  using ofp_stats_types_detail::ofp_stats_types::OFPST_FLOW;
  using ofp_stats_types_detail::ofp_stats_types::OFPMP_FLOW;
  using ofp_stats_types_detail::ofp_stats_types::OFPST_AGGREGATE;
  using ofp_stats_types_detail::ofp_stats_types::OFPMP_AGGREGATE;
  using ofp_stats_types_detail::ofp_stats_types::OFPST_TABLE;
  using ofp_stats_types_detail::ofp_stats_types::OFPMP_TABLE;
  using ofp_stats_types_detail::ofp_stats_types::OFPST_PORT;
  using ofp_stats_types_detail::ofp_stats_types::OFPMP_PORT_STATS;
  using ofp_stats_types_detail::ofp_stats_types::OFPST_QUEUE;
  using ofp_stats_types_detail::ofp_stats_types::OFPMP_QUEUE;
  using ofp_stats_types_detail::ofp_stats_types::OFPST_VENDOR;
  using ofp_stats_types_detail::ofp_stats_types::OFPMP_EXPERIMENTER;

  using stats_type = ofp_stats_types_detail::ofp_stats_types;

  namespace ofp_stats_reply_flags_detail {

    enum ofp_stats_reply_flags : std::uint16_t
    {
      OFPSF_REPLY_MORE = 1,
      OFPMPF_REPLY_MORE = OFPSF_REPLY_MORE,
      reply_more = OFPSF_REPLY_MORE,
    };

  } // namespace ofp_stats_reply_flags_detail

  using ofp_stats_reply_flags_detail::ofp_stats_reply_flags;
  using ofp_stats_reply_flags_detail::ofp_stats_reply_flags::OFPSF_REPLY_MORE;
  using ofp_stats_reply_flags_detail::ofp_stats_reply_flags::OFPMPF_REPLY_MORE;

  using stats_reply_flags = ofp_stats_reply_flags_detail::ofp_stats_reply_flags;

  namespace ofp_queue_properties_detail {

    enum ofp_queue_properties : std::uint16_t
    {
      OFPQT_NONE = 0,
      none = OFPQT_NONE,
      OFPQT_MIN_RATE = 1,
      min_rate = OFPQT_MIN_RATE,
    };

  } // namespace ofp_queue_properties_detail

  using ofp_queue_properties_detail::ofp_queue_properties;
  using ofp_queue_properties_detail::ofp_queue_properties::OFPQT_NONE;
  using ofp_queue_properties_detail::ofp_queue_properties::OFPQT_MIN_RATE;

  using queue_prop_type = ofp_queue_properties_detail::ofp_queue_properties;

} // namespace protocol
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_OPENFLOW_HPP
