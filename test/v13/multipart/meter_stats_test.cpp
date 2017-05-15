#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/meter_stats.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace multipart = v13::messages::multipart;

namespace {

  struct meter_band_stats_parameter {
    std::uint64_t packet_band_count = 0x0102030405060708;
    std::uint64_t byte_band_count = 0x1020304050607080;
  };
  struct meter_band_stats_fixture :  meter_band_stats_parameter {
    multipart::meter_band_stats sut{packet_band_count, byte_band_count};
    std::vector<std::uint8_t> bin
      = "\x01\x02\x03\x04\x05\x06\x07\x08""\x10\x20\x30\x40\x50\x60\x70\x80"
        ""_bin;
  };

  struct meter_stats_parameter {
    using band_stats_type = multipart::meter_stats::band_stats_type;
    std::uint32_t meter_id = 0x12345678;
    std::uint32_t flow_count = 0x87654321;
    std::uint64_t packet_in_count = 0x0102030405060708;
    std::uint64_t byte_in_count = 0x1020304050607080;
    std::uint32_t duration_sec = 0xf0f0f0f0;
    std::uint32_t duration_nsec = 0x0f0f0f0f;
    multipart::meter_band_stats stats1{0x1112131415161718, 0x1121314151617181};
    multipart::meter_band_stats stats2{0x2122232425262728, 0x1222324252627282};
    multipart::meter_band_stats stats3{0x3132333435363738, 0x1323334353637383};
    band_stats_type band_stats{stats1, stats2, stats3};
  };
  struct meter_stats_fixture : meter_stats_parameter {
    multipart::meter_stats sut{
        meter_id, flow_count, packet_in_count, byte_in_count
      , duration_sec, duration_nsec, band_stats
    };
    std::vector<std::uint8_t> bin
      = "\x12\x34\x56\x78\x00\x58\x00\x00""\x00\x00\x00\x00\x87\x65\x43\x21"
        "\x01\x02\x03\x04\x05\x06\x07\x08""\x10\x20\x30\x40\x50\x60\x70\x80"
        "\xf0\xf0\xf0\xf0\x0f\x0f\x0f\x0f"
        "\x11\x12\x13\x14\x15\x16\x17\x18""\x11\x21\x31\x41\x51\x61\x71\x81"
        "\x21\x22\x23\x24\x25\x26\x27\x28""\x12\x22\x32\x42\x52\x62\x72\x82"
        "\x31\x32\x33\x34\x35\x36\x37\x38""\x13\x23\x33\x43\x53\x63\x73\x83"
        ""_bin;
  };

  struct meter_stats_request_parameter {
    std::uint32_t meter_id = 0x12345678;
    std::uint32_t xid = 0x87654321;
  };
  struct meter_stats_request_fixture : meter_stats_request_parameter {
    multipart::meter_stats_request sut{meter_id, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x18\x87\x65\x43\x21""\x00\x09\x00\x00\x00\x00\x00\x00"
        "\x12\x34\x56\x78\x00\x00\x00\x00"
        ""_bin;
  };

  struct meter_stats_reply_parameter {
    multipart::meter_stats stats1{
        0x11111111, 0x12121212, 0x1313131313131313, 0x1414141414141414
      , 0x15151515, 0x16161616
      , multipart::meter_stats::band_stats_type{
            multipart::meter_band_stats{0x1717171717171717, 0x1818181818181818}
          , multipart::meter_band_stats{0x1919191919191919, 0x1a1a1a1a1a1a1a1a}
        }
    };
    multipart::meter_stats stats2{
        0x21212121, 0x22222222, 0x2323232323232323, 0x2424242424242424
      , 0x25252525, 0x26262626
      , multipart::meter_stats::band_stats_type{
            multipart::meter_band_stats{0x2727272727272727, 0x2828282828282828}
        }
    };
    multipart::meter_stats stats3{
        0x31313131, 0x32323232, 0x3333333333333333, 0x3434343434343434
      , 0x35353535, 0x36363636
      , multipart::meter_stats::band_stats_type{}
    };
    using body_type = multipart::meter_stats_reply::body_type;
    body_type body{stats1, stats2, stats3};
    std::uint16_t flags = 1;
    std::uint32_t xid = 0x01020304;
  };
  struct meter_stats_reply_fixture : meter_stats_reply_parameter {
    multipart::meter_stats_reply sut{body, flags, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\xb8\x01\x02\x03\x04""\x00\x09\x00\x01\x00\x00\x00\x00"
        "\x11\x11\x11\x11\x00\x48\x00\x00""\x00\x00\x00\x00\x12\x12\x12\x12"
        "\x13\x13\x13\x13\x13\x13\x13\x13""\x14\x14\x14\x14\x14\x14\x14\x14"
        "\x15\x15\x15\x15\x16\x16\x16\x16"
        "\x17\x17\x17\x17\x17\x17\x17\x17""\x18\x18\x18\x18\x18\x18\x18\x18"
        "\x19\x19\x19\x19\x19\x19\x19\x19""\x1a\x1a\x1a\x1a\x1a\x1a\x1a\x1a"
        "\x21\x21\x21\x21\x00\x38\x00\x00""\x00\x00\x00\x00\x22\x22\x22\x22"
        "\x23\x23\x23\x23\x23\x23\x23\x23""\x24\x24\x24\x24\x24\x24\x24\x24"
        "\x25\x25\x25\x25\x26\x26\x26\x26"
        "\x27\x27\x27\x27\x27\x27\x27\x27""\x28\x28\x28\x28\x28\x28\x28\x28"
        "\x31\x31\x31\x31\x00\x28\x00\x00""\x00\x00\x00\x00\x32\x32\x32\x32"
        "\x33\x33\x33\x33\x33\x33\x33\x33""\x34\x34\x34\x34\x34\x34\x34\x34"
        "\x35\x35\x35\x35\x36\x36\x36\x36"
        ""_bin;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(meter_band_stats)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const packet_band_count = std::uint64_t{1234};
      auto const byte_band_count = std::uint64_t{5678};

      multipart::meter_band_stats const sut{packet_band_count, byte_band_count};

      BOOST_TEST(sut.packet_band_count() == packet_band_count);
      BOOST_TEST(sut.byte_band_count() == byte_band_count);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, meter_band_stats_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut
        = multipart::meter_band_stats{packet_band_count, byte_band_count};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::meter_band_stats{packet_band_count, byte_band_count}
        == multipart::meter_band_stats{packet_band_count, byte_band_count}));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_band_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_band_stats{1, byte_band_count}
        != multipart::meter_band_stats{2, byte_band_count}));
    }
    BOOST_AUTO_TEST_CASE(false_if_byte_band_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_band_stats{packet_band_count, 1}
        != multipart::meter_band_stats{packet_band_count, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_band_stats_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_band_stats_fixture)
    {
      auto it = bin.begin();

      auto const meter_band_stats
        = multipart::meter_band_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_band_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_band_stats

BOOST_AUTO_TEST_SUITE(meter_stats)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const meter_id = std::uint32_t{32};
      auto const flow_count = std::uint32_t{11};
      auto const packet_in_count = std::uint64_t{219};
      auto const byte_in_count = std::uint64_t{1923};
      auto const duration_sec = std::uint32_t{112};
      auto const duration_nsec = std::uint32_t{1393};
      auto const band_stats = multipart::meter_stats::band_stats_type{
          multipart::meter_band_stats{1, 2}
        , multipart::meter_band_stats{3, 4}
        , multipart::meter_band_stats{5, 6}
      };

      multipart::meter_stats const sut{
          meter_id, flow_count, packet_in_count, byte_in_count
        , duration_sec, duration_nsec, band_stats
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_meter_stats) + band_stats.length());
      BOOST_TEST(sut.meter_id() == meter_id);
      BOOST_TEST(sut.flow_count() == flow_count);
      BOOST_TEST(sut.packet_in_count() == packet_in_count);
      BOOST_TEST(sut.byte_in_count() == byte_in_count);
      BOOST_TEST(sut.duration_sec() == duration_sec);
      BOOST_TEST(sut.duration_nsec() == duration_nsec);
      BOOST_TEST((sut.band_stats() == band_stats));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_empty_band_stats)
    {
      auto const meter_id = std::uint32_t{30};
      auto const flow_count = std::uint32_t{9};
      auto const packet_in_count = std::uint64_t{217};
      auto const byte_in_count = std::uint64_t{1918};
      auto const duration_sec = std::uint32_t{104};
      auto const duration_nsec = std::uint32_t{1390};

      multipart::meter_stats const sut{
          meter_id, flow_count, packet_in_count, byte_in_count
        , duration_sec, duration_nsec, {}
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_meter_stats));
      BOOST_TEST(sut.meter_id() == meter_id);
      BOOST_TEST(sut.flow_count() == flow_count);
      BOOST_TEST(sut.packet_in_count() == packet_in_count);
      BOOST_TEST(sut.byte_in_count() == byte_in_count);
      BOOST_TEST(sut.duration_sec() == duration_sec);
      BOOST_TEST(sut.duration_nsec() == duration_nsec);
      BOOST_TEST(sut.band_stats().empty());
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, meter_stats_fixture)
    {
      auto const copy = add_const(sut);

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, meter_stats_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_meter_stats));
      BOOST_TEST(moved.band_stats().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, meter_stats_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = multipart::meter_stats{0, 0, 0, 0, 0, 0, {}};

      copy = add_const(sut);

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = multipart::meter_stats{0, 0, 0, 0, 0, 0, {}};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_meter_stats));
      BOOST_TEST(moved.band_stats().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, meter_stats_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::meter_stats{
          meter_id, flow_count, packet_in_count, byte_in_count
        , duration_sec, duration_nsec, band_stats
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats{
               meter_id, flow_count, packet_in_count, byte_in_count
             , duration_sec, duration_nsec, band_stats
           }
        == multipart::meter_stats{
               meter_id, flow_count, packet_in_count, byte_in_count
             , duration_sec, duration_nsec, band_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_meter_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats{
               1, flow_count, packet_in_count, byte_in_count
             , duration_sec, duration_nsec, band_stats
           }
        != multipart::meter_stats{
               2, flow_count, packet_in_count, byte_in_count
             , duration_sec, duration_nsec, band_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_flow_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats{
               meter_id, 1, packet_in_count, byte_in_count
             , duration_sec, duration_nsec, band_stats
           }
        != multipart::meter_stats{
               meter_id, 2, packet_in_count, byte_in_count
             , duration_sec, duration_nsec, band_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_in_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats{
               meter_id, flow_count, 1, byte_in_count
             , duration_sec, duration_nsec, band_stats
           }
        != multipart::meter_stats{
               meter_id, flow_count, 2, byte_in_count
             , duration_sec, duration_nsec, band_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_byte_in_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats{
               meter_id, flow_count, packet_in_count, 1
             , duration_sec, duration_nsec, band_stats
           }
        != multipart::meter_stats{
               meter_id, flow_count, packet_in_count, 2
             , duration_sec, duration_nsec, band_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_sec_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats{
               meter_id, flow_count, packet_in_count, byte_in_count
             , 1, duration_nsec, band_stats
           }
        != multipart::meter_stats{
               meter_id, flow_count, packet_in_count, byte_in_count
             , 2, duration_nsec, band_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_nsec_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats{
               meter_id, flow_count, packet_in_count, byte_in_count
             , duration_sec, 1, band_stats
           }
        != multipart::meter_stats{
               meter_id, flow_count, packet_in_count, byte_in_count
             , duration_sec, 2, band_stats
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_band_stats_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats{
               meter_id, flow_count, packet_in_count, byte_in_count
             , duration_sec, duration_nsec, band_stats_type{stats1}
           }
        != multipart::meter_stats{
               meter_id, flow_count, packet_in_count, byte_in_count
             , duration_sec, duration_nsec, band_stats_type{stats2}
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(extract_band_stats)
    BOOST_FIXTURE_TEST_CASE(move_band_stats_ownership, meter_stats_fixture)
    {
      auto const original_band_stats = sut.band_stats();

      auto const band_stats = sut.extract_band_stats();

      BOOST_TEST((band_stats == original_band_stats));
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_meter_stats));
      BOOST_TEST(sut.band_stats().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_band_stats

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_stats_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_stats_fixture)
    {
      auto it = bin.begin();

      auto const meter_stats = multipart::meter_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_stats

BOOST_AUTO_TEST_SUITE(meter_stats_request)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::meter_stats_request;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_request);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::meter_stats_request;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::meter);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const meter_id = std::uint32_t{12};
      auto const xid = std::uint32_t{10};

      multipart::meter_stats_request const sut{meter_id, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_request)
        + sizeof(protocol::ofp_meter_multipart_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.meter_id() == meter_id);
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      auto const meter_id = std::uint32_t{1};

      multipart::meter_stats_request const sut{meter_id};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_request)
        + sizeof(protocol::ofp_meter_multipart_request));
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.meter_id() == meter_id);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, meter_stats_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::meter_stats_request{meter_id, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats_request{meter_id, xid}
        == multipart::meter_stats_request{meter_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_meter_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats_request{1, xid}
        != multipart::meter_stats_request{2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats_request{meter_id, 1}
        != multipart::meter_stats_request{meter_id, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_stats_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_stats_request_fixture)
    {
      auto it = bin.begin();

      auto const meter_stats_request
        = multipart::meter_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_stats_request

BOOST_AUTO_TEST_SUITE(meter_stats_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_parameters, meter_stats_reply_parameter)
    {
      auto const body = body_type{stats1, stats2};
      auto const flags = 1;
      auto const xid = 1345;

      multipart::meter_stats_reply sut{body, flags, xid};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_without_xid, meter_stats_reply_parameter)
    {
      auto const body = body_type{stats3};
      auto const flags = 1;

      multipart::meter_stats_reply sut{body, flags};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_without_flags, meter_stats_reply_parameter)
    {
      auto const body = body_type{};

      multipart::meter_stats_reply sut{body};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.body() == body));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, meter_stats_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::meter_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats_reply{body,flags, xid}
        == multipart::meter_stats_reply{body,flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats_reply{body_type{stats1, stats2}, flags, xid}
        != multipart::meter_stats_reply{body_type{stats2, stats1}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats_reply{body, 1, xid}
        != multipart::meter_stats_reply{body, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_stats_reply{body, flags, 1}
        != multipart::meter_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_stats_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_stats_reply_fixture)
    {
      auto it = bin.begin();

      auto const meter_stats_reply
        = multipart::meter_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_stats_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
