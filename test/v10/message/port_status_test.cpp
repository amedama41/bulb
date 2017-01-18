#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/port_status.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace protocol = v10::protocol;

namespace {
struct parameters {
  protocol::ofp_port_reason reason = protocol::OFPPR_MODIFY;
  std::uint32_t config = protocol::OFPPC_PORT_DOWN | protocol::OFPPC_NO_FLOOD;
  std::uint32_t state = protocol::OFPPS_LINK_DOWN | protocol::OFPPS_STP_FORWARD;
  std::uint32_t curr
    = protocol::OFPPF_10MB_HD | protocol::OFPPF_COPPER | protocol::OFPPF_AUTONEG;
  std::uint32_t advertised
    = protocol::OFPPF_10MB_HD | protocol::OFPPF_10MB_FD | protocol::OFPPF_100MB_HD
    | protocol::OFPPF_100MB_FD | protocol::OFPPF_COPPER | protocol::OFPPF_AUTONEG;
  std::uint32_t supported
    = protocol::OFPPF_10MB_HD | protocol::OFPPF_10MB_FD | protocol::OFPPF_100MB_HD
    | protocol::OFPPF_100MB_FD | protocol::OFPPF_1GB_HD | protocol::OFPPF_1GB_FD
    | protocol::OFPPF_10GB_FD | protocol::OFPPF_COPPER | protocol::OFPPF_AUTONEG;
  std::uint32_t peer
    = protocol::OFPPF_10MB_HD | protocol::OFPPF_COPPER | protocol::OFPPF_AUTONEG;
  v10::port port{
      1, "\xA1\xA2\xA3\xA4\xA5\xA6"_mac, "123456789abcdef"
    , config, state, curr, advertised, supported, peer
  };
  std::uint32_t xid = 0x12345678;
};
struct port_status_fixture : parameters {
  msg::port_status sut{reason, port, xid};
  std::vector<unsigned char> bin
    = "\x01\x0c\x00\x40\x12\x34\x56\x78""\x02\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x01\xA1\xA2\xA3\xA4\xA5\xA6""12345678"
      "9abcdef"                   "\x00""\x00\x00\x00\x11\x00\x00\x02\x01"
      "\x00\x00\x02\x81\x00\x00\x02\x8f""\x00\x00\x02\xff\x00\x00\x02\x81"_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(port_status)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_reason_and_port, parameters)
    {
      auto const reason = protocol::OFPPR_ADD;
      auto const port = v10::port{
          1, "\x01\x02\x03\x04\x05\x06"_mac, "eth0"
        , config, state, curr, advertised, supported, peer
      };

      msg::port_status sut{reason, port, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_status));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST((sut.port() == port));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_reason_and_port_without_xid, parameters)
    {
      auto const reason = protocol::OFPPR_DELETE;
      auto const port = v10::port{
          protocol::OFPP_MAX, "\x01\x02\x03\x04\x05\x06"_mac, "veth1"
        , config, state, curr, advertised, supported, peer
      };

      msg::port_status sut{reason, port};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_status));
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST((sut.port() == port));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, port_status_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, port_status_fixture)
    {
      auto const& const_port_status = sut;

      auto const copy = const_port_status;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(encode, port_status_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, port_status_fixture)
    BOOST_AUTO_TEST_CASE(constructs_port_status_from_binary)
    {
      auto it = bin.begin();

      auto const port_status = msg::port_status::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_status == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // port_status
BOOST_AUTO_TEST_SUITE_END() // message_test
