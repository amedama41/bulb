#ifndef CANARD_NET_OFP_V10_IO_OPENFLOW_HPP
#define CANARD_NET_OFP_V10_IO_OPENFLOW_HPP

#include <cstdint>
#include <ostream>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace protocol {

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port const value)
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
    case OFPP_NONE: return os << "OFPP_NONE";
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
    case OFPT_VENDOR: return os << "OFPT_VENDOR";
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
    case OFPT_PORT_MOD: return os << "OFPT_PORT_MOD";
    case OFPT_STATS_REQUEST: return os << "OFPT_STATS_REQUEST";
    case OFPT_STATS_REPLY: return os << "OFPT_STATS_REPLY";
    case OFPT_BARRIER_REQUEST: return os << "OFPT_BARRIER_REQUEST";
    case OFPT_BARRIER_REPLY: return os << "OFPT_BARRIER_REPLY";
    case OFPT_QUEUE_GET_CONFIG_REQUEST: return os << "OFPT_QUEUE_GET_CONFIG_REQUEST";
    case OFPT_QUEUE_GET_CONFIG_REPLY: return os << "OFPT_QUEUE_GET_CONFIG_REPLY";
    default: return os << "unkonw_ofp_type(" << std::uint64_t(value) << ")";
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
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_capabilities const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPC_FLOW_STATS: return os << "OFPC_FLOW_STATS";
    case OFPC_TABLE_STATS: return os << "OFPC_TABLE_STATS";
    case OFPC_PORT_STATS: return os << "OFPC_PORT_STATS";
    case OFPC_STP: return os << "OFPC_STP";
    case OFPC_RESERVED: return os << "OFPC_RESERVED";
    case OFPC_IP_REASM: return os << "OFPC_IP_REASM";
    case OFPC_QUEUE_STATS: return os << "OFPC_QUEUE_STATS";
    case OFPC_ARP_MATCH_IP: return os << "OFPC_ARP_MATCH_IP";
    default: return os << "unkonw_ofp_capabilities(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port_config const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPPC_PORT_DOWN: return os << "OFPPC_PORT_DOWN";
    case OFPPC_NO_STP: return os << "OFPPC_NO_STP";
    case OFPPC_NO_RECV: return os << "OFPPC_NO_RECV";
    case OFPPC_NO_RECV_STP: return os << "OFPPC_NO_RECV_STP";
    case OFPPC_NO_FLOOD: return os << "OFPPC_NO_FLOOD";
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
    case OFPPS_STP_LISTEN: return os << "OFPPS_STP_LISTEN";
    case OFPPS_STP_LEARN: return os << "OFPPS_STP_LEARN";
    case OFPPS_STP_FORWARD: return os << "OFPPS_STP_FORWARD";
    case OFPPS_STP_BLOCK: return os << "OFPPS_STP_BLOCK";
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
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_packet_in_reason const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPR_NO_MATCH: return os << "OFPR_NO_MATCH";
    case OFPR_ACTION: return os << "OFPR_ACTION";
    default: return os << "unkonw_ofp_packet_in_reason(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_action_type const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPAT_OUTPUT: return os << "OFPAT_OUTPUT";
    case OFPAT_SET_VLAN_VID: return os << "OFPAT_SET_VLAN_VID";
    case OFPAT_SET_VLAN_PCP: return os << "OFPAT_SET_VLAN_PCP";
    case OFPAT_STRIP_VLAN: return os << "OFPAT_STRIP_VLAN";
    case OFPAT_SET_DL_SRC: return os << "OFPAT_SET_DL_SRC";
    case OFPAT_SET_DL_DST: return os << "OFPAT_SET_DL_DST";
    case OFPAT_SET_NW_SRC: return os << "OFPAT_SET_NW_SRC";
    case OFPAT_SET_NW_DST: return os << "OFPAT_SET_NW_DST";
    case OFPAT_SET_NW_TOS: return os << "OFPAT_SET_NW_TOS";
    case OFPAT_SET_TP_SRC: return os << "OFPAT_SET_TP_SRC";
    case OFPAT_SET_TP_DST: return os << "OFPAT_SET_TP_DST";
    case OFPAT_ENQUEUE: return os << "OFPAT_ENQUEUE";
    case OFPAT_VENDOR: return os << "OFPAT_VENDOR";
    default: return os << "unkonw_ofp_action_type(" << std::uint64_t(value) << ")";
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
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_flow_wildcards const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPFW_IN_PORT: return os << "OFPFW_IN_PORT";
    case OFPFW_DL_VLAN: return os << "OFPFW_DL_VLAN";
    case OFPFW_DL_SRC: return os << "OFPFW_DL_SRC";
    case OFPFW_DL_DST: return os << "OFPFW_DL_DST";
    case OFPFW_DL_TYPE: return os << "OFPFW_DL_TYPE";
    case OFPFW_NW_PROTO: return os << "OFPFW_NW_PROTO";
    case OFPFW_TP_SRC: return os << "OFPFW_TP_SRC";
    case OFPFW_TP_DST: return os << "OFPFW_TP_DST";
    case OFPFW_NW_SRC_MASK: return os << "OFPFW_NW_SRC_MASK";
    case OFPFW_NW_SRC_ALL: return os << "OFPFW_NW_SRC_ALL";
    case OFPFW_NW_DST_MASK: return os << "OFPFW_NW_DST_MASK";
    case OFPFW_NW_DST_ALL: return os << "OFPFW_NW_DST_ALL";
    case OFPFW_DL_VLAN_PCP: return os << "OFPFW_DL_VLAN_PCP";
    case OFPFW_NW_TOS: return os << "OFPFW_NW_TOS";
    case OFPFW_ALL: return os << "OFPFW_ALL";
    default: return os << "unkonw_ofp_flow_wildcards(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_flags const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPFF_SEND_FLOW_REM: return os << "OFPFF_SEND_FLOW_REM";
    case OFPFF_CHECK_OVERLAP: return os << "OFPFF_CHECK_OVERLAP";
    case OFPFF_EMERG: return os << "OFPFF_EMERG";
    default: return os << "unkonw_ofp_flow_mod_flags(" << std::uint64_t(value) << ")";
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
    default: return os << "unkonw_ofp_flow_removed_reason(" << std::uint64_t(value) << ")";
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
    case OFPET_FLOW_MOD_FAILED: return os << "OFPET_FLOW_MOD_FAILED";
    case OFPET_PORT_MOD_FAILED: return os << "OFPET_PORT_MOD_FAILED";
    case OFPET_QUEUE_OP_FAILED: return os << "OFPET_QUEUE_OP_FAILED";
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
    case OFPBRC_BAD_STAT: return os << "OFPBRC_BAD_STAT";
    case OFPBRC_BAD_VENDOR: return os << "OFPBRC_BAD_VENDOR";
    case OFPBRC_BAD_SUBTYPE: return os << "OFPBRC_BAD_SUBTYPE";
    case OFPBRC_EPERM: return os << "OFPBRC_EPERM";
    case OFPBRC_BAD_LEN: return os << "OFPBRC_BAD_LEN";
    case OFPBRC_BUFFER_EMPTY: return os << "OFPBRC_BUFFER_EMPTY";
    case OFPBRC_BUFFER_UNKNOWN: return os << "OFPBRC_BUFFER_UNKNOWN";
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
    case OFPBAC_BAD_VENDOR: return os << "OFPBAC_BAD_VENDOR";
    case OFPBAC_BAD_VENDOR_TYPE: return os << "OFPBAC_BAD_VENDOR_TYPE";
    case OFPBAC_BAD_OUT_PORT: return os << "OFPBAC_BAD_OUT_PORT";
    case OFPBAC_BAD_ARGUMENT: return os << "OFPBAC_BAD_ARGUMENT";
    case OFPBAC_EPERM: return os << "OFPBAC_EPERM";
    case OFPBAC_TOO_MANY: return os << "OFPBAC_TOO_MANY";
    case OFPBAC_BAD_QUEUE: return os << "OFPBAC_BAD_QUEUE";
    default: return os << "unkonw_ofp_bad_action_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPFMFC_ALL_TABLES_FULL: return os << "OFPFMFC_ALL_TABLES_FULL";
    case OFPFMFC_OVERLAP: return os << "OFPFMFC_OVERLAP";
    case OFPFMFC_EPERM: return os << "OFPFMFC_EPERM";
    case OFPFMFC_BAD_EMERG_TIMEOUT: return os << "OFPFMFC_BAD_EMERG_TIMEOUT";
    case OFPFMFC_BAD_COMMAND: return os << "OFPFMFC_BAD_COMMAND";
    case OFPFMFC_UNSUPPORTED: return os << "OFPFMFC_UNSUPPORTED";
    default: return os << "unkonw_ofp_flow_mod_failed_code(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_port_mod_failed_code const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPPMFC_BAD_PORT: return os << "OFPPMFC_BAD_PORT";
    case OFPPMFC_BAD_HW_ADDR: return os << "OFPPMFC_BAD_HW_ADDR";
    default: return os << "unkonw_ofp_port_mod_failed_code(" << std::uint64_t(value) << ")";
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
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_stats_types const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPST_DESC: return os << "OFPST_DESC";
    case OFPST_FLOW: return os << "OFPST_FLOW";
    case OFPST_AGGREGATE: return os << "OFPST_AGGREGATE";
    case OFPST_TABLE: return os << "OFPST_TABLE";
    case OFPST_PORT: return os << "OFPST_PORT";
    case OFPST_QUEUE: return os << "OFPST_QUEUE";
    case OFPST_VENDOR: return os << "OFPST_VENDOR";
    default: return os << "unkonw_ofp_stats_types(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_stats_reply_flags const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPSF_REPLY_MORE: return os << "OFPSF_REPLY_MORE";
    default: return os << "unkonw_ofp_stats_reply_flags(" << std::uint64_t(value) << ")";
    }
  }

  template <class CharT, class Traits>
  auto operator<<(std::basic_ostream<CharT, Traits>& os, ofp_queue_properties const value)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (value) {
    case OFPQT_NONE: return os << "OFPQT_NONE";
    case OFPQT_MIN_RATE: return os << "OFPQT_MIN_RATE";
    default: return os << "unkonw_ofp_queue_properties(" << std::uint64_t(value) << ")";
    }
  }

  namespace protocol_detail {

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_type::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_config_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_config_flags::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_capabilities::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_capabilities::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_config::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_config::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_state::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_state::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_features::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_features::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_reason::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_reason::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_packet_in_reason::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_packet_in_reason::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_action_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_action_type::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_command::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_mod_command::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_wildcards::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_wildcards::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_mod_flags::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_removed_reason::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_removed_reason::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_error_type::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_error_type::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_hello_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_hello_failed_code::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_bad_request_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_bad_request_code::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_bad_action_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_bad_action_code::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_flow_mod_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_flow_mod_failed_code::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_port_mod_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_port_mod_failed_code::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_queue_op_failed_code::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_queue_op_failed_code::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_stats_types::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_stats_types::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_stats_reply_flags::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_stats_reply_flags::original_type(value));
    }

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_queue_properties::type const value)
      -> std::basic_ostream<CharT, Traits>&
    {
      return protocol::operator<<(os, ofp_queue_properties::original_type(value));
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
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_IO_OPENFLOW_HPP