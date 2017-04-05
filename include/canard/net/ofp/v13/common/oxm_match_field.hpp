#ifndef CANARD_NET_OFP_V13_OXM_MATCH_FIELD_HPP
#define CANARD_NET_OFP_V13_OXM_MATCH_FIELD_HPP

#include <cstdint>
#include <stdexcept>
#include <tuple>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>
#include <boost/endian/arithmetic.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>
#include <boost/fusion/support/pair.hpp>
#include <canard/mac_address.hpp>
#include <canard/net/ofp/v13/detail/basic_oxm_match_field.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace oxm_match_fields {

  namespace oxm_match_detail {

    template <class T, bool NeedsByteOrderConversion = true>
    struct generic_type
    {
      using value_type = T;
      using oxm_value_type = value_type;

      static constexpr bool needs_byte_order_conversion
        = NeedsByteOrderConversion;

      static auto to_oxm_value(value_type const& v) noexcept
        -> oxm_value_type
      { return v; }
    };

    struct mac_address_type
    {
      using value_type = canard::mac_address;
      using oxm_value_type = value_type::bytes_type;

      static constexpr bool needs_byte_order_conversion = false;

      static auto to_oxm_value(value_type const& v) noexcept
        -> oxm_value_type
      { return v.to_bytes(); }
    };

    struct address_v4_type
    {
      using value_type = boost::asio::ip::address_v4;
      using oxm_value_type = value_type::bytes_type;

      static constexpr bool needs_byte_order_conversion = false;

      static auto to_oxm_value(value_type const& v) noexcept
        -> oxm_value_type
      { return v.to_bytes(); }
    };

    struct address_v6_type
    {
      using value_type = boost::asio::ip::address_v6;
      using oxm_value_type = value_type::bytes_type;

      static constexpr bool needs_byte_order_conversion = false;

      static auto to_oxm_value(value_type const& v) noexcept
        -> oxm_value_type
      { return v.to_bytes(); }
    };

    template <protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField>
    struct oxm_type
    {
    };

    template <std::uint8_t OXMField>
    using basic_oxm_class
      = oxm_type<protocol::ofp_oxm_class::openflow_basic, OXMField>;

    using protocol::oxm_ofb_match_fields;

    template <oxm_ofb_match_fields OXMField, class T>
    using pair = boost::fusion::pair<basic_oxm_class<OXMField>, T>;

    using boost::endian::big_uint24_t;

    using oxm_type_info_table = boost::fusion::map<
        pair<oxm_ofb_match_fields::in_port        , generic_type<std::uint32_t>>
      , pair<oxm_ofb_match_fields::in_phy_port    , generic_type<std::uint32_t>>
      , pair<oxm_ofb_match_fields::metadata       , generic_type<std::uint64_t>>
      , pair<oxm_ofb_match_fields::eth_dst        , mac_address_type>
      , pair<oxm_ofb_match_fields::eth_src        , mac_address_type>
      , pair<oxm_ofb_match_fields::eth_type       , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::vlan_vid       , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::vlan_pcp       , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::ip_dscp        , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::ip_ecn         , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::ip_proto       , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::ipv4_src       , address_v4_type>
      , pair<oxm_ofb_match_fields::ipv4_dst       , address_v4_type>
      , pair<oxm_ofb_match_fields::tcp_src        , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::tcp_dst        , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::udp_src        , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::udp_dst        , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::sctp_src       , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::sctp_dst       , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::icmpv4_type    , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::icmpv4_code    , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::arp_op         , generic_type<std::uint16_t>>
      , pair<oxm_ofb_match_fields::arp_spa        , address_v4_type>
      , pair<oxm_ofb_match_fields::arp_tpa        , address_v4_type>
      , pair<oxm_ofb_match_fields::arp_sha        , mac_address_type>
      , pair<oxm_ofb_match_fields::arp_tha        , mac_address_type>
      , pair<oxm_ofb_match_fields::ipv6_src       , address_v6_type>
      , pair<oxm_ofb_match_fields::ipv6_dst       , address_v6_type>
      , pair<oxm_ofb_match_fields::ipv6_flabel    , generic_type<std::uint32_t>>
      , pair<oxm_ofb_match_fields::icmpv6_type    , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::icmpv6_code    , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::ipv6_nd_target , address_v6_type>
      , pair<oxm_ofb_match_fields::ipv6_nd_sll    , mac_address_type>
      , pair<oxm_ofb_match_fields::ipv6_nd_tll    , mac_address_type>
      , pair<oxm_ofb_match_fields::mpls_label     , generic_type<std::uint32_t>>
      , pair<oxm_ofb_match_fields::mpls_tc        , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::mpls_bos       , generic_type<std::uint8_t>>
      , pair<oxm_ofb_match_fields::pbb_isid       , generic_type<big_uint24_t, false>>
      , pair<oxm_ofb_match_fields::tunnel_id      , generic_type<std::uint64_t>>
      , pair<oxm_ofb_match_fields::ipv6_exthdr    , generic_type<std::uint16_t>>
    >;

    template <class T, class OXMTypeInfo>
    void validate_value(T const&, OXMTypeInfo)
    {
    }

    inline void validate_value(
          std::uint32_t const port_no
        , basic_oxm_class<oxm_ofb_match_fields::in_port>)
    {
      if (port_no == 0) {
        throw std::runtime_error{"port no zero is invalid"};
      }
      if (port_no > protocol::OFPP_MAX
          && port_no != protocol::OFPP_LOCAL
          && port_no != protocol::OFPP_CONTROLLER) {
        throw std::runtime_error{"invalid port no"};
      }
    }

    inline void validate_value(
          std::uint32_t const port_no
        , basic_oxm_class<oxm_ofb_match_fields::in_phy_port>)
    {
      if (port_no == 0) {
        throw std::runtime_error{"in physical port no zero is invalid"};
      }
      if (port_no > protocol::OFPP_MAX) {
        throw std::runtime_error{"invalid port no"};
      }
    }

    inline void validate_value(
          std::uint16_t const vid
        , basic_oxm_class<oxm_ofb_match_fields::vlan_vid>)
    {
      if (vid > (0x0fff | protocol::OFPVID_PRESENT)) {
        throw std::runtime_error{"invalid vlan vid"};
      }
      if (vid != protocol::OFPVID_NONE && !(vid & protocol::OFPVID_PRESENT)) {
        throw std::runtime_error{"no OFPVID_PRESENT"};
      }
    }

    inline void validate_value(
          std::uint8_t const pcp
        , basic_oxm_class<oxm_ofb_match_fields::vlan_pcp>)
    {
      if (pcp > 0x07) {
        throw std::runtime_error{"invalid vlan pcp"};
      }
    }

    inline void validate_value(
          std::uint8_t const dscp
        , basic_oxm_class<oxm_ofb_match_fields::ip_dscp>)
    {
      if (dscp > 0x3f) {
        throw std::runtime_error{"invalid ip dscp"};
      }
    }

    inline void validate_value(
          std::uint8_t const ecn
        , basic_oxm_class<oxm_ofb_match_fields::ip_ecn>)
    {
      if (ecn > 0x03) {
        throw std::runtime_error{"invalid ip ecn"};
      }
    }

    inline void validate_value(
          std::uint32_t const flow_label
        , basic_oxm_class<oxm_ofb_match_fields::ipv6_flabel>)
    {
      if (flow_label > 0x000fffff) {
        throw std::runtime_error{"invalid ipv6 flow label"};
      }
    }

    inline void validate_value(
          std::uint32_t const mpls_label
        , basic_oxm_class<oxm_ofb_match_fields::mpls_label>)
    {
      if (mpls_label > 0x000fffff) {
        throw std::runtime_error{"invalid mpls label"};
      }
    }

    inline void validate_value(
          std::uint8_t const mpls_tc
        , basic_oxm_class<oxm_ofb_match_fields::mpls_tc>)
    {
      if (mpls_tc > 0x07) {
        throw std::runtime_error{"invalid mpls tc"};
      }
    }

    inline void validate_value(
          std::uint8_t const mpls_bos
        , basic_oxm_class<oxm_ofb_match_fields::mpls_bos>)
    {
      if (mpls_bos > 0x01) {
        throw std::runtime_error{"invalid mpls bos"};
      }
    }

    inline void validate_value(
          std::uint16_t const ipv6_exthdr_flags
        , basic_oxm_class<oxm_ofb_match_fields::ipv6_exthdr>)
    {
      if (ipv6_exthdr_flags > 0x01ff) {
        throw std::runtime_error{"invalid ipv6 extension header flags"};
      }
    }

    template <protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField>
    using oxm_type_info_t = typename boost::fusion::result_of::value_at_key<
      oxm_type_info_table, oxm_type<OXMClass, OXMField>
    >::type;

  } // namespace oxm_match_detail


  template <
      protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField
    , class TypeInfo = oxm_match_detail::oxm_type_info_t<OXMClass, OXMField>
  >
  class oxm_match_field;

  namespace oxm_match_detail {

    template <protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField>
    using oxm_match_field_base_t = detail::v13::basic_oxm_match_field<
      oxm_match_field<OXMClass, OXMField>, oxm_type_info_t<OXMClass, OXMField>
    >;

  } // namespace oxm_match_detail


  template <
    protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField, class TypeInfo
  >
  class oxm_match_field
    : public oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>
  {
    using base_t = oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>;

  public:
    using value_type = typename base_t::value_type;

    explicit oxm_match_field(value_type const value) noexcept
      : base_t{value}
    {
    }

    oxm_match_field(value_type const value, value_type const mask)
      : base_t{value, mask}
    {
    }

  private:
    friend base_t;

    static constexpr protocol::ofp_oxm_class oxm_class_value = OXMClass;
    static constexpr std::uint8_t oxm_field_value = OXMField;

    static void validate_value(value_type const& value)
    {
      oxm_match_detail::validate_value(
          value, oxm_match_detail::oxm_type<OXMClass, OXMField>{});
    }
  };

  template <protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField>
  class oxm_match_field<OXMClass, OXMField, oxm_match_detail::mac_address_type>
    : public oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>
  {
    using base_t = oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>;

  public:
    using value_type = typename base_t::value_type;

    explicit oxm_match_field(value_type const& value) noexcept
      : base_t{value}
    {
    }

    oxm_match_field(value_type const& value, value_type const& mask)
      : base_t{value, mask}
    {
    }

  private:
    friend base_t;

    using oxm_value_type = typename base_t::oxm_value_type;

    static constexpr protocol::ofp_oxm_class oxm_class_value = OXMClass;
    static constexpr std::uint8_t oxm_field_value = OXMField;

    explicit oxm_match_field(oxm_value_type const& value)
      : base_t{value_type{value}}
    {
    }

    oxm_match_field(oxm_value_type const& value, oxm_value_type const& mask)
      : base_t{value_type{value}, value_type{mask}}
    {
    }

    static void validate_value(value_type const& value)
    {
    }
  };

  template <protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField>
  class oxm_match_field<OXMClass, OXMField, oxm_match_detail::address_v4_type>
    : public oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>
  {
    using base_t = oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>;

  public:
    using value_type = typename base_t::value_type;

    explicit oxm_match_field(value_type const& value)
      : base_t{value}
    {
    }

    oxm_match_field(value_type const& value, value_type const& mask)
      : base_t{value, mask}
    {
    }

    explicit oxm_match_field(boost::asio::ip::address const& value)
      : oxm_match_field{value.to_v4()}
    {
    }

    oxm_match_field(
          boost::asio::ip::address const& value
        , boost::asio::ip::address const& mask)
      : oxm_match_field{value.to_v4(), mask.to_v4()}
    {
    }

    oxm_match_field(value_type const& value, std::uint8_t const prefix_length)
      : base_t{value, create_ipv4_addr(prefix_length)}
    {
    }

    oxm_match_field(
        boost::asio::ip::address const& value, std::uint8_t const prefix_length)
      : oxm_match_field{value.to_v4(), prefix_length}
    {
    }

  private:
    friend base_t;

    using oxm_value_type = typename base_t::oxm_value_type;

    static constexpr protocol::ofp_oxm_class oxm_class_value = OXMClass;
    static constexpr std::uint8_t oxm_field_value = OXMField;

    explicit oxm_match_field(oxm_value_type const value)
      : base_t{value_type{value}}
    {
    }

    oxm_match_field(oxm_value_type const value, oxm_value_type const mask)
      : base_t{value_type{value}, value_type{mask}}
    {
    }

    static void validate_value(value_type const&)
    {
    }

    static auto create_ipv4_addr(std::uint8_t const prefix_length)
      -> value_type
    {
      return value_type{
        std::uint32_t(std::uint64_t{0xffffffff} << (32 - prefix_length))
      };
    }
  };

  template <protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField>
  class oxm_match_field<OXMClass, OXMField, oxm_match_detail::address_v6_type>
    : public oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>
  {
    using base_t = oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>;

  public:
    using value_type = typename base_t::value_type;

    explicit oxm_match_field(value_type const& value)
      : base_t{value}
    {
    }

    oxm_match_field(value_type const& value, value_type const& mask)
      : base_t{value, mask}
    {
    }

    explicit oxm_match_field(boost::asio::ip::address const& value)
      : oxm_match_field{value.to_v6()}
    {
    }

    oxm_match_field(
          boost::asio::ip::address const& value
        , boost::asio::ip::address const& mask)
      : oxm_match_field{value.to_v6(), mask.to_v6()}
    {
    }

    oxm_match_field(value_type const& value, std::uint8_t const prefix_length)
      : base_t{value, create_ipv6_addr(prefix_length)}
    {
    }

    oxm_match_field(
        boost::asio::ip::address const& value, std::uint8_t const prefix_length)
      : oxm_match_field{value.to_v6(), prefix_length}
    {
    }

  private:
    friend base_t;

    using oxm_value_type = typename base_t::oxm_value_type;

    static constexpr protocol::ofp_oxm_class oxm_class_value = OXMClass;
    static constexpr std::uint8_t oxm_field_value = OXMField;

    explicit oxm_match_field(oxm_value_type const value)
      : base_t{value_type{value}}
    {
    }

    oxm_match_field(oxm_value_type const value, oxm_value_type const mask)
      : base_t{value_type{value}, value_type{mask}}
    {
    }

    static void validate_value(value_type const&)
    {
    }

    static auto create_ipv6_addr(std::uint8_t prefix_length)
      -> value_type
    {
      auto bytes = oxm_value_type{};
      for (std::uint8_t& byte : bytes) {
        if (prefix_length < 8) {
          byte = (std::uint16_t{0xff} << (8 - prefix_length));
          break;
        }
        byte = 0xff;
        prefix_length -= 8;
      }
      return value_type{bytes};
    }
  };

  template <protocol::oxm_ofb_match_fields OXMField>
  using openflow_basic_match_field
    = oxm_match_field<protocol::ofp_oxm_class::openflow_basic, OXMField>;

  using in_port        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::in_port>;
  using in_phy_port    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::in_phy_port>;
  using metadata       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::metadata>;
  using eth_dst        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::eth_dst>;
  using eth_src        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::eth_src>;
  using eth_type       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::eth_type>;
  using vlan_vid       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::vlan_vid>;
  using vlan_pcp       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::vlan_pcp>;
  using ip_dscp        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ip_dscp>;
  using ip_ecn         = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ip_ecn>;
  using ip_proto       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ip_proto>;
  using ipv4_src       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv4_src>;
  using ipv4_dst       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv4_dst>;
  using tcp_src        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::tcp_src>;
  using tcp_dst        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::tcp_dst>;
  using udp_src        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::udp_src>;
  using udp_dst        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::udp_dst>;
  using sctp_src       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::sctp_src>;
  using sctp_dst       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::sctp_dst>;
  using icmpv4_type    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::icmpv4_type>;
  using icmpv4_code    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::icmpv4_code>;
  using arp_op         = openflow_basic_match_field<protocol::oxm_ofb_match_fields::arp_op>;
  using arp_spa        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::arp_spa>;
  using arp_tpa        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::arp_tpa>;
  using arp_sha        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::arp_sha>;
  using arp_tha        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::arp_tha>;
  using ipv6_src       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv6_src>;
  using ipv6_dst       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv6_dst>;
  using ipv6_flabel    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv6_flabel>;
  using icmpv6_type    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::icmpv6_type>;
  using icmpv6_code    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::icmpv6_code>;
  using ipv6_nd_target = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv6_nd_target>;
  using ipv6_nd_sll    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv6_nd_sll>;
  using ipv6_nd_tll    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv6_nd_tll>;
  using mpls_label     = openflow_basic_match_field<protocol::oxm_ofb_match_fields::mpls_label>;
  using mpls_tc        = openflow_basic_match_field<protocol::oxm_ofb_match_fields::mpls_tc>;
  using mpls_bos       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::mpls_bos>;
  using pbb_isid       = openflow_basic_match_field<protocol::oxm_ofb_match_fields::pbb_isid>;
  using tunnel_id      = openflow_basic_match_field<protocol::oxm_ofb_match_fields::tunnel_id>;
  using ipv6_exthdr    = openflow_basic_match_field<protocol::oxm_ofb_match_fields::ipv6_exthdr>;

} // namespace oxm_match_fields

using default_oxm_match_field_list = std::tuple<
    oxm_match_fields::in_port
  , oxm_match_fields::in_phy_port
  , oxm_match_fields::metadata
  , oxm_match_fields::eth_dst
  , oxm_match_fields::eth_src
  , oxm_match_fields::eth_type
  , oxm_match_fields::vlan_vid
  , oxm_match_fields::vlan_pcp
  , oxm_match_fields::ip_dscp
  , oxm_match_fields::ip_ecn
  , oxm_match_fields::ip_proto
  , oxm_match_fields::ipv4_src
  , oxm_match_fields::ipv4_dst
  , oxm_match_fields::tcp_src
  , oxm_match_fields::tcp_dst
  , oxm_match_fields::udp_src
  , oxm_match_fields::udp_dst
  , oxm_match_fields::sctp_src
  , oxm_match_fields::sctp_dst
  , oxm_match_fields::icmpv4_type
  , oxm_match_fields::icmpv4_code
  , oxm_match_fields::arp_op
  , oxm_match_fields::arp_spa
  , oxm_match_fields::arp_tpa
  , oxm_match_fields::arp_sha
  , oxm_match_fields::arp_tha
  , oxm_match_fields::ipv6_src
  , oxm_match_fields::ipv6_dst
  , oxm_match_fields::ipv6_flabel
  , oxm_match_fields::icmpv6_type
  , oxm_match_fields::icmpv6_code
  , oxm_match_fields::ipv6_nd_target
  , oxm_match_fields::ipv6_nd_sll
  , oxm_match_fields::ipv6_nd_tll
  , oxm_match_fields::mpls_label
  , oxm_match_fields::mpls_tc
  , oxm_match_fields::mpls_bos
  , oxm_match_fields::pbb_isid
  , oxm_match_fields::tunnel_id
  , oxm_match_fields::ipv6_exthdr
>;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OXM_MATCH_FIELD_HPP
