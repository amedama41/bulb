#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/flow_removed.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace fields = v13::oxm_match_fields;
namespace protocol = v13::protocol;

namespace {

  struct oxm_match_fixture {
    fields::in_port in_port{1};
    fields::eth_dst eth_dst{"\x01\x02\x03\x04\x05\x06"_mac};
    fields::eth_src eth_src{"\x11\x12\x13\x14\x15\x16"_mac};
    fields::eth_type eth_type{0x0800, 0x0800};
    v13::oxm_match const match{in_port, eth_dst, eth_src, eth_type};
  };
  struct flow_removed_parameter : oxm_match_fixture {
    std::uint16_t priority = 0xff00;
    std::uint64_t cookie = 0xf010f010f010f010;
    protocol::flow_removed_reason reason = protocol::OFPRR_DELETE;
    std::uint8_t table_id = protocol::OFPTT_MAX;
    v13::elapsed_time elapsed_time{0x10, 0xff0011};
    v13::timeouts timeouts{0xff3e, 0xff13};
    v13::counters counters{0xfedcba, 0xfedcba01};
    std::uint32_t xid = 0x12345678;
  };
  struct flow_removed_fixutre : flow_removed_parameter {
    v13::messages::flow_removed sut{
        match, priority, cookie, reason, table_id
      , elapsed_time, timeouts, counters, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x0b\x00\x58\x12\x34\x56\x78" "\xf0\x10\xf0\x10\xf0\x10\xf0\x10"
        "\xff\x00\x02\xfe\x00\x00\x00\x10" "\x00\xff\x00\x11\xff\x3e\xff\x13"
        "\x00\x00\x00\x00\x00\xfe\xdc\xba" "\x00\x00\x00\x00\xfe\xdc\xba\x01"
        "\x00\x01\x00\x28\x80\x00\x00\x04" "\x00\x00\x00\x01\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00" "\x08\x06\x11\x12\x13\x14\x15\x16"
        "\x80\x00\x0b\x04\x08\x00\x08\x00"_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(flow_removed)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_flow_entry, oxm_match_fixture)
    {
      auto const entry = v13::flow_entry{
          match
        , protocol::OFP_DEFAULT_PRIORITY
        , 0xff00ff0012345678
        , v13::instruction_set{}
      };
      auto const reason = protocol::OFPRR_HARD_TIMEOUT;
      auto const table_id = 1;
      auto const elapsed_time = v13::elapsed_time{1, 2};
      auto const timeouts = v13::timeouts{10, 30};
      auto const counters = v13::counters{32, 32 * 1500};
      auto const xid = 0xff001200;

      v13::messages::flow_removed const sut{
        entry, reason, table_id, elapsed_time, timeouts, counters, xid
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_REMOVED);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_removed) + 40);
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.duration_sec() == elapsed_time.duration_sec());
      BOOST_TEST(sut.duration_nsec() == elapsed_time.duration_nsec());
      BOOST_TEST(sut.idle_timeout() == timeouts.idle_timeout());
      BOOST_TEST(sut.hard_timeout() == timeouts.hard_timeout());
      BOOST_TEST(sut.packet_count() == counters.packet_count());
      BOOST_TEST(sut.byte_count() == counters.byte_count());
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, flow_removed_fixutre)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, flow_removed_fixutre)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length()
          == sizeof(protocol::ofp_flow_removed) + sizeof(protocol::ofp_match));
      BOOST_TEST(moved.match().length() == 4);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, flow_removed_fixutre)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = v13::messages::flow_removed{
          v13::oxm_match{}, 0, 0, protocol::OFPRR_IDLE_TIMEOUT, 0
        , v13::elapsed_time{0, 0}, v13::timeouts{0, 0}, v13::counters{0, 0}
      };

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto moved = sut;
      auto copy = v13::messages::flow_removed{
          v13::oxm_match{}, 0, 0, protocol::OFPRR_IDLE_TIMEOUT, 0
        , v13::elapsed_time{0, 0}, v13::timeouts{0, 0}, v13::counters{0, 0}
      };

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_removed)
          + sizeof(protocol::ofp_match));
      BOOST_TEST(moved.match().length() == 4);
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, flow_removed_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::flow_removed{
          match, priority, cookie, reason, table_id
        , elapsed_time, timeouts, counters, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, timeouts, counters, xid
           }
        == v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, timeouts, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               v13::oxm_match{eth_src}, priority, cookie, reason, table_id
             , elapsed_time, timeouts, counters, xid
           }
        != v13::messages::flow_removed{
               v13::oxm_match{eth_dst}, priority, cookie, reason, table_id
             , elapsed_time, timeouts, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_priority_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, 1, cookie, reason, table_id
             , elapsed_time, timeouts, counters, xid
           }
        != v13::messages::flow_removed{
               match, 2, cookie, reason, table_id
             , elapsed_time, timeouts, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, 1, reason, table_id
             , elapsed_time, timeouts, counters, xid
           }
        != v13::messages::flow_removed{
               match, priority, 2, reason, table_id
             , elapsed_time, timeouts, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_reason_is_not_equal)
    {
      using reason = protocol::flow_removed_reason;

      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason::by_idle_timeout, table_id
             , elapsed_time, timeouts, counters, xid
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason::by_hard_timeout, table_id
             , elapsed_time, timeouts, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, 1
             , elapsed_time, timeouts, counters, xid
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason, 2
             , elapsed_time, timeouts, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_sec_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , v13::elapsed_time{1, 0}, timeouts, counters, xid
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , v13::elapsed_time{2, 0}, timeouts, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_nsec_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , v13::elapsed_time{0, 2}, timeouts, counters, xid
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , v13::elapsed_time{0, 1}, timeouts, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_idle_timeout_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, v13::timeouts{1, 0}, counters, xid
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, v13::timeouts{2, 0}, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_hard_timeout_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, v13::timeouts{0, 1}, counters, xid
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, v13::timeouts{0, 2}, counters, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_count_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, timeouts, v13::counters{1, 0}, xid
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, timeouts, v13::counters{2, 0}, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_byte_count_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, timeouts, v13::counters{0, 1}, xid
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, timeouts, v13::counters{0, 2}, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, timeouts, counters, 1
           }
        != v13::messages::flow_removed{
               match, priority, cookie, reason, table_id
             , elapsed_time, timeouts, counters, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_removed_fixutre)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, flow_removed_fixutre)
    {
      auto it = bin.begin();

      auto flow_removed = v13::messages::flow_removed::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_removed == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // flow_removed

BOOST_AUTO_TEST_SUITE_END() // message_test

