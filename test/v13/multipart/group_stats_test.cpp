#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/group_stats.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace multipart = v13::messages::multipart;

namespace {

  struct bucket_counter_parameter {
    std::uint64_t packet_count = 0x0102030405060708;
    std::uint64_t byte_count = 0x1122334455667788;
  };
  struct bucket_counter_fixture : bucket_counter_parameter {
    multipart::bucket_counter sut{packet_count, byte_count};
    std::vector<std::uint8_t> bin
      = "\x01\x02\x03\x04\x05\x06\x07\x08""\x11\x22\x33\x44\x55\x66\x77\x88"_bin;
  };

  struct group_stats_parameter {
    using bucket_stats_type = multipart::group_stats::bucket_stats_type;
    std::uint32_t group_id = 0x12345678;
    std::uint32_t ref_count = 0x87654321;
    std::uint64_t packet_count = 0x0102030405060708;
    std::uint64_t byte_count = 0x1122334455667788;
    std::uint32_t duration_sec = 0x11111111;
    std::uint32_t duration_nsec = 0x22222222;
    multipart::bucket_counter counter1{0x1111222233334444, 0x0101020203030404};
    multipart::bucket_counter counter2{0x5555666677778888, 0x0505060607070808};
    multipart::bucket_counter counter3{0xaaaabbbbccccdddd, 0x0a0a0b0b0c0c0d0d};
    bucket_stats_type bucket_stats{counter1, counter2, counter3};
  };
  struct group_stats_fixture : group_stats_parameter {
    multipart::group_stats sut{
        group_id, ref_count
      , packet_count, byte_count, duration_sec, duration_nsec
      , bucket_stats
    };
    std::vector<std::uint8_t> bin
      = "\x00\x58\x00\x00\x12\x34\x56\x78""\x87\x65\x43\x21\x00\x00\x00\x00"
        "\x01\x02\x03\x04\x05\x06\x07\x08""\x11\x22\x33\x44\x55\x66\x77\x88"
        "\x11\x11\x11\x11\x22\x22\x22\x22""\x11\x11\x22\x22\x33\x33\x44\x44"
        "\x01\x01\x02\x02\x03\x03\x04\x04""\x55\x55\x66\x66\x77\x77\x88\x88"
        "\x05\x05\x06\x06\x07\x07\x08\x08""\xaa\xaa\xbb\xbb\xcc\xcc\xdd\xdd"
        "\x0a\x0a\x0b\x0b\x0c\x0c\x0d\x0d"
        ""_bin;
  };

  struct group_stats_request_parameter {
    std::uint32_t group_id = 0x01020304;
    std::uint32_t xid = 0x12345678;
  };
  struct group_stats_request_fixture : group_stats_request_parameter {
    multipart::group_stats_request sut{group_id, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x18\x12\x34\x56\x78""\x00\x06\x00\x00\x00\x00\x00\x00"
        "\x01\x02\x03\x04\x00\x00\x00\x00"
        ""_bin;
  };

  struct group_stats_reply_parameter {
    multipart::bucket_counter counter1{0x1111222233334444, 0x0101020203030404};
    multipart::bucket_counter counter2{0x5555666677778888, 0x0505060607070808};
    multipart::bucket_counter counter3{0xaaaabbbbccccdddd, 0x0a0a0b0b0c0c0d0d};
    multipart::group_stats stats1{
        0x11111111, 0x01010101
      , 0x0102030405060708, 0x1020304050607080, 0x1a1a1a1a, 0x1b1b1b1b
      , { counter1, counter2 }
    };
    multipart::group_stats stats2{
        0x22222222, 0x02020202
      , 0x1100220033004400, 0x0011002200330044, 0x1c1c1c1c, 0x1d1d1d1d
      , { counter3, counter1 }
    };
    multipart::group_stats stats3{
        0x33333333, 0x03030303
      , 0x1111000022220000, 0x0000111100002222, 0x1e1e1e1e, 0x1f1f1f1f
      , {}
    };
    using body_type = multipart::group_stats_reply::body_type;
    body_type body{stats1, stats2, stats3};
    std::uint16_t flags = 1;
    std::uint32_t xid = 0x12345678;
  };
  struct group_stats_reply_fixture : group_stats_reply_parameter {
    multipart::group_stats_reply sut{body, flags, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\xc8\x12\x34\x56\x78""\x00\x06\x00\x01\x00\x00\x00\x00"

        "\x00\x48\x00\x00\x11\x11\x11\x11""\x01\x01\x01\x01\x00\x00\x00\x00"
        "\x01\x02\x03\x04\x05\x06\x07\x08""\x10\x20\x30\x40\x50\x60\x70\x80"
        "\x1a\x1a\x1a\x1a\x1b\x1b\x1b\x1b""\x11\x11\x22\x22\x33\x33\x44\x44"
        "\x01\x01\x02\x02\x03\x03\x04\x04""\x55\x55\x66\x66\x77\x77\x88\x88"
        "\x05\x05\x06\x06\x07\x07\x08\x08"

        "\x00\x48\x00\x00\x22\x22\x22\x22""\x02\x02\x02\x02\x00\x00\x00\x00"
        "\x11\x00\x22\x00\x33\x00\x44\x00""\x00\x11\x00\x22\x00\x33\x00\x44"
        "\x1c\x1c\x1c\x1c\x1d\x1d\x1d\x1d""\xaa\xaa\xbb\xbb\xcc\xcc\xdd\xdd"
        "\x0a\x0a\x0b\x0b\x0c\x0c\x0d\x0d""\x11\x11\x22\x22\x33\x33\x44\x44"
        "\x01\x01\x02\x02\x03\x03\x04\x04"

        "\x00\x28\x00\x00\x33\x33\x33\x33""\x03\x03\x03\x03\x00\x00\x00\x00"
        "\x11\x11\x00\x00\x22\x22\x00\x00""\x00\x00\x11\x11\x00\x00\x22\x22"
        "\x1e\x1e\x1e\x1e\x1f\x1f\x1f\x1f"

        ""_bin;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(bucket_counter)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const packet_count = std::uint64_t{1234};
      auto const byte_count = std::uint64_t{9384};

      multipart::bucket_counter const sut{packet_count, byte_count};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_bucket_counter));
      BOOST_TEST(sut.packet_count() == packet_count);
      BOOST_TEST(sut.byte_count() == byte_count);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, bucket_counter_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::bucket_counter{packet_count, byte_count};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::bucket_counter{packet_count, byte_count}
        == multipart::bucket_counter{packet_count, byte_count}));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::bucket_counter{1, byte_count}
        != multipart::bucket_counter{2, byte_count}));
    }
    BOOST_AUTO_TEST_CASE(false_if_byte_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::bucket_counter{packet_count, 1}
        != multipart::bucket_counter{packet_count, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, bucket_counter_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, bucket_counter_fixture)
    {
      auto it = bin.begin();

      auto const bucket_counter
        = multipart::bucket_counter::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((bucket_counter == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // bucket_counter

BOOST_AUTO_TEST_SUITE(group_stats)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const group_id = std::uint32_t{0x12345678};
      auto const ref_count = std::uint32_t{5};
      auto const packet_count = std::uint64_t{1234};
      auto const byte_count = std::uint64_t{1235};
      auto const duration_sec = std::uint32_t{32};
      auto const duration_nsec = std::uint32_t{91};
      auto const bucket_stats = multipart::group_stats::bucket_stats_type{
          multipart::bucket_counter{1, 2}
        , multipart::bucket_counter{3, 4}
        , multipart::bucket_counter{5, 6}
      };

      multipart::group_stats const sut{
          group_id, ref_count
        , packet_count, byte_count, duration_sec, duration_nsec
        , bucket_stats
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_group_stats) + bucket_stats.length());
      BOOST_TEST(sut.group_id() == group_id);
      BOOST_TEST(sut.ref_count() == ref_count);
      BOOST_TEST(sut.packet_count() == packet_count);
      BOOST_TEST(sut.byte_count() == byte_count);
      BOOST_TEST(sut.duration_sec() == duration_sec);
      BOOST_TEST(sut.duration_nsec() == duration_nsec);
      BOOST_TEST((sut.bucket_stats() == bucket_stats));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_empty_bucket_stats)
    {
      auto const group_id = std::uint32_t{1000};
      auto const ref_count = std::uint32_t{2000};
      auto const packet_count = std::uint64_t{3000};
      auto const byte_count = std::uint64_t{4000};
      auto const duration_sec = std::uint32_t{5000};
      auto const duration_nsec = std::uint32_t{6000};

      multipart::group_stats const sut{
          group_id, ref_count
        , packet_count, byte_count, duration_sec, duration_nsec
        , multipart::group_stats::bucket_stats_type{}
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_group_stats));
      BOOST_TEST(sut.group_id() == group_id);
      BOOST_TEST(sut.ref_count() == ref_count);
      BOOST_TEST(sut.packet_count() == packet_count);
      BOOST_TEST(sut.byte_count() == byte_count);
      BOOST_TEST(sut.duration_sec() == duration_sec);
      BOOST_TEST(sut.duration_nsec() == duration_nsec);
      BOOST_TEST(sut.bucket_stats().empty());
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, group_stats_fixture)
    {
      auto const copy = add_const(sut);

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, group_stats_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_group_stats));
      BOOST_TEST(moved.bucket_stats().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, group_stats_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = multipart::group_stats{0, 0, 0, 0, 0, 0, {}};

      copy = add_const(sut);

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = multipart::group_stats{0, 0, 0, 0, 0, 0, {}};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_group_stats));
      BOOST_TEST(moved.bucket_stats().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, group_stats_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::group_stats{
          group_id, ref_count, packet_count, byte_count
        , duration_sec, duration_nsec, bucket_stats
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::group_stats{
               group_id, ref_count, packet_count, byte_count
             , duration_sec, duration_nsec, bucket_stats
           }
        == multipart::group_stats{
               group_id, ref_count, packet_count, byte_count
             , duration_sec, duration_nsec, bucket_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_group_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats{
               1, ref_count, packet_count, byte_count
             , duration_sec, duration_nsec, bucket_stats
           }
        != multipart::group_stats{
               2, ref_count, packet_count, byte_count
             , duration_sec, duration_nsec, bucket_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_ref_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats{
               group_id, 1, packet_count, byte_count
             , duration_sec, duration_nsec, bucket_stats
           }
        != multipart::group_stats{
               group_id, 2, packet_count, byte_count
             , duration_sec, duration_nsec, bucket_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats{
               group_id, ref_count, 1, byte_count
             , duration_sec, duration_nsec, bucket_stats
           }
        != multipart::group_stats{
               group_id, ref_count, 2, byte_count
             , duration_sec, duration_nsec, bucket_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_byte_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats{
               group_id, ref_count, packet_count, 1
             , duration_sec, duration_nsec, bucket_stats
           }
        != multipart::group_stats{
               group_id, ref_count, packet_count, 2
             , duration_sec, duration_nsec, bucket_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_sec_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats{
               group_id, ref_count, packet_count, byte_count
             , 1, duration_nsec, bucket_stats
           }
        != multipart::group_stats{
               group_id, ref_count, packet_count, byte_count
             , 2, duration_nsec, bucket_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_nsec_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats{
               group_id, ref_count, packet_count, byte_count
             , duration_sec, 1, bucket_stats
           }
        != multipart::group_stats{
               group_id, ref_count, packet_count, byte_count
             , duration_sec, 2, bucket_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_bucket_stats_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats{
               group_id, ref_count, packet_count, byte_count
             , duration_sec, duration_nsec, bucket_stats_type{counter1}
           }
        != multipart::group_stats{
               group_id, ref_count, packet_count, byte_count
             , duration_sec, duration_nsec, bucket_stats_type{counter2}
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(extract_bucket_stats)
    BOOST_FIXTURE_TEST_CASE(move_bucket_stats_ownership, group_stats_fixture)
    {
      auto const original_bucket_stats = sut.bucket_stats();

      auto const bucket_stats = sut.extract_bucket_stats();

      BOOST_TEST((bucket_stats == original_bucket_stats));
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_group_stats));
      BOOST_TEST(sut.bucket_stats().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_bucket_stats

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, group_stats_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, group_stats_fixture)
    {
      auto it = bin.begin();

      auto const group_stats = multipart::group_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // group_stats

BOOST_AUTO_TEST_SUITE(group_stats_request)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::group_stats_request;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_request);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::group_stats_request;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::group);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const group_id = std::uint32_t{32};
      auto const xid = std::uint32_t{34129};

      multipart::group_stats_request const sut{group_id, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_request)
        + sizeof(protocol::ofp_group_stats_request));
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.group_id() == group_id);
      BOOST_TEST(sut.xid() == xid);
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      auto const group_id = std::uint32_t{0};
      multipart::group_stats_request const sut{group_id};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_request)
        + sizeof(protocol::ofp_group_stats_request));
      BOOST_TEST(sut.flags() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, group_stats_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::group_stats_request{group_id, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      BOOST_TEST(
          (multipart::group_stats_request{group_id, xid}
        == multipart::group_stats_request{group_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_group_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats_request{1, xid}
        != multipart::group_stats_request{2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats_request{group_id, 1}
        != multipart::group_stats_request{group_id, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, group_stats_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, group_stats_request_fixture)
    {
      auto it = bin.begin();

      auto const group_stats_request
        = multipart::group_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // group_stats_request

BOOST_AUTO_TEST_SUITE(group_stats_reply)
BOOST_AUTO_TEST_SUITE_END() // group_stats_reply

BOOST_AUTO_TEST_SUITE(group_stats_reply)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::group_stats_reply;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_reply);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::group_stats_reply;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::group);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_parameters, group_stats_reply_parameter)
    {
      auto const body = body_type{stats1, stats2};
      auto const flags = std::uint16_t{1};
      auto const xid = std::uint32_t{1234};

      multipart::group_stats_reply const sut{body, flags, xid};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_without_xid, group_stats_reply_parameter)
    {
      auto const body = body_type{stats3};
      auto const flags = std::uint16_t{0};

      multipart::group_stats_reply const sut{body, flags};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_without_flags, group_stats_reply_parameter)
    {
      auto const body = body_type{};

      multipart::group_stats_reply const sut{body};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, group_stats_reply_fixture)
    {
      auto const copy = add_const(sut);

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, group_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, group_stats_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::group_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::group_stats_reply{body, flags, xid}
        == multipart::group_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats_reply{body_type{stats1, stats2}, flags, xid}
        != multipart::group_stats_reply{body_type{stats2, stats1}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats_reply{body, 1, xid}
        != multipart::group_stats_reply{body, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_stats_reply{body, flags, 1}
        != multipart::group_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, group_stats_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, group_stats_reply_fixture)
    {
      auto it = bin.begin();

      auto const group_stats_reply
        = multipart::group_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // group_stats_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
