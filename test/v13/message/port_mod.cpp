#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/port_mod.hpp>
#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace msg = v13::messages;
namespace protocol = v13::protocol;

namespace {

  struct port_mod_parameter {
    std::uint32_t port_no = protocol::OFPP_MAX;
    canard::mac_address hw_addr{{0xff, 0x11, 0xff, 0x12, 0x13, 0x14}};
    std::uint32_t config = protocol::OFPPC_NO_FWD;
    std::uint32_t mask = protocol::OFPPC_PORT_DOWN | protocol::OFPPC_NO_RECV;
    std::uint32_t advertise = protocol::OFPPF_10GB_FD | protocol::OFPPF_AUTONEG;
    std::uint32_t xid = 0x01010101;
  };
  struct port_mod_fixture : port_mod_parameter {
    v13::messages::port_mod sut{
      port_no, hw_addr, config, mask, advertise, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x10\x00\x28\x01\x01\x01\x01" "\xff\xff\xff\x00\x00\x00\x00\x00"
        "\xff\x11\xff\x12\x13\x14\x00\x00" "\x00\x00\x00\x20\x00\x00\x00\x05"
        "\x00\x00\x20\x40\x00\x00\x00\x00"_bin
      ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(port_mod)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const port_no = std::uint32_t{protocol::OFPP_MAX};
      auto const hw_addr
        = canard::mac_address{{0x10, 0x20, 0x30, 0x40, 0x50, 0x60}};
      auto const config = std::uint32_t{protocol::OFPPC_NO_RECV};
      auto const mask = std::uint32_t{
        protocol::OFPPC_PORT_DOWN | protocol::OFPPC_NO_RECV
      };
      auto const advertise = std::uint32_t{
        protocol::OFPPF_10GB_FD | protocol::OFPPF_AUTONEG
      };
      auto const xid = std::uint32_t{0x01010101};

      v13::messages::port_mod const sut{
        port_no, hw_addr, config, mask, advertise, xid
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_PORT_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_mod));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST(sut.hardware_address() == hw_addr);
      BOOST_TEST(sut.config() == config);
      BOOST_TEST(sut.mask() == mask);
      BOOST_TEST(sut.advertised_features() == advertise);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_port)
    {
      auto const port = v13::port::from_ofp_port({
            9, {0}, {0x11, 0x21, 0x31, 0x41, 0x51, 0x61}, {0}
          , "eth0", protocol::OFPPC_PORT_DOWN, protocol::OFPPS_LINK_DOWN
          , protocol::OFPPF_10GB_FD | protocol::OFPPF_FIBER
          , protocol::OFPPF_10GB_FD | protocol::OFPPF_FIBER | protocol::OFPPF_AUTONEG
          , protocol::OFPPF_10GB_FD | protocol::OFPPF_1GB_FD | protocol::OFPPF_FIBER | protocol::OFPPF_AUTONEG
          , protocol::OFPPF_10GB_FD | protocol::OFPPF_1GB_FD | protocol::OFPPF_COPPER | protocol::OFPPF_AUTONEG
          , 10000, 12000
      });
      auto const config = std::uint32_t{protocol::OFPPC_NO_RECV};
      auto const mask = std::uint32_t{
        protocol::OFPPC_PORT_DOWN | protocol::OFPPC_NO_RECV
      };
      auto const advertise = std::uint32_t{
        protocol::OFPPF_10GB_FD | protocol::OFPPF_AUTONEG
      };
      auto const xid = std::uint32_t{0x01010101};

      v13::messages::port_mod const sut{port, config, mask, advertise, xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_PORT_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_mod));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.port_no() == port.port_no());
      BOOST_TEST(sut.hardware_address() == port.hardware_address());
      BOOST_TEST(sut.config() == config);
      BOOST_TEST(sut.mask() == mask);
      BOOST_TEST(sut.advertised_features() == advertise);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, port_mod_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, port_mod_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = msg::port_mod{
        port_no, hw_addr, config, mask, advertise, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, hw_addr, config, mask, advertise, xid}
        == msg::port_mod{port_no, hw_addr, config, mask, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{1, hw_addr, config, mask, advertise, xid}
        != msg::port_mod{2, hw_addr, config, mask, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_hw_addr_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, "\x00\x00\x00\x00\x00\x01"_mac
           , config, mask, advertise, xid}
        != msg::port_mod{port_no, "\x00\x00\x00\x00\x00\x02"_mac
           , config, mask, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_config_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, hw_addr, 1, mask, advertise, xid}
        != msg::port_mod{port_no, hw_addr, 2, mask, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, hw_addr, config, 1, advertise, xid}
        != msg::port_mod{port_no, hw_addr, config, 2, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_advertise_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, hw_addr, config, mask, 1, xid}
        != msg::port_mod{port_no, hw_addr, config, mask, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, hw_addr, config, mask, advertise, 1}
        != msg::port_mod{port_no, hw_addr, config, mask, advertise, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, port_mod_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, port_mod_fixture)
    {
      auto it = bin.begin();

      auto const port_mod = v13::messages::port_mod::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_mod == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // port_mod
BOOST_AUTO_TEST_SUITE_END() // message_test

