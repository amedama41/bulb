#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/common/match_fields.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>

#include <cstdint>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v10 = of::v10;
namespace fields = v10::match_fields;

namespace proto = v10::protocol;

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(match_fields_test)

BOOST_AUTO_TEST_SUITE(in_port_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const port_no = std::uint16_t{1};

        auto const sut = fields::in_port{port_no};

        BOOST_TEST(sut.value() == port_no);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = fields::in_port{1};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_port_is_equal)
      {
        auto const port = std::uint16_t{proto::OFPP_MAX};

        BOOST_TEST((fields::in_port{port} == fields::in_port{port}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_port_is_not_equal)
      {
        BOOST_TEST((fields::in_port{1} != fields::in_port{2}));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = fields::in_port{1};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_port_is_equal)
      {
        auto const port = std::uint16_t{proto::OFPP_MAX};

        BOOST_TEST(equivalent(fields::in_port{port}, fields::in_port{port}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_port_is_not_equal)
      {
        BOOST_TEST(!equivalent(fields::in_port{1}, fields::in_port{2}));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_DATA_TEST_CASE(
              create_success_test
            , bdata::make(std::vector<std::uint16_t>{
                1, proto::OFPP_MAX, proto::OFPP_CONTROLLER, proto::OFPP_LOCAL
              })
            , port_no)
    {
        auto const sut = fields::in_port::create(port_no);

        BOOST_TEST(sut.value() == port_no);
    }

    BOOST_DATA_TEST_CASE(
              create_failure_test
            , bdata::make(std::vector<std::uint16_t>{
                  0
                , proto::OFPP_IN_PORT, proto::OFPP_TABLE, proto::OFPP_NORMAL
                , proto::OFPP_FLOOD, proto::OFPP_ALL, proto::OFPP_NONE
              })
            , port_no)
    {
        BOOST_CHECK_THROW(fields::in_port::create(port_no), std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END() // in_port_test

BOOST_AUTO_TEST_SUITE(vlan_vid_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const vid = std::uint16_t{0x0fff};

        auto const sut = fields::vlan_vid{vid};

        BOOST_TEST(sut.value() == vid);
    }

    BOOST_AUTO_TEST_CASE(construct_from_over_vid_test)
    {
        auto const vid = std::uint16_t{0xffff};

        auto const sut = fields::vlan_vid{vid};

        BOOST_TEST(sut.value() == 0xffff);
    }

    BOOST_DATA_TEST_CASE(
              create_success_test
            , bdata::make(std::vector<std::uint16_t>{
                0x0000, 0x1000 / 2, 0x0fff, proto::OFP_VLAN_NONE
              })
            , vid)
    {
        auto const sut = fields::vlan_vid::create(vid);

        BOOST_TEST(sut.value() == vid);
    }

    BOOST_DATA_TEST_CASE(
              create_failure_test
            , bdata::make(std::vector<std::uint16_t>{
                0x1000, (0x1000 + 0xffff) / 2, 0xfffe
              })
            , vid)
    {
        BOOST_CHECK_THROW(fields::vlan_vid::create(vid), std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END() // vlan_vid_test

BOOST_AUTO_TEST_SUITE(vlan_pcp_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const pcp = std::uint8_t{0x01};

        auto const sut = fields::vlan_pcp{pcp};

        BOOST_TEST(sut.value() == pcp);
    }

    BOOST_DATA_TEST_CASE(create_success_test, bdata::xrange(0x00, 0x08), pcp)
    {
        auto const sut = fields::vlan_pcp::create(pcp);

        BOOST_TEST(sut.value() == pcp);
    }

    BOOST_DATA_TEST_CASE(create_failure_test, bdata::xrange(0x08, 0xff), pcp)
    {
        BOOST_CHECK_THROW(fields::vlan_pcp::create(pcp), std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END() // vlan_pcp_test

BOOST_AUTO_TEST_SUITE(ip_dscp_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const dscp = std::uint8_t{0x01};

        auto const sut = fields::ip_dscp{dscp};

        BOOST_TEST(sut.value() == dscp);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = fields::ip_dscp{0x01};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_dscp_is_equal)
      {
        auto const dscp = 0x1f;

        BOOST_TEST((fields::ip_dscp{dscp} == fields::ip_dscp{dscp}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_dscp_is_not_equal)
      {
        BOOST_TEST((fields::ip_dscp{0x01} != fields::ip_dscp{0x02}));
      }
      // BOOST_AUTO_TEST_CASE(
      //     is_false_if_dspc_is_equal_but_tos_lower_2_bits_are_not_equal)
      // {
      //   auto const dscp = std::uint32_t{0x11};
      //   auto match = v10::protocol::ofp_match{};
      //   match.nw_tos = dscp << 2 | 0x3;

      //   BOOST_TEST((fields::ip_dscp{dscp} != fields::ip_dscp{match}));
      // }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = fields::ip_dscp{0x01};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_dscp_is_equal)
      {
        auto const dscp = 0x1f;

        BOOST_TEST(equivalent(fields::ip_dscp{dscp}, fields::ip_dscp{dscp}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_dscp_is_not_equal)
      {
        BOOST_TEST(!equivalent(fields::ip_dscp{0x01}, fields::ip_dscp{0x02}));
      }
      // BOOST_AUTO_TEST_CASE(
      //     is_true_if_dspc_is_equal_but_tos_lower_2_bits_are_not_equal)
      // {
      //   auto const dscp = std::uint32_t{0x11};
      //   auto match = v10::protocol::ofp_match{};
      //   match.nw_tos = dscp << 2 | 0x3;

      //   BOOST_TEST(equivalent(fields::ip_dscp{dscp}, fields::ip_dscp{match}));
      // }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_DATA_TEST_CASE(create_success_test, bdata::xrange(0x00, 0x40), dscp)
    {
        auto const sut = fields::ip_dscp::create(dscp);

        BOOST_TEST(sut.value() == dscp);
    }

BOOST_AUTO_TEST_SUITE_END() // ip_dscp_test

BOOST_AUTO_TEST_SUITE(eth_src_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const addr
            = canard::mac_address{{0x01, 0x02, 0x03, 0x04, 0x05, 0x06}};

        auto const sut = fields::eth_src{addr};

        BOOST_TEST(sut.value() == addr);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = fields::eth_src{"\x01\x02\x03\x04\x05\x06"_mac};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_address_is_equal)
      {
        auto const addr = "\x01\x02\x03\x04\x05\x06"_mac;

        BOOST_TEST((fields::eth_src{addr} == fields::eth_src{addr}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_address_is_not_equal)
      {
        BOOST_TEST(
            (fields::eth_src{"\x01\x02\x03\x04\x05\x06"_mac}
          != fields::eth_src{"\x11\x12\x13\x14\x15\x16"_mac}));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = fields::eth_src{"\x01\x02\x03\x04\x05\x06"_mac};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_address_is_equal)
      {
        auto const addr = "\x01\x02\x03\x04\x05\x06"_mac;

        BOOST_TEST(equivalent(fields::eth_src{addr}, fields::eth_src{addr}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_address_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                fields::eth_src{"\x01\x02\x03\x04\x05\x06"_mac}
              , fields::eth_src{"\x11\x12\x13\x14\x15\x16"_mac}));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_AUTO_TEST_CASE(create_test)
    {
        auto const addr
            = canard::mac_address{{0x01, 0x02, 0x03, 0x04, 0x05, 0x06}};

        auto const sut = fields::eth_src::create(addr);

        BOOST_TEST(sut.value() == addr);
    }

BOOST_AUTO_TEST_SUITE_END() // eth_src_test

BOOST_AUTO_TEST_SUITE(ipv4_src_test)

    BOOST_AUTO_TEST_CASE(construct_from_ipaddress_test)
    {
        auto const addr = boost::asio::ip::address::from_string("127.0.0.1");

        auto const sut = fields::ipv4_src{addr};

        BOOST_TEST(sut.value() == addr);
        BOOST_TEST(sut.prefix_length() == 32);
        BOOST_TEST(sut.wildcard_bit_count() == 0);
    }

    BOOST_AUTO_TEST_CASE(construct_from_ipaddress_with_prefix_length_test)
    {
        auto const addr = boost::asio::ip::address::from_string("127.0.0.1");
        auto const prefix_length = 8;

        auto const sut = fields::ipv4_src{addr, prefix_length};

        BOOST_TEST(sut.value() == addr);
        BOOST_TEST(sut.prefix_length() == prefix_length);
        BOOST_TEST(sut.wildcard_bit_count() == 32 - prefix_length);
    }

    BOOST_AUTO_TEST_CASE(construct_from_ipv4address_test)
    {
        auto const addr = boost::asio::ip::address_v4::from_string("127.0.0.1");

        auto const sut = fields::ipv4_src{addr};

        BOOST_TEST(sut.value() == addr);
        BOOST_TEST(sut.prefix_length() == 32);
        BOOST_TEST(sut.wildcard_bit_count() == 0);
    }

    BOOST_AUTO_TEST_CASE(construct_from_ipv4address_with_prefix_length_test)
    {
        auto const addr = boost::asio::ip::address_v4::from_string("127.0.0.1");
        auto const prefix_length = 24;

        auto const sut = fields::ipv4_src{addr, prefix_length};

        BOOST_TEST(sut.value() == addr);
        BOOST_TEST(sut.prefix_length() == prefix_length);
        BOOST_TEST(sut.wildcard_bit_count() == 32 - prefix_length);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = fields::ipv4_src{"192.168.1.0"_ipv4, 24};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_address_and_prefix_length_are_equal)
      {
        auto const addr = "192.168.1.0"_ipv4;
        auto const prefix_len = 16;

        BOOST_TEST(
            (fields::ipv4_src{addr, prefix_len}
          == fields::ipv4_src{addr, prefix_len}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_address_is_not_equal)
      {
        auto const prefix_len = 32;

        BOOST_TEST(
            (fields::ipv4_src{"192.168.1.1"_ipv4, prefix_len}
          != fields::ipv4_src{"192.168.1.2"_ipv4, prefix_len}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_prefix_length_is_not_equal)
      {
        auto const addr = "192.168.1.1"_ipv4;

        BOOST_TEST((fields::ipv4_src{addr, 16} != fields::ipv4_src{addr, 8}));
      }
      BOOST_AUTO_TEST_CASE(
          is_false_if_prefix_address_is_equal_but_other_is_not_equal)
      {
        BOOST_TEST(
            (fields::ipv4_src{"192.168.1.1"_ipv4, 16}
          != fields::ipv4_src{"192.168.2.1"_ipv4, 16}));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = fields::ipv4_src{"192.168.1.0"_ipv4, 24};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_address_and_prefix_length_are_equal)
      {
        auto const addr = "192.168.1.0"_ipv4;
        auto const prefix_len = 16;

        BOOST_TEST(
            equivalent(
                fields::ipv4_src{addr, prefix_len}
              , fields::ipv4_src{addr, prefix_len}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_address_is_not_equal)
      {
        auto const prefix_len = 32;

        BOOST_TEST(
            !equivalent(
                fields::ipv4_src{"192.168.1.1"_ipv4, prefix_len}
              , fields::ipv4_src{"192.168.1.2"_ipv4, prefix_len}));
      }
      BOOST_AUTO_TEST_CASE(is_false_if_prefix_length_is_not_equal)
      {
        auto const addr = "192.168.1.1"_ipv4;

        BOOST_TEST(
            !equivalent(fields::ipv4_src{addr, 16} , fields::ipv4_src{addr, 8}));
      }
      BOOST_AUTO_TEST_CASE(
          is_true_if_prefix_address_is_equal_but_other_is_not_equal)
      {
        BOOST_TEST(
            equivalent(
                fields::ipv4_src{"192.168.1.1"_ipv4, 16}
              , fields::ipv4_src{"192.168.2.1"_ipv4, 16}));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_DATA_TEST_CASE(
              create_success_test
            , bdata::make(std::vector<std::uint8_t>{0, 8, 16, 24, 32})
            , prefix_length)
    {
        auto const addr = boost::asio::ip::address::from_string("192.168.2.1");

        auto const sut = fields::ipv4_src::create(addr, prefix_length);

        BOOST_TEST(sut.value() == addr);
        BOOST_TEST(sut.prefix_length() == prefix_length);
        BOOST_TEST(sut.wildcard_bit_count() == 32 - prefix_length);
    }

    BOOST_DATA_TEST_CASE(
              create_failure_test
            , bdata::make(std::vector<std::uint8_t>{33, 64, 128, 224})
            , prefix_length)
    {
        auto const addr = boost::asio::ip::address::from_string("192.168.2.1");

        BOOST_CHECK_THROW(
                  fields::ipv4_src::create(addr, prefix_length)
                , std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END() // ipv4_src_test

BOOST_AUTO_TEST_SUITE_END() // match_fields_test
