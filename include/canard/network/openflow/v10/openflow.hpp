#ifndef CANARD_NET_OFP_V10_OPENFLOW_HPP
#define CANARD_NET_OFP_V10_OPENFLOW_HPP

#include <cstddef>
#include <cstdint>

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

    class protocol
    {
    public:
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
    };

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_OPENFLOW_HPP
