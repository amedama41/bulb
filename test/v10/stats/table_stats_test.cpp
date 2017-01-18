#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/stats/table_stats.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace stats = msg::statistics;
namespace protocol = v10::protocol;

namespace {
struct table_stats_parameters {
  std::uint8_t table_id = 0x11;
  std::string name = "table1";
  std::uint32_t wildcards
    = protocol::OFPFW_ALL & ~protocol::OFPFW_IN_PORT & ~protocol::OFPFW_DL_TYPE;
  std::uint32_t max_entries = 0xf0f0f0f0;
  std::uint32_t active_count = 0x0f0f0f0f;
  std::uint64_t lookup_count = 0xf1f2f3f4f5f6f7f8;
  std::uint64_t matched_count = 0x0102030405060708;
};
struct table_stats_fixture : table_stats_parameters {
  stats::table_stats sut{
      table_id, name, wildcards, max_entries
    , active_count, lookup_count, matched_count
  };
  std::vector<unsigned char> bin
    = "\x11\x00\x00\x00"
      "t" "a" "b" "l" "e" "1" "\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x3f\xff\xee\xf0\xf0\xf0\xf0""\x0f\x0f\x0f\x0f"
      "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8""\x01\x02\x03\x04\x05\x06\x07\x08"
      ""_bin;
};
struct table_stats_request_fixture {
  std::uint32_t xid = 0x12345678;
  stats::table_stats_request sut{xid};
  std::vector<unsigned char> bin
    = "\x01\x10\x00\x0c\x12\x34\x56\x78""\x00\x03\x00\x00"_bin;
};
using body_type = stats::table_stats_reply::body_type;
struct table_stats_reply_parameters {
  stats::table_stats stats1{
      0x11, "table1"
    , protocol::OFPFW_ALL & ~protocol::OFPFW_IN_PORT & ~protocol::OFPFW_DL_TYPE
    , 0xf0f0f0f0, 0x0f0f0f0f, 0xf1f2f3f4f5f6f7f8, 0x0102030405060708
  };
  stats::table_stats stats2{
      0x12, "table2"
    , protocol::OFPFW_ALL
    , 0xe0e0e0e0, 0x0e0e0e0e, 0xe1e2e3e4e5e6e7e8, 0xa1a2a3a4a5a6a7a8
  };
  stats::table_stats stats3{
      0x13, "table3"
    , 0
    , 0xd0d0d0d0, 0x0d0d0d0d, 0xd1d2d3d4d5d6d7d8, 0xb1b2b3b4b5b6b7b8
  };
  body_type body{ stats1, stats2, stats3 };
  std::uint16_t flags = protocol::OFPSF_REPLY_MORE;
  std::uint32_t xid = 0x12345678;
};
struct table_stats_reply_fixture : table_stats_reply_parameters {
  stats::table_stats_reply sut{body, flags, xid};
  std::vector<unsigned char> bin
    = "\x01\x11\x00\xcc\x12\x34\x56\x78""\x00\x03\x00\x01"
      "\x11\x00\x00\x00"
      "t" "a" "b" "l" "e" "1" "\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x3f\xff\xee\xf0\xf0\xf0\xf0""\x0f\x0f\x0f\x0f"
      "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8""\x01\x02\x03\x04\x05\x06\x07\x08"
      "\x12\x00\x00\x00"
      "t" "a" "b" "l" "e" "2" "\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x3f\xff\xff\xe0\xe0\xe0\xe0""\x0e\x0e\x0e\x0e"
      "\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8""\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8"
      "\x13\x00\x00\x00"
      "t" "a" "b" "l" "e" "3" "\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\xd0\xd0\xd0\xd0""\x0d\x0d\x0d\x0d"
      "\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8""\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(stats_test)

BOOST_AUTO_TEST_SUITE(table_stats)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_parameters)
    {
      constexpr auto table_id = 0x12;
      constexpr auto name = "table1";
      constexpr auto wildcards = protocol::OFPFW_ALL;
      constexpr auto max_entries  = 255;
      constexpr auto active_count = 100;
      constexpr auto lookup_count = 0xffffffffffffffff;
      constexpr auto matched_count = 0xf000000000000000;

      stats::table_stats sut{
          table_id, name, wildcards, max_entries
        , active_count, lookup_count, matched_count
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_table_stats));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.name() == name);
      BOOST_TEST(sut.wildcards() == wildcards);
      BOOST_TEST(sut.max_entries() == max_entries);
      BOOST_TEST(sut.active_count() == active_count);
      BOOST_TEST(sut.lookup_count() == lookup_count);
      BOOST_TEST(sut.matched_count() == matched_count);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_too_large_name)
    {
      constexpr auto table_id = 0x12;
      auto const name = std::string(protocol::OFP_MAX_TABLE_NAME_LEN, 'A');
      constexpr auto wildcards = protocol::OFPFW_ALL;
      constexpr auto max_entries  = 0xffffffff;
      constexpr auto active_count = 0;
      constexpr auto lookup_count = 32;
      constexpr auto matched_count = 1;

      stats::table_stats sut{
          table_id, name, wildcards, max_entries
        , active_count, lookup_count, matched_count
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_table_stats));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.name().size() == protocol::OFP_MAX_TABLE_NAME_LEN - 1);
      BOOST_TEST(
          sut.name() == boost::string_ref(name.data(), sut.name().size()));
      BOOST_TEST(sut.wildcards() == wildcards);
      BOOST_TEST(sut.max_entries() == max_entries);
      BOOST_TEST(sut.active_count() == active_count);
      BOOST_TEST(sut.lookup_count() == lookup_count);
      BOOST_TEST(sut.matched_count() == matched_count);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, table_stats_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, table_stats_fixture)
    {
      auto const& const_table_stats = sut;

      auto const copy = const_table_stats;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, table_stats_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::table_stats{
          table_id, name, wildcards, max_entries
        , active_count, lookup_count, matched_count
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::table_stats{
              table_id, name, wildcards, max_entries
            , active_count, lookup_count, matched_count}
        == stats::table_stats{
              table_id, name, wildcards, max_entries
            , active_count, lookup_count, matched_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats{
              1, name, wildcards, max_entries
            , active_count, lookup_count, matched_count}
        != stats::table_stats{
              2, name, wildcards, max_entries
            , active_count, lookup_count, matched_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_name_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats{
              table_id, "table1", wildcards, max_entries
            , active_count, lookup_count, matched_count}
        != stats::table_stats{
              table_id, "table2", wildcards, max_entries
            , active_count, lookup_count, matched_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_wildcards_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats{
              table_id, name, protocol::OFPFW_DL_DST, max_entries
            , active_count, lookup_count, matched_count}
        != stats::table_stats{
              table_id, name, protocol::OFPFW_DL_SRC, max_entries
            , active_count, lookup_count, matched_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_max_entries_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats{
              table_id, name, wildcards, 1
            , active_count, lookup_count, matched_count}
        != stats::table_stats{
              table_id, name, wildcards, 2
            , active_count, lookup_count, matched_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_active_count_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats{
              table_id, name, wildcards, max_entries
            , 1, lookup_count, matched_count}
        != stats::table_stats{
              table_id, name, wildcards, max_entries
            , 2, lookup_count, matched_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_lookup_count_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats{
              table_id, name, wildcards, max_entries
            , active_count, 1, matched_count}
        != stats::table_stats{
              table_id, name, wildcards, max_entries
            , active_count, 2, matched_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_matched_count_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats{
              table_id, name, wildcards, max_entries
            , active_count, lookup_count, 1}
        != stats::table_stats{
              table_id, name, wildcards, max_entries
            , active_count, lookup_count, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, table_stats_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, table_stats_fixture)
    BOOST_AUTO_TEST_CASE(constructs_table_stats_from_binary)
    {
      auto it = bin.begin();

      auto const table_stats = stats::table_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((table_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // table_stats


BOOST_AUTO_TEST_SUITE(table_stats_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = 0x12003400;

      stats::table_stats_request sut{xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request));
      BOOST_TEST(sut.xid() == xid);
    }
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      stats::table_stats_request sut{};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request));
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_non_const_lvalue
        , table_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_const_lvalue, table_stats_request_fixture)
    {
      auto const& const_table_stats_request = sut;

      auto const copy = const_table_stats_request;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::table_stats_request{0x12345678};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_xid_is_equal)
    {
      constexpr auto xid = 0x01020304;

      BOOST_TEST(
          (stats::table_stats_request{xid} == stats::table_stats_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats_request{1} != stats::table_stats_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, table_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, table_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_table_stats_request_from_binary)
    {
      auto it = bin.begin();

      auto const table_stats_request
        = stats::table_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((table_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // table_stats_request


BOOST_AUTO_TEST_SUITE(table_stats_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_multiple_table_stats
        , table_stats_reply_parameters)
    {
      auto const flags = protocol::OFPSF_REPLY_MORE;
      auto const xid = 0x01020304;

      stats::table_stats_reply sut{{ stats1, stats2 }, flags, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply)
                               + stats1.length() + stats2.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body_type{ stats1, stats2 }));
    }
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_single_table_stats
        , table_stats_reply_parameters)
    {
      auto const flags = 0;
      auto const xid = 0xf102f304;

      stats::table_stats_reply sut{{ stats3 }, flags, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply)
                               + stats3.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body_type{ stats3 }));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_no_table_stats)
    {
      auto const flags = 0;
      auto const xid = 0x00000001;

      stats::table_stats_reply sut{{}, flags, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.body().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_without_xid)
    {
      auto const flags = protocol::OFPSF_REPLY_MORE;

      stats::table_stats_reply sut{{}, flags};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.body().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, table_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, table_stats_reply_fixture)
    {
      auto const& const_table_stats_reply = sut;

      auto const copy = const_table_stats_reply;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, table_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_stats_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, table_stats_reply_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::table_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::table_stats_reply{body, flags, xid}
        == stats::table_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats_reply{body_type{ stats1 }, flags, xid}
        != stats::table_stats_reply{body_type{ stats2 }, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats_reply{body, 0, xid}
        != stats::table_stats_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::table_stats_reply{body, flags, 1}
        != stats::table_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, table_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, table_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_table_stats_reply_from_binary)
    {
      auto it = bin.begin();

      auto const table_stats_reply
        = stats::table_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((table_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // table_stats_reply

BOOST_AUTO_TEST_SUITE_END() // stats_test
