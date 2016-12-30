#ifndef CANARD_NET_OFP_V10_OPENFLOW_HPP
#define CANARD_NET_OFP_V10_OPENFLOW_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  namespace v10_detail {

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

  } // namespace v10_detail

  namespace protocol {

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

    enum ofp_port
    {
      OFPP_MAX = 65280,
      OFPP_IN_PORT = 65528,
      OFPP_TABLE = 65529,
      OFPP_NORMAL = 65530,
      OFPP_FLOOD = 65531,
      OFPP_ALL = 65532,
      OFPP_CONTROLLER = 65533,
      OFPP_LOCAL = 65534,
      OFPP_NONE = 65535,
      OFPP_ANY = OFPP_NONE,
    };

    enum ofp_type
    {
      OFPT_HELLO = 0,
      OFPT_ERROR = 1,
      OFPT_ECHO_REQUEST = 2,
      OFPT_ECHO_REPLY = 3,
      OFPT_VENDOR = 4,
      OFPT_EXPERIMENTER = OFPT_VENDOR,
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
      OFPT_PORT_MOD = 15,
      OFPT_STATS_REQUEST = 16,
      OFPT_STATS_REPLY = 17,
      OFPT_BARRIER_REQUEST = 18,
      OFPT_BARRIER_REPLY = 19,
      OFPT_QUEUE_GET_CONFIG_REQUEST = 20,
      OFPT_QUEUE_GET_CONFIG_REPLY = 21,
    };

    enum ofp_config_flags
    {
      OFPC_FRAG_NORMAL = 0,
      OFPC_FRAG_DROP = 1,
      OFPC_FRAG_REASM = 2,
      OFPC_FRAG_MASK = 3,
    };

    enum ofp_capabilities
    {
      OFPC_FLOW_STATS = 1,
      OFPC_TABLE_STATS = 2,
      OFPC_PORT_STATS = 4,
      OFPC_STP = 8,
      OFPC_RESERVED = 16,
      OFPC_IP_REASM = 32,
      OFPC_QUEUE_STATS = 64,
      OFPC_ARP_MATCH_IP = 128,
    };

    enum ofp_port_config
    {
      OFPPC_PORT_DOWN = 1,
      OFPPC_NO_STP = 2,
      OFPPC_NO_RECV = 4,
      OFPPC_NO_RECV_STP = 8,
      OFPPC_NO_FLOOD = 16,
      OFPPC_NO_FWD = 32,
      OFPPC_NO_PACKET_IN = 64,
    };

    enum ofp_port_state
    {
      OFPPS_LINK_DOWN = 1,
      OFPPS_STP_LISTEN = 0,
      OFPPS_STP_LEARN = 256,
      OFPPS_STP_FORWARD = 512,
      OFPPS_STP_BLOCK = 768,
      OFPPS_STP_MASK = 768,
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
      OFPPF_COPPER = 128,
      OFPPF_FIBER = 256,
      OFPPF_AUTONEG = 512,
      OFPPF_PAUSE = 1024,
      OFPPF_PAUSE_ASYM = 2048,
    };

    enum ofp_port_reason
    {
      OFPPR_ADD = 0,
      OFPPR_DELETE = 1,
      OFPPR_MODIFY = 2,
    };

    enum ofp_packet_in_reason
    {
      OFPR_NO_MATCH = 0,
      OFPR_ACTION = 1,
    };

    enum ofp_action_type
    {
      OFPAT_OUTPUT = 0,
      OFPAT_SET_VLAN_VID = 1,
      OFPAT_SET_VLAN_PCP = 2,
      OFPAT_STRIP_VLAN = 3,
      OFPAT_POP_VLAN = OFPAT_STRIP_VLAN,
      OFPAT_SET_DL_SRC = 4,
      OFPAT_SET_DL_DST = 5,
      OFPAT_SET_NW_SRC = 6,
      OFPAT_SET_NW_DST = 7,
      OFPAT_SET_NW_TOS = 8,
      OFPAT_SET_TP_SRC = 9,
      OFPAT_SET_TP_DST = 10,
      OFPAT_ENQUEUE = 11,
      OFPAT_SET_QUEUE = OFPAT_ENQUEUE,
      OFPAT_VENDOR = 65535,
      OFPAT_EXPERIMENTER = OFPAT_VENDOR,
    };

    enum ofp_flow_mod_command
    {
      OFPFC_ADD = 0,
      OFPFC_MODIFY = 1,
      OFPFC_MODIFY_STRICT = 2,
      OFPFC_DELETE = 3,
      OFPFC_DELETE_STRICT = 4,
    };

    enum ofp_flow_wildcards
    {
      OFPFW_IN_PORT = 1,
      OFPXMT_OFB_IN_PORT = OFPFW_IN_PORT,
      OFPFW_DL_VLAN = 2,
      OFPXMT_OFB_VLAN_VID = OFPFW_DL_VLAN,
      OFPFW_DL_SRC = 4,
      OFPXMT_OFB_ETH_SRC = OFPFW_DL_SRC,
      OFPFW_DL_DST = 8,
      OFPXMT_OFB_ETH_DST = OFPFW_DL_DST,
      OFPFW_DL_TYPE = 16,
      OFPXMT_OFB_ETH_TYPE = OFPFW_DL_TYPE,
      OFPFW_NW_PROTO = 32,
      OFPXMT_OFB_IP_PROTO = OFPFW_NW_PROTO,
      OFPXMT_OFB_ARP_OP = OFPFW_NW_PROTO,
      OFPFW_TP_SRC = 64,
      OFPXMT_OFB_TCP_SRC = OFPFW_TP_SRC,
      OFPXMT_OFB_UDP_SRC = OFPFW_TP_SRC,
      OFPXMT_OFB_ICMPV4_TYPE = OFPFW_TP_SRC,
      OFPFW_TP_DST = 128,
      OFPXMT_OFB_TCP_DST = OFPFW_TP_DST,
      OFPXMT_OFB_UDP_DST = OFPFW_TP_DST,
      OFPXMT_OFB_ICMPV4_CODE = OFPFW_TP_DST,
      OFPFW_NW_SRC_SHIFT = 8,
      OFPFW_NW_SRC_BITS = 6,
      OFPFW_NW_SRC_MASK = 16128,
      OFPFW_NW_SRC_ALL = 8192,
      OFPXMT_OFB_IPV4_SRC = OFPFW_NW_SRC_ALL,
      OFPXMT_OFB_ARP_SPA = OFPFW_NW_SRC_ALL,
      OFPFW_NW_DST_SHIFT = 14,
      OFPFW_NW_DST_BITS = 6,
      OFPFW_NW_DST_MASK = 1032192,
      OFPFW_NW_DST_ALL = 524288,
      OFPXMT_OFB_IPV4_DST = OFPFW_NW_DST_ALL,
      OFPXMT_OFB_ARP_TPA = OFPFW_NW_DST_ALL,
      OFPFW_DL_VLAN_PCP = 1048576,
      OFPXMT_OFB_VLAN_PCP = OFPFW_DL_VLAN_PCP,
      OFPFW_NW_TOS = 2097152,
      OFPXMT_OFB_IP_DSCP = OFPFW_NW_TOS,
      OFPFW_ALL = 4194303,
    };

    enum ofp_flow_mod_flags
    {
      OFPFF_SEND_FLOW_REM = 1,
      OFPFF_CHECK_OVERLAP = 2,
      OFPFF_EMERG = 4,
    };

    enum ofp_flow_removed_reason
    {
      OFPRR_IDLE_TIMEOUT = 0,
      OFPRR_HARD_TIMEOUT = 1,
      OFPRR_DELETE = 2,
    };

    enum ofp_error_type
    {
      OFPET_HELLO_FAILED = 0,
      OFPET_BAD_REQUEST = 1,
      OFPET_BAD_ACTION = 2,
      OFPET_FLOW_MOD_FAILED = 3,
      OFPET_PORT_MOD_FAILED = 4,
      OFPET_QUEUE_OP_FAILED = 5,
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
      OFPBRC_BAD_STAT = 2,
      OFPBRC_BAD_VENDOR = 3,
      OFPBRC_BAD_SUBTYPE = 4,
      OFPBRC_EPERM = 5,
      OFPBRC_BAD_LEN = 6,
      OFPBRC_BUFFER_EMPTY = 7,
      OFPBRC_BUFFER_UNKNOWN = 8,
    };

    enum ofp_bad_action_code
    {
      OFPBAC_BAD_TYPE = 0,
      OFPBAC_BAD_LEN = 1,
      OFPBAC_BAD_VENDOR = 2,
      OFPBAC_BAD_VENDOR_TYPE = 3,
      OFPBAC_BAD_OUT_PORT = 4,
      OFPBAC_BAD_ARGUMENT = 5,
      OFPBAC_EPERM = 6,
      OFPBAC_TOO_MANY = 7,
      OFPBAC_BAD_QUEUE = 8,
    };

    enum ofp_flow_mod_failed_code
    {
      OFPFMFC_ALL_TABLES_FULL = 0,
      OFPFMFC_OVERLAP = 1,
      OFPFMFC_EPERM = 2,
      OFPFMFC_BAD_EMERG_TIMEOUT = 3,
      OFPFMFC_BAD_COMMAND = 4,
      OFPFMFC_UNSUPPORTED = 5,
    };

    enum ofp_port_mod_failed_code
    {
      OFPPMFC_BAD_PORT = 0,
      OFPPMFC_BAD_HW_ADDR = 1,
    };

    enum ofp_queue_op_failed_code
    {
      OFPQOFC_BAD_PORT = 0,
      OFPQOFC_BAD_QUEUE = 1,
      OFPQOFC_EPERM = 2,
    };

    enum ofp_stats_types
    {
      OFPST_DESC = 0,
      OFPMP_DESC = OFPST_DESC,
      OFPST_FLOW = 1,
      OFPMP_FLOW = OFPST_FLOW,
      OFPST_AGGREGATE = 2,
      OFPMP_AGGREGATE = OFPST_AGGREGATE,
      OFPST_TABLE = 3,
      OFPMP_TABLE = OFPST_TABLE,
      OFPST_PORT = 4,
      OFPMP_PORT_STATS = OFPST_PORT,
      OFPST_QUEUE = 5,
      OFPMP_QUEUE = OFPST_QUEUE,
      OFPST_VENDOR = 65535,
      OFPMP_EXPERIMENTER = OFPST_VENDOR,
    };

    enum ofp_stats_reply_flags
    {
      OFPSF_REPLY_MORE = 1,
      OFPMPF_REPLY_MORE = OFPSF_REPLY_MORE,
    };

    enum ofp_queue_properties
    {
      OFPQT_NONE = 0,
      OFPQT_MIN_RATE = 1,
    };

    namespace protocol_detail {

      struct ofp_port
      {
        using original_type = protocol::ofp_port;

        enum type : std::uint16_t
        {
          max = protocol::OFPP_MAX,
          in_port = protocol::OFPP_IN_PORT,
          table = protocol::OFPP_TABLE,
          normal = protocol::OFPP_NORMAL,
          flood = protocol::OFPP_FLOOD,
          all = protocol::OFPP_ALL,
          controller = protocol::OFPP_CONTROLLER,
          local = protocol::OFPP_LOCAL,
          none = protocol::OFPP_NONE,
          any = none,
        };
      };

      struct ofp_type
      {
        using original_type = protocol::ofp_type;

        enum type : std::uint8_t
        {
          hello = protocol::OFPT_HELLO,
          error = protocol::OFPT_ERROR,
          echo_request = protocol::OFPT_ECHO_REQUEST,
          echo_reply = protocol::OFPT_ECHO_REPLY,
          vendor = protocol::OFPT_VENDOR,
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
          port_mod = protocol::OFPT_PORT_MOD,
          stats_request = protocol::OFPT_STATS_REQUEST,
          stats_reply = protocol::OFPT_STATS_REPLY,
          barrier_request = protocol::OFPT_BARRIER_REQUEST,
          barrier_reply = protocol::OFPT_BARRIER_REPLY,
          queue_get_config_request = protocol::OFPT_QUEUE_GET_CONFIG_REQUEST,
          queue_get_config_reply = protocol::OFPT_QUEUE_GET_CONFIG_REPLY,
        };
      };

      struct ofp_config_flags
      {
        using original_type = protocol::ofp_config_flags;

        enum type : std::uint16_t
        {
          normal = protocol::OFPC_FRAG_NORMAL,
          drop = protocol::OFPC_FRAG_DROP,
          reasm = protocol::OFPC_FRAG_REASM,
          mask = protocol::OFPC_FRAG_MASK,
        };
      };

      struct ofp_capabilities
      {
        using original_type = protocol::ofp_capabilities;

        enum type : std::uint32_t
        {
          flow_stats = protocol::OFPC_FLOW_STATS,
          table_stats = protocol::OFPC_TABLE_STATS,
          port_stats = protocol::OFPC_PORT_STATS,
          stp = protocol::OFPC_STP,
          reserved = protocol::OFPC_RESERVED,
          ip_reasm = protocol::OFPC_IP_REASM,
          queue_stats = protocol::OFPC_QUEUE_STATS,
          arp_match_ip = protocol::OFPC_ARP_MATCH_IP,
        };
      };

      struct ofp_port_config
      {
        using original_type = protocol::ofp_port_config;

        enum type : std::uint32_t
        {
          port_down = protocol::OFPPC_PORT_DOWN,
          no_stp = protocol::OFPPC_NO_STP,
          no_recv = protocol::OFPPC_NO_RECV,
          no_recv_stp = protocol::OFPPC_NO_RECV_STP,
          no_flood = protocol::OFPPC_NO_FLOOD,
          no_fwd = protocol::OFPPC_NO_FWD,
          no_packet_in = protocol::OFPPC_NO_PACKET_IN,
        };
      };

      struct ofp_port_state
      {
        using original_type = protocol::ofp_port_state;

        enum type : std::uint32_t
        {
          link_down = protocol::OFPPS_LINK_DOWN,
          stp_listen = protocol::OFPPS_STP_LISTEN,
          stp_learn = protocol::OFPPS_STP_LEARN,
          stp_forward = protocol::OFPPS_STP_FORWARD,
          stp_block = protocol::OFPPS_STP_BLOCK,
          stp_mask = protocol::OFPPS_STP_MASK,
        };
      };

      struct ofp_port_features
      {
        using original_type = protocol::ofp_port_features;

        enum type : std::uint32_t
        {
          mode_10mb_hd = protocol::OFPPF_10MB_HD,
          mode_10mb_fd = protocol::OFPPF_10MB_FD,
          mode_100mb_hd = protocol::OFPPF_100MB_HD,
          mode_100mb_fd = protocol::OFPPF_100MB_FD,
          mode_1gb_hd = protocol::OFPPF_1GB_HD,
          mode_1gb_fd = protocol::OFPPF_1GB_FD,
          mode_10gb_fd = protocol::OFPPF_10GB_FD,
          copper = protocol::OFPPF_COPPER,
          fiber = protocol::OFPPF_FIBER,
          autoneg = protocol::OFPPF_AUTONEG,
          pause = protocol::OFPPF_PAUSE,
          pause_asym = protocol::OFPPF_PAUSE_ASYM,
        };
      };

      struct ofp_port_reason
      {
        using original_type = protocol::ofp_port_reason;

        enum type : std::uint8_t
        {
          by_add = protocol::OFPPR_ADD,
          by_delete = protocol::OFPPR_DELETE,
          by_modify = protocol::OFPPR_MODIFY,
        };
      };

      struct ofp_packet_in_reason
      {
        using original_type = protocol::ofp_packet_in_reason;

        enum type : std::uint8_t
        {
          no_match = protocol::OFPR_NO_MATCH,
          action = protocol::OFPR_ACTION,
        };
      };

      struct ofp_action_type
      {
        using original_type = protocol::ofp_action_type;

        enum type : std::uint16_t
        {
          output = protocol::OFPAT_OUTPUT,
          set_vlan_vid = protocol::OFPAT_SET_VLAN_VID,
          set_vlan_pcp = protocol::OFPAT_SET_VLAN_PCP,
          strip_vlan = protocol::OFPAT_STRIP_VLAN,
          pop_vlan = strip_vlan,
          set_dl_src = protocol::OFPAT_SET_DL_SRC,
          set_dl_dst = protocol::OFPAT_SET_DL_DST,
          set_nw_src = protocol::OFPAT_SET_NW_SRC,
          set_nw_dst = protocol::OFPAT_SET_NW_DST,
          set_nw_tos = protocol::OFPAT_SET_NW_TOS,
          set_tp_src = protocol::OFPAT_SET_TP_SRC,
          set_tp_dst = protocol::OFPAT_SET_TP_DST,
          enqueue = protocol::OFPAT_ENQUEUE,
          set_queue = enqueue,
          vendor = protocol::OFPAT_VENDOR,
          experimenter = vendor,
        };
      };

      struct ofp_flow_mod_command
      {
        using original_type = protocol::ofp_flow_mod_command;

        enum type : std::uint16_t
        {
          add_cmd = protocol::OFPFC_ADD,
          modify_cmd = protocol::OFPFC_MODIFY,
          modify_strict_cmd = protocol::OFPFC_MODIFY_STRICT,
          delete_cmd = protocol::OFPFC_DELETE,
          delete_strict_cmd = protocol::OFPFC_DELETE_STRICT,
        };
      };

      struct ofp_flow_wildcards
      {
        using original_type = protocol::ofp_flow_wildcards;

        enum type : std::uint32_t
        {
          in_port = protocol::OFPFW_IN_PORT,
          dl_vlan = protocol::OFPFW_DL_VLAN,
          dl_src = protocol::OFPFW_DL_SRC,
          dl_dst = protocol::OFPFW_DL_DST,
          dl_type = protocol::OFPFW_DL_TYPE,
          nw_proto = protocol::OFPFW_NW_PROTO,
          tp_src = protocol::OFPFW_TP_SRC,
          tp_dst = protocol::OFPFW_TP_DST,
          nw_src_shift = protocol::OFPFW_NW_SRC_SHIFT,
          nw_src_bits = protocol::OFPFW_NW_SRC_BITS,
          nw_src_mask = protocol::OFPFW_NW_SRC_MASK,
          nw_src_all = protocol::OFPFW_NW_SRC_ALL,
          nw_dst_shift = protocol::OFPFW_NW_DST_SHIFT,
          nw_dst_bits = protocol::OFPFW_NW_DST_BITS,
          nw_dst_mask = protocol::OFPFW_NW_DST_MASK,
          nw_dst_all = protocol::OFPFW_NW_DST_ALL,
          dl_vlan_pcp = protocol::OFPFW_DL_VLAN_PCP,
          nw_tos = protocol::OFPFW_NW_TOS,
          all = protocol::OFPFW_ALL,
        };
      };

      struct ofp_flow_mod_flags
      {
        using original_type = protocol::ofp_flow_mod_flags;

        enum type : std::uint16_t
        {
          send_flow_rem = protocol::OFPFF_SEND_FLOW_REM,
          check_overlap = protocol::OFPFF_CHECK_OVERLAP,
          emerg = protocol::OFPFF_EMERG,
        };
      };

      struct ofp_flow_removed_reason
      {
        using original_type = protocol::ofp_flow_removed_reason;

        enum type : std::uint8_t
        {
          by_idle_timeout = protocol::OFPRR_IDLE_TIMEOUT,
          by_hard_timeout = protocol::OFPRR_HARD_TIMEOUT,
          by_delete = protocol::OFPRR_DELETE,
        };
      };

      struct ofp_error_type
      {
        using original_type = protocol::ofp_error_type;

        enum type : std::uint16_t
        {
          hello_failed = protocol::OFPET_HELLO_FAILED,
          bad_request = protocol::OFPET_BAD_REQUEST,
          bad_action = protocol::OFPET_BAD_ACTION,
          flow_mod_failed = protocol::OFPET_FLOW_MOD_FAILED,
          port_mod_failed = protocol::OFPET_PORT_MOD_FAILED,
          queue_op_failed = protocol::OFPET_QUEUE_OP_FAILED,
        };
      };

      struct ofp_hello_failed_code
      {
        using original_type = protocol::ofp_hello_failed_code;

        enum type : std::uint16_t
        {
          incompatible = protocol::OFPHFC_INCOMPATIBLE,
          eperm = protocol::OFPHFC_EPERM,
        };
      };

      struct ofp_bad_request_code
      {
        using original_type = protocol::ofp_bad_request_code;

        enum type : std::uint16_t
        {
          bad_version = protocol::OFPBRC_BAD_VERSION,
          bad_type = protocol::OFPBRC_BAD_TYPE,
          bad_stat = protocol::OFPBRC_BAD_STAT,
          bad_vendor = protocol::OFPBRC_BAD_VENDOR,
          bad_subtype = protocol::OFPBRC_BAD_SUBTYPE,
          eperm = protocol::OFPBRC_EPERM,
          bad_len = protocol::OFPBRC_BAD_LEN,
          buffer_empty = protocol::OFPBRC_BUFFER_EMPTY,
          buffer_unknown = protocol::OFPBRC_BUFFER_UNKNOWN,
        };
      };

      struct ofp_bad_action_code
      {
        using original_type = protocol::ofp_bad_action_code;

        enum type : std::uint16_t
        {
          bad_type = protocol::OFPBAC_BAD_TYPE,
          bad_len = protocol::OFPBAC_BAD_LEN,
          bad_vendor = protocol::OFPBAC_BAD_VENDOR,
          bad_vendor_type = protocol::OFPBAC_BAD_VENDOR_TYPE,
          bad_out_port = protocol::OFPBAC_BAD_OUT_PORT,
          bad_argument = protocol::OFPBAC_BAD_ARGUMENT,
          eperm = protocol::OFPBAC_EPERM,
          too_many = protocol::OFPBAC_TOO_MANY,
          bad_queue = protocol::OFPBAC_BAD_QUEUE,
        };
      };

      struct ofp_flow_mod_failed_code
      {
        using original_type = protocol::ofp_flow_mod_failed_code;

        enum type : std::uint16_t
        {
          all_tables_full = protocol::OFPFMFC_ALL_TABLES_FULL,
          overlap = protocol::OFPFMFC_OVERLAP,
          eperm = protocol::OFPFMFC_EPERM,
          bad_emerg_timeout = protocol::OFPFMFC_BAD_EMERG_TIMEOUT,
          bad_command = protocol::OFPFMFC_BAD_COMMAND,
          unsupported = protocol::OFPFMFC_UNSUPPORTED,
        };
      };

      struct ofp_port_mod_failed_code
      {
        using original_type = protocol::ofp_port_mod_failed_code;

        enum type : std::uint16_t
        {
          bad_port = protocol::OFPPMFC_BAD_PORT,
          bad_hw_addr = protocol::OFPPMFC_BAD_HW_ADDR,
        };
      };

      struct ofp_queue_op_failed_code
      {
        using original_type = protocol::ofp_queue_op_failed_code;

        enum type : std::uint16_t
        {
          bad_port = protocol::OFPQOFC_BAD_PORT,
          bad_queue = protocol::OFPQOFC_BAD_QUEUE,
          eperm = protocol::OFPQOFC_EPERM,
        };
      };

      struct ofp_stats_types
      {
        using original_type = protocol::ofp_stats_types;

        enum type : std::uint16_t
        {
          desc = protocol::OFPST_DESC,
          flow = protocol::OFPST_FLOW,
          aggregate = protocol::OFPST_AGGREGATE,
          table = protocol::OFPST_TABLE,
          port = protocol::OFPST_PORT,
          port_stats = port,
          queue = protocol::OFPST_QUEUE,
          vendor = protocol::OFPST_VENDOR,
          experimenter = vendor,
        };
      };

      struct ofp_stats_reply_flags
      {
        using original_type = protocol::ofp_stats_reply_flags;

        enum type : std::uint16_t
        {
          reply_more = protocol::OFPSF_REPLY_MORE,
        };
      };

      struct ofp_queue_properties
      {
        using original_type = protocol::ofp_queue_properties;

        enum type : std::uint16_t
        {
          none = protocol::OFPQT_NONE,
          min_rate = protocol::OFPQT_MIN_RATE,
        };
      };

    } // namespace protocol_detail

    template <class T>
    class scoped_enum : public T
    {
      typename T::type value_;
    public:
      using type = typename T::type;
      constexpr scoped_enum() noexcept : value_{} {}
      constexpr scoped_enum(type v) noexcept : value_(v) {}
      explicit constexpr scoped_enum(
        typename std::underlying_type<type>::type v) noexcept
        : value_(type(v)) {}
      constexpr operator type() const noexcept { return value_; }
    };

    using port_no = scoped_enum<protocol_detail::ofp_port>;
    using msg_type = scoped_enum<protocol_detail::ofp_type>;
    using config_flags = scoped_enum<protocol_detail::ofp_config_flags>;
    using capabilities = scoped_enum<protocol_detail::ofp_capabilities>;
    using port_config = scoped_enum<protocol_detail::ofp_port_config>;
    using port_state = scoped_enum<protocol_detail::ofp_port_state>;
    using port_features = scoped_enum<protocol_detail::ofp_port_features>;
    using port_reason = scoped_enum<protocol_detail::ofp_port_reason>;
    using packet_in_reason = scoped_enum<protocol_detail::ofp_packet_in_reason>;
    using action_type = scoped_enum<protocol_detail::ofp_action_type>;
    using flow_mod_command = scoped_enum<protocol_detail::ofp_flow_mod_command>;
    using flow_wildcards = scoped_enum<protocol_detail::ofp_flow_wildcards>;
    using flow_mod_flags = scoped_enum<protocol_detail::ofp_flow_mod_flags>;
    using flow_removed_reason = scoped_enum<protocol_detail::ofp_flow_removed_reason>;
    using error_type = scoped_enum<protocol_detail::ofp_error_type>;
    using hello_failed_code = scoped_enum<protocol_detail::ofp_hello_failed_code>;
    using bad_request_code = scoped_enum<protocol_detail::ofp_bad_request_code>;
    using bad_action_code = scoped_enum<protocol_detail::ofp_bad_action_code>;
    using flow_mod_failed_code = scoped_enum<protocol_detail::ofp_flow_mod_failed_code>;
    using port_mod_failed_code = scoped_enum<protocol_detail::ofp_port_mod_failed_code>;
    using queue_op_failed_code = scoped_enum<protocol_detail::ofp_queue_op_failed_code>;
    using stats_type = scoped_enum<protocol_detail::ofp_stats_types>;
    using stats_reply_flags = scoped_enum<protocol_detail::ofp_stats_reply_flags>;
    using queue_prop_type = scoped_enum<protocol_detail::ofp_queue_properties>;

  } // namespace protocol

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_OPENFLOW_HPP
