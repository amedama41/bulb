#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/action/set_field.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>
#include <boost/asio/ip/address_v4.hpp>
#include <canard/mac_address.hpp>
#include <canard/network/openflow/v10/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v10 = of::v10;
namespace actions = v10::actions;

namespace bdata = boost::unit_test::data;

namespace {

struct set_eth_src_fixture
{
    actions::set_eth_src sut{
        canard::mac_address{{0x01, 0x02, 0x03, 0x04, 0x05, 0x06}}
    };
    std::vector<std::uint8_t> binary
        = "\x00\x04\x00\x10\x01\x02\x03\x04""\x05\x06\x00\x00\x00\x00\x00\x00"
          ""_bin;
    std::vector<std::uint8_t> non_zero_padding_binary
        = "\x00\x04\x00\x10\x01\x02\x03\x04""\x05\x06\x00\x00\x00\x00\x00\x01"
          ""_bin;
};

struct set_ipv4_dst_fixture
{
    actions::set_ipv4_dst sut{
        boost::asio::ip::address_v4::from_string("192.168.1.254")
    };
    std::vector<std::uint8_t> binary
        = "\x00\x07\x00\x08\xc0\xa8\x01\xfe"_bin;
};

struct set_tcp_src_fixture
{
    actions::set_tcp_src sut{0x1234};
    std::vector<std::uint8_t> binary
        = "\x00\x09\x00\x08\x12\x34\x00\x00"_bin;
    std::vector<std::uint8_t> non_zero_padding_binary
        = "\x00\x09\x00\x08\x12\x34\x00\x01"_bin;
};

}

BOOST_AUTO_TEST_SUITE(actions_test)

BOOST_AUTO_TEST_SUITE(set_vlan_vid_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const vid = std::uint16_t{0x0fff};

        auto const sut = actions::set_vlan_vid{vid};

        BOOST_TEST(sut.type() == v10::protocol::OFPAT_SET_VLAN_VID);
        BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_action_vlan_vid));
        BOOST_TEST(sut.value() == vid);
    }

    BOOST_DATA_TEST_CASE(
              create_success_test
            , bdata::make(std::vector<std::uint16_t>{
                0x0000, 0x1000 / 2, 0x0fff, v10::protocol::OFP_VLAN_NONE
              })
            , vid)
    {
        auto const sut = actions::set_vlan_vid::create(vid);

        BOOST_TEST(sut.value() == vid);
    }

    BOOST_DATA_TEST_CASE(
              create_failure_test
            , bdata::make(std::vector<std::uint16_t>{
                0x1000, (0x1000 + 0xffff) / 2, 0xfffe
              })
            , vid)
    {
        BOOST_CHECK_THROW(
                actions::set_vlan_vid::create(vid), std::runtime_error);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_vlan_vid{1};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_vlan_vid_is_equal)
      {
        auto const vid = 0x0123;

        BOOST_TEST((actions::set_vlan_vid{vid} == actions::set_vlan_vid{vid}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_vlan_vid_is_not_equal)
      {
        BOOST_TEST((actions::set_vlan_vid{1} != actions::set_vlan_vid{2}));
      }
      BOOST_AUTO_TEST_CASE(
          is_false_if_vid_lower_12_bits_are_equal_but_other_bits_are_not_equal)
      {
        BOOST_TEST(
            (actions::set_vlan_vid{0x1001} != actions::set_vlan_vid{0x0001}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_padding_is_not_equal)
      {
        auto const non_zero_padding_binary
          = "\x00\x02\x00\x01\x01\x23\x00\x01"_bin;
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_vlan_vid::decode(it, non_zero_padding_binary.end());

        BOOST_TEST((actions::set_vlan_vid{0x0123} != non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_vlan_vid{1};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_vlan_vid_is_equal)
      {
        auto const vid = 0x0123;

        BOOST_TEST(
            equivalent(actions::set_vlan_vid{vid}, actions::set_vlan_vid{vid}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_vlan_vid_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(actions::set_vlan_vid{1}, actions::set_vlan_vid{2}));
      }
      BOOST_AUTO_TEST_CASE(
          is_false_if_vid_lower_12_bits_are_equal_but_other_bits_are_not_equal)
      {
        BOOST_TEST(
            !equivalent(
              actions::set_vlan_vid{0x1001}, actions::set_vlan_vid{0x0001}));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_padding_is_not_equal)
      {
        auto const non_zero_padding_binary
          = "\x00\x02\x00\x01\x01\x23\x00\x01"_bin;
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_vlan_vid::decode(it, non_zero_padding_binary.end());

        BOOST_TEST(equivalent(actions::set_vlan_vid{0x0123}, non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

BOOST_AUTO_TEST_SUITE_END() // set_vlan_vid_test

BOOST_AUTO_TEST_SUITE(set_vlan_pcp_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const pcp = std::uint8_t{0x01};

        auto const sut = actions::set_vlan_pcp{pcp};

        BOOST_TEST(sut.type() == v10::protocol::OFPAT_SET_VLAN_PCP);
        BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_action_vlan_pcp));
        BOOST_TEST(sut.value() == pcp);
    }

    BOOST_DATA_TEST_CASE(create_success_test, bdata::xrange(0x00, 0x08), pcp)
    {
        auto const sut = actions::set_vlan_pcp::create(pcp);

        BOOST_TEST(sut.value() == pcp);
    }

    BOOST_DATA_TEST_CASE(create_failure_test, bdata::xrange(0x08, 0xff), pcp)
    {
        BOOST_CHECK_THROW(
                actions::set_vlan_pcp::create(pcp), std::runtime_error);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_vlan_pcp{0x1};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_vlan_pcp_is_equal)
      {
        auto const pcp = std::uint8_t{0x2};

        BOOST_TEST((actions::set_vlan_pcp{pcp} == actions::set_vlan_pcp{pcp}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_vlan_pcp_is_not_equal)
      {
        BOOST_TEST((actions::set_vlan_pcp{0x0} != actions::set_vlan_pcp{0x1}));
      }
      BOOST_AUTO_TEST_CASE(
          is_false_if_pcp_lower_3_bits_are_equal_but_other_bits_are_not_equal)
      {
        BOOST_TEST((actions::set_vlan_pcp{0x9} != actions::set_vlan_pcp{0x1}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_padding_is_not_equal)
      {
        auto const non_zero_padding_binary
          = "\x00\x03\x00\x08\x03\x00\x00\x01"_bin;
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_vlan_pcp::decode(it, non_zero_padding_binary.end());

        BOOST_TEST((actions::set_vlan_pcp{0x3} != non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_vlan_pcp{0x1};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_vlan_pcp_is_equal)
      {
        auto const pcp = std::uint8_t{0x2};

        BOOST_TEST(
            equivalent(actions::set_vlan_pcp{pcp}, actions::set_vlan_pcp{pcp}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_vlan_pcp_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
              actions::set_vlan_pcp{0x0}, actions::set_vlan_pcp{0x1}));
      }
      BOOST_AUTO_TEST_CASE(
          is_false_if_pcp_lower_3_bits_are_equal_but_other_bits_are_not_equal)
      {
        BOOST_TEST(
            !equivalent(
              actions::set_vlan_pcp{0x9}, actions::set_vlan_pcp{0x1}));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_padding_is_not_equal)
      {
        auto const non_zero_padding_binary
          = "\x00\x03\x00\x08\x03\x00\x00\x01"_bin;
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_vlan_pcp::decode(it, non_zero_padding_binary.end());

        BOOST_TEST(equivalent(actions::set_vlan_pcp{0x3}, non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

BOOST_AUTO_TEST_SUITE_END() // set_vlan_pcp_test

BOOST_AUTO_TEST_SUITE(set_eth_src_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const mac
            = canard::mac_address{{0x01, 0x02, 0x03, 0x04, 0x05, 0x06}};

        auto const sut = actions::set_eth_src{mac};

        BOOST_TEST(sut.type() == v10::protocol::OFPAT_SET_DL_SRC);
        BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_action_dl_addr));
        BOOST_TEST(sut.value() == mac);
    }

    BOOST_AUTO_TEST_CASE(create_test)
    {
        auto const mac
            = canard::mac_address{{0x10, 0x20, 0x30, 0x40, 0x50, 0x60}};

        auto const sut = actions::set_eth_src::create(mac);

        BOOST_TEST(sut.value() == mac);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_mac_is_equal)
      {
        auto const mac = "\x11\x12\x13\x14\x15\x16"_mac;

        BOOST_TEST((actions::set_eth_src{mac} == actions::set_eth_src{mac}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_mac_is_equal)
      {
        BOOST_TEST(
            (actions::set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac}
          != actions::set_eth_src{"\x01\x02\x03\x04\x05\x16"_mac}));
      }
      BOOST_FIXTURE_TEST_CASE(
          is_false_if_padding_is_not_equal, set_eth_src_fixture)
      {
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_eth_src::decode(it, non_zero_padding_binary.end());

        BOOST_TEST((sut != non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_mac_is_equal)
      {
        auto const mac = "\x11\x12\x13\x14\x15\x16"_mac;

        BOOST_TEST(
            equivalent(actions::set_eth_src{mac}, actions::set_eth_src{mac}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_mac_is_equal)
      {
        BOOST_TEST(
            !equivalent(
                actions::set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac}
              , actions::set_eth_src{"\x01\x02\x03\x04\x05\x16"_mac}));
      }
      BOOST_FIXTURE_TEST_CASE(
          is_true_if_padding_is_not_equal, set_eth_src_fixture)
      {
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_eth_src::decode(it, non_zero_padding_binary.end());

        BOOST_TEST(equivalent(sut, non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, set_eth_src_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, set_eth_src_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const set_eth_src = actions::set_eth_src::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((set_eth_src == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // set_eth_src_test

BOOST_AUTO_TEST_SUITE(set_ipv4_dst_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const addr
            = boost::asio::ip::address_v4::from_string("127.0.0.1");

        auto const sut = actions::set_ipv4_dst{addr};

        BOOST_TEST(sut.type() == v10::protocol::OFPAT_SET_NW_DST);
        BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_action_nw_addr));
        BOOST_TEST(sut.value() == addr);
    }

    BOOST_AUTO_TEST_CASE(create_test)
    {
        auto const addr
            = boost::asio::ip::address_v4::from_string("255.255.255.0");

        auto const sut = actions::set_ipv4_dst::create(addr);

        BOOST_TEST(sut.value() == addr);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_ipv4_dst{"127.0.0.1"_ipv4};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_ip_address_is_equal)
      {
        auto const ipaddr = "192.168.1.3"_ipv4;

        BOOST_TEST(
            (actions::set_ipv4_dst{ipaddr} == actions::set_ipv4_dst{ipaddr}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_ip_address_is_not_equal)
      {
        BOOST_TEST(
            (actions::set_ipv4_dst{"0.0.0.0"_ipv4}
          != actions::set_ipv4_dst{"0.0.0.1"_ipv4}));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_ipv4_dst{"127.0.0.1"_ipv4};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_ip_address_is_equal)
      {
        auto const ipaddr = "192.168.1.3"_ipv4;

        BOOST_TEST(
            equivalent(
              actions::set_ipv4_dst{ipaddr}, actions::set_ipv4_dst{ipaddr}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_ip_address_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                actions::set_ipv4_dst{"0.0.0.0"_ipv4}
              , actions::set_ipv4_dst{"0.0.0.1"_ipv4}));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, set_ipv4_dst_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, set_ipv4_dst_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const set_ipv4_dst = actions::set_ipv4_dst::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((set_ipv4_dst == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // set_ipv4_dst_test

BOOST_AUTO_TEST_SUITE(set_ip_dscp_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const dscp = std::uint8_t{0x01};

        auto const sut = actions::set_ip_dscp{dscp};

        BOOST_TEST(sut.type() == v10::protocol::OFPAT_SET_NW_TOS);
        BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_action_nw_tos));
        BOOST_TEST(sut.value() == dscp);
    }

    BOOST_DATA_TEST_CASE(create_success_test, bdata::xrange(0x00, 0x40), dscp)
    {
        auto const sut = actions::set_ip_dscp::create(dscp);

        BOOST_TEST(sut.value() == dscp);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_ip_dscp{0x01};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_ip_dscp_is_equal)
      {
        auto const dscp = 0x1f;

        BOOST_TEST((actions::set_ip_dscp{dscp} == actions::set_ip_dscp{dscp}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_ip_dscp_is_equal)
      {
        BOOST_TEST((actions::set_ip_dscp{0x01} != actions::set_ip_dscp{0x02}));
      }
      BOOST_AUTO_TEST_CASE(
          is_false_if_tos_6_upper_bits_are_equal_not_other_bits_are_not_equal)
      {
        auto const bin = "\x00\x08\x00\x08\xff\x00\x00\x00"_bin;
        auto it = bin.begin();
        auto const sut = actions::set_ip_dscp::decode(it, bin.end());
        BOOST_TEST((actions::set_ip_dscp{sut.value()} != sut));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_padding_is_not_equal)
      {
        auto const non_zero_padding_binary
          = "\x00\x08\x00\x08\xfc\x00\x00\x01"_bin;
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_ip_dscp::decode(it, non_zero_padding_binary.end());

        BOOST_TEST(
            (actions::set_ip_dscp{non_zero_padding.value()}
          != non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_ip_dscp{0x01};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_ip_dscp_is_equal)
      {
        auto const dscp = 0x1f;

        BOOST_TEST(
            equivalent(actions::set_ip_dscp{dscp}, actions::set_ip_dscp{dscp}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_ip_dscp_is_equal)
      {
        BOOST_TEST(
            !equivalent(
              actions::set_ip_dscp{0x01}, actions::set_ip_dscp{0x02}));
      }
      BOOST_AUTO_TEST_CASE(
          is_true_if_tos_6_upper_bits_are_equal_not_other_bits_are_not_equal)
      {
        auto const bin = "\x00\x08\x00\x08\xff\x00\x00\x00"_bin;
        auto it = bin.begin();
        auto const sut = actions::set_ip_dscp::decode(it, bin.end());
        BOOST_TEST(equivalent(actions::set_ip_dscp{sut.value()}, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_padding_is_not_equal)
      {
        auto const non_zero_padding_binary
          = "\x00\x08\x00\x08\xfc\x00\x00\x01"_bin;
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_ip_dscp::decode(it, non_zero_padding_binary.end());

        BOOST_TEST(
            equivalent(
                actions::set_ip_dscp{non_zero_padding.value()}
              , non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

BOOST_AUTO_TEST_SUITE_END() // set_ip_dscp_test

BOOST_AUTO_TEST_SUITE(set_tcp_src_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const port = std::uint16_t{53};

        auto const sut = actions::set_tcp_src{port};

        BOOST_TEST(sut.type() == v10::protocol::OFPAT_SET_TP_SRC);
        BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_action_tp_port));
        BOOST_TEST(sut.value() == port);
    }

    BOOST_AUTO_TEST_CASE(create_test)
    {
        auto const port = 6653;

        auto const sut = actions::set_tcp_src::create(port);

        BOOST_TEST(sut.value() == port);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_tcp_src{0x1234};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_port_is_equal)
      {
        BOOST_TEST((actions::set_tcp_src{80} == actions::set_tcp_src{80}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_port_is_not_equal)
      {
        BOOST_TEST((actions::set_tcp_src{80} != actions::set_tcp_src{443}));
      }
      BOOST_FIXTURE_TEST_CASE(
          is_false_if_padding_is_not_equal, set_tcp_src_fixture)
      {
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_tcp_src::decode(it, non_zero_padding_binary.end());

        BOOST_TEST((sut != non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::set_tcp_src{0x1234};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_port_is_equal)
      {
        BOOST_TEST(
            equivalent(actions::set_tcp_src{80}, actions::set_tcp_src{80}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_port_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(actions::set_tcp_src{80}, actions::set_tcp_src{443}));
      }
      BOOST_FIXTURE_TEST_CASE(
          is_true_if_padding_is_not_equal, set_tcp_src_fixture)
      {
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::set_tcp_src::decode(it, non_zero_padding_binary.end());

        BOOST_TEST(equivalent(sut, non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, set_tcp_src_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, set_tcp_src_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const set_tcp_src = actions::set_tcp_src::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((set_tcp_src == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // set_tcp_src_test

BOOST_AUTO_TEST_SUITE_END() // actions_test
