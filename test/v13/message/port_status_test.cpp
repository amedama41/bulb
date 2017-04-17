#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/port_status.hpp>
#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;

namespace {

  struct ofp_port_fixture
  {
    protocol::ofp_port const ofp_port{
        1, {0}, {0x10, 0x20, 0x30, 0x40, 0x50, 0x60}, {0}
      , "eth0", protocol::OFPPC_PORT_DOWN, protocol::OFPPS_LINK_DOWN
        , protocol::OFPPF_10GB_FD | protocol::OFPPF_FIBER
        , protocol::OFPPF_10GB_FD | protocol::OFPPF_FIBER | protocol::OFPPF_AUTONEG
        , protocol::OFPPF_10GB_FD | protocol::OFPPF_1GB_FD | protocol::OFPPF_FIBER | protocol::OFPPF_AUTONEG
        , protocol::OFPPF_10GB_FD | protocol::OFPPF_1GB_FD | protocol::OFPPF_COPPER | protocol::OFPPF_AUTONEG
        , 10000, 12000
    };
  };
  struct port_status_parameter {
    protocol::ofp_port ofp_port{
        protocol::OFPP_MAX, {0}, {0x10, 0x20, 0x30, 0x40, 0x50, 0x60}, {0}
      , "eth1"
      , protocol::OFPPC_PORT_DOWN | protocol::OFPPC_NO_FWD
      , protocol::OFPPS_LINK_DOWN | protocol::OFPPS_LIVE
      , protocol::OFPPF_10GB_FD | protocol::OFPPF_FIBER
      , protocol::OFPPF_10GB_FD | protocol::OFPPF_FIBER | protocol::OFPPF_AUTONEG
      , protocol::OFPPF_10GB_FD | protocol::OFPPF_1GB_FD | protocol::OFPPF_FIBER | protocol::OFPPF_AUTONEG
      , protocol::OFPPF_10GB_FD | protocol::OFPPF_1GB_FD | protocol::OFPPF_COPPER | protocol::OFPPF_AUTONEG
      , 10000, 12000
    };
    v13::port port = v13::port::from_ofp_port(ofp_port);
    protocol::port_reason reason = protocol::OFPPR_DELETE;
    std::uint32_t xid = 0x87654321;
  };
  struct port_status_fixture : port_status_parameter {
    v13::messages::port_status sut{reason, port, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x0c\x00\x50\x87\x65\x43\x21" "\x01\x00\x00\x00\x00\x00\x00\x00"
        "\xff\xff\xff\x00\x00\x00\x00\x00" "\x10\x20\x30\x40\x50\x60\x00\x00"
        "e" "t" "h" "1" "\x00\x00\x00\x00" "\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x21\x00\x00\x00\x05" "\x00\x00\x10\x40\x00\x00\x30\x40"
        "\x00\x00\x30\x60\x00\x00\x28\x60" "\x00\x00\x27\x10\x00\x00\x2e\xe0"_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(port_status)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible, ofp_port_fixture)
    {
      auto const reason = protocol::OFPPR_ADD;
      auto const xid = 0x0123;

      auto const sut = v13::messages::port_status{
        reason, v13::port::from_ofp_port(ofp_port), xid
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_PORT_STATUS);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_status));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST(sut.port_no() == ofp_port.port_no);
      BOOST_TEST(
          sut.hardware_address() == canard::mac_address{ofp_port.hw_addr});
      BOOST_TEST(sut.name() == ofp_port.name);
      BOOST_TEST(sut.config() == ofp_port.config);
      BOOST_TEST(sut.state() == ofp_port.state);
      BOOST_TEST(sut.current_features() == ofp_port.curr);
      BOOST_TEST(sut.advertised_features() == ofp_port.advertised);
      BOOST_TEST(sut.supported_features() == ofp_port.supported);
      BOOST_TEST(sut.peer_advertised_features() == ofp_port.peer);
      BOOST_TEST(sut.current_speed() == ofp_port.curr_speed);
      BOOST_TEST(sut.max_speed() == ofp_port.max_speed);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, port_status_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, port_status_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::port_status{reason, port, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::port_status{reason, port, xid}
        == v13::messages::port_status{reason, port, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_reason_is_not_equal)
    {
      using reason = protocol::port_reason;

      BOOST_TEST(
          (v13::messages::port_status{reason::by_delete, port, xid}
        != v13::messages::port_status{reason::by_modify, port, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_is_not_equal)
    {
      auto const port1 = v13::port::from_ofp_port(ofp_port);
      ofp_port.port_no = protocol::port_no::local;
      auto const port2 = v13::port::from_ofp_port(ofp_port);

      BOOST_TEST(
          (v13::messages::port_status{reason, port1, xid}
        != v13::messages::port_status{reason, port2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::port_status{reason, port, 1}
        != v13::messages::port_status{reason, port, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, port_status_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(decode, port_status_fixture)
    {
      auto it = bin.begin();

      auto const port_status
        = v13::messages::port_status::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_status == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // port_status
BOOST_AUTO_TEST_SUITE_END() // message_test
