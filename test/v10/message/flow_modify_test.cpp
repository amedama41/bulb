#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/flow_modify.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace act = v10::actions;
namespace protocol = v10::protocol;

namespace {

struct parameters : match_fixture, action_fixture {
  v10::match match{
    in_port, eth_src, vlan_vid, eth_type, ip_proto, ipv4_dst, tcp_src
  };
  std::uint16_t priority = 0x4321;
  std::uint64_t cookie = 0x0102030405060708;
  v10::action_list actions{
    set_vlan_vid, output, strip_vlan, enqueue, set_eth_src, set_ipv4_src, output
  };
  v10::timeouts timeouts{0x3232, 0x8989};
  std::uint16_t flags
    = protocol::OFPFF_SEND_FLOW_REM | protocol::OFPFF_CHECK_OVERLAP;
  std::uint32_t buffer_id = 0x01020304;
  std::uint32_t xid = 0x12345678;
};
struct flow_modify_fixture : parameters {
  msg::flow_modify sut{match, cookie, actions, buffer_id, xid};
  std::vector<unsigned char> bin
    = "\x01\x0e\x00\x90\x12\x34\x56\x78"
      "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x18\x23\x00\x00"
      "\x01\x02\x03\x04\x05\x06\x07\x08""\x00\x01\x00\x00\x00\x00\x00\x00"
      "\x01\x02\x03\x04\x00\x00\x00\x00"
      "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"
      "\x00\x03\x00\x08\x00\x00\x00\x00""\x00\x0b\x00\x10\x00\x01\x00\x00"
      "\x00\x00\x00\x00\x12\x34\x56\x78""\x00\x04\x00\x10\x01\x02\x03\x04"
      "\x05\x06\x00\x00\x00\x00\x00\x00""\x00\x06\x00\x08\xc0\xa8\x01\x03"
      "\x00\x00\x00\x08\xff\xfd\x00\x40"
      ""_bin;
};
struct flow_modify_strict_fixture : parameters {
  msg::flow_modify_strict sut{match, priority, cookie, actions, buffer_id, xid};
  std::vector<unsigned char> bin
    = "\x01\x0e\x00\x90\x12\x34\x56\x78"
      "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x18\x23\x00\x00"
      "\x01\x02\x03\x04\x05\x06\x07\x08""\x00\x02\x00\x00\x00\x00\x43\x21"
      "\x01\x02\x03\x04\x00\x00\x00\x00"
      "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"
      "\x00\x03\x00\x08\x00\x00\x00\x00""\x00\x0b\x00\x10\x00\x01\x00\x00"
      "\x00\x00\x00\x00\x12\x34\x56\x78""\x00\x04\x00\x10\x01\x02\x03\x04"
      "\x05\x06\x00\x00\x00\x00\x00\x00""\x00\x06\x00\x08\xc0\xa8\x01\x03"
      "\x00\x00\x00\x08\xff\xfd\x00\x40"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(flow_modify)

  BOOST_AUTO_TEST_SUITE(command)
    BOOST_AUTO_TEST_CASE(returns_flow_modify_command)
    {
      BOOST_TEST(msg::flow_modify::command() == protocol::OFPFC_MODIFY);
    }
  BOOST_AUTO_TEST_SUITE_END() // command

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_match_and_cookie_and_actions, parameters)
    {
      msg::flow_modify sut{match, cookie, actions};

      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.priority() == 0);
      BOOST_TEST((sut.timeouts() == v10::timeouts{0, 0}));
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
    }
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_all_parameters, parameters)
    {
      msg::flow_modify sut{
        match, priority, cookie, actions, timeouts, flags, buffer_id, xid
      };

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + actions.length());
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
      auto const new_actions = v10::action_list{ set_vlan_vid, output };

      msg::flow_modify sut{entry, new_actions, timeouts, flags, buffer_id, xid};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + new_actions.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == entry.id().match()));
      BOOST_TEST(sut.priority() == entry.id().priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST((sut.actions() == new_actions));
      BOOST_TEST((sut.timeouts() == timeouts));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.buffer_id() == buffer_id);
    }
    BOOST_FIXTURE_TEST_CASE(
          is_move_constructible_and_makes_moved_objects_actions_empty
        , flow_modify_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_mod));
      BOOST_TEST((moved.actions() == v10::action_list{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(encode, flow_modify_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_modify_fixture)
    BOOST_AUTO_TEST_CASE(constructs_flow_modify_from_binary)
    {
      auto it = bin.begin();

      auto const flow_modify = msg::flow_modify::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_modify == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_modify


BOOST_AUTO_TEST_SUITE(flow_modify_strict)

  BOOST_AUTO_TEST_SUITE(command)
    BOOST_AUTO_TEST_CASE(returns_flow_modify_strict_command)
    {
      BOOST_TEST(
          msg::flow_modify_strict::command() == protocol::OFPFC_MODIFY_STRICT);
    }
  BOOST_AUTO_TEST_SUITE_END() // command

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_match_and_priority_and_cookie_and_actions
        , parameters)
    {
      msg::flow_modify_strict sut{match, priority, cookie, actions};

      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST((sut.timeouts() == v10::timeouts{0, 0}));
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
    }
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_all_parameters, parameters)
    {
      msg::flow_modify_strict sut{
        match, priority, cookie, actions, timeouts, flags, buffer_id, xid
      };

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + actions.length());
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
      auto const new_actions = v10::action_list{ set_vlan_vid, output };

      msg::flow_modify_strict sut{
        entry, new_actions, timeouts, flags, buffer_id, xid
      };

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + new_actions.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == entry.id().match()));
      BOOST_TEST(sut.priority() == entry.id().priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST((sut.actions() == new_actions));
      BOOST_TEST((sut.timeouts() == timeouts));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.buffer_id() == buffer_id);
    }
    BOOST_FIXTURE_TEST_CASE(
          is_move_constructible_and_makes_moved_objects_actions_empty
        , flow_modify_strict_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_mod));
      BOOST_TEST((moved.actions() == v10::action_list{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(encode, flow_modify_strict_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_modify_strict_fixture)
    BOOST_AUTO_TEST_CASE(constructs_flow_modify_from_binary)
    {
      auto it = bin.begin();

      auto const flow_modify_strict
        = msg::flow_modify_strict::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_modify_strict == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_modify_strict

BOOST_AUTO_TEST_SUITE_END() // message_test
