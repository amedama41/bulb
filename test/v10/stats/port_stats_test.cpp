#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/stats/port_stats.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace stats = msg::statistics;
namespace detail = v10::v10_detail;

namespace proto = v10::protocol;

namespace {
struct port_stats_parameters {
  std::uint16_t port_no = 0xff00;
  std::uint64_t rx_packets = 0x1010100012345678;
  std::uint64_t tx_packets = 0x1010200012345678;
  std::uint64_t rx_bytes = 0x1020100012345678;
  std::uint64_t tx_bytes = 0x1020200012345678;
  std::uint64_t rx_dropped = 0x1030100012345678;
  std::uint64_t tx_dropped = 0x1030200012345678;
  std::uint64_t rx_errors = 0x1040100012345678;
  std::uint64_t tx_errors = 0x1040200012345678;
  std::uint64_t rx_frame_err = 0x1050000012345678;
  std::uint64_t rx_over_err = 0x1060000012345678;
  std::uint64_t rx_crc_err = 0x1070000012345678;
  std::uint64_t collisions = 0x1080000012345678;
};
struct port_stats_fixture : port_stats_parameters {
  stats::port_stats sut{
      port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
    , rx_dropped, tx_dropped, rx_errors, tx_errors
    , rx_frame_err, rx_over_err, rx_crc_err, collisions
  };
  std::vector<unsigned char> bin
    = "\xff\x00\x00\x00\x00\x00\x00\x00"
      "\x10\x10\x10\x00\x12\x34\x56\x78""\x10\x10\x20\x00\x12\x34\x56\x78"
      "\x10\x20\x10\x00\x12\x34\x56\x78""\x10\x20\x20\x00\x12\x34\x56\x78"
      "\x10\x30\x10\x00\x12\x34\x56\x78""\x10\x30\x20\x00\x12\x34\x56\x78"
      "\x10\x40\x10\x00\x12\x34\x56\x78""\x10\x40\x20\x00\x12\x34\x56\x78"
      "\x10\x50\x00\x00\x12\x34\x56\x78""\x10\x60\x00\x00\x12\x34\x56\x78"
      "\x10\x70\x00\x00\x12\x34\x56\x78""\x10\x80\x00\x00\x12\x34\x56\x78"
      ""_bin;
};
struct port_stats_request_fixture {
  std::uint16_t port_no = proto::OFPP_MAX;
  std::uint32_t xid = 0x12345678;
  stats::port_stats_request sut{port_no, xid};
  std::vector<unsigned char> bin
    = "\x01\x10\x00\x14\x12\x34\x56\x78""\x00\x04\x00\x00"
      "\xff\x00\x00\x00\x00\x00\x00\x00"_bin;
};
struct port_stats_reply_parameters {
  using body_type = stats::port_stats_reply::body_type;
  stats::port_stats stats1{
      0xff00
    , 0x1010100012345678, 0x1010200012345678
    , 0x1020100012345678, 0x1020200012345678
    , 0x1030100012345678, 0x1030200012345678
    , 0x1040100012345678, 0x1040200012345678
    , 0x1050000012345678, 0x1060000012345678
    , 0x1070000012345678, 0x1080000012345678
  };
  stats::port_stats stats2{
      0x0001
    , 0x2010100012345678, 0x2010200012345678
    , 0x2020100012345678, 0x2020200012345678
    , 0x2030100012345678, 0x2030200012345678
    , 0x2040100012345678, 0x2040200012345678
    , 0x2050000012345678, 0x2060000012345678
    , 0x2070000012345678, 0x2080000012345678
  };
  stats::port_stats stats3{
      0x1234
    , 0x3010100012345678, 0x3010200012345678
    , 0x3020100012345678, 0x3020200012345678
    , 0x3030100012345678, 0x3030200012345678
    , 0x3040100012345678, 0x3040200012345678
    , 0x3050000012345678, 0x3060000012345678
    , 0x3070000012345678, 0x3080000012345678
  };
  body_type body{ stats1, stats2, stats3 };
  std::uint16_t flags = proto::OFPSF_REPLY_MORE;
  std::uint32_t xid = 0x12345678;
};
struct port_stats_reply_fixture : port_stats_reply_parameters {
  stats::port_stats_reply sut{body, flags, xid};
  std::vector<unsigned char> bin
    = "\x01\x11\x01\x44\x12\x34\x56\x78""\x00\x04\x00\x01"
      "\xff\x00\x00\x00\x00\x00\x00\x00"
      "\x10\x10\x10\x00\x12\x34\x56\x78""\x10\x10\x20\x00\x12\x34\x56\x78"
      "\x10\x20\x10\x00\x12\x34\x56\x78""\x10\x20\x20\x00\x12\x34\x56\x78"
      "\x10\x30\x10\x00\x12\x34\x56\x78""\x10\x30\x20\x00\x12\x34\x56\x78"
      "\x10\x40\x10\x00\x12\x34\x56\x78""\x10\x40\x20\x00\x12\x34\x56\x78"
      "\x10\x50\x00\x00\x12\x34\x56\x78""\x10\x60\x00\x00\x12\x34\x56\x78"
      "\x10\x70\x00\x00\x12\x34\x56\x78""\x10\x80\x00\x00\x12\x34\x56\x78"
      "\x00\x01\x00\x00\x00\x00\x00\x00"
      "\x20\x10\x10\x00\x12\x34\x56\x78""\x20\x10\x20\x00\x12\x34\x56\x78"
      "\x20\x20\x10\x00\x12\x34\x56\x78""\x20\x20\x20\x00\x12\x34\x56\x78"
      "\x20\x30\x10\x00\x12\x34\x56\x78""\x20\x30\x20\x00\x12\x34\x56\x78"
      "\x20\x40\x10\x00\x12\x34\x56\x78""\x20\x40\x20\x00\x12\x34\x56\x78"
      "\x20\x50\x00\x00\x12\x34\x56\x78""\x20\x60\x00\x00\x12\x34\x56\x78"
      "\x20\x70\x00\x00\x12\x34\x56\x78""\x20\x80\x00\x00\x12\x34\x56\x78"
      "\x12\x34\x00\x00\x00\x00\x00\x00"
      "\x30\x10\x10\x00\x12\x34\x56\x78""\x30\x10\x20\x00\x12\x34\x56\x78"
      "\x30\x20\x10\x00\x12\x34\x56\x78""\x30\x20\x20\x00\x12\x34\x56\x78"
      "\x30\x30\x10\x00\x12\x34\x56\x78""\x30\x30\x20\x00\x12\x34\x56\x78"
      "\x30\x40\x10\x00\x12\x34\x56\x78""\x30\x40\x20\x00\x12\x34\x56\x78"
      "\x30\x50\x00\x00\x12\x34\x56\x78""\x30\x60\x00\x00\x12\x34\x56\x78"
      "\x30\x70\x00\x00\x12\x34\x56\x78""\x30\x80\x00\x00\x12\x34\x56\x78"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(stats_test)

BOOST_AUTO_TEST_SUITE(port_stats)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_parameters)
    {
      constexpr auto port_no = 0xff00;
      constexpr auto rx_packets = 1;
      constexpr auto tx_packets = 2;
      constexpr auto rx_bytes = 3;
      constexpr auto tx_bytes = 4;
      constexpr auto rx_dropped = 5;
      constexpr auto tx_dropped = 6;
      constexpr auto rx_errors = 7;
      constexpr auto tx_errors = 8;
      constexpr auto rx_frame_err = 9;
      constexpr auto rx_over_err = 10;
      constexpr auto rx_crc_err = 11;
      constexpr auto collisions = 12;

      stats::port_stats sut{
          port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
        , rx_dropped, tx_dropped, rx_errors, tx_errors
        , rx_frame_err, rx_over_err, rx_crc_err, collisions
      };

      BOOST_TEST(sut.length() == sizeof(detail::ofp_port_stats));
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
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, port_stats_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, port_stats_fixture)
    {
      auto const& const_port_stats = sut;

      auto const copy = const_port_stats;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, port_stats_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::port_stats{
          port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
        , rx_dropped, tx_dropped, rx_errors, tx_errors
        , rx_frame_err, rx_over_err, rx_crc_err, collisions
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        == stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               1, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               2, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rx_packets_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, 1, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, 2, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_tx_packets_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, 1, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, 2, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rx_bytes_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, 1, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, 2, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_tx_bytes_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, 1
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, 2
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rx_dropped_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , 1, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , 2, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_tx_dropped_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, 1, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, 2, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rx_errors_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, 1, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, 2, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_tx_errors_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, 1
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, 2
             , rx_frame_err, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rx_frame_error_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , 1, rx_over_err, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , 2, rx_over_err, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rx_over_error_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, 1, rx_crc_err, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, 2, rx_crc_err, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rx_crc_error_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, 1, collisions }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, 2, collisions }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_collisions_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, 1 }
        != stats::port_stats{
               port_no, rx_packets, tx_packets, rx_bytes, tx_bytes
             , rx_dropped, tx_dropped, rx_errors, tx_errors
             , rx_frame_err, rx_over_err, rx_crc_err, 2 }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, port_stats_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, port_stats_fixture)
    BOOST_AUTO_TEST_CASE(constructs_port_stats_from_binary)
    {
      auto it = bin.begin();

      auto const port_stats = stats::port_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // port_stats


BOOST_AUTO_TEST_SUITE(port_stats_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_port_no_and_xid)
    {
      auto const port_no = proto::OFPP_MAX;
      auto const xid = 0x11002200;

      stats::port_stats_request sut{port_no, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_stats_request)
                               + sizeof(detail::ofp_port_stats_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.port_no() == port_no);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_port_no)
    {
      auto const port_no = 1;

      stats::port_stats_request sut{port_no};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_stats_request)
                               + sizeof(detail::ofp_port_stats_request));
      BOOST_TEST(sut.port_no() == port_no);
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_non_const_lvalue
        , port_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, port_stats_request_fixture)
    {
      auto const& const_port_stats_request = sut;

      auto const copy = const_port_stats_request;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::port_stats_request{1, 2};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_port_no_and_xid_are_equal)
    {
      auto const port_no = 0xffdd;
      auto const xid = 0x12345678;

      BOOST_TEST(
          (stats::port_stats_request{port_no, xid}
        == stats::port_stats_request{port_no, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      auto const xid = 0x12345678;

      BOOST_TEST(
          (stats::port_stats_request{1, xid}
        != stats::port_stats_request{2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      auto const port_no = proto::OFPP_NONE;

      BOOST_TEST(
          (stats::port_stats_request{port_no, 1}
        != stats::port_stats_request{port_no, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, port_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, port_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_port_stats_request_from_binary)
    {
      auto it = bin.begin();

      auto const port_stats_request
        = stats::port_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // port_stats_request


BOOST_AUTO_TEST_SUITE(port_stats_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_multiple_port_stats, port_stats_reply_parameters)
    {
      auto const flags = 0;
      auto const xid = 0x00110022;

      stats::port_stats_reply sut{{ stats1, stats2 }, flags, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_stats_reply)
                               + stats1.length() + stats2.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.body() == body_type{ stats1, stats2 }));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_single_port_stats, port_stats_reply_parameters)
    {
      auto const flags = proto::OFPSF_REPLY_MORE;
      auto const xid = 0x00000001;

      stats::port_stats_reply sut{{ stats3 }, flags, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_stats_reply)
                               + stats3.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.body() == body_type{ stats3 }));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_no_port_stats, port_stats_reply_parameters)
    {
      auto const flags = proto::OFPSF_REPLY_MORE;
      auto const xid = 0;

      stats::port_stats_reply sut{{}, flags, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_stats_reply));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.body().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, port_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, port_stats_reply_fixture)
    {
      auto const& const_port_stats_reply = sut;

      auto const copy = const_port_stats_reply;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, port_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_stats_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, port_stats_reply_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::port_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::port_stats_reply{body, flags, xid}
        == stats::port_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats_reply{body_type{ stats1 }, flags, xid}
        != stats::port_stats_reply{body_type{ stats2 }, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats_reply{body, 0, xid}
        != stats::port_stats_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::port_stats_reply{body, flags, 1}
        != stats::port_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, port_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, port_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_port_stats_reply_from_binary)
    {
      auto it = bin.begin();

      auto const port_stats_reply
        = stats::port_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // port_stats_reply

BOOST_AUTO_TEST_SUITE_END() // stats_test
