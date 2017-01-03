#ifndef CANARD_NET_OFP_V13_IO_OPENFLOW_HPP
#define CANARD_NET_OFP_V13_IO_OPENFLOW_HPP

#include <cstdint>
#include <ostream>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace protocol {

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port_no const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPP_MAX: return os << "OFPP_MAX";
    case OFPP_IN_PORT: return os << "OFPP_IN_PORT";
    case OFPP_TABLE: return os << "OFPP_TABLE";
    case OFPP_NORMAL: return os << "OFPP_NORMAL";
    case OFPP_FLOOD: return os << "OFPP_FLOOD";
    case OFPP_ALL: return os << "OFPP_ALL";
    case OFPP_CONTROLLER: return os << "OFPP_CONTROLLER";
    case OFPP_LOCAL: return os << "OFPP_LOCAL";
    case OFPP_ANY: return os << "OFPP_ANY";
    default: return os << "switch_port(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPT_HELLO: return os << "OFPT_HELLO";
    case OFPT_ERROR: return os << "OFPT_ERROR";
    case OFPT_ECHO_REQUEST: return os << "OFPT_ECHO_REQUEST";
    case OFPT_ECHO_REPLY: return os << "OFPT_ECHO_REPLY";
    case OFPT_EXPERIMENTER: return os << "OFPT_EXPERIMENTER";
    case OFPT_FEATURES_REQUEST: return os << "OFPT_FEATURES_REQUEST";
    case OFPT_FEATURES_REPLY: return os << "OFPT_FEATURES_REPLY";
    case OFPT_GET_CONFIG_REQUEST: return os << "OFPT_GET_CONFIG_REQUEST";
    case OFPT_GET_CONFIG_REPLY: return os << "OFPT_GET_CONFIG_REPLY";
    case OFPT_SET_CONFIG: return os << "OFPT_SET_CONFIG";
    case OFPT_PACKET_IN: return os << "OFPT_PACKET_IN";
    case OFPT_FLOW_REMOVED: return os << "OFPT_FLOW_REMOVED";
    case OFPT_PORT_STATUS: return os << "OFPT_PORT_STATUS";
    case OFPT_PACKET_OUT: return os << "OFPT_PACKET_OUT";
    case OFPT_FLOW_MOD: return os << "OFPT_FLOW_MOD";
    case OFPT_GROUP_MOD: return os << "OFPT_GROUP_MOD";
    case OFPT_PORT_MOD: return os << "OFPT_PORT_MOD";
    case OFPT_TABLE_MOD: return os << "OFPT_TABLE_MOD";
    case OFPT_MULTIPART_REQUEST: return os << "OFPT_MULTIPART_REQUEST";
    case OFPT_MULTIPART_REPLY: return os << "OFPT_MULTIPART_REPLY";
    case OFPT_BARRIER_REQUEST: return os << "OFPT_BARRIER_REQUEST";
    case OFPT_BARRIER_REPLY: return os << "OFPT_BARRIER_REPLY";
    case OFPT_QUEUE_GET_CONFIG_REQUEST: return os << "OFPT_QUEUE_GET_CONFIG_REQUEST";
    case OFPT_QUEUE_GET_CONFIG_REPLY: return os << "OFPT_QUEUE_GET_CONFIG_REPLY";
    case OFPT_ROLE_REQUEST: return os << "OFPT_ROLE_REQUEST";
    case OFPT_ROLE_REPLY: return os << "OFPT_ROLE_REPLY";
    case OFPT_GET_ASYNC_REQUEST: return os << "OFPT_GET_ASYNC_REQUEST";
    case OFPT_GET_ASYNC_REPLY: return os << "OFPT_GET_ASYNC_REPLY";
    case OFPT_SET_ASYNC: return os << "OFPT_SET_ASYNC";
    case OFPT_METER_MOD: return os << "OFPT_METER_MOD";
    default: return os << "unkonw_ofp_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_hello_elem_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPHET_VERSIONBITMAP: return os << "OFPHET_VERSIONBITMAP";
    default: return os << "unkonw_ofp_hello_elem_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_config_flags const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPC_FRAG_NORMAL: return os << "OFPC_FRAG_NORMAL";
    case OFPC_FRAG_DROP: return os << "OFPC_FRAG_DROP";
    case OFPC_FRAG_REASM: return os << "OFPC_FRAG_REASM";
    case OFPC_FRAG_MASK: return os << "OFPC_FRAG_MASK";
    default: return os << "unkonw_ofp_config_flags(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_table_config const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPTC_DEPRECATED_MASK: return os << "OFPTC_DEPRECATED_MASK";
    default: return os << "unkonw_ofp_table_config(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_table const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPTT_MAX: return os << "OFPTT_MAX";
    case OFPTT_ALL: return os << "OFPTT_ALL";
    default: return os << "table_id(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_capabilities const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPC_FLOW_STATS: return os << "OFPC_FLOW_STATS";
    case OFPC_TABLE_STATS: return os << "OFPC_TABLE_STATS";
    case OFPC_PORT_STATS: return os << "OFPC_PORT_STATS";
    case OFPC_GROUP_STATS: return os << "OFPC_GROUP_STATS";
    case OFPC_IP_REASM: return os << "OFPC_IP_REASM";
    case OFPC_QUEUE_STATS: return os << "OFPC_QUEUE_STATS";
    case OFPC_PORT_BLOCKED: return os << "OFPC_PORT_BLOCKED";
    default: return os << "unkonw_ofp_capabilities(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port_config const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPPC_PORT_DOWN: return os << "OFPPC_PORT_DOWN";
    case OFPPC_NO_RECV: return os << "OFPPC_NO_RECV";
    case OFPPC_NO_FWD: return os << "OFPPC_NO_FWD";
    case OFPPC_NO_PACKET_IN: return os << "OFPPC_NO_PACKET_IN";
    default: return os << "unkonw_ofp_port_config(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port_state const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPPS_LINK_DOWN: return os << "OFPPS_LINK_DOWN";
    case OFPPS_BLOCKED: return os << "OFPPS_BLOCKED";
    case OFPPS_LIVE: return os << "OFPPS_LIVE";
    default: return os << "unkonw_ofp_port_state(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port_features const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPPF_10MB_HD: return os << "OFPPF_10MB_HD";
    case OFPPF_10MB_FD: return os << "OFPPF_10MB_FD";
    case OFPPF_100MB_HD: return os << "OFPPF_100MB_HD";
    case OFPPF_100MB_FD: return os << "OFPPF_100MB_FD";
    case OFPPF_1GB_HD: return os << "OFPPF_1GB_HD";
    case OFPPF_1GB_FD: return os << "OFPPF_1GB_FD";
    case OFPPF_10GB_FD: return os << "OFPPF_10GB_FD";
    case OFPPF_40GB_FD: return os << "OFPPF_40GB_FD";
    case OFPPF_100GB_FD: return os << "OFPPF_100GB_FD";
    case OFPPF_1TB_FD: return os << "OFPPF_1TB_FD";
    case OFPPF_OTHER: return os << "OFPPF_OTHER";
    case OFPPF_COPPER: return os << "OFPPF_COPPER";
    case OFPPF_FIBER: return os << "OFPPF_FIBER";
    case OFPPF_AUTONEG: return os << "OFPPF_AUTONEG";
    case OFPPF_PAUSE: return os << "OFPPF_PAUSE";
    case OFPPF_PAUSE_ASYM: return os << "OFPPF_PAUSE_ASYM";
    default: return os << "unkonw_ofp_port_features(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port_reason const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPPR_ADD: return os << "OFPPR_ADD";
    case OFPPR_DELETE: return os << "OFPPR_DELETE";
    case OFPPR_MODIFY: return os << "OFPPR_MODIFY";
    default: return os << "unkonw_ofp_port_reason(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_match_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPMT_STANDARD: return os << "OFPMT_STANDARD";
    case OFPMT_OXM: return os << "OFPMT_OXM";
    default: return os << "unkonw_ofp_match_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_oxm_class const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPXMC_NXM_0: return os << "OFPXMC_NXM_0";
    case OFPXMC_NXM_1: return os << "OFPXMC_NXM_1";
    case OFPXMC_OPENFLOW_BASIC: return os << "OFPXMC_OPENFLOW_BASIC";
    case OFPXMC_EXPERIMENTER: return os << "OFPXMC_EXPERIMENTER";
    default: return os << "unkonw_ofp_oxm_class(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, oxm_ofb_match_fields const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPXMT_OFB_IN_PORT: return os << "OFPXMT_OFB_IN_PORT";
    case OFPXMT_OFB_IN_PHY_PORT: return os << "OFPXMT_OFB_IN_PHY_PORT";
    case OFPXMT_OFB_METADATA: return os << "OFPXMT_OFB_METADATA";
    case OFPXMT_OFB_ETH_DST: return os << "OFPXMT_OFB_ETH_DST";
    case OFPXMT_OFB_ETH_SRC: return os << "OFPXMT_OFB_ETH_SRC";
    case OFPXMT_OFB_ETH_TYPE: return os << "OFPXMT_OFB_ETH_TYPE";
    case OFPXMT_OFB_VLAN_VID: return os << "OFPXMT_OFB_VLAN_VID";
    case OFPXMT_OFB_VLAN_PCP: return os << "OFPXMT_OFB_VLAN_PCP";
    case OFPXMT_OFB_IP_DSCP: return os << "OFPXMT_OFB_IP_DSCP";
    case OFPXMT_OFB_IP_ECN: return os << "OFPXMT_OFB_IP_ECN";
    case OFPXMT_OFB_IP_PROTO: return os << "OFPXMT_OFB_IP_PROTO";
    case OFPXMT_OFB_IPV4_SRC: return os << "OFPXMT_OFB_IPV4_SRC";
    case OFPXMT_OFB_IPV4_DST: return os << "OFPXMT_OFB_IPV4_DST";
    case OFPXMT_OFB_TCP_SRC: return os << "OFPXMT_OFB_TCP_SRC";
    case OFPXMT_OFB_TCP_DST: return os << "OFPXMT_OFB_TCP_DST";
    case OFPXMT_OFB_UDP_SRC: return os << "OFPXMT_OFB_UDP_SRC";
    case OFPXMT_OFB_UDP_DST: return os << "OFPXMT_OFB_UDP_DST";
    case OFPXMT_OFB_SCTP_SRC: return os << "OFPXMT_OFB_SCTP_SRC";
    case OFPXMT_OFB_SCTP_DST: return os << "OFPXMT_OFB_SCTP_DST";
    case OFPXMT_OFB_ICMPV4_TYPE: return os << "OFPXMT_OFB_ICMPV4_TYPE";
    case OFPXMT_OFB_ICMPV4_CODE: return os << "OFPXMT_OFB_ICMPV4_CODE";
    case OFPXMT_OFB_ARP_OP: return os << "OFPXMT_OFB_ARP_OP";
    case OFPXMT_OFB_ARP_SPA: return os << "OFPXMT_OFB_ARP_SPA";
    case OFPXMT_OFB_ARP_TPA: return os << "OFPXMT_OFB_ARP_TPA";
    case OFPXMT_OFB_ARP_SHA: return os << "OFPXMT_OFB_ARP_SHA";
    case OFPXMT_OFB_ARP_THA: return os << "OFPXMT_OFB_ARP_THA";
    case OFPXMT_OFB_IPV6_SRC: return os << "OFPXMT_OFB_IPV6_SRC";
    case OFPXMT_OFB_IPV6_DST: return os << "OFPXMT_OFB_IPV6_DST";
    case OFPXMT_OFB_IPV6_FLABEL: return os << "OFPXMT_OFB_IPV6_FLABEL";
    case OFPXMT_OFB_ICMPV6_TYPE: return os << "OFPXMT_OFB_ICMPV6_TYPE";
    case OFPXMT_OFB_ICMPV6_CODE: return os << "OFPXMT_OFB_ICMPV6_CODE";
    case OFPXMT_OFB_IPV6_ND_TARGET: return os << "OFPXMT_OFB_IPV6_ND_TARGET";
    case OFPXMT_OFB_IPV6_ND_SLL: return os << "OFPXMT_OFB_IPV6_ND_SLL";
    case OFPXMT_OFB_IPV6_ND_TLL: return os << "OFPXMT_OFB_IPV6_ND_TLL";
    case OFPXMT_OFB_MPLS_LABEL: return os << "OFPXMT_OFB_MPLS_LABEL";
    case OFPXMT_OFB_MPLS_TC: return os << "OFPXMT_OFB_MPLS_TC";
    case OFPXMT_OFB_MPLS_BOS: return os << "OFPXMT_OFB_MPLS_BOS";
    case OFPXMT_OFB_PBB_ISID: return os << "OFPXMT_OFB_PBB_ISID";
    case OFPXMT_OFB_TUNNEL_ID: return os << "OFPXMT_OFB_TUNNEL_ID";
    case OFPXMT_OFB_IPV6_EXTHDR: return os << "OFPXMT_OFB_IPV6_EXTHDR";
    default: return os << "unkonw_oxm_ofb_match_fields(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_vlan_id const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPVID_PRESENT: return os << "OFPVID_PRESENT";
    case OFPVID_NONE: return os << "OFPVID_NONE";
    default: return os << "unkonw_ofp_vlan_id(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_ipv6exthdr_flags const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPIEH_NONEXT: return os << "OFPIEH_NONEXT";
    case OFPIEH_ESP: return os << "OFPIEH_ESP";
    case OFPIEH_AUTH: return os << "OFPIEH_AUTH";
    case OFPIEH_DEST: return os << "OFPIEH_DEST";
    case OFPIEH_FRAG: return os << "OFPIEH_FRAG";
    case OFPIEH_ROUTER: return os << "OFPIEH_ROUTER";
    case OFPIEH_HOP: return os << "OFPIEH_HOP";
    case OFPIEH_UNREP: return os << "OFPIEH_UNREP";
    case OFPIEH_UNSEQ: return os << "OFPIEH_UNSEQ";
    default: return os << "unkonw_ofp_ipv6exthdr_flags(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_action_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPAT_OUTPUT: return os << "OFPAT_OUTPUT";
    case OFPAT_COPY_TTL_OUT: return os << "OFPAT_COPY_TTL_OUT";
    case OFPAT_COPY_TTL_IN: return os << "OFPAT_COPY_TTL_IN";
    case OFPAT_SET_MPLS_TTL: return os << "OFPAT_SET_MPLS_TTL";
    case OFPAT_DEC_MPLS_TTL: return os << "OFPAT_DEC_MPLS_TTL";
    case OFPAT_PUSH_VLAN: return os << "OFPAT_PUSH_VLAN";
    case OFPAT_POP_VLAN: return os << "OFPAT_POP_VLAN";
    case OFPAT_PUSH_MPLS: return os << "OFPAT_PUSH_MPLS";
    case OFPAT_POP_MPLS: return os << "OFPAT_POP_MPLS";
    case OFPAT_SET_QUEUE: return os << "OFPAT_SET_QUEUE";
    case OFPAT_GROUP: return os << "OFPAT_GROUP";
    case OFPAT_SET_NW_TTL: return os << "OFPAT_SET_NW_TTL";
    case OFPAT_DEC_NW_TTL: return os << "OFPAT_DEC_NW_TTL";
    case OFPAT_SET_FIELD: return os << "OFPAT_SET_FIELD";
    case OFPAT_PUSH_PBB: return os << "OFPAT_PUSH_PBB";
    case OFPAT_POP_PBB: return os << "OFPAT_POP_PBB";
    case OFPAT_EXPERIMENTER: return os << "OFPAT_EXPERIMENTER";
    default: return os << "unkonw_ofp_action_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_controller_max_len const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPCML_MAX: return os << "OFPCML_MAX";
    case OFPCML_NO_BUFFER: return os << "OFPCML_NO_BUFFER";
    default: return os << "unkonw_ofp_controller_max_len(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_instruction_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPIT_GOTO_TABLE: return os << "OFPIT_GOTO_TABLE";
    case OFPIT_WRITE_METADATA: return os << "OFPIT_WRITE_METADATA";
    case OFPIT_WRITE_ACTIONS: return os << "OFPIT_WRITE_ACTIONS";
    case OFPIT_APPLY_ACTIONS: return os << "OFPIT_APPLY_ACTIONS";
    case OFPIT_CLEAR_ACTIONS: return os << "OFPIT_CLEAR_ACTIONS";
    case OFPIT_METER: return os << "OFPIT_METER";
    case OFPIT_EXPERIMENTER: return os << "OFPIT_EXPERIMENTER";
    default: return os << "unkonw_ofp_instruction_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_command const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPFC_ADD: return os << "OFPFC_ADD";
    case OFPFC_MODIFY: return os << "OFPFC_MODIFY";
    case OFPFC_MODIFY_STRICT: return os << "OFPFC_MODIFY_STRICT";
    case OFPFC_DELETE: return os << "OFPFC_DELETE";
    case OFPFC_DELETE_STRICT: return os << "OFPFC_DELETE_STRICT";
    default: return os << "unkonw_ofp_flow_mod_command(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_flags const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPFF_SEND_FLOW_REM: return os << "OFPFF_SEND_FLOW_REM";
    case OFPFF_CHECK_OVERLAP: return os << "OFPFF_CHECK_OVERLAP";
    case OFPFF_RESET_COUNTS: return os << "OFPFF_RESET_COUNTS";
    case OFPFF_NO_PKT_COUNTS: return os << "OFPFF_NO_PKT_COUNTS";
    case OFPFF_NO_BYT_COUNTS: return os << "OFPFF_NO_BYT_COUNTS";
    default: return os << "unkonw_ofp_flow_mod_flags(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_group const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPG_MAX: return os << "OFPG_MAX";
    case OFPG_ALL: return os << "OFPG_ALL";
    case OFPG_ANY: return os << "OFPG_ANY";
    default: return os << "unkonw_ofp_group(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_group_mod_command const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPGC_ADD: return os << "OFPGC_ADD";
    case OFPGC_MODIFY: return os << "OFPGC_MODIFY";
    case OFPGC_DELETE: return os << "OFPGC_DELETE";
    default: return os << "unkonw_ofp_group_mod_command(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_group_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPGT_ALL: return os << "OFPGT_ALL";
    case OFPGT_SELECT: return os << "OFPGT_SELECT";
    case OFPGT_INDIRECT: return os << "OFPGT_INDIRECT";
    case OFPGT_FF: return os << "OFPGT_FF";
    default: return os << "unkonw_ofp_group_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_packet_in_reason const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPR_NO_MATCH: return os << "OFPR_NO_MATCH";
    case OFPR_ACTION: return os << "OFPR_ACTION";
    case OFPR_INVALID_TTL: return os << "OFPR_INVALID_TTL";
    default: return os << "unkonw_ofp_packet_in_reason(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_flow_removed_reason const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPRR_IDLE_TIMEOUT: return os << "OFPRR_IDLE_TIMEOUT";
    case OFPRR_HARD_TIMEOUT: return os << "OFPRR_HARD_TIMEOUT";
    case OFPRR_DELETE: return os << "OFPRR_DELETE";
    case OFPRR_GROUP_DELETE: return os << "OFPRR_GROUP_DELETE";
    default: return os << "unkonw_ofp_flow_removed_reason(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_meter const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPM_MAX: return os << "OFPM_MAX";
    case OFPM_SLOWPATH: return os << "OFPM_SLOWPATH";
    case OFPM_CONTROLLER: return os << "OFPM_CONTROLLER";
    case OFPM_ALL: return os << "OFPM_ALL";
    default: return os << "unkonw_ofp_meter(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_meter_band_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPMBT_DROP: return os << "OFPMBT_DROP";
    case OFPMBT_DSCP_REMARK: return os << "OFPMBT_DSCP_REMARK";
    case OFPMBT_EXPERIMENTER: return os << "OFPMBT_EXPERIMENTER";
    default: return os << "unkonw_ofp_meter_band_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_meter_mod_command const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPMC_ADD: return os << "OFPMC_ADD";
    case OFPMC_MODIFY: return os << "OFPMC_MODIFY";
    case OFPMC_DELETE: return os << "OFPMC_DELETE";
    default: return os << "unkonw_ofp_meter_mod_command(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_meter_flags const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPMF_KBPS: return os << "OFPMF_KBPS";
    case OFPMF_PKTPS: return os << "OFPMF_PKTPS";
    case OFPMF_BURST: return os << "OFPMF_BURST";
    case OFPMF_STATS: return os << "OFPMF_STATS";
    default: return os << "unkonw_ofp_meter_flags(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_error_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPET_HELLO_FAILED: return os << "OFPET_HELLO_FAILED";
    case OFPET_BAD_REQUEST: return os << "OFPET_BAD_REQUEST";
    case OFPET_BAD_ACTION: return os << "OFPET_BAD_ACTION";
    case OFPET_BAD_INSTRUCTION: return os << "OFPET_BAD_INSTRUCTION";
    case OFPET_BAD_MATCH: return os << "OFPET_BAD_MATCH";
    case OFPET_FLOW_MOD_FAILED: return os << "OFPET_FLOW_MOD_FAILED";
    case OFPET_GROUP_MOD_FAILED: return os << "OFPET_GROUP_MOD_FAILED";
    case OFPET_PORT_MOD_FAILED: return os << "OFPET_PORT_MOD_FAILED";
    case OFPET_TABLE_MOD_FAILED: return os << "OFPET_TABLE_MOD_FAILED";
    case OFPET_QUEUE_OP_FAILED: return os << "OFPET_QUEUE_OP_FAILED";
    case OFPET_SWITCH_CONFIG_FAILED: return os << "OFPET_SWITCH_CONFIG_FAILED";
    case OFPET_ROLE_REQUEST_FAILED: return os << "OFPET_ROLE_REQUEST_FAILED";
    case OFPET_METER_MOD_FAILED: return os << "OFPET_METER_MOD_FAILED";
    case OFPET_TABLE_FEATURES_FAILED: return os << "OFPET_TABLE_FEATURES_FAILED";
    case OFPET_EXPERIMENTER: return os << "OFPET_EXPERIMENTER";
    default: return os << "unkonw_ofp_error_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_hello_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPHFC_INCOMPATIBLE: return os << "OFPHFC_INCOMPATIBLE";
    case OFPHFC_EPERM: return os << "OFPHFC_EPERM";
    default: return os << "unkonw_ofp_hello_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_bad_request_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPBRC_BAD_VERSION: return os << "OFPBRC_BAD_VERSION";
    case OFPBRC_BAD_TYPE: return os << "OFPBRC_BAD_TYPE";
    case OFPBRC_BAD_MULTIPART: return os << "OFPBRC_BAD_MULTIPART";
    case OFPBRC_BAD_EXPERIMENTER: return os << "OFPBRC_BAD_EXPERIMENTER";
    case OFPBRC_BAD_EXP_TYPE: return os << "OFPBRC_BAD_EXP_TYPE";
    case OFPBRC_EPERM: return os << "OFPBRC_EPERM";
    case OFPBRC_BAD_LEN: return os << "OFPBRC_BAD_LEN";
    case OFPBRC_BUFFER_EMPTY: return os << "OFPBRC_BUFFER_EMPTY";
    case OFPBRC_BUFFER_UNKNOWN: return os << "OFPBRC_BUFFER_UNKNOWN";
    case OFPBRC_BAD_TABLE_ID: return os << "OFPBRC_BAD_TABLE_ID";
    case OFPBRC_IS_SLAVE: return os << "OFPBRC_IS_SLAVE";
    case OFPBRC_BAD_PORT: return os << "OFPBRC_BAD_PORT";
    case OFPBRC_BAD_PACKET: return os << "OFPBRC_BAD_PACKET";
    case OFPBRC_MULTIPART_BUFFER_OVERFLOW: return os << "OFPBRC_MULTIPART_BUFFER_OVERFLOW";
    default: return os << "unkonw_ofp_bad_request_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_bad_action_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPBAC_BAD_TYPE: return os << "OFPBAC_BAD_TYPE";
    case OFPBAC_BAD_LEN: return os << "OFPBAC_BAD_LEN";
    case OFPBAC_BAD_EXPERIMENTER: return os << "OFPBAC_BAD_EXPERIMENTER";
    case OFPBAC_BAD_EXP_TYPE: return os << "OFPBAC_BAD_EXP_TYPE";
    case OFPBAC_BAD_OUT_PORT: return os << "OFPBAC_BAD_OUT_PORT";
    case OFPBAC_BAD_ARGUMENT: return os << "OFPBAC_BAD_ARGUMENT";
    case OFPBAC_EPERM: return os << "OFPBAC_EPERM";
    case OFPBAC_TOO_MANY: return os << "OFPBAC_TOO_MANY";
    case OFPBAC_BAD_QUEUE: return os << "OFPBAC_BAD_QUEUE";
    case OFPBAC_BAD_OUT_GROUP: return os << "OFPBAC_BAD_OUT_GROUP";
    case OFPBAC_MATCH_INCONSISTENT: return os << "OFPBAC_MATCH_INCONSISTENT";
    case OFPBAC_UNSUPPORTED_ORDER: return os << "OFPBAC_UNSUPPORTED_ORDER";
    case OFPBAC_BAD_TAG: return os << "OFPBAC_BAD_TAG";
    case OFPBAC_BAD_SET_TYPE: return os << "OFPBAC_BAD_SET_TYPE";
    case OFPBAC_BAD_SET_LEN: return os << "OFPBAC_BAD_SET_LEN";
    case OFPBAC_BAD_SET_ARGUMENT: return os << "OFPBAC_BAD_SET_ARGUMENT";
    default: return os << "unkonw_ofp_bad_action_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_bad_instruction_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPBIC_UNKNOWN_INST: return os << "OFPBIC_UNKNOWN_INST";
    case OFPBIC_UNSUP_INST: return os << "OFPBIC_UNSUP_INST";
    case OFPBIC_BAD_TABLE_ID: return os << "OFPBIC_BAD_TABLE_ID";
    case OFPBIC_UNSUP_METADATA: return os << "OFPBIC_UNSUP_METADATA";
    case OFPBIC_UNSUP_METADATA_MASK: return os << "OFPBIC_UNSUP_METADATA_MASK";
    case OFPBIC_BAD_EXPERIMENTER: return os << "OFPBIC_BAD_EXPERIMENTER";
    case OFPBIC_BAD_EXP_TYPE: return os << "OFPBIC_BAD_EXP_TYPE";
    case OFPBIC_BAD_LEN: return os << "OFPBIC_BAD_LEN";
    case OFPBIC_EPERM: return os << "OFPBIC_EPERM";
    default: return os << "unkonw_ofp_bad_instruction_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_bad_match_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPBMC_BAD_TYPE: return os << "OFPBMC_BAD_TYPE";
    case OFPBMC_BAD_LEN: return os << "OFPBMC_BAD_LEN";
    case OFPBMC_BAD_TAG: return os << "OFPBMC_BAD_TAG";
    case OFPBMC_BAD_DL_ADDR_MASK: return os << "OFPBMC_BAD_DL_ADDR_MASK";
    case OFPBMC_BAD_NW_ADDR_MASK: return os << "OFPBMC_BAD_NW_ADDR_MASK";
    case OFPBMC_BAD_WILDCARDS: return os << "OFPBMC_BAD_WILDCARDS";
    case OFPBMC_BAD_FIELD: return os << "OFPBMC_BAD_FIELD";
    case OFPBMC_BAD_VALUE: return os << "OFPBMC_BAD_VALUE";
    case OFPBMC_BAD_MASK: return os << "OFPBMC_BAD_MASK";
    case OFPBMC_BAD_PREREQ: return os << "OFPBMC_BAD_PREREQ";
    case OFPBMC_DUP_FIELD: return os << "OFPBMC_DUP_FIELD";
    case OFPBMC_EPERM: return os << "OFPBMC_EPERM";
    default: return os << "unkonw_ofp_bad_match_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPFMFC_UNKNOWN: return os << "OFPFMFC_UNKNOWN";
    case OFPFMFC_TABLE_FULL: return os << "OFPFMFC_TABLE_FULL";
    case OFPFMFC_BAD_TABLE_ID: return os << "OFPFMFC_BAD_TABLE_ID";
    case OFPFMFC_OVERLAP: return os << "OFPFMFC_OVERLAP";
    case OFPFMFC_EPERM: return os << "OFPFMFC_EPERM";
    case OFPFMFC_BAD_TIMEOUT: return os << "OFPFMFC_BAD_TIMEOUT";
    case OFPFMFC_BAD_COMMAND: return os << "OFPFMFC_BAD_COMMAND";
    case OFPFMFC_BAD_FLAGS: return os << "OFPFMFC_BAD_FLAGS";
    default: return os << "unkonw_ofp_flow_mod_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_group_mod_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPGMFC_GROUP_EXISTS: return os << "OFPGMFC_GROUP_EXISTS";
    case OFPGMFC_INVALID_GROUP: return os << "OFPGMFC_INVALID_GROUP";
    case OFPGMFC_WEIGHT_UNSUPPORTED: return os << "OFPGMFC_WEIGHT_UNSUPPORTED";
    case OFPGMFC_OUT_OF_GROUPS: return os << "OFPGMFC_OUT_OF_GROUPS";
    case OFPGMFC_OUT_OF_BUCKETS: return os << "OFPGMFC_OUT_OF_BUCKETS";
    case OFPGMFC_CHAINING_UNSUPPORTED: return os << "OFPGMFC_CHAINING_UNSUPPORTED";
    case OFPGMFC_WATCH_UNSUPPORTED: return os << "OFPGMFC_WATCH_UNSUPPORTED";
    case OFPGMFC_LOOP: return os << "OFPGMFC_LOOP";
    case OFPGMFC_UNKNOWN_GROUP: return os << "OFPGMFC_UNKNOWN_GROUP";
    case OFPGMFC_CHAINED_GROUP: return os << "OFPGMFC_CHAINED_GROUP";
    case OFPGMFC_BAD_TYPE: return os << "OFPGMFC_BAD_TYPE";
    case OFPGMFC_BAD_COMMAND: return os << "OFPGMFC_BAD_COMMAND";
    case OFPGMFC_BAD_BUCKET: return os << "OFPGMFC_BAD_BUCKET";
    case OFPGMFC_BAD_WATCH: return os << "OFPGMFC_BAD_WATCH";
    case OFPGMFC_EPERM: return os << "OFPGMFC_EPERM";
    default: return os << "unkonw_ofp_group_mod_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port_mod_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPPMFC_BAD_PORT: return os << "OFPPMFC_BAD_PORT";
    case OFPPMFC_BAD_HW_ADDR: return os << "OFPPMFC_BAD_HW_ADDR";
    case OFPPMFC_BAD_CONFIG: return os << "OFPPMFC_BAD_CONFIG";
    case OFPPMFC_BAD_ADVERTISE: return os << "OFPPMFC_BAD_ADVERTISE";
    case OFPPMFC_EPERM: return os << "OFPPMFC_EPERM";
    default: return os << "unkonw_ofp_port_mod_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_table_mod_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPTMFC_BAD_TABLE: return os << "OFPTMFC_BAD_TABLE";
    case OFPTMFC_BAD_CONFIG: return os << "OFPTMFC_BAD_CONFIG";
    case OFPTMFC_EPERM: return os << "OFPTMFC_EPERM";
    default: return os << "unkonw_ofp_table_mod_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_queue_op_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPQOFC_BAD_PORT: return os << "OFPQOFC_BAD_PORT";
    case OFPQOFC_BAD_QUEUE: return os << "OFPQOFC_BAD_QUEUE";
    case OFPQOFC_EPERM: return os << "OFPQOFC_EPERM";
    default: return os << "unkonw_ofp_queue_op_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_switch_config_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPSCFC_BAD_FLAGS: return os << "OFPSCFC_BAD_FLAGS";
    case OFPSCFC_BAD_LEN: return os << "OFPSCFC_BAD_LEN";
    case OFPSCFC_EPERM: return os << "OFPSCFC_EPERM";
    default: return os << "unkonw_ofp_switch_config_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_role_request_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPRRFC_STALE: return os << "OFPRRFC_STALE";
    case OFPRRFC_UNSUP: return os << "OFPRRFC_UNSUP";
    case OFPRRFC_BAD_ROLE: return os << "OFPRRFC_BAD_ROLE";
    default: return os << "unkonw_ofp_role_request_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_meter_mod_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPMMFC_UNKNOWN: return os << "OFPMMFC_UNKNOWN";
    case OFPMMFC_METER_EXISTS: return os << "OFPMMFC_METER_EXISTS";
    case OFPMMFC_INVALID_METER: return os << "OFPMMFC_INVALID_METER";
    case OFPMMFC_UNKNOWN_METER: return os << "OFPMMFC_UNKNOWN_METER";
    case OFPMMFC_BAD_COMMAND: return os << "OFPMMFC_BAD_COMMAND";
    case OFPMMFC_BAD_FLAGS: return os << "OFPMMFC_BAD_FLAGS";
    case OFPMMFC_BAD_RATE: return os << "OFPMMFC_BAD_RATE";
    case OFPMMFC_BAD_BURST: return os << "OFPMMFC_BAD_BURST";
    case OFPMMFC_BAD_BAND: return os << "OFPMMFC_BAD_BAND";
    case OFPMMFC_BAD_BAND_VALUE: return os << "OFPMMFC_BAD_BAND_VALUE";
    case OFPMMFC_OUT_OF_METERS: return os << "OFPMMFC_OUT_OF_METERS";
    case OFPMMFC_OUT_OF_BANDS: return os << "OFPMMFC_OUT_OF_BANDS";
    default: return os << "unkonw_ofp_meter_mod_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_table_features_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPTFFC_BAD_TABLE: return os << "OFPTFFC_BAD_TABLE";
    case OFPTFFC_BAD_METADATA: return os << "OFPTFFC_BAD_METADATA";
    case OFPTFFC_BAD_TYPE: return os << "OFPTFFC_BAD_TYPE";
    case OFPTFFC_BAD_LEN: return os << "OFPTFFC_BAD_LEN";
    case OFPTFFC_BAD_ARGUMENT: return os << "OFPTFFC_BAD_ARGUMENT";
    case OFPTFFC_EPERM: return os << "OFPTFFC_EPERM";
    default: return os << "unkonw_ofp_table_features_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_multipart_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPMP_DESC: return os << "OFPMP_DESC";
    case OFPMP_FLOW: return os << "OFPMP_FLOW";
    case OFPMP_AGGREGATE: return os << "OFPMP_AGGREGATE";
    case OFPMP_TABLE: return os << "OFPMP_TABLE";
    case OFPMP_PORT_STATS: return os << "OFPMP_PORT_STATS";
    case OFPMP_QUEUE: return os << "OFPMP_QUEUE";
    case OFPMP_GROUP: return os << "OFPMP_GROUP";
    case OFPMP_GROUP_DESC: return os << "OFPMP_GROUP_DESC";
    case OFPMP_GROUP_FEATURES: return os << "OFPMP_GROUP_FEATURES";
    case OFPMP_METER: return os << "OFPMP_METER";
    case OFPMP_METER_CONFIG: return os << "OFPMP_METER_CONFIG";
    case OFPMP_METER_FEATURES: return os << "OFPMP_METER_FEATURES";
    case OFPMP_TABLE_FEATURES: return os << "OFPMP_TABLE_FEATURES";
    case OFPMP_PORT_DESC: return os << "OFPMP_PORT_DESC";
    case OFPMP_EXPERIMENTER: return os << "OFPMP_EXPERIMENTER";
    default: return os << "unkonw_ofp_multipart_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_multipart_request_flags const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPMPF_REQ_MORE: return os << "OFPMPF_REQ_MORE";
    default: return os << "unkonw_ofp_multipart_request_flags(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_multipart_reply_flags const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPMPF_REPLY_MORE: return os << "OFPMPF_REPLY_MORE";
    default: return os << "unkonw_ofp_multipart_reply_flags(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_table_feature_prop_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPTFPT_INSTRUCTIONS: return os << "OFPTFPT_INSTRUCTIONS";
    case OFPTFPT_INSTRUCTIONS_MISS: return os << "OFPTFPT_INSTRUCTIONS_MISS";
    case OFPTFPT_NEXT_TABLES: return os << "OFPTFPT_NEXT_TABLES";
    case OFPTFPT_NEXT_TABLES_MISS: return os << "OFPTFPT_NEXT_TABLES_MISS";
    case OFPTFPT_WRITE_ACTIONS: return os << "OFPTFPT_WRITE_ACTIONS";
    case OFPTFPT_WRITE_ACTIONS_MISS: return os << "OFPTFPT_WRITE_ACTIONS_MISS";
    case OFPTFPT_APPLY_ACTIONS: return os << "OFPTFPT_APPLY_ACTIONS";
    case OFPTFPT_APPLY_ACTIONS_MISS: return os << "OFPTFPT_APPLY_ACTIONS_MISS";
    case OFPTFPT_MATCH: return os << "OFPTFPT_MATCH";
    case OFPTFPT_WILDCARDS: return os << "OFPTFPT_WILDCARDS";
    case OFPTFPT_WRITE_SETFIELD: return os << "OFPTFPT_WRITE_SETFIELD";
    case OFPTFPT_WRITE_SETFIELD_MISS: return os << "OFPTFPT_WRITE_SETFIELD_MISS";
    case OFPTFPT_APPLY_SETFIELD: return os << "OFPTFPT_APPLY_SETFIELD";
    case OFPTFPT_APPLY_SETFIELD_MISS: return os << "OFPTFPT_APPLY_SETFIELD_MISS";
    case OFPTFPT_EXPERIMENTER: return os << "OFPTFPT_EXPERIMENTER";
    case OFPTFPT_EXPERIMENTER_MISS: return os << "OFPTFPT_EXPERIMENTER_MISS";
    default: return os << "unkonw_ofp_table_feature_prop_type(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_group_capabilities const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPGFC_SELECT_WEIGHT: return os << "OFPGFC_SELECT_WEIGHT";
    case OFPGFC_SELECT_LIVENESS: return os << "OFPGFC_SELECT_LIVENESS";
    case OFPGFC_CHAINING: return os << "OFPGFC_CHAINING";
    case OFPGFC_CHAINING_CHECKS: return os << "OFPGFC_CHAINING_CHECKS";
    default: return os << "unkonw_ofp_group_capabilities(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_queue_properties const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPQT_MIN_RATE: return os << "OFPQT_MIN_RATE";
    case OFPQT_MAX_RATE: return os << "OFPQT_MAX_RATE";
    case OFPQT_EXPERIMENTER: return os << "OFPQT_EXPERIMENTER";
    default: return os << "unkonw_ofp_queue_properties(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_controller_role const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPCR_ROLE_NOCHANGE: return os << "OFPCR_ROLE_NOCHANGE";
    case OFPCR_ROLE_EQUAL: return os << "OFPCR_ROLE_EQUAL";
    case OFPCR_ROLE_MASTER: return os << "OFPCR_ROLE_MASTER";
    case OFPCR_ROLE_SLAVE: return os << "OFPCR_ROLE_SLAVE";
    default: return os << "unkonw_ofp_controller_role(" << std::uint64_t(value) << ")";
    }
  }

  namespace protocol_detail {

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_no::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_no::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_hello_elem_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_hello_elem_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_config_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_config_flags::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_table_config::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_table_config::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_table::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_table::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_capabilities::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_capabilities::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_config::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_config::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_state::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_state::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_features::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_features::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_reason::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_reason::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_match_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_match_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_oxm_class::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_oxm_class::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, oxm_ofb_match_fields::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, oxm_ofb_match_fields::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_vlan_id::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_vlan_id::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_ipv6exthdr_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_ipv6exthdr_flags::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_action_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_action_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_controller_max_len::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_controller_max_len::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_instruction_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_instruction_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_command::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_mod_command::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_mod_flags::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_group::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_group::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_group_mod_command::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_group_mod_command::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_group_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_group_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_packet_in_reason::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_packet_in_reason::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_removed_reason::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_removed_reason::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_meter::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_meter::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_meter_band_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_meter_band_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_meter_mod_command::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_meter_mod_command::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_meter_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_meter_flags::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_error_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_error_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_hello_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_hello_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_bad_request_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_bad_request_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_bad_action_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_bad_action_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_bad_instruction_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_bad_instruction_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_bad_match_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_bad_match_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_mod_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_group_mod_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_group_mod_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_mod_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_mod_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_table_mod_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_table_mod_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_queue_op_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_queue_op_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_switch_config_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_switch_config_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_role_request_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_role_request_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_meter_mod_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_meter_mod_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_table_features_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_table_features_failed_code::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_multipart_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_multipart_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_multipart_request_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_multipart_request_flags::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_multipart_reply_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_multipart_reply_flags::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_table_feature_prop_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_table_feature_prop_type::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_group_capabilities::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_group_capabilities::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_queue_properties::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_queue_properties::unscoped_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_controller_role::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_controller_role::unscoped_type(value));
    }

  } // namespace protocol_detail

  template <class CharT, class Traits, class T>
  auto operator<<(
      std::basic_ostream<CharT, Traits>& os, scoped_enum<T> const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    return protocol_detail::operator<<(os, value);
  }

} // namespace protocol
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_IO_OPENFLOW_HPP