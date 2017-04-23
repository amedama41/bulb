#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/port_stats.hpp>
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

  using body_type = multipart::port_stats_reply::body_type;

  struct port_stats_parameter {
    std::uint32_t port_no = protocol::OFPP_MAX;
    std::uint64_t rx_packets = 0x12;
    std::uint64_t tx_packets = 0x34;
    std::uint64_t rx_bytes = 0x56;
    std::uint64_t tx_bytes = 0x78;
    std::uint64_t rx_dropped = 0x9a;
    std::uint64_t tx_dropped = 0xbc;
    std::uint64_t rx_errors = 0xde;
    std::uint64_t tx_errors = 0xf0;
    std::uint64_t rx_frame_err = 0xf1f2f3f4f5f6f7f8;
    std::uint64_t rx_over_err = 0x0f0f0f0f0f0f0f0f;
    std::uint64_t rx_crc_err = 0x1f2f3f4f5f6f7f8f;
    std::uint64_t collisions = 0x1234567887654321;
    std::uint32_t duration_sec = 0x01020304;
    std::uint32_t duration_nsec = 0x05060708;
    v13::elapsed_time elapsed_time{duration_sec, duration_nsec};
  };
  struct port_stats_fixutre : port_stats_parameter {
    multipart::port_stats sut{
        port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
      , rx_dropped, tx_dropped, rx_errors, tx_errors
      , rx_frame_err, rx_over_err, rx_crc_err, collisions, elapsed_time
    };
    std::vector<std::uint8_t> bin
      = "\xff\xff\xff\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x12"
        "\x00\x00\x00\x00\x00\x00\x00\x34""\x00\x00\x00\x00\x00\x00\x00\x56"
        "\x00\x00\x00\x00\x00\x00\x00\x78""\x00\x00\x00\x00\x00\x00\x00\x9a"
        "\x00\x00\x00\x00\x00\x00\x00\xbc""\x00\x00\x00\x00\x00\x00\x00\xde"
        "\x00\x00\x00\x00\x00\x00\x00\xf0""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f""\x1f\x2f\x3f\x4f\x5f\x6f\x7f\x8f"
        "\x12\x34\x56\x78\x87\x65\x43\x21""\x01\x02\x03\x04\x05\x06\x07\x08"
        ""_bin
        ;
  };

  struct body_fixture : port_stats_parameter {
    multipart::port_stats port_stats0{
        0x12345678, rx_packets, tx_packets, rx_bytes, tx_bytes
      , rx_dropped, tx_dropped, rx_errors, tx_errors
      , rx_frame_err, rx_over_err, rx_crc_err, collisions, elapsed_time
    };
    multipart::port_stats port_stats1{
        0xffffff00, rx_packets, tx_packets, rx_bytes, tx_bytes
      , rx_dropped, tx_dropped, rx_errors, tx_errors
      , rx_frame_err, rx_over_err, rx_crc_err, collisions, elapsed_time
    };
    multipart::port_stats port_stats2{
        0xfffffffe, rx_packets, tx_packets, rx_bytes, tx_bytes
      , rx_dropped, tx_dropped, rx_errors, tx_errors
      , rx_frame_err, rx_over_err, rx_crc_err, collisions, elapsed_time
    };
  };

  struct port_stats_request_parameter {
    std::uint32_t port_no = 1;
    std::uint32_t xid = 0x12345678;
  };
  struct port_stats_request_fixture : port_stats_request_parameter {
    multipart::port_stats_request sut{port_no, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x18\x12\x34\x56\x78""\x00\x04\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x01\x00\x00\x00\x00"
        ""_bin
        ;
  };

  struct port_stats_reply_parameter : body_fixture {
    body_type body{port_stats0, port_stats1, port_stats2};
    std::uint16_t flags = 0;
    std::uint32_t xid = 0x12345678;
  };
  struct port_stats_reply_fixture : port_stats_reply_parameter {
    multipart::port_stats_reply sut{body, flags, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x01\x60\x12\x34\x56\x78""\x00\x04\x00\x00\x00\x00\x00\x00"

        "\x12\x34\x56\x78\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x12"
        "\x00\x00\x00\x00\x00\x00\x00\x34""\x00\x00\x00\x00\x00\x00\x00\x56"
        "\x00\x00\x00\x00\x00\x00\x00\x78""\x00\x00\x00\x00\x00\x00\x00\x9a"
        "\x00\x00\x00\x00\x00\x00\x00\xbc""\x00\x00\x00\x00\x00\x00\x00\xde"
        "\x00\x00\x00\x00\x00\x00\x00\xf0""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f""\x1f\x2f\x3f\x4f\x5f\x6f\x7f\x8f"
        "\x12\x34\x56\x78\x87\x65\x43\x21""\x01\x02\x03\x04\x05\x06\x07\x08"

        "\xff\xff\xff\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x12"
        "\x00\x00\x00\x00\x00\x00\x00\x34""\x00\x00\x00\x00\x00\x00\x00\x56"
        "\x00\x00\x00\x00\x00\x00\x00\x78""\x00\x00\x00\x00\x00\x00\x00\x9a"
        "\x00\x00\x00\x00\x00\x00\x00\xbc""\x00\x00\x00\x00\x00\x00\x00\xde"
        "\x00\x00\x00\x00\x00\x00\x00\xf0""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f""\x1f\x2f\x3f\x4f\x5f\x6f\x7f\x8f"
        "\x12\x34\x56\x78\x87\x65\x43\x21""\x01\x02\x03\x04\x05\x06\x07\x08"

        "\xff\xff\xff\xfe\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x12"
        "\x00\x00\x00\x00\x00\x00\x00\x34""\x00\x00\x00\x00\x00\x00\x00\x56"
        "\x00\x00\x00\x00\x00\x00\x00\x78""\x00\x00\x00\x00\x00\x00\x00\x9a"
        "\x00\x00\x00\x00\x00\x00\x00\xbc""\x00\x00\x00\x00\x00\x00\x00\xde"
        "\x00\x00\x00\x00\x00\x00\x00\xf0""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f""\x1f\x2f\x3f\x4f\x5f\x6f\x7f\x8f"
        "\x12\x34\x56\x78\x87\x65\x43\x21""\x01\x02\x03\x04\x05\x06\x07\x08"

        ""_bin
      ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(port_stats)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const port_no = std::uint32_t{1};
      auto const rx_packets = std::uint64_t{12};
      auto const tx_packets = std::uint64_t{34};
      auto const rx_bytes = std::uint64_t{12 * 1500};
      auto const tx_bytes = std::uint64_t{34 * 1500};
      auto const rx_dropped = std::uint64_t{3};
      auto const tx_dropped = std::uint64_t{2};
      auto const rx_errors = std::uint64_t{3434};
      auto const tx_errors = std::uint64_t{4343};
      auto const rx_frame_err = std::uint64_t{87};
      auto const rx_over_err = std::uint64_t{67};
      auto const rx_crc_err = std::uint64_t{90};
      auto const collisions = std::uint64_t{123456};
      auto const elapsed_time = v13::elapsed_time{123, 456};

      multipart::port_stats const sut{
          port_no
        , rx_packets, tx_packets
        , rx_bytes, tx_bytes
        , rx_dropped, tx_dropped
        , rx_errors, tx_errors
        , rx_frame_err
        , rx_over_err
        , rx_crc_err
        , collisions
        , elapsed_time
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_port_stats));
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST(sut.rx_packets() == rx_packets);
      BOOST_TEST(sut.tx_packets() == tx_packets);
      BOOST_TEST(sut.rx_bytes() == rx_bytes);
      BOOST_TEST(sut.tx_bytes() == tx_bytes);
      BOOST_TEST(sut.rx_dropped() == rx_dropped);
      BOOST_TEST(sut.tx_dropped() == tx_dropped);
      BOOST_TEST(sut.rx_errors() == rx_errors);
      BOOST_TEST(sut.tx_errors() == tx_errors);
      BOOST_TEST(sut.rx_frame_errors() == rx_frame_err);
      BOOST_TEST(sut.rx_over_errors() == rx_over_err);
      BOOST_TEST(sut.rx_crc_errors() == rx_crc_err);
      BOOST_TEST(sut.collisions() == collisions);
      BOOST_TEST(sut.duration_sec() == elapsed_time.duration_sec());
      BOOST_TEST(sut.duration_nsec() == elapsed_time.duration_nsec());
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, port_stats_fixutre)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, port_stats_fixutre)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, port_stats_fixutre)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, port_stats_fixutre)
    {
      auto it = bin.begin();

      auto const port_stats = multipart::port_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // port_stats


BOOST_AUTO_TEST_SUITE(port_stats_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const port_no = std::uint32_t{protocol::OFPP_ANY};

      multipart::port_stats_request const sut{port_no};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_port_stats_request));
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_PORT_STATS);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.port_no() == port_no);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, port_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, port_stats_request_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sut.length());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, port_stats_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::port_stats_request{port_no, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::port_stats_request{port_no, xid}
        == multipart::port_stats_request{port_no, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_stats_request{1, xid}
        != multipart::port_stats_request{2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_stats_request{port_no, 1}
        != multipart::port_stats_request{port_no, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, port_stats_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, port_stats_request_fixture)
    {
      auto it = bin.begin();

      auto const port_stats_request
        = multipart::port_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // port_stats_request


BOOST_AUTO_TEST_SUITE(port_stats_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible, body_fixture)
    {
      auto const body = ::body_type{port_stats0, port_stats1};

      multipart::port_stats_reply const sut{body};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_reply)
          + sizeof(protocol::ofp_port_stats) * body.size());
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_PORT_STATS);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(constructible_with_flag, body_fixture)
    {
      auto const body = ::body_type{};
      auto const flags = std::uint16_t{protocol::OFPMPF_REPLY_MORE};

      multipart::port_stats_reply const sut{body, flags};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_reply)
          + sizeof(protocol::ofp_port_stats) * body.size());
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_PORT_STATS);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, port_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, port_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, port_stats_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::port_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::port_stats_reply{body, flags, xid}
        == multipart::port_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_stats_reply{body_type{port_stats1}, flags, xid}
        != multipart::port_stats_reply{body_type{port_stats2}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_stats_reply{body, 0, xid}
        != multipart::port_stats_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_stats_reply{body, flags, 1}
        != multipart::port_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, port_stats_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, port_stats_reply_fixture)
    {
      auto it = bin.begin();

      auto const port_stats_reply
        = multipart::port_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // port_stats_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
