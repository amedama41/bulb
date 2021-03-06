#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/common/match.hpp>
#include <boost/test/unit_test.hpp>

#include <limits>
#include <boost/asio/ip/address_v4.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v10 = of::v10;
namespace fields = v10::match_fields;

namespace proto = v10::protocol;

namespace {

    struct match_field_fixture
    {
        using mac_address = canard::mac_address;
        using address_v4 = boost::asio::ip::address_v4;

        fields::in_port const in_port{proto::OFPP_MAX};
        fields::eth_src const eth_src{
            mac_address{{0x01, 0x02, 0x03, 0x04, 0x05, 0x06}}
        };
        fields::eth_dst const eth_dst{
            mac_address{{0x11, 0x12, 0x13, 0x14, 0x15, 0x16}}
        };
        fields::vlan_vid const vlan_vid{0x0fff};
        fields::vlan_pcp const vlan_pcp{0x07};
        fields::eth_type const eth_type{0x8100};
        fields::ip_dscp const ip_dscp{0x3f};
        fields::ip_proto const ip_proto{6};
        fields::ipv4_src const ipv4_src{
            address_v4::from_string("192.168.1.0"), 24
        };
        fields::ipv4_dst const ipv4_dst{
            address_v4::from_string("127.0.0.1"), 32
        };
        fields::tcp_src const tcp_src{6633};
        fields::tcp_dst const tcp_dst{6653};
    };

    struct match_set_fixture : match_field_fixture
    {
        v10::match sut{
              in_port, eth_src, eth_dst, vlan_vid, vlan_pcp, eth_type
            , ip_dscp, ip_proto, ipv4_src, ipv4_dst, tcp_src, tcp_dst
        };
    };
}

BOOST_AUTO_TEST_SUITE(common_types_test)
BOOST_AUTO_TEST_SUITE(match_set_test)

    BOOST_AUTO_TEST_CASE(default_construct_test)
    {
        auto const sut = v10::match{};

        BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_match));
        BOOST_TEST(sut.empty());
        BOOST_TEST(sut.wildcards() == proto::OFPFW_ALL);
        BOOST_TEST((sut.get<fields::in_port>() == boost::none));
    }

    BOOST_AUTO_TEST_CASE(construct_from_single_field_test)
    {
        auto const in_port = fields::in_port{1};

        auto const sut = v10::match{in_port};

        BOOST_TEST(!sut.empty());
        BOOST_TEST(sut.ofp_match().in_port == in_port.value());
        BOOST_TEST(~sut.wildcards() & proto::OFPFW_IN_PORT);
        BOOST_TEST((sut.get<fields::in_port>() == in_port));
    }

    BOOST_AUTO_TEST_CASE(construct_from_multiple_different_fields_test)
    {
        auto const in_port = fields::in_port{1};
        auto const eth_src = fields::eth_src{
            canard::mac_address{{0x01, 0x02, 0x03, 0x04, 0x05, 0x06}}
        };
        auto const vlan_vid = fields::vlan_vid{0x0fff};
        auto const ipv4_dst = fields::ipv4_dst{
            boost::asio::ip::address_v4::from_string("172.16.0.0"), 16
        };
        auto const icmpv4_type = fields::icmpv4_type{8};

        auto const sut = v10::match{
            in_port, eth_src, vlan_vid, ipv4_dst, icmpv4_type
        };

        BOOST_TEST(!sut.empty());

        BOOST_TEST(sut.ofp_match().in_port == in_port.value());
        BOOST_TEST(~sut.wildcards() & proto::OFPFW_IN_PORT);
        BOOST_TEST((sut.get<fields::in_port>() == in_port));

        BOOST_TEST(canard::mac_address{sut.ofp_match().dl_src} == eth_src.value());
        BOOST_TEST(~sut.wildcards() & proto::OFPFW_DL_SRC);
        BOOST_TEST((sut.get<fields::eth_src>() == eth_src));

        BOOST_TEST(sut.ofp_match().dl_vlan == vlan_vid.value());
        BOOST_TEST(~sut.wildcards() & proto::OFPFW_DL_VLAN);
        BOOST_TEST((sut.get<fields::vlan_vid>() == vlan_vid));

        BOOST_TEST(sut.ofp_match().nw_dst == ipv4_dst.value().to_ulong());
        BOOST_TEST((sut.get<fields::ipv4_dst>() == ipv4_dst));
        BOOST_TEST(((sut.wildcards() & proto::OFPFW_NW_DST_MASK)
                    >> proto::OFPFW_NW_DST_SHIFT)
                == ipv4_dst.wildcard_bit_count());

        BOOST_TEST(sut.ofp_match().tp_src == icmpv4_type.value());
        BOOST_TEST(~sut.wildcards() & proto::OFPFW_TP_SRC);
        BOOST_TEST((sut.get<fields::icmpv4_type>() == icmpv4_type));
    }

    BOOST_AUTO_TEST_CASE(construct_from_multiple_same_fields_test)
    {
        using boost::asio::ip::address_v4;
        auto const ipv4_src1
            = fields::ipv4_src{address_v4::from_string("172.16.0.0"), 16};
        auto const ipv4_src2
            = fields::ipv4_src{address_v4::from_string("192.168.1.0"), 24};
        auto const tcp_dst1 = fields::tcp_dst{11};
        auto const tcp_dst2 = fields::tcp_dst{21};

        auto const sut = v10::match{
            ipv4_src1, tcp_dst1, tcp_dst2, ipv4_src2
        };

        BOOST_TEST(sut.ofp_match().nw_src == ipv4_src2.value().to_ulong());
        BOOST_TEST(((sut.wildcards() & proto::OFPFW_NW_SRC_MASK)
                    >> proto::OFPFW_NW_SRC_SHIFT)
                == ipv4_src2.wildcard_bit_count());
        BOOST_TEST((sut.get<fields::ipv4_src>() == ipv4_src2));

        BOOST_TEST(sut.ofp_match().tp_dst == tcp_dst2.value());
        BOOST_TEST(~sut.wildcards() & proto::OFPFW_TP_DST);
        BOOST_TEST((sut.get<fields::tcp_dst>() == tcp_dst2));
    }

    BOOST_AUTO_TEST_CASE(construct_from_dscp_field_test)
    {
        auto const ip_dscp = fields::ip_dscp{0x11};

        auto const sut = v10::match{ip_dscp};

        BOOST_TEST(~sut.wildcards() & proto::OFPFW_NW_TOS);
        BOOST_TEST(sut.ofp_match().nw_tos == (ip_dscp.value() << 2));
        BOOST_TEST((sut.get<fields::ip_dscp>() == ip_dscp));
    }

    BOOST_AUTO_TEST_CASE(construct_from_invalid_fields_test)
    {
        auto const in_port = fields::in_port{1};
        auto const vlan_vid = fields::vlan_vid{0xffff};
        auto const vlan_pcp = fields::vlan_pcp{0xff};

        auto const sut = v10::match{
            in_port, vlan_vid, vlan_pcp
        };

        BOOST_TEST((sut.get<fields::in_port>() == in_port));
        BOOST_TEST((sut.get<fields::vlan_vid>() == vlan_vid));
        BOOST_TEST((sut.get<fields::vlan_pcp>() == vlan_pcp));
    }

    BOOST_AUTO_TEST_CASE(construct_from_ofp_match_test)
    {
        auto ofp_match = v10::protocol::ofp_match{};
        ofp_match.wildcards = proto::OFPFW_ALL;
        ofp_match.wildcards &= ~proto::OFPFW_IN_PORT;
        ofp_match.in_port = proto::OFPP_ANY;
        ofp_match.wildcards &= ~proto::OFPFW_DL_VLAN;
        ofp_match.dl_vlan = 0xfffe;
        ofp_match.wildcards &= ~proto::OFPFW_DL_VLAN_PCP;
        ofp_match.dl_vlan_pcp = 0xff;
        ofp_match.wildcards &= ~proto::OFPFW_NW_TOS;
        ofp_match.nw_tos = 0xff;

        auto const sut = v10::match{ofp_match};

        BOOST_TEST((sut.get<fields::in_port>() != boost::none));
        BOOST_TEST(sut.get<fields::in_port>()->value() == ofp_match.in_port);
        BOOST_TEST((sut.get<fields::vlan_vid>() != boost::none));
        BOOST_TEST(sut.get<fields::vlan_vid>()->value() == ofp_match.dl_vlan);
        BOOST_TEST((sut.get<fields::vlan_pcp>() != boost::none));
        BOOST_TEST(sut.get<fields::vlan_pcp>()->value() == ofp_match.dl_vlan_pcp);
        BOOST_TEST((sut.get<fields::ip_dscp>() != boost::none));
        BOOST_TEST(sut.get<fields::ip_dscp>()->value() == (ofp_match.nw_tos >> 2));
        BOOST_TEST((sut.get<fields::ipv4_src>() == boost::none));
        BOOST_TEST((sut.get<fields::ipv4_dst>() == boost::none));
    }

    BOOST_AUTO_TEST_CASE(set_field_test)
    {
        auto const eth_type = fields::eth_type{0x0800};
        auto sut = v10::match{};

        sut.set(eth_type);

        BOOST_TEST(~sut.wildcards() & proto::OFPFW_DL_TYPE);
        BOOST_TEST(sut.ofp_match().dl_type == eth_type.value());
        BOOST_TEST((sut.get<fields::eth_type>() == eth_type));
    }

    BOOST_FIXTURE_TEST_CASE(set_existed_field_test, match_set_fixture)
    {
        auto const in_port = fields::in_port{1};

        sut.set(in_port);

        BOOST_TEST(~sut.wildcards() & proto::OFPFW_IN_PORT);
        BOOST_TEST(sut.ofp_match().in_port == in_port.value());
        BOOST_TEST((sut.get<fields::in_port>() == in_port));
    }

    BOOST_FIXTURE_TEST_CASE(set_all_mask_ip_addr_field_test, match_set_fixture)
    {
        auto const ipv4_src
            = fields::ipv4_src{address_v4::from_string("0.0.0.0"), 0};

        sut.set(ipv4_src);

        BOOST_TEST(sut.wildcards() & proto::OFPFW_NW_SRC_ALL);
        BOOST_TEST((sut.get<fields::ipv4_src>() == boost::none));
    }

    BOOST_FIXTURE_TEST_CASE(erase_test, match_set_fixture)
    {
        sut.erase<fields::in_port>();

        BOOST_TEST(sut.wildcards() & proto::OFPFW_IN_PORT);
        BOOST_TEST(sut.ofp_match().in_port == 0);
        BOOST_TEST((sut.get<fields::in_port>() == boost::none));
    }

    BOOST_FIXTURE_TEST_CASE(erase_ip_addr_test, match_set_fixture)
    {
        sut.erase<fields::ipv4_src>();

        BOOST_TEST(sut.wildcards() & proto::OFPFW_NW_SRC_ALL);
        BOOST_TEST(sut.ofp_match().nw_src == 0);
        BOOST_TEST((sut.get<fields::ipv4_src>() == boost::none));
    }

    BOOST_FIXTURE_TEST_SUITE(equality, match_field_fixture)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = v10::match{
              in_port, eth_src, eth_dst, vlan_vid, vlan_pcp, eth_type
            , ip_dscp, ip_proto, ipv4_src, ipv4_dst, tcp_src, tcp_dst
        };

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_both_are_empty_set)
      {
        BOOST_TEST((v10::match{} == v10::match{}));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_fields_are_equal)
      {
        BOOST_TEST(
            (v10::match{
                 in_port, eth_src, eth_dst, vlan_vid, vlan_pcp, eth_type
               , ip_dscp, ip_proto, ipv4_src, ipv4_dst, tcp_src, tcp_dst
             }
          == v10::match{
                 in_port, eth_src, eth_dst, vlan_vid, vlan_pcp, eth_type
               , ip_dscp, ip_proto, ipv4_src, ipv4_dst, tcp_src, tcp_dst
             }));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_field_is_not_equal)
      {
        BOOST_TEST(
            (v10::match{fields::in_port{1}}
          != v10::match{fields::in_port{2}}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_field_is_not_equal_but_equivalent)
      {
        BOOST_TEST(
            (v10::match{fields::ipv4_src{"10.0.0.2"_ipv4, 8}}
          != v10::match{fields::ipv4_src{"10.0.0.1"_ipv4, 8}}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_size_is_not_equal)
      {
        BOOST_TEST(
            (v10::match{in_port, ip_proto} != v10::match{in_port}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_wildcards_is_not_equal_but_equivalent)
      {
        auto match = v10::protocol::ofp_match{};
        match.wildcards = proto::OFPFW_ALL;
        auto const sut1 = v10::match{match};
        match.wildcards = std::numeric_limits<std::uint32_t>::max();
        auto const sut2 = v10::match{match};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_padding_is_not_equal)
      {
        auto match = v10::protocol::ofp_match{};
        auto const sut1 = v10::match{match};
        match.pad1[0] = 0xff;
        auto const sut2 = v10::match{match};

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_FIXTURE_TEST_SUITE(function_equivalent, match_field_fixture)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = v10::match{
              in_port, eth_src, eth_dst, vlan_vid, vlan_pcp, eth_type
            , ip_dscp, ip_proto, ipv4_src, ipv4_dst, tcp_src, tcp_dst
        };

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_both_are_empty_set)
      {
        BOOST_TEST(equivalent(v10::match{}, v10::match{}));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_fields_are_equal)
      {
        BOOST_TEST(
            equivalent(
                v10::match{
                    in_port, eth_src, eth_dst, vlan_vid, vlan_pcp, eth_type
                  , ip_dscp, ip_proto, ipv4_src, ipv4_dst, tcp_src, tcp_dst
                }
              , v10::match{
                    in_port, eth_src, eth_dst, vlan_vid, vlan_pcp, eth_type
                  , ip_dscp, ip_proto, ipv4_src, ipv4_dst, tcp_src, tcp_dst
                }));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_field_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                v10::match{fields::in_port{1}}
              , v10::match{fields::in_port{2}}));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_field_is_not_equal_but_equivalent)
      {
        BOOST_TEST(
            equivalent(
                v10::match{fields::ipv4_src{"10.0.0.2"_ipv4, 8}}
              , v10::match{fields::ipv4_src{"10.0.0.1"_ipv4, 8}}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_size_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
              v10::match{in_port, ip_proto}, v10::match{in_port}));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_wildcards_is_not_equal_but_equivalent)
      {
        auto match = v10::protocol::ofp_match{};
        match.wildcards = proto::OFPFW_ALL;
        auto const sut1 = v10::match{match};
        match.wildcards = std::numeric_limits<std::uint32_t>::max();
        auto const sut2 = v10::match{match};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_padding_is_not_equal)
      {
        auto match = v10::protocol::ofp_match{};
        auto const sut1 = v10::match{match};
        match.pad1[0] = 0xff;
        auto const sut2 = v10::match{match};

        BOOST_TEST(equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

BOOST_AUTO_TEST_SUITE_END() // match_set_test
BOOST_AUTO_TEST_SUITE_END() // common_types_test
