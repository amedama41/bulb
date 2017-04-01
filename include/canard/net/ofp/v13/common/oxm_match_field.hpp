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
            = oxm_type<protocol::OFPXMC_OPENFLOW_BASIC, OXMField>;

        using boost::fusion::pair;
        using boost::endian::big_uint24_t;

        using oxm_type_info_table = boost::fusion::map<
              pair<basic_oxm_class<protocol::OFPXMT_OFB_IN_PORT>        , generic_type<std::uint32_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IN_PHY_PORT>    , generic_type<std::uint32_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_METADATA>       , generic_type<std::uint64_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ETH_DST>        , mac_address_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ETH_SRC>        , mac_address_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ETH_TYPE>       , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_VLAN_VID>       , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_VLAN_PCP>       , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IP_DSCP>        , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IP_ECN>         , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IP_PROTO>       , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV4_SRC>       , address_v4_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV4_DST>       , address_v4_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_TCP_SRC>        , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_TCP_DST>        , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_UDP_SRC>        , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_UDP_DST>        , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_SCTP_SRC>       , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_SCTP_DST>       , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ICMPV4_TYPE>    , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ICMPV4_CODE>    , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ARP_OP>         , generic_type<std::uint16_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ARP_SPA>        , address_v4_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ARP_TPA>        , address_v4_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ARP_SHA>        , mac_address_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ARP_THA>        , mac_address_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV6_SRC>       , address_v6_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV6_DST>       , address_v6_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV6_FLABEL>    , generic_type<std::uint32_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ICMPV6_TYPE>    , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_ICMPV6_CODE>    , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV6_ND_TARGET> , address_v6_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV6_ND_SLL>    , mac_address_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV6_ND_TLL>    , mac_address_type>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_MPLS_LABEL>     , generic_type<std::uint32_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_MPLS_TC>        , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_MPLS_BOS>       , generic_type<std::uint8_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_PBB_ISID>       , generic_type<big_uint24_t, false>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_TUNNEL_ID>      , generic_type<std::uint64_t>>
            , pair<basic_oxm_class<protocol::OFPXMT_OFB_IPV6_EXTHDR>    , generic_type<std::uint16_t>>
        >;

        template <class T, class OXMTypeInfo>
        void validate_value(T const&, OXMTypeInfo)
        {
        }

        inline void validate_value(
                  std::uint32_t const port_no
                , basic_oxm_class<protocol::OFPXMT_OFB_IN_PORT>)
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
                , basic_oxm_class<protocol::OFPXMT_OFB_IN_PHY_PORT>)
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
                , basic_oxm_class<protocol::OFPXMT_OFB_VLAN_VID>)
        {
            if (vid > (0x0fff | protocol::OFPVID_PRESENT)) {
                throw std::runtime_error{"invalid vlan vid"};
            }
            if (vid != protocol::OFPVID_NONE
                    && !(vid & protocol::OFPVID_PRESENT)) {
                throw std::runtime_error{"no OFPVID_PRESENT"};
            }
        }

        inline void validate_value(
                  std::uint8_t const pcp
                , basic_oxm_class<protocol::OFPXMT_OFB_VLAN_PCP>)
        {
            if (pcp > 0x07) {
                throw std::runtime_error{"invalid vlan pcp"};
            }
        }

        inline void validate_value(
                  std::uint8_t const dscp
                , basic_oxm_class<protocol::OFPXMT_OFB_IP_DSCP>)
        {
            if (dscp > 0x3f) {
                throw std::runtime_error{"invalid ip dscp"};
            }
        }

        inline void validate_value(
                  std::uint8_t const ecn
                , basic_oxm_class<protocol::OFPXMT_OFB_IP_ECN>)
        {
            if (ecn > 0x03) {
                throw std::runtime_error{"invalid ip ecn"};
            }
        }

        inline void validate_value(
                  std::uint32_t const flow_label
                , basic_oxm_class<protocol::OFPXMT_OFB_IPV6_FLABEL>)
        {
            if (flow_label > 0x000fffff) {
                throw std::runtime_error{"invalid ipv6 flow label"};
            }
        }

        inline void validate_value(
                  std::uint32_t const mpls_label
                , basic_oxm_class<protocol::OFPXMT_OFB_MPLS_LABEL>)
        {
            if (mpls_label > 0x000fffff) {
                throw std::runtime_error{"invalid mpls label"};
            }
        }

        inline void validate_value(
                  std::uint8_t const mpls_tc
                , basic_oxm_class<protocol::OFPXMT_OFB_MPLS_TC>)
        {
            if (mpls_tc > 0x07) {
                throw std::runtime_error{"invalid mpls tc"};
            }
        }

        inline void validate_value(
                  std::uint8_t const mpls_bos
                , basic_oxm_class<protocol::OFPXMT_OFB_MPLS_BOS>)
        {
            if (mpls_bos > 0x01) {
                throw std::runtime_error{"invalid mpls bos"};
            }
        }

        inline void validate_value(
                  std::uint16_t const ipv6_exthdr_flags
                , basic_oxm_class<protocol::OFPXMT_OFB_IPV6_EXTHDR>)
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
              oxm_match_field<OXMClass, OXMField>
            , oxm_type_info_t<OXMClass, OXMField>
        >;

    } // namespace oxm_match_detail


    template <
        protocol::ofp_oxm_class OXMClass, std::uint8_t OXMField, class TypeInfo
    >
    class oxm_match_field
        : public oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>
    {
        using base_t
            = oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>;

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
        using base_t
            = oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>;

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
        using base_t
            = oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>;

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

        oxm_match_field(
                value_type const& value, std::uint8_t const prefix_length)
            : base_t{value, create_ipv4_addr(prefix_length)}
        {
        }

        oxm_match_field(
                  boost::asio::ip::address const& value
                , std::uint8_t const prefix_length)
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
        using base_t
            = oxm_match_detail::oxm_match_field_base_t<OXMClass, OXMField>;

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

        oxm_match_field(
                value_type const& value, std::uint8_t const prefix_length)
            : base_t{value, create_ipv6_addr(prefix_length)}
        {
        }

        oxm_match_field(
                  boost::asio::ip::address const& value
                , std::uint8_t const prefix_length)
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

    using in_port        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IN_PORT>;
    using in_phy_port    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IN_PHY_PORT>;
    using metadata       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_METADATA>;
    using eth_dst        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ETH_DST>;
    using eth_src        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ETH_SRC>;
    using eth_type       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ETH_TYPE>;
    using vlan_vid       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_VLAN_VID>;
    using vlan_pcp       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_VLAN_PCP>;
    using ip_dscp        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IP_DSCP>;
    using ip_ecn         = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IP_ECN>;
    using ip_proto       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IP_PROTO>;
    using ipv4_src       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV4_SRC>;
    using ipv4_dst       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV4_DST>;
    using tcp_src        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_TCP_SRC>;
    using tcp_dst        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_TCP_DST>;
    using udp_src        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_UDP_SRC>;
    using udp_dst        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_UDP_DST>;
    using sctp_src       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_SCTP_SRC>;
    using sctp_dst       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_SCTP_DST>;
    using icmpv4_type    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ICMPV4_TYPE>;
    using icmpv4_code    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ICMPV4_CODE>;
    using arp_op         = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ARP_OP>;
    using arp_spa        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ARP_SPA>;
    using arp_tpa        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ARP_TPA>;
    using arp_sha        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ARP_SHA>;
    using arp_tha        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ARP_THA>;
    using ipv6_src       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV6_SRC>;
    using ipv6_dst       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV6_DST>;
    using ipv6_flabel    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV6_FLABEL>;
    using icmpv6_type    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ICMPV6_TYPE>;
    using icmpv6_code    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_ICMPV6_CODE>;
    using ipv6_nd_target = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV6_ND_TARGET>;
    using ipv6_nd_sll    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV6_ND_SLL>;
    using ipv6_nd_tll    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV6_ND_TLL>;
    using mpls_label     = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_MPLS_LABEL>;
    using mpls_tc        = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_MPLS_TC>;
    using mpls_bos       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_MPLS_BOS>;
    using pbb_isid       = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_PBB_ISID>;
    using tunnel_id      = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_TUNNEL_ID>;
    using ipv6_exthdr    = oxm_match_field<protocol::OFPXMC_OPENFLOW_BASIC, protocol::OFPXMT_OFB_IPV6_EXTHDR>;

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
