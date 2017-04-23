#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/table_stats.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace multipart = v13::messages::multipart;
namespace protocol = v13::protocol;

namespace {

  using body_type = multipart::table_stats_reply::body_type;

  struct table_stats_parameter {
    std::uint8_t table_id = protocol::OFPTT_MAX;
    std::uint32_t active_count = 0x12345678;
    std::uint64_t lookup_count = 0xf1f2f3f4f5f6f7f8;
    std::uint64_t matched_count = 0x0102030405060708;
  };
  struct table_stats_fixture : table_stats_parameter {
    multipart::table_stats sut{
      table_id, active_count, lookup_count, matched_count
    };
    std::vector<std::uint8_t> bin
      = "\xfe\x00\x00\x00\x12\x34\x56\x78""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x01\x02\x03\x04\x05\x06\x07\x08"_bin
        ;
    multipart::table_stats& table_stats = sut;
  };

  struct table_stats_request_fixture {
    multipart::table_stats_request sut{0x12345678};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x10\x12\x34\x56\x78""\x00\x03\x00\x00\x00\x00\x00\x00"_bin
        ;
  };

  struct table_stats_reply_parameter : table_stats_fixture {
    ::body_type body{3, table_stats};
    std::uint16_t flags = protocol::OFPMPF_REPLY_MORE;
    std::uint32_t xid = 0x12345678;
  };
  struct table_stats_reply_fixture : table_stats_reply_parameter {
    multipart::table_stats_reply sut{body, flags, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\x58\x12\x34\x56\x78""\x00\x03\x00\x01\x00\x00\x00\x00"
        "\xfe\x00\x00\x00\x12\x34\x56\x78""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x01\x02\x03\x04\x05\x06\x07\x08"
        "\xfe\x00\x00\x00\x12\x34\x56\x78""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x01\x02\x03\x04\x05\x06\x07\x08"
        "\xfe\x00\x00\x00\x12\x34\x56\x78""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x01\x02\x03\x04\x05\x06\x07\x08"_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(table_stats)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const table_id = std::uint8_t{protocol::OFPTT_ALL};
      auto const active_count = std::uint32_t{43};
      auto const lookup_count = std::uint32_t{83232};
      auto const matched_count = std::uint32_t{4323};

      multipart::table_stats const sut{
        table_id, active_count, lookup_count, matched_count
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_table_stats));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.active_count() == active_count);
      BOOST_TEST(sut.lookup_count() == lookup_count);
      BOOST_TEST(sut.matched_count() == matched_count);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, table_stats_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, table_stats_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, table_stats_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::table_stats{
        table_id, active_count, lookup_count, matched_count
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::table_stats{
             table_id, active_count, lookup_count, matched_count
           }
        == multipart::table_stats{
             table_id, active_count, lookup_count, matched_count
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::table_stats{
             1, active_count, lookup_count, matched_count
           }
        != multipart::table_stats{
             2, active_count, lookup_count, matched_count
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_active_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::table_stats{
             table_id, 1, lookup_count, matched_count
           }
        != multipart::table_stats{
             table_id, 2, lookup_count, matched_count
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_lookup_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::table_stats{
             table_id, active_count, 1, matched_count
           }
        != multipart::table_stats{
             table_id, active_count, 2, matched_count
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_matched_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::table_stats{
             table_id, active_count, lookup_count, 1
           }
        != multipart::table_stats{
             table_id, active_count, lookup_count, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, table_stats_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, table_stats_fixture)
    {
      auto it = bin.begin();

      auto const table_stats = multipart::table_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((table_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // table_stats


BOOST_AUTO_TEST_SUITE(table_stats_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      multipart::table_stats_request const sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_TABLE);
      BOOST_TEST(sut.flags() == 0);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, table_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, table_stats_request_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::table_stats_request{1};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = 23;

      BOOST_TEST(
          (multipart::table_stats_request{xid}
        == multipart::table_stats_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::table_stats_request{1}
        != multipart::table_stats_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, table_stats_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, table_stats_request_fixture)
    {
      auto it = bin.begin();

      auto const table_stats_request
        = multipart::table_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((table_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // table_stats_request


BOOST_AUTO_TEST_SUITE(table_stats_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible, table_stats_fixture)
    {
      auto const size = 5;
      auto const body = ::body_type(size, table_stats);

      multipart::table_stats_reply const sut{body};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_reply)
          + sizeof(protocol::ofp_table_stats) * size);
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_TABLE);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(constructible_with_flag, table_stats_fixture)
    {
      auto const size = 0;
      auto const body = ::body_type(size, table_stats);
      auto const flags = std::uint16_t(protocol::OFPMPF_REPLY_MORE);

      multipart::table_stats_reply const sut{body, flags};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_reply)
          + sizeof(protocol::ofp_table_stats) * size);
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_TABLE);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, table_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, table_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, table_stats_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::table_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::table_stats_reply{body, flags, xid}
        == multipart::table_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      auto const table_stats1 = multipart::table_stats{1, 1, 1, 1};
      auto const table_stats2 = multipart::table_stats{2, 2, 2, 2};

      BOOST_TEST(
          (multipart::table_stats_reply{body_type{table_stats1}, flags, xid}
        != multipart::table_stats_reply{body_type{table_stats2}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::table_stats_reply{body, 0, xid}
        != multipart::table_stats_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::table_stats_reply{body, flags, 1}
        != multipart::table_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, table_stats_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, table_stats_reply_fixture)
    {
      auto it = bin.begin();

      auto const table_stats_reply
        = multipart::table_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((table_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // table_stats_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
