#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/message/stats/aggregate_stats.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace stats = msg::statistics;
namespace protocol = v10::protocol;

namespace {
struct aggregate_stats_request_parameter : match_fixture {
  v10::match match{ in_port, eth_src, eth_type, ipv4_dst };
  std::uint8_t table_id = 0x56;
  std::uint16_t out_port = 0x0123;
  std::uint32_t xid = 0x11223344;
};
struct aggregate_stats_request_fixture : aggregate_stats_request_parameter {
  stats::aggregate_stats_request sut{match, table_id, out_port, xid};
  std::vector<unsigned char> bin
    = "\x01\x10\x00\x38\x11\x22\x33\x44""\x00\x02\x00\x00"
      "\x00\x31\x3f\xea\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x08\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\xc0\xa8\x01\x00\x00\x00\x00\x00"
      "\x56\x00\x01\x23"_bin;
};
struct aggregate_stats_reply_parameter {
  v10::counters counters{0x1122334455667788, 0x0102030405060708};
  std::uint32_t flow_count = 0x02040608;
  std::uint32_t xid = 0x11223344;
};
struct aggregate_stats_reply_fixture : aggregate_stats_reply_parameter {
  stats::aggregate_stats_reply sut{counters, flow_count, xid};
  std::vector<unsigned char> bin
    = "\x01\x11\x00\x24\x11\x22\x33\x44""\x00\x02\x00\x00"
      "\x11\x22\x33\x44\x55\x66\x77\x88""\x01\x02\x03\x04\x05\x06\x07\x08"
      "\x02\x04\x06\x08\x00\x00\x00\x00"_bin;
};
}

BOOST_AUTO_TEST_SUITE(stats_test)

BOOST_AUTO_TEST_SUITE(aggregate_stats_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_parameters, match_fixture)
    {
      auto const match = v10::match{ in_port, eth_src, eth_dst };
      auto const table_id = 0x12;
      auto const out_port = 0x5678;
      auto const xid = 0x11223344;

      stats::aggregate_stats_request sut{match, table_id, out_port, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request)
                               + sizeof(protocol::ofp_aggregate_stats_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.out_port() == out_port);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_parameters_without_xid, match_fixture)
    {
      auto const match = v10::match{ in_port, eth_src, eth_dst };
      auto const table_id = 0x12;
      auto const out_port = 0x5678;

      stats::aggregate_stats_request sut{match, table_id, out_port};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request)
                               + sizeof(protocol::ofp_aggregate_stats_request));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.out_port() == out_port);
    }
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_parameters_without_out_port_and_xid
        , match_fixture)
    {
      auto const match = v10::match{ in_port, eth_src, eth_dst };
      auto const table_id = 0x12;

      stats::aggregate_stats_request sut{match, table_id};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request)
                               + sizeof(protocol::ofp_aggregate_stats_request));
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.out_port() == protocol::OFPP_NONE);
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_non_const_lvalue
        , aggregate_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_const_lvalue
        , aggregate_stats_request_fixture)
    {
      auto const& const_aggregate_stats_request = sut;

      auto const copy = const_aggregate_stats_request;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, aggregate_stats_request_parameter)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut
        = stats::aggregate_stats_request{match, table_id, out_port, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_request{match, table_id, out_port, xid}
        == stats::aggregate_stats_request{match, table_id, out_port, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_request{
            v10::match{ ipv4_dst }, table_id, out_port, xid}
        != stats::aggregate_stats_request{
            v10::match{ ipv4_src }, table_id, out_port, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_request{match, 1, out_port, xid}
        != stats::aggregate_stats_request{match, 2, out_port, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_out_port_is_not_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_request{match, table_id, 1, xid}
        != stats::aggregate_stats_request{match, table_id, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_request{match, table_id, out_port, 1}
        != stats::aggregate_stats_request{match, table_id, out_port, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, aggregate_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, aggregate_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_aggregate_stats_request_from_binary)
    {
      auto it = bin.begin();

      auto const aggregate_stats_request
        = stats::aggregate_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((aggregate_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // aggregate_stats_request


BOOST_AUTO_TEST_SUITE(aggregate_stats_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_parameters)
    {
      auto const counters = v10::counters{0x1, 0x2};
      auto const flow_count = 0x3;
      auto const xid = 0x88000088;

      stats::aggregate_stats_reply sut{counters, flow_count, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request)
                               + sizeof(protocol::ofp_aggregate_stats_reply));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.packet_count() == counters.packet_count());
      BOOST_TEST(sut.byte_count() == counters.byte_count());
      BOOST_TEST(sut.flow_count() == flow_count);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_parameters_without_xid)
    {
      auto const counters = v10::counters{0x1, 0x2};
      auto const flow_count = 0x3;

      stats::aggregate_stats_reply sut{counters, flow_count};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request)
                               + sizeof(protocol::ofp_aggregate_stats_reply));
      BOOST_TEST(sut.packet_count() == counters.packet_count());
      BOOST_TEST(sut.byte_count() == counters.byte_count());
      BOOST_TEST(sut.flow_count() == flow_count);
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_non_const_lvalue
        , aggregate_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, aggregate_stats_reply_fixture)
    {
      auto const& const_aggregate_stats_reply = sut;

      auto const copy = const_aggregate_stats_reply;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, aggregate_stats_reply_parameter)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::aggregate_stats_reply{counters, flow_count, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_reply{counters, flow_count, xid}
        == stats::aggregate_stats_reply{counters, flow_count, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_packet_count_is_not_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_reply{v10::counters{1, 1}, flow_count, xid}
        != stats::aggregate_stats_reply{v10::counters{2, 1}, flow_count, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_byte_count_is_not_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_reply{v10::counters{1, 1}, flow_count, xid}
        != stats::aggregate_stats_reply{v10::counters{1, 2}, flow_count, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flow_count_is_not_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_reply{counters, 1, xid}
        != stats::aggregate_stats_reply{counters, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::aggregate_stats_reply{counters, flow_count, 1}
        != stats::aggregate_stats_reply{counters, flow_count, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, aggregate_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, aggregate_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_aggregate_stats_reply_from_binary)
    {
      auto it = bin.begin();

      auto const aggregate_stats_reply
        = stats::aggregate_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((aggregate_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // aggregate_stats_reply

BOOST_AUTO_TEST_SUITE_END() // stats_test
