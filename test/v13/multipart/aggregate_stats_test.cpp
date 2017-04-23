#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/aggregate_stats.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/common/oxm_match.hpp>
#include <canard/net/ofp/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace match = v13::oxm_match_fields;
namespace multipart = v13::messages::multipart;
namespace protocol = v13::protocol;

namespace {

  struct aggregate_stats_request_parameter {
    v13::oxm_match match{
      match::in_port{4}, match::eth_dst{"\x01\x02\x03\x04\x05\x06"_mac}
    }; // 4 + 8 + 10 = 22
    std::uint8_t table_id = protocol::OFPTT_ALL;
    v13::cookie_mask cookie_mask{0xf1f2f3f4f5f6f7f8, 0x0f0f0f0f0f0f0f0f};
    std::uint32_t out_port = protocol::OFPP_TABLE;
    std::uint32_t out_group = protocol::OFPG_MAX;
    std::uint32_t xid = 0x12345678;
  };
  struct aggregate_stats_request_fixture : aggregate_stats_request_parameter {
    multipart::aggregate_stats_request sut{
      match, table_id, cookie_mask, out_port, out_group, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x48\x12\x34\x56\x78""\x00\x02\x00\x00\x00\x00\x00\x00"
        "\xff\x00\x00\x00\xff\xff\xff\xf9""\xff\xff\xff\x00\x00\x00\x00\x00"
        "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8""\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f"
        "\x00\x01\x00\x16\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x00\x00"_bin
        ;
  };

  struct aggregate_stats_reply_parameter {
    std::uint64_t packet_count = 0xf1f2f3f4f5f6f7f8;
    std::uint64_t byte_count = 0xffff0000ffff0000;
    v13::counters counters{packet_count, byte_count};
    std::uint32_t flow_count = 0x87654321;
    std::uint32_t xid = 0x1f3f5f7f;
  };
  struct aggregate_stats_reply_fixture : aggregate_stats_reply_parameter {
    multipart::aggregate_stats_reply sut{counters, flow_count, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\x28\x1f\x3f\x5f\x7f""\x00\x02\x00\x00\x00\x00\x00\x00"
        "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8""\xff\xff\x00\x00\xff\xff\x00\x00"
        "\x87\x65\x43\x21\x00\x00\x00\x00"_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(aggregate_stats_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_match)
    {
      auto const match = v13::oxm_match{
          match::in_port{4}
        , match::eth_dst{"\x01\x02\x03\x04\x05\x06"_mac}
        , match::eth_src{"\x11\x12\x13\x14\x15\x16"_mac}
      }; // 4 + 8 + 10 + 10 = 32
      auto const table_id = std::uint8_t{1};

      multipart::aggregate_stats_request const sut{match, table_id};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_aggregate_stats_request)
          + 32);
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_AGGREGATE);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.out_port() == protocol::OFPP_ANY);
      BOOST_TEST(sut.out_group() == protocol::OFPG_ANY);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_cookie_mask)
    {
      auto const match = v13::oxm_match{match::in_port{4}}; // 4 + 8 = 12
      auto const table_id = std::uint8_t{protocol::OFPTT_ALL};
      auto const cookie_mask = v13::cookie_mask{32, 64};

      multipart::aggregate_stats_request const sut{
        match, table_id, cookie_mask
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_aggregate_stats_request)
          + 16);
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_AGGREGATE);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == cookie_mask.value());
      BOOST_TEST(sut.cookie_mask() == cookie_mask.mask());
      BOOST_TEST(sut.out_port() == protocol::OFPP_ANY);
      BOOST_TEST(sut.out_group() == protocol::OFPG_ANY);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_out_port)
    {
      auto const match = v13::oxm_match{}; // 4
      auto const table_id = std::uint8_t{254};
      auto const out_port = std::uint32_t{protocol::OFPP_CONTROLLER};
      auto const out_group = std::uint32_t{1};

      multipart::aggregate_stats_request const sut{
        match, table_id, out_port, out_group
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_aggregate_stats_request)
          + 8);
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_AGGREGATE);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.out_port() == out_port);
      BOOST_TEST(sut.out_group() == out_group);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, aggregate_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, aggregate_stats_request_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_aggregate_stats_request)
          + 8);
      BOOST_TEST((moved.match() == v13::oxm_match{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, aggregate_stats_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::aggregate_stats_request{
        match, table_id, cookie_mask, out_port, out_group, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_request{
             match, table_id, cookie_mask, out_port, out_group, xid
           }
       == multipart::aggregate_stats_request{
             match, table_id, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_request{
               v13::oxm_match{match::tcp_src{1}}
             , table_id, cookie_mask, out_port, out_group, xid
           }
       != multipart::aggregate_stats_request{
               v13::oxm_match{match::tcp_src{2}}
             , table_id, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_request{
             match, 1, cookie_mask, out_port, out_group, xid
           }
       != multipart::aggregate_stats_request{
             match, 2, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_request{
               match, table_id, v13::cookie_mask{1, 0}
             , out_port, out_group, xid
           }
       != multipart::aggregate_stats_request{
               match, table_id, v13::cookie_mask{2, 0}
             , out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_mask_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_request{
               match, table_id, v13::cookie_mask{0, 1}
             , out_port, out_group, xid
           }
       != multipart::aggregate_stats_request{
               match, table_id, v13::cookie_mask{0, 2}
             , out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_out_port_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_request{
             match, table_id, cookie_mask, 1, out_group, xid
           }
       != multipart::aggregate_stats_request{
             match, table_id, cookie_mask, 2, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_out_group_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_request{
             match, table_id, cookie_mask, out_port, 1, xid
           }
       != multipart::aggregate_stats_request{
             match, table_id, cookie_mask, out_port, 2, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_request{
             match, table_id, cookie_mask, out_port, out_group, 1
           }
       != multipart::aggregate_stats_request{
             match, table_id, cookie_mask, out_port, out_group, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(
        generate_binary, aggregate_stats_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        construct_from_binary, aggregate_stats_request_fixture)
    {
      auto it = bin.begin();

      auto const aggregate_stats_request
        = multipart::aggregate_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((aggregate_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // aggregate_stats_request


BOOST_AUTO_TEST_SUITE(aggregate_stats_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const counters = v13::counters{32, 48};
      auto const flow_count = std::uint32_t{82};

      multipart::aggregate_stats_reply const sut{counters, flow_count};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_reply)
          + sizeof(protocol::ofp_aggregate_stats_reply));
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_AGGREGATE);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.packet_count() == counters.packet_count());
      BOOST_TEST(sut.byte_count() == counters.byte_count());
      BOOST_TEST(sut.flow_count() == flow_count);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, aggregate_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, aggregate_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sut.length());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, aggregate_stats_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::aggregate_stats_reply{
        counters, flow_count, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_reply{counters, flow_count, xid}
        == multipart::aggregate_stats_reply{counters, flow_count, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_reply{
            v13::counters{1, byte_count}, flow_count, xid}
        != multipart::aggregate_stats_reply{
            v13::counters{2, byte_count}, flow_count, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_byte_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_reply{
            v13::counters{packet_count, 1}, flow_count, xid}
        != multipart::aggregate_stats_reply{
            v13::counters{packet_count, 2}, flow_count, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flow_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_reply{counters, 1, xid}
        != multipart::aggregate_stats_reply{counters, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::aggregate_stats_reply{counters, flow_count, 1}
        != multipart::aggregate_stats_reply{counters, flow_count, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(
        generate_binary, aggregate_stats_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        construct_from_binary, aggregate_stats_reply_fixture)
    {
      auto it = bin.begin();

      auto const aggregate_stats_reply
        = multipart::aggregate_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((aggregate_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // aggregate_stats_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
