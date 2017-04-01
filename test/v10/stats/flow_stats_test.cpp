#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/message/stats/flow_stats.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace stats = msg::statistics;
namespace protocol = v10::protocol;

namespace {
struct flow_stats_parameters : match_fixture, action_fixture {
  v10::match match{ in_port, eth_src, eth_type, ip_proto, ipv4_dst };
  std::uint16_t priority = 0x4000;
  std::uint64_t cookie = 0x0102030405060708;
  v10::action_list actions{ set_vlan_vid, output };
  v10::flow_entry entry{{match, priority}, cookie, actions};
  std::uint8_t table_id = 0x34;
  v10::timeouts timeouts{0x1234, 0x2468};
  v10::elapsed_time elapsed_time{0x01020304, 0x02040608};
  v10::counters counters{0xa1a2a3a4a5a6a7a8, 0xb1b2b3b4b5b6b7b8};
  v10::flow_entry no_actions_entry{
      {match, priority}, cookie, v10::action_list{}
  };
};
struct flow_stats_fixure : flow_stats_parameters {
  stats::flow_stats sut{entry, table_id, timeouts, elapsed_time, counters};
  std::vector<unsigned char> bin
    = "\x00\x68\x34\x00"
      "\x00\x31\x3f\xca\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x00\x00\x00\x00"
      "\x01\x02\x03\x04\x02\x04\x06\x08""\x40\x00\x12\x34\x24\x68\x00\x00"
      "\x00\x00\x00\x00"
      "\x01\x02\x03\x04\x05\x06\x07\x08""\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8"
      "\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8"
      "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"
      ""_bin;
  stats::flow_stats no_actions_sut{
    no_actions_entry, table_id, timeouts, elapsed_time, counters
  };
  std::vector<unsigned char> no_actions_bin
    = "\x00\x58\x34\x00"
      "\x00\x31\x3f\xca\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x00\x00\x00\x00"
      "\x01\x02\x03\x04\x02\x04\x06\x08""\x40\x00\x12\x34\x24\x68\x00\x00"
      "\x00\x00\x00\x00"
      "\x01\x02\x03\x04\x05\x06\x07\x08""\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8"
      "\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8"
      "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"
      ""_bin;
};
struct flow_stats_request_parameters : match_fixture {
  v10::match match { in_port, eth_src, eth_type, ip_proto, ipv4_dst};
  std::uint8_t table_id = 0x34;
  std::uint16_t out_port = 0xffdd;
  std::uint32_t xid = 0x01020304;
};
struct flow_stats_request_fixture : flow_stats_request_parameters {
  stats::flow_stats_request sut{match, table_id, out_port, xid};
  std::vector<unsigned char> bin
    = "\x01\x10\x00\x38\x01\x02\x03\x04""\x00\x01\x00\x00"
      "\x00\x31\x3f\xca\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x00\x00\x00\x00"
      "\x34\x00\xff\xdd"_bin;
};
using body_type = stats::flow_stats_reply::body_type;
struct flow_stats_reply_parameters : match_fixture, action_fixture {
  stats::flow_stats stats1{
      v10::flow_entry{
          {v10::match{ eth_src, eth_type, ip_proto, ipv4_dst }, 0x4000}
        , 0x0102030405060708
        , v10::action_list{ set_vlan_vid, output }
      }
    , 0xa2
    , v10::timeouts{0x1111, 0x2222}
    , v10::elapsed_time{0x01020304, 0x02040608}
    , v10::counters{0xa1a2a3a4a5a6a7a8, 0xb1b2b3b4b5b6b7b8}
  };
  stats::flow_stats stats2{
      v10::flow_entry{
          {v10::match{ in_port }, 0x4001}
        , 0xf1f2f3f4f5f6f7f8
        , v10::action_list{}
      }
    , 0xb2
    , v10::timeouts{0x3333, 0x4444}
    , v10::elapsed_time{0x02020304, 0x04040608}
    , v10::counters{0xc1c2c3c4c5c6c7c8, 0xd1d2d3d4d5d6d7d8}
  };
  stats::flow_stats stats3{
      v10::flow_entry{
          {v10::match{}, 0x4002}
        , 0x01f203f405f607f8
        , v10::action_list{ output }
      }
    , 0xb2
    , v10::timeouts{0x5555, 0x6666}
    , v10::elapsed_time{0x00020304, 0x00040608}
    , v10::counters{0xe1e2e3e4e5e6e7e8, 0xf1f2f3f4f5f6f7f8}
  };
  body_type body{ stats1, stats2, stats3 };
  std::uint16_t flags = protocol::OFPSF_REPLY_MORE;
  std::uint32_t xid= 0x12345678;
};
struct flow_stats_reply_fixture : flow_stats_reply_parameters {
  stats::flow_stats_reply sut{body, flags, xid};
  std::vector<unsigned char> bin
    = "\x01\x11\x01\x2c\x12\x34\x56\x78""\x00\x01\x00\x01"

      "\x00\x68\xa2\x00"
      "\x00\x31\x3f\xcb\x00\x00\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x00\x00\x00\x00"
      "\x01\x02\x03\x04\x02\x04\x06\x08""\x40\x00\x11\x11\x22\x22\x00\x00"
      "\x00\x00\x00\x00"
      "\x01\x02\x03\x04\x05\x06\x07\x08""\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8"
      "\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8"
      "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"

      "\x00\x58\xb2\x00"
      "\x00\x3f\xff\xfe\x00\x01\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x02\x02\x03\x04\x04\x04\x06\x08""\x40\x01\x33\x33\x44\x44\x00\x00"
      "\x00\x00\x00\x00"
      "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8""\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8"
      "\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8"

      "\x00\x60\xb2\x00"
      "\x00\x3f\xff\xff\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x02\x03\x04\x00\x04\x06\x08""\x40\x02\x55\x55\x66\x66\x00\x00"
      "\x00\x00\x00\x00"
      "\x01\xf2\x03\xf4\x05\xf6\x07\xf8""\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8"
      "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
      "\x00\x00\x00\x08\xff\xfd\x00\x40"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(stats_test)

BOOST_AUTO_TEST_SUITE(flow_stats)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_parameters, flow_stats_parameters)
    {
      auto const entry = v10::flow_entry{
          {v10::match{ in_port }, 1}
        , 2
        , v10::action_list{ set_vlan_vid, output }
      };
      auto const table_id = 3;
      auto const timeouts = v10::timeouts{4, 5};
      auto const elapsed_time = v10::elapsed_time{6, 7};
      auto const counters = v10::counters{8, 9};

      stats::flow_stats sut{entry, table_id, timeouts, elapsed_time, counters};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_flow_stats) + entry.actions().length());
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST((sut.actions() == entry.actions()));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST((sut.timeouts() == timeouts));
      BOOST_TEST((sut.elapsed_time() == elapsed_time));
      BOOST_TEST((sut.counters() == counters));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, flow_stats_fixure)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, flow_stats_fixure)
    {
      auto const& const_flow_stats = sut;

      auto const copy = const_flow_stats;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, flow_stats_fixure)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_stats));
      BOOST_TEST(moved.actions().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, flow_stats_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::flow_stats{
        entry, table_id, timeouts, elapsed_time, counters
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              entry, table_id, timeouts, elapsed_time, counters}
        == stats::flow_stats{
              entry, table_id, timeouts, elapsed_time, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              v10::flow_entry{
                {v10::match{ in_port }, priority}, cookie, actions
              }, table_id, timeouts, elapsed_time, counters}
        != stats::flow_stats{
              v10::flow_entry{
                {v10::match{ eth_src }, priority}, cookie, actions
              }, table_id, timeouts, elapsed_time, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_priority_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              v10::flow_entry{
                {match, 1}, cookie, actions
              }, table_id, timeouts, elapsed_time, counters}
        != stats::flow_stats{
              v10::flow_entry{
                {match, 2}, cookie, actions
              }, table_id, timeouts, elapsed_time, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              v10::flow_entry{
                {match, priority}, 1, actions
              }, table_id, timeouts, elapsed_time, counters}
        != stats::flow_stats{
              v10::flow_entry{
                {match, priority}, 2, actions
              }, table_id, timeouts, elapsed_time, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_actions_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              v10::flow_entry{
                {match, priority}, cookie, v10::action_list{set_vlan_vid}
              }, table_id, timeouts, elapsed_time, counters}
        != stats::flow_stats{
              v10::flow_entry{
                {match, priority}, cookie, v10::action_list{set_vlan_pcp}
              }, table_id, timeouts, elapsed_time, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_idle_timeout_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              entry, table_id, v10::timeouts{1, 2}, elapsed_time, counters}
        != stats::flow_stats{
              entry, table_id, v10::timeouts{2, 2}, elapsed_time, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_hard_timeout_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              entry, table_id, v10::timeouts{1, 1}, elapsed_time, counters}
        != stats::flow_stats{
              entry, table_id, v10::timeouts{1, 2}, elapsed_time, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_duration_sec_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              entry, table_id, timeouts, v10::elapsed_time{1, 2}, counters}
        != stats::flow_stats{
              entry, table_id, timeouts, v10::elapsed_time{2, 2}, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_duration_nsec_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              entry, table_id, timeouts, v10::elapsed_time{1, 1}, counters}
        != stats::flow_stats{
              entry, table_id, timeouts, v10::elapsed_time{1, 2}, counters}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_packet_count_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              entry, table_id, timeouts, elapsed_time, v10::counters{1, 2}}
        != stats::flow_stats{
              entry, table_id, timeouts, elapsed_time, v10::counters{2, 2}}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_byte_count_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats{
              entry, table_id, timeouts, elapsed_time, v10::counters{1, 1}}
        != stats::flow_stats{
              entry, table_id, timeouts, elapsed_time, v10::counters{1, 2}}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, flow_stats_fixure)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_no_actions_binary_from_no_actions_flow_stats)
    {
      no_actions_bin.resize(sizeof(protocol::ofp_flow_stats));
      auto buf = std::vector<unsigned char>{};

      no_actions_sut.encode(buf);

      BOOST_TEST(buf.size() == no_actions_sut.length());
      BOOST_TEST(buf == no_actions_bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_stats_fixure)
    BOOST_AUTO_TEST_CASE(constructs_flow_stats_from_binary)
    {
      auto it = bin.begin();

      auto const flow_stats = stats::flow_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_stats == sut));
    }
    BOOST_AUTO_TEST_CASE(
        constructs_no_actions_flow_stats_from_no_actions_binary)
    {
      auto it = no_actions_bin.begin();

      auto const flow_stats
        = stats::flow_stats::decode(it, no_actions_bin.end());

      BOOST_TEST((it == std::next(
              no_actions_bin.begin(), sizeof(protocol::ofp_flow_stats))));
      BOOST_TEST((flow_stats == no_actions_sut));
    }
    BOOST_AUTO_TEST_CASE(throws_exception_if_length_is_too_small)
    {
      bin[1] = sizeof(protocol::ofp_flow_stats) - 1;
      auto it = bin.begin();
      using ex_error_type = v10::exception::ex_error_type;
      using ex_error_code = v10::exception::ex_error_code;

      BOOST_CHECK_EXCEPTION(
            stats::flow_stats::decode(it, bin.end())
          , v10::exception
          , [](v10::exception const& e) {
              return e.error_type() == ex_error_type::bad_stats_element
                  && e.error_code() == ex_error_code::bad_length;
            });
      BOOST_TEST(
          (it == std::next(bin.begin(), sizeof(protocol::ofp_flow_stats))));
    }
    BOOST_AUTO_TEST_CASE(throws_exception_if_length_is_too_large)
    {
      bin[1] = bin.size() + 1;
      auto it = bin.begin();

      BOOST_CHECK_EXCEPTION(
            stats::flow_stats::decode(it, bin.end())
          , v10::exception
          , [](v10::exception const& e) {
              return e.error_type() == protocol::error_type::bad_request
                  && e.error_code() == protocol::bad_request_code::bad_len;
            });
      BOOST_TEST(
          (it == std::next(bin.begin(), sizeof(protocol::ofp_flow_stats))));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_stats

BOOST_AUTO_TEST_SUITE(flow_stats_request)

  BOOST_FIXTURE_TEST_SUITE(constructor, match_fixture)
    BOOST_AUTO_TEST_CASE(is_constructible_from_parameters)
    {
      auto const match = v10::match{ in_port, eth_dst, eth_src };
      auto const table_id = 0x01;
      auto const out_port = 0x02;
      auto const xid = 0x12345678;

      stats::flow_stats_request sut{match, table_id, out_port, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_stats_request)
        + sizeof(protocol::ofp_flow_stats_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.out_port() == out_port);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_parameters_without_xid)
    {
      auto const match = v10::match{ eth_type, ip_proto, tcp_src };
      auto const table_id = 0x01;
      auto const out_port = 0x02;

      stats::flow_stats_request sut{match, table_id, out_port};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_stats_request)
        + sizeof(protocol::ofp_flow_stats_request));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.out_port() == out_port);
    }
    BOOST_AUTO_TEST_CASE(
        is_constructible_from_parameters_without_out_port_and_xid)
    {
      auto const match = v10::match{ eth_type, eth_src, eth_dst };
      auto const table_id = 0x01;

      stats::flow_stats_request sut{match, table_id};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_stats_request)
        + sizeof(protocol::ofp_flow_stats_request));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.out_port() == protocol::OFPP_NONE);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, flow_stats_request_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut
        = stats::flow_stats_request{match, table_id, out_port, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_request{match, table_id, out_port, xid}
        == stats::flow_stats_request{match, table_id, out_port, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_request{
            v10::match{ ip_proto }, table_id, out_port, xid}
        != stats::flow_stats_request{
            v10::match{ eth_type }, table_id, out_port, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_request{match, 1, out_port, xid}
        != stats::flow_stats_request{match, 2, out_port, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_out_port_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_request{match, table_id, 1, xid}
        != stats::flow_stats_request{match, table_id, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_request{match, table_id, out_port, 1}
        != stats::flow_stats_request{match, table_id, out_port, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, flow_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_flow_stats_request_from_binary)
    {
      auto it = bin.begin();

      auto const flow_stats_request
        = stats::flow_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_stats_request


BOOST_AUTO_TEST_SUITE(flow_stats_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_multiple_flow_stats, flow_stats_reply_parameters)
    {
      auto const flags = 0;
      auto const xid = 0x12345678;

      stats::flow_stats_reply sut{{ stats1, stats2 }, flags, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_stats_reply) + stats1.length() + stats2.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body_type{ stats1, stats2 }));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_single_flow_stats, flow_stats_reply_parameters)
    {
      auto const flags = protocol::OFPSF_REPLY_MORE;
      auto const xid = 0x01010202;

      stats::flow_stats_reply sut{{ stats3 }, flags, xid};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_stats_reply) + stats3.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body_type{ stats3 }));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_no_flow_stats)
    {
      auto const flags = 0;
      auto const xid = 0x11111111;

      stats::flow_stats_reply sut{{}, flags, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body_type{}));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_without_xid)
    {
      auto const flags = protocol::OFPSF_REPLY_MORE;

      stats::flow_stats_reply sut{{}, flags};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body_type{}));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, flow_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, flow_stats_reply_fixture)
    {
      auto const& const_flow_stats_reply = sut;

      auto const copy = const_flow_stats_reply;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, flow_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_stats_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, flow_stats_reply_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::flow_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_body_and_flags_and_xid_are_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_reply{body, flags, xid}
        == stats::flow_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_reply{{ stats1 }, flags, xid}
        != stats::flow_stats_reply{{ stats2 }, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_reply{body, 0, xid}
        != stats::flow_stats_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::flow_stats_reply{body, flags, 1}
        != stats::flow_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, flow_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(
        generates_no_flow_stats_binary_from_no_flow_stats_reply)
    {
      auto const sut = stats::flow_stats_reply{{}, flags, xid};
      auto buf = std::vector<unsigned char>{};
      constexpr auto size = sizeof(protocol::ofp_stats_reply);
      bin[2] = 0;
      bin[3] = size;
      bin.resize(size);

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, flow_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_flow_stats_reply_from_binary)
    {
      auto it = bin.begin();

      auto const flow_stats_reply
        = stats::flow_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_stats_reply == sut));
    }
    BOOST_AUTO_TEST_CASE(
        constructs_no_flow_stats_reply_from_no_flow_stats_binary)
    {
      constexpr auto size = sizeof(protocol::ofp_stats_reply);
      bin[2] = 0;
      bin[3] = size;
      auto it = bin.begin();

      auto const flow_stats_reply
        = stats::flow_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), size)));
      BOOST_TEST(flow_stats_reply.length() == size);
      BOOST_TEST(flow_stats_reply.body().empty());
    }
    BOOST_AUTO_TEST_CASE(throws_exception_if_length_is_too_small)
    {
      bin[2] = 0;
      bin[3]
        = sizeof(protocol::ofp_stats_reply) + sizeof(protocol::ofp_flow_stats) - 1;
      auto it = bin.begin();

      BOOST_CHECK_THROW(
          stats::flow_stats_reply::decode(it, bin.end()), std::runtime_error);
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_stats_reply

BOOST_AUTO_TEST_SUITE_END() // stats_test
