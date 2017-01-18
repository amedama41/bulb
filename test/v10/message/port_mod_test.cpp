#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/port_mod.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace protocol = v10::protocol;

namespace {
struct parameters {
  std::uint16_t port_no = 0xff00;
  canard::mac_address addr = "\x01\x02\x03\x04\x05\x06"_mac;
  std::uint32_t config = protocol::OFPPC_NO_FLOOD;
  std::uint32_t mask = protocol::OFPPC_PORT_DOWN | config;
  std::uint32_t advertise = protocol::OFPPF_100MB_FD | protocol::OFPPF_AUTONEG;
  std::uint32_t xid = 0x12345678;
};
struct port_mod_fixture : parameters {
  msg::port_mod sut{port_no, addr, config, mask, advertise, xid};
  std::vector<unsigned char> bin
    = "\x01\x0f\x00\x20\x12\x34\x56\x78""\xff\x00\x01\x02\x03\x04\x05\x06"
      "\x00\x00\x00\x10\x00\x00\x00\x11""\x00\x00\x02\x08\x00\x00\x00\x00"_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(port_mod)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_port_no_and_mac_address)
    {
      auto const port_no = std::uint16_t{12};
      auto const addr = "\x01\x02\x03\x04\x05\x06"_mac;
      auto const config = protocol::OFPPC_NO_FLOOD;
      auto const mask = protocol::OFPPC_PORT_DOWN | config;
      auto const advertise = protocol::OFPPF_100MB_FD | protocol::OFPPF_AUTONEG;
      auto const xid = 0x12345678;

      msg::port_mod sut{port_no, addr, config, mask, advertise, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_mod));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST(sut.hardware_address() == addr);
      BOOST_TEST(sut.config() == config);
      BOOST_TEST(sut.mask() == mask);
      BOOST_TEST(sut.advertised_features() == advertise);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_without_xid)
    {
      auto const port_no = std::uint16_t{12};
      auto const addr = "\x01\x02\x03\x04\x05\x06"_mac;
      auto const config = protocol::OFPPC_NO_FLOOD;
      auto const mask = protocol::OFPPC_PORT_DOWN | config;
      auto const advertise = protocol::OFPPF_100MB_FD | protocol::OFPPF_AUTONEG;

      msg::port_mod sut{port_no, addr, config, mask, advertise};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_mod));
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST(sut.hardware_address() == addr);
      BOOST_TEST(sut.config() == config);
      BOOST_TEST(sut.mask() == mask);
      BOOST_TEST(sut.advertised_features() == advertise);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_port)
    {
      auto const port = v10::port{
          1, "\x01\x02\x03\x04\x05\x06"_mac, "eth0"
        , protocol::OFPPC_PORT_DOWN, 0, 0, 0, 0, 0
      };
      auto const config = protocol::OFPPC_NO_FLOOD;
      auto const mask = protocol::OFPPC_PORT_DOWN | config;
      auto const advertise = protocol::OFPPF_100MB_FD | protocol::OFPPF_AUTONEG;
      auto const xid = 0x12345678;

      msg::port_mod sut{port, config, mask, advertise, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_mod));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.port_no() == port.port_no());
      BOOST_TEST(sut.hardware_address() == port.hardware_address());
      BOOST_TEST(sut.config() == config);
      BOOST_TEST(sut.mask() == mask);
      BOOST_TEST(sut.advertised_features() == advertise);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_port_without_xid)
    {
      auto const port = v10::port{
          1, "\x01\x02\x03\x04\x05\x06"_mac, "eth0"
        , protocol::OFPPC_PORT_DOWN, 0, 0, 0, 0, 0
      };
      auto const config = protocol::OFPPC_NO_FLOOD;
      auto const mask = protocol::OFPPC_PORT_DOWN | config;
      auto const advertise = protocol::OFPPF_100MB_FD | protocol::OFPPF_AUTONEG;

      msg::port_mod sut{port, config, mask, advertise};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_mod));
      BOOST_TEST(sut.port_no() == port.port_no());
      BOOST_TEST(sut.hardware_address() == port.hardware_address());
      BOOST_TEST(sut.config() == config);
      BOOST_TEST(sut.mask() == mask);
      BOOST_TEST(sut.advertised_features() == advertise);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::port_mod{port_no, addr, config, mask, advertise};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_port_no_and_address_and_config_and_advertise_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, addr, config, mask, advertise, xid}
        == msg::port_mod{port_no, addr, config, mask, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, addr, config, mask, advertise, 1}
        != msg::port_mod{port_no, addr, config, mask, advertise, 2}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{1, addr, config, mask, advertise, xid}
        != msg::port_mod{2, addr, config, mask, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_address_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{
             port_no, "\x01\02\x03\x04\x05\x06"_mac
           , config, mask, advertise, xid}
        != msg::port_mod{
             port_no, "\x11\12\x13\x14\x15\x16"_mac
           , config, mask, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_config_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, addr, 1, mask, advertise, xid}
        != msg::port_mod{port_no, addr, 2, mask, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, addr, config, 1, advertise, xid}
        != msg::port_mod{port_no, addr, config, 2, advertise, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_advertise_is_not_equal)
    {
      BOOST_TEST(
          (msg::port_mod{port_no, addr, config, mask, 1, xid}
        != msg::port_mod{port_no, addr, config, mask, 2, xid}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, port_mod_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, port_mod_fixture)
    BOOST_AUTO_TEST_CASE(constructs_port_mod_from_binary)
    {
      auto it = bin.begin();

      auto const port_mod = msg::port_mod::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_mod == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // port_mod
BOOST_AUTO_TEST_SUITE_END() // message_test
