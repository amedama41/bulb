#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/flow_add.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace act = v10::actions;
namespace detail = v10::v10_detail;

using proto = v10::protocol;

namespace {
struct parameters : match_fixture, action_fixture {
  v10::match_set match{
    in_port, eth_src, vlan_vid, eth_type, ip_proto, ipv4_dst, tcp_src
  };
  std::uint16_t priority = 0x4321;
  std::uint64_t cookie = 0x0102030405060708;
  v10::action_list actions{
    set_vlan_vid, output, strip_vlan, enqueue, set_eth_src, set_ipv4_src, output
  };
  v10::timeouts timeouts{0x3232, 0x8989};
  std::uint16_t flags = proto::OFPFF_SEND_FLOW_REM | proto::OFPFF_CHECK_OVERLAP;
  std::uint32_t buffer_id = 0x01020304;
  std::uint32_t xid = 0x12345678;
};
struct flow_add_fixture : parameters {
  msg::flow_add sut{
    match, priority, cookie, actions, timeouts, flags, buffer_id, xid
  };
  std::vector<unsigned char> bin
    = "\x01\x0e\x00\x90\x12\x34\x56\x78"
      "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x18\x23\x00\x00"
      "\x01\x02\x03\x04\x05\x06\x07\x08""\x00\x00\x32\x32\x89\x89\x43\x21"
      "\x01\x02\x03\x04\x00\x00\x00\x03"
      "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"
      "\x00\x03\x00\x08\x00\x00\x00\x00""\x00\x0b\x00\x10\x00\x01\x00\x00"
      "\x00\x00\x00\x00\x12\x34\x56\x78""\x00\x04\x00\x10\x01\x02\x03\x04"
      "\x05\x06\x00\x00\x00\x00\x00\x00""\x00\x06\x00\x08\xc0\xa8\x01\x03"
      "\x00\x00\x00\x08\xff\xfd\x00\x40"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(flow_add)

  BOOST_AUTO_TEST_SUITE(command)
    BOOST_AUTO_TEST_CASE(returns_flow_add_command)
    {
      BOOST_TEST(msg::flow_add::command() == proto::OFPFC_ADD);
    }
  BOOST_AUTO_TEST_SUITE_END() // command

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_match_and_priority_and_cookie_and_actions
        , parameters)
    {
      auto const match = v10::match_set{ in_port, eth_type, eth_src, eth_dst };
      constexpr auto priority = std::uint16_t{0xff12};
      constexpr auto cookie = std::uint64_t{0x0102030405060708};
      auto const actions = v10::action_list{ set_ipv4_src, output };
      constexpr auto timeouts = v10::timeouts{1234, 5678};
      constexpr auto flags = proto::OFPFF_CHECK_OVERLAP;
      constexpr auto buffer_id = 0x12345678;

      msg::flow_add sut{
        match, priority, cookie, actions, timeouts, flags, buffer_id
      };

      BOOST_TEST(
          sut.length() == sizeof(detail::ofp_flow_mod) + actions.length());
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST((sut.timeouts() == timeouts));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.buffer_id() == buffer_id);
    }
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_xid, parameters)
    {
      auto const match = v10::match_set{ in_port, eth_type, eth_src, eth_dst };
      constexpr auto priority = std::uint16_t{0xff12};
      constexpr auto cookie = std::uint64_t{0x0102030405060708};
      auto const actions = v10::action_list{ set_ipv4_src, output };
      constexpr auto timeouts = v10::timeouts{1234, 5678};
      constexpr auto flags = proto::OFPFF_CHECK_OVERLAP;
      constexpr auto buffer_id = 0x12345678;
      constexpr auto xid = std::uint32_t{0x12003400};

      msg::flow_add sut{
        match, priority, cookie, actions, timeouts, flags, buffer_id, xid
      };

      BOOST_TEST(
          sut.length() == sizeof(detail::ofp_flow_mod) + actions.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST((sut.timeouts() == timeouts));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.buffer_id() == buffer_id);
    }
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_flow_entry, parameters)
    {
      auto entry = v10::flow_entry{{match, priority}, cookie, actions};

      msg::flow_add sut{entry, timeouts, flags, buffer_id, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_flow_mod) + entry.actions().length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == entry.id().match()));
      BOOST_TEST(sut.priority() == entry.id().priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST((sut.actions() == entry.actions()));
      BOOST_TEST((sut.timeouts() == timeouts));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.buffer_id() == buffer_id);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, flow_add_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, flow_add_fixture)
    {
      auto const& const_flow_add = sut;

      auto const copy = const_flow_add;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          is_move_constructible_and_makes_moved_objects_actions_empty
        , flow_add_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_flow_mod));
      BOOST_TEST((moved.actions() == v10::action_list{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(encode, flow_add_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_empty_actions_binary_when_actions_is_empty)
    {
      auto actions = v10::action_list{};
      auto const sut = msg::flow_add{
        match, priority, cookie, actions, timeouts, flags, buffer_id, xid
      };
      bin[2] = 0;
      bin[3] = sizeof(detail::ofp_flow_mod);
      bin.resize(sizeof(detail::ofp_flow_mod));
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_add_fixture)
    BOOST_AUTO_TEST_CASE(constructs_flow_add_from_binary)
    {
      auto it = bin.begin();

      auto flow_add = msg::flow_add::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_add == sut));
    }
    BOOST_AUTO_TEST_CASE(constructs_flow_add_from_empty_actions_binary)
    {
      constexpr auto len = sizeof(detail::ofp_flow_mod);
      bin[2] = 0;
      bin[3] = len;
      auto it = bin.begin();

      auto flow_add = msg::flow_add::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), len)));
      BOOST_TEST(flow_add.length() == len);
      BOOST_TEST(flow_add.actions() == v10::action_list());
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_add
BOOST_AUTO_TEST_SUITE_END() // message_test
