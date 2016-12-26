#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/flow_delete.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace act = v10::actions;
namespace detail = v10::v10_detail;

namespace proto = v10::protocol;

namespace {
struct parameters : match_fixture, action_fixture {
  v10::match match{
    in_port, eth_src, vlan_vid, eth_type, ip_proto, ipv4_dst, tcp_src
  };
  std::uint16_t priority = 0x4321;
  std::uint16_t out_port = 0x1234;
  std::uint32_t xid = 0x12345678;
};
struct flow_delete_fixture : parameters {
  msg::flow_delete sut{match, out_port, xid};
  std::vector<unsigned char> bin
    = "\x01\x0e\x00\x48\x12\x34\x56\x78"
      "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x18\x23\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x03\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x12\x34\x00\x00"
      ""_bin;
};
struct flow_delete_strict_fixture : parameters {
  msg::flow_delete_strict sut{match, priority, out_port, xid};
  std::vector<unsigned char> bin
    = "\x01\x0e\x00\x48\x12\x34\x56\x78"
      "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x18\x23\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x04\x00\x00\x00\x00\x43\x21"
      "\x00\x00\x00\x00\x12\x34\x00\x00"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(flow_delete)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_match, parameters)
    {
      msg::flow_delete sut{match};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.out_port() == proto::OFPP_NONE);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_match_and_out_port, parameters)
    {
      msg::flow_delete sut{match, out_port};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.out_port() == out_port);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_match_and_out_port_and_xid, parameters)
    {
      msg::flow_delete sut{match, out_port, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.out_port() == out_port);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, flow_delete_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, flow_delete_fixture)
    {
      auto const& const_flow_delete = sut;

      auto const copy = const_flow_delete;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, flow_delete_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST((moved.actions() == v10::action_list{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(encode, flow_delete_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_delete_fixture)
    BOOST_AUTO_TEST_CASE(constructs_flow_delete_from_binary)
    {
      auto it = bin.begin();

      auto const flow_delete = msg::flow_delete::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_delete == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_delete


BOOST_AUTO_TEST_SUITE(flow_delete_strict)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_match_and_priority, parameters)
    {
      msg::flow_delete_strict sut{match, priority};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.out_port() == proto::OFPP_NONE);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_match_and_priority_and_out_port, parameters)
    {
      msg::flow_delete_strict sut{match, priority, out_port};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.out_port() == out_port);
    }
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_match_and_priority_and_out_port_and_xid
        , parameters)
    {
      msg::flow_delete_strict sut{match, priority, out_port, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.out_port() == out_port);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, flow_delete_strict_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, flow_delete_strict_fixture)
    {
      auto const& const_flow_delete = sut;

      auto const copy = const_flow_delete;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, flow_delete_strict_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST((moved.actions() == v10::action_list{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(encode, flow_delete_strict_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_delete_strict_fixture)
    BOOST_AUTO_TEST_CASE(constructs_flow_delete_from_binary)
    {
      auto it = bin.begin();

      auto const flow_delete_strict
        = msg::flow_delete_strict::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_delete_strict == sut));
    }
    BOOST_AUTO_TEST_CASE(constructs_flow_delete_with_actions_from_binary)
    {
      bin.insert(bin.end(), output_bin.begin(), output_bin.end());
      bin[3] += output_bin.size();
      auto it = bin.begin();

      auto const flow_delete_strict
        = msg::flow_delete_strict::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_delete_strict.actions() == v10::action_list{output}));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_delete_strict

BOOST_AUTO_TEST_SUITE_END() // message_test
