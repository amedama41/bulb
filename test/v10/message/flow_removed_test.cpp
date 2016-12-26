#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/flow_removed.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace detail = v10::v10_detail;

namespace proto = v10::protocol;

namespace {
struct parameters : match_fixture {
  v10::match match{
    in_port, eth_src, vlan_vid, eth_type, ip_proto, ipv4_dst, tcp_src
  };
  std::uint16_t priority = 0x1024;
  std::uint64_t cookie = 0x0102030405060708;
  proto::ofp_flow_removed_reason reason = proto::OFPRR_DELETE;
  v10::elapsed_time elapsed_time{0x10203040, 0x50607080};
  std::uint16_t idle_timeout = 0x1200;
  v10::counters counters{0x0101010140302010, 0x0101010144332211};
  std::uint32_t xid = 0x12345678;
};
struct flow_removed_fixture : parameters {
  msg::flow_removed sut{
    match, priority, cookie, reason, elapsed_time, idle_timeout, counters, xid
  };
  std::vector<unsigned char> bin
    = "\x01\x0b\x00\x58\x12\x34\x56\x78"
      "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x18\x23\x00\x00"
      "\x01\x02\x03\x04\x05\x06\x07\x08""\x10\x24\x02\x00\x10\x20\x30\x40"
      "\x50\x60\x70\x80\x12\x00\x00\x00""\x01\x01\x01\x01\x40\x30\x20\x10"
      "\x01\x01\x01\x01\x44\x33\x22\x11"_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(flow_removed)

  BOOST_FIXTURE_TEST_SUITE(constructor, parameters)
    BOOST_AUTO_TEST_CASE(is_constructible_from_match_and_priority_and_cookie)
    {
      msg::flow_removed sut{
          match, priority, cookie, reason
        , elapsed_time, idle_timeout, counters, xid
      };

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_removed));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST((sut.elapsed_time() == elapsed_time));
      BOOST_TEST(sut.idle_timeout() == idle_timeout);
      BOOST_TEST((sut.counters() == counters));
    }
    BOOST_AUTO_TEST_CASE(
        is_constructible_from_match_and_priority_and_cookie_without_xid)
    {
      msg::flow_removed sut{
        match, priority, cookie, reason, elapsed_time, idle_timeout, counters
      };

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_removed));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST((sut.elapsed_time() == elapsed_time));
      BOOST_TEST(sut.idle_timeout() == idle_timeout);
      BOOST_TEST((sut.counters() == counters));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_flow_entry)
    {
      auto const entry
        = v10::flow_entry{{match, priority}, cookie, v10::action_list{}};

      msg::flow_removed sut{
        entry, reason, elapsed_time, idle_timeout, counters, xid
      };

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_removed));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST((sut.elapsed_time() == elapsed_time));
      BOOST_TEST(sut.idle_timeout() == idle_timeout);
      BOOST_TEST((sut.counters() == counters));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_flow_entry_without_xid)
    {
      auto const entry
        = v10::flow_entry{{match, priority}, cookie, v10::action_list{}};

      msg::flow_removed sut{
        entry, reason, elapsed_time, idle_timeout, counters
      };

      BOOST_TEST(sut.length() == sizeof(detail::ofp_flow_removed));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST((sut.elapsed_time() == elapsed_time));
      BOOST_TEST(sut.idle_timeout() == idle_timeout);
      BOOST_TEST((sut.counters() == counters));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, flow_removed_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, flow_removed_fixture)
    {
      auto const& const_flow_removed = sut;

      auto const copy = const_flow_removed;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::flow_removed{
          match, priority, cookie, reason
        , elapsed_time, idle_timeout, counters, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_parameters_are_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               match, priority, cookie, reason
             , elapsed_time, idle_timeout, counters, xid
           }
        == msg::flow_removed{
               match, priority, cookie, reason
             , elapsed_time, idle_timeout, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               v10::match{ipv4_dst}, priority, cookie, reason
             , elapsed_time, idle_timeout, counters, xid
           }
        != msg::flow_removed{
               v10::match{ipv4_src}, priority, cookie, reason
             , elapsed_time, idle_timeout, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_priority_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               match, 1, cookie, reason
             , elapsed_time, idle_timeout, counters, xid
           }
        != msg::flow_removed{
               match, 2, cookie, reason
             , elapsed_time, idle_timeout, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               match, priority, 1, reason
             , elapsed_time, idle_timeout, counters, xid
           }
        != msg::flow_removed{
               match, priority, 2, reason
             , elapsed_time, idle_timeout, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_reason_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               match, priority, cookie, proto::OFPRR_IDLE_TIMEOUT
             , elapsed_time, idle_timeout, counters, xid
           }
        != msg::flow_removed{
               match, priority, cookie, proto::OFPRR_HARD_TIMEOUT
             , elapsed_time, idle_timeout, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_elapsed_time_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               match, priority, cookie, reason
             , v10::elapsed_time{1, 1}, idle_timeout, counters, xid
           }
        != msg::flow_removed{
               match, priority, cookie, reason
             , v10::elapsed_time{1, 2}, idle_timeout, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_idle_timeout_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               match, priority, cookie, reason
             , elapsed_time, 1, counters, xid
           }
        != msg::flow_removed{
               match, priority, cookie, reason
             , elapsed_time, 2, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_counters_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               match, priority, cookie, reason
             , elapsed_time, idle_timeout, v10::counters{1, 1}, xid
           }
        != msg::flow_removed{
               match, priority, cookie, reason
             , elapsed_time, idle_timeout, v10::counters{1, 2}, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_removed{
               match, priority, cookie, reason
             , elapsed_time, idle_timeout, counters, 1
           }
        != msg::flow_removed{
               match, priority, cookie, reason
             , elapsed_time, idle_timeout, counters, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, flow_removed_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_removed_fixture)
    BOOST_AUTO_TEST_CASE(constructs_flow_removed_from_binary)
    {
      auto it = bin.begin();

      auto const flow_removed = msg::flow_removed::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_removed == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_removed
BOOST_AUTO_TEST_SUITE_END() // message_test
