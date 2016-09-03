#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/switch_features.hpp>
#include <boost/test/unit_test.hpp>

#include <iterator>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace detail = v10::v10_detail;

using proto = v10::protocol;

namespace {

struct features_request_fixture {
  msg::features_request sut{0x12345678};
  std::vector<unsigned char> bin = "\x01\x05\x00\x08\x12\x34\x56\x78"_bin;
};

struct features_reply_param_fixture {
  v10::port port1{
      1, "\xA1\xA2\xA3\xA4\xA5\xA6"_mac, "123456789abcdef"
    , proto::OFPPC_PORT_DOWN | proto::OFPPC_NO_FLOOD
    , proto::OFPPS_LINK_DOWN | proto::OFPPS_STP_FORWARD
    , proto::OFPPF_10MB_HD | proto::OFPPF_COPPER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_10MB_FD | proto::OFPPF_100MB_HD
    | proto::OFPPF_100MB_FD | proto::OFPPF_COPPER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_10MB_FD | proto::OFPPF_100MB_HD
    | proto::OFPPF_100MB_FD | proto::OFPPF_1GB_HD | proto::OFPPF_1GB_FD
    | proto::OFPPF_10GB_FD | proto::OFPPF_COPPER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_COPPER | proto::OFPPF_AUTONEG
  };
  v10::port port2{
      proto::OFPP_MAX, "\xB1\xB2\xB3\xB4\xB5\xB6"_mac, "maxportno"
    , proto::OFPPC_NO_PACKET_IN
    , proto::OFPPS_STP_BLOCK
    , proto::OFPPF_10MB_HD | proto::OFPPF_COPPER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_10MB_FD | proto::OFPPF_100MB_HD
    | proto::OFPPF_100MB_FD | proto::OFPPF_COPPER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_10MB_FD | proto::OFPPF_100MB_HD
    | proto::OFPPF_100MB_FD | proto::OFPPF_1GB_HD | proto::OFPPF_1GB_FD
    | proto::OFPPF_10GB_FD | proto::OFPPF_COPPER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_COPPER | proto::OFPPF_AUTONEG
  };
  v10::port port3{
      proto::OFPP_LOCAL, "\xc1\xc2\xc3\xc4\xc5\xc6"_mac, "local"
    , proto::OFPPC_NO_PACKET_IN
    , proto::OFPPS_STP_BLOCK
    , proto::OFPPF_10MB_HD | proto::OFPPF_FIBER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_10MB_FD | proto::OFPPF_100MB_HD
    | proto::OFPPF_100MB_FD | proto::OFPPF_FIBER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_10MB_FD | proto::OFPPF_100MB_HD
    | proto::OFPPF_100MB_FD | proto::OFPPF_1GB_HD | proto::OFPPF_1GB_FD
    | proto::OFPPF_10GB_FD | proto::OFPPF_FIBER | proto::OFPPF_AUTONEG
    , proto::OFPPF_10MB_HD | proto::OFPPF_FIBER | proto::OFPPF_AUTONEG
  };
  std::uint64_t dpid = 0x0102030405060708;
  std::uint32_t n_buffers = 0x12345678;
  std::uint8_t n_tables = 0x1f;
  std::uint32_t capabilities
    = proto::OFPC_FLOW_STATS | proto::OFPC_TABLE_STATS
    | proto::OFPC_PORT_STATS | proto::OFPC_STP
    | proto::OFPC_IP_REASM | proto::OFPC_QUEUE_STATS
    | proto::OFPC_ARP_MATCH_IP;
  std::uint32_t actions
    = (1 << proto::OFPAT_OUTPUT)
    | (1 << proto::OFPAT_SET_VLAN_VID)
    | (1 << proto::OFPAT_SET_VLAN_PCP)
    | (1 << proto::OFPAT_STRIP_VLAN)
    | (1 << proto::OFPAT_SET_DL_SRC)
    | (1 << proto::OFPAT_SET_DL_DST)
    | (1 << proto::OFPAT_SET_NW_SRC)
    | (1 << proto::OFPAT_SET_NW_DST)
    | (1 << proto::OFPAT_SET_NW_TOS)
    | (1 << proto::OFPAT_SET_TP_SRC)
    | (1 << proto::OFPAT_SET_TP_DST)
    | (1 << proto::OFPAT_ENQUEUE);
  msg::features_reply::port_list ports{ port1, port2, port3 };
  std::uint32_t xid = 0x12345678;
};

struct features_reply_fixture : features_reply_param_fixture {
  msg::features_reply sut{
      dpid, n_buffers, n_tables, capabilities, actions, ports, xid
  };
  std::vector<unsigned char> bin
    = "\x01\x06\x00\xb0\x12\x34\x56\x78""\x01\x02\x03\x04\x05\x06\x07\x08"
      "\x12\x34\x56\x78\x1f\x00\x00\x00""\x00\x00\x00\xef\x00\x00\x0f\xff"

      "\x00\x01\xA1\xA2\xA3\xA4\xA5\xA6""12345678"
      "9abcdef"                   "\x00""\x00\x00\x00\x11\x00\x00\x02\x01"
      "\x00\x00\x02\x81\x00\x00\x02\x8f""\x00\x00\x02\xff\x00\x00\x02\x81"

      "\xff\x00\xB1\xB2\xB3\xB4\xB5\xB6""maxportno"
          "\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x40\x00\x00\x03\x00"
      "\x00\x00\x02\x81\x00\x00\x02\x8f""\x00\x00\x02\xff\x00\x00\x02\x81"

      "\xff\xfe\xC1\xC2\xC3\xC4\xC5\xC6""local"             "\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x40\x00\x00\x03\x00"
      "\x00\x00\x03\x01\x00\x00\x03\x0f""\x00\x00\x03\x7f\x00\x00\x03\x01"_bin;
};

}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(features_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      auto const sut = msg::features_request{};

      BOOST_TEST(sut.version() == proto::OFP_VERSION);
      BOOST_TEST(sut.type() == proto::OFPT_FEATURES_REQUEST);
      BOOST_TEST(sut.length() == sizeof(detail::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = std::uint32_t{42};

      auto const sut = msg::features_request{xid};

      BOOST_TEST(sut.xid() == xid);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::features_request{0x12345678};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_xid_is_equal)
    {
      auto const xid = 0x11002200;

      BOOST_TEST((msg::features_request{xid} == msg::features_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST((msg::features_request{1} != msg::features_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, features_request_fixture)
    BOOST_AUTO_TEST_CASE(generate_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, features_request_fixture)
    BOOST_AUTO_TEST_CASE(construct_features_request_from_binary)
    {
      auto it = bin.begin();

      auto const features_request
        = msg::features_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((features_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // features_request


BOOST_AUTO_TEST_SUITE(features_reply)

  BOOST_FIXTURE_TEST_SUITE(constructor, features_reply_param_fixture)
    BOOST_AUTO_TEST_CASE(is_constructible)
    {
      auto const sut = msg::features_reply{
        dpid, n_buffers, n_tables, capabilities, actions, ports
      };

      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_switch_features)
        + ports.size() * sizeof(detail::ofp_phy_port));
      BOOST_TEST(sut.datapath_id() == dpid);
      BOOST_TEST(sut.num_buffers() == n_buffers);
      BOOST_TEST(sut.num_tables() == n_tables);
      BOOST_TEST(sut.capabilities() == capabilities);
      BOOST_TEST(sut.actions() == actions);
      BOOST_TEST((sut.ports() == ports));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = std::uint32_t{1234};

      auto const sut = msg::features_reply{
        dpid, n_buffers, n_tables, capabilities, actions, ports, xid
      };

      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_switch_features)
        + ports.size() * sizeof(detail::ofp_phy_port));
      BOOST_TEST(sut.datapath_id() == dpid);
      BOOST_TEST(sut.num_buffers() == n_buffers);
      BOOST_TEST(sut.num_tables() == n_tables);
      BOOST_TEST(sut.capabilities() == capabilities);
      BOOST_TEST(sut.actions() == actions);
      BOOST_TEST((sut.ports() == ports));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_features_request)
    {
      auto const request = msg::features_request{};

      auto const sut = msg::features_reply{
        request, dpid, n_buffers, n_tables, capabilities, actions, ports
      };

      BOOST_TEST(sut.xid() == request.xid());
      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_switch_features)
        + ports.size() * sizeof(detail::ofp_phy_port));
      BOOST_TEST(sut.datapath_id() == dpid);
      BOOST_TEST(sut.num_buffers() == n_buffers);
      BOOST_TEST(sut.num_tables() == n_tables);
      BOOST_TEST(sut.capabilities() == capabilities);
      BOOST_TEST(sut.actions() == actions);
      BOOST_TEST((sut.ports() == ports));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_empty_port_list)
    {
      auto const sut = msg::features_reply{
        dpid, n_buffers, n_tables, capabilities, actions, {}
      };

      BOOST_TEST(sut.length() == sizeof(detail::ofp_switch_features));
      BOOST_TEST(sut.datapath_id() == dpid);
      BOOST_TEST(sut.num_buffers() == n_buffers);
      BOOST_TEST(sut.num_tables() == n_tables);
      BOOST_TEST(sut.capabilities() == capabilities);
      BOOST_TEST(sut.actions() == actions);
      BOOST_TEST(sut.ports().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(extract_ports, features_reply_fixture)
    BOOST_AUTO_TEST_CASE(returns_ports_and_msgs_port_list_is_empty)
    {
      auto ports = sut.extract_ports();

      BOOST_TEST(sut.length() == sizeof(detail::ofp_switch_features));
      BOOST_TEST(sut.ports().empty());
      BOOST_TEST(
          (ports == msg::features_reply::port_list{ port1, port2, port3 }));
    }
    BOOST_AUTO_TEST_CASE(returns_empty_port_list)
    {
      auto sut = msg::features_reply{
        dpid, n_buffers, n_tables, capabilities, actions, {}, xid
      };

      auto ports = sut.extract_ports();

      BOOST_TEST(sut.length() == sizeof(detail::ofp_switch_features));
      BOOST_TEST(sut.ports().empty());
      BOOST_TEST(ports.empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_ports

  BOOST_FIXTURE_TEST_SUITE(equality, features_reply_param_fixture)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::features_reply{
        dpid, n_buffers, n_tables, capabilities, actions, ports, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (msg::features_reply{
             dpid, n_buffers, n_tables, capabilities, actions, ports, xid}
        == msg::features_reply{
             dpid, n_buffers, n_tables, capabilities, actions, ports, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_dpid_is_not_equal)
    {
      BOOST_TEST(
          (msg::features_reply{
             1, n_buffers, n_tables, capabilities, actions, ports, xid}
        != msg::features_reply{
             2, n_buffers, n_tables, capabilities, actions, ports, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_n_buffers_is_not_equal)
    {
      BOOST_TEST(
          (msg::features_reply{
             dpid, 1, n_tables, capabilities, actions, ports, xid}
        != msg::features_reply{
             dpid, 2, n_tables, capabilities, actions, ports, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_n_tables_is_not_equal)
    {
      BOOST_TEST(
          (msg::features_reply{
             dpid, n_buffers, 1, capabilities, actions, ports, xid}
        != msg::features_reply{
             dpid, n_buffers, 2, capabilities, actions, ports, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_capabilities_is_not_equal)
    {
      BOOST_TEST(
          (msg::features_reply{
             dpid, n_buffers, n_tables, 1, actions, ports, xid}
        != msg::features_reply{
             dpid, n_buffers, n_tables, 2, actions, ports, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_actions_is_not_equal)
    {
      BOOST_TEST(
          (msg::features_reply{
             dpid, n_buffers, n_tables, capabilities, 1, ports, xid}
        != msg::features_reply{
             dpid, n_buffers, n_tables, capabilities, 2, ports, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_ports_is_not_equal)
    {
      BOOST_TEST(
          (msg::features_reply{
             dpid, n_buffers, n_tables, capabilities, actions, { port1 }, xid}
        != msg::features_reply{
             dpid, n_buffers, n_tables, capabilities, actions, { port2 }, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::features_reply{
             dpid, n_buffers, n_tables, capabilities, actions, ports, 1}
        != msg::features_reply{
             dpid, n_buffers, n_tables, capabilities, actions, ports, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, features_reply_fixture)
    BOOST_AUTO_TEST_CASE(generate_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generate_binary_when_port_list_is_empty)
    {
      sut.extract_ports();
      auto buf = std::vector<unsigned char>{};
      bin.resize(sizeof(detail::ofp_switch_features));
      bin[3] = sizeof(detail::ofp_switch_features);

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, features_reply_fixture)
    BOOST_AUTO_TEST_CASE(construct_features_reply_from_binary)
    {
      auto it = bin.begin();

      auto const features_reply = msg::features_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((features_reply == sut));
    }
    BOOST_AUTO_TEST_CASE(construct_features_reply_from_no_port_binary)
    {
      auto const bin_len = sizeof(detail::ofp_switch_features);
      bin[3] = bin_len;
      auto it = bin.begin();

      auto const features_reply = msg::features_reply::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), bin_len)));
      BOOST_TEST(
          (features_reply
        == msg::features_reply{
            dpid, n_buffers, n_tables, capabilities, actions, {}, xid}));
    }
    BOOST_AUTO_TEST_CASE(construct_features_reply_from_single_port_binary)
    {
      auto const bin_len
        = sizeof(detail::ofp_switch_features) + sizeof(detail::ofp_phy_port);
      bin[3] = bin_len;
      auto it = bin.begin();

      auto const features_reply = msg::features_reply::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), bin_len)));
      BOOST_TEST(
          (features_reply
        == msg::features_reply{
            dpid, n_buffers, n_tables, capabilities, actions, { port1 }, xid}));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // features_reply

BOOST_AUTO_TEST_SUITE_END() // message_test
