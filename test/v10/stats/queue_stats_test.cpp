#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/stats/queue_stats.hpp>
#include <boost/test/unit_test.hpp>

#include <canard/network/openflow/v10/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace stats = msg::statistics;
namespace protocol = v10::protocol;

namespace {
struct queue_stats_parameters {
  std::uint32_t queue_id = 0x4001ffee;
  std::uint16_t port_no = 0x3001;
  std::uint64_t tx_packets = 0x1001020304050607;
  std::uint64_t tx_bytes = 0x10f1f2f3f4f5f6f7;
  std::uint64_t tx_errors = 0x10ffffff12345678;
};
struct queue_stats_fixture : queue_stats_parameters {
  stats::queue_stats sut{queue_id, port_no, tx_packets, tx_bytes, tx_errors};
  std::vector<unsigned char> bin
    = "\x30\x01\x00\x00\x40\x01\xff\xee"
      "\x10\xf1\xf2\xf3\xf4\xf5\xf6\xf7""\x10\x01\x02\x03\x04\x05\x06\x07"
      "\x10\xff\xff\xff\x12\x34\x56\x78"_bin;
};
struct queue_stats_request_parameters {
  std::uint32_t queue_id = protocol::OFPQ_ALL;
  std::uint16_t port_no = protocol::OFPP_MAX;
  std::uint32_t xid = 0x10305070;
};
struct queue_stats_request_fixture : queue_stats_request_parameters {
  stats::queue_stats_request sut{queue_id, port_no, xid};
  std::vector<unsigned char> bin
    = "\x01\x10\x00\x14\x10\x30\x50\x70""\x00\x05\x00\x00"
      "\xff\x00\x00\x00\xff\xff\xff\xff"_bin;
};
struct queue_stats_reply_parameters {
  using body_type = stats::queue_stats_reply::body_type;
  stats::queue_stats stats1{
      0x4001ffee, 0x3001
    , 0x1001020304050607, 0x10f1f2f3f4f5f6f7, 0x10ffffff12345678
  };
  stats::queue_stats stats2{
      0x4002ffee, 0x3002
    , 0x2001020304050607, 0x20f1f2f3f4f5f6f7, 0x20ffffff12345678
  };
  stats::queue_stats stats3{
      0x4003ffee, 0x3003
    , 0x3001020304050607, 0x30f1f2f3f4f5f6f7, 0x30ffffff12345678
  };
  body_type body{ stats1, stats2, stats3 };
  std::uint16_t flags = 0;
  std::uint32_t xid = 0x10305070;
};
struct queue_stats_reply_fixture : queue_stats_reply_parameters {
  stats::queue_stats_reply sut{body, flags, xid};
  std::vector<unsigned char> bin
    = "\x01\x11\x00\x6c\x10\x30\x50\x70""\x00\x05\x00\x00"
      "\x30\x01\x00\x00\x40\x01\xff\xee"
      "\x10\xf1\xf2\xf3\xf4\xf5\xf6\xf7""\x10\x01\x02\x03\x04\x05\x06\x07"
      "\x10\xff\xff\xff\x12\x34\x56\x78"
      "\x30\x02\x00\x00\x40\x02\xff\xee"
      "\x20\xf1\xf2\xf3\xf4\xf5\xf6\xf7""\x20\x01\x02\x03\x04\x05\x06\x07"
      "\x20\xff\xff\xff\x12\x34\x56\x78"
      "\x30\x03\x00\x00\x40\x03\xff\xee"
      "\x30\xf1\xf2\xf3\xf4\xf5\xf6\xf7""\x30\x01\x02\x03\x04\x05\x06\x07"
      "\x30\xff\xff\xff\x12\x34\x56\x78"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(stats_test)

BOOST_AUTO_TEST_SUITE(queue_stats)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_parameters)
    {
      auto const queue_id = 0x10203040;
      auto const port_no = protocol::OFPP_MAX;
      auto const tx_packets = 0x0102030405060708;
      auto const tx_bytes = 0x1020304050607080;
      auto const tx_errors = 0xffffffffffffffff;

      stats::queue_stats sut{
        queue_id, port_no, tx_packets, tx_bytes, tx_errors
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_queue_stats));
      BOOST_TEST(sut.queue_id() == queue_id);
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST(sut.tx_packets() == tx_packets);
      BOOST_TEST(sut.tx_bytes() == tx_bytes);
      BOOST_TEST(sut.tx_errors() == tx_errors);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, queue_stats_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, queue_stats_fixture)
    {
      auto const& const_queue_stats = sut;

      auto const copy = const_queue_stats;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, queue_stats_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::queue_stats{
        queue_id, port_no, tx_packets, tx_bytes, tx_errors
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::queue_stats{
             queue_id, port_no, tx_packets, tx_bytes, tx_errors}
        == stats::queue_stats{
             queue_id, port_no, tx_packets, tx_bytes, tx_errors}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_queue_id_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats{
             1, port_no, tx_packets, tx_bytes, tx_errors}
        != stats::queue_stats{
             2, port_no, tx_packets, tx_bytes, tx_errors}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats{
             queue_id, 1, tx_packets, tx_bytes, tx_errors}
        != stats::queue_stats{
             queue_id, 2, tx_packets, tx_bytes, tx_errors}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_tx_packets_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats{
             queue_id, port_no, 1, tx_bytes, tx_errors }
        != stats::queue_stats{
             queue_id, port_no, 2, tx_bytes, tx_errors}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_tx_bytes_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats{
             queue_id, port_no, tx_packets, 1, tx_errors}
        != stats::queue_stats{
             queue_id, port_no, tx_packets, 2, tx_errors}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_tx_errors_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats{
             queue_id, port_no, tx_packets, tx_bytes, 1}
        != stats::queue_stats{
             queue_id, port_no, tx_packets, tx_bytes, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, queue_stats_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, queue_stats_fixture)
    BOOST_AUTO_TEST_CASE(constructs_queue_stats_from_binary)
    {
      auto it = bin.begin();

      auto const queue_stats = stats::queue_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // queue_stats


BOOST_AUTO_TEST_SUITE(queue_stats_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_queue_id_and_port_no_and_xid)
    {
      auto const queue_id = 1;
      auto const port_no = protocol::OFPP_MAX;
      auto const xid = 0x01020304;

      stats::queue_stats_request sut{queue_id, port_no, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request)
                               + sizeof(protocol::ofp_queue_stats_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.queue_id() == queue_id);
      BOOST_TEST(sut.port_no() == port_no);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_without_xid)
    {
      auto const queue_id = 1;
      auto const port_no = protocol::OFPP_MAX;

      stats::queue_stats_request sut{queue_id, port_no};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request)
                               + sizeof(protocol::ofp_queue_stats_request));
      BOOST_TEST(sut.queue_id() == queue_id);
      BOOST_TEST(sut.port_no() == port_no);
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_non_const_lvalue
        , queue_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, queue_stats_request_fixture)
    {
      auto const& const_queue_stats_request = sut;

      auto const copy = const_queue_stats_request;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, queue_stats_request_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::queue_stats_request{queue_id, port_no, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::queue_stats_request{queue_id, port_no, xid}
        == stats::queue_stats_request{queue_id, port_no, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_queue_id_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats_request{1, port_no, xid}
        != stats::queue_stats_request{2, port_no, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats_request{queue_id, 1, xid}
        != stats::queue_stats_request{queue_id, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats_request{queue_id, port_no, 1}
        != stats::queue_stats_request{queue_id, port_no, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, queue_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, queue_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_queue_stats_request_from_binary)
    {
      auto it = bin.begin();

      auto const queue_stats_request
        = stats::queue_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // queue_stats_request


BOOST_AUTO_TEST_SUITE(queue_stats_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_multiple_queue_stats
        , queue_stats_reply_parameters)
    {
      auto const flags = protocol::OFPSF_REPLY_MORE;
      auto const xid = 0x12345678;

      stats::queue_stats_reply sut{{ stats1, stats2 }, flags, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply)
                               + stats1.length() + stats2.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body_type{ stats1, stats2 }));
    }
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_single_queue_stats
        , queue_stats_reply_parameters)
    {
      auto const flags = 0;
      auto const xid = 0x12340000;

      stats::queue_stats_reply sut{{ stats3 }, flags, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply)
                               + stats3.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body_type{ stats3 }));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_no_queue_stats)
    {
      auto const flags = protocol::OFPSF_REPLY_MORE;
      auto const xid = 0x00005678;

      stats::queue_stats_reply sut{{}, flags, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_reply));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.body().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, queue_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, queue_stats_reply_fixture)
    {
      auto const& const_queue_stats_reply = sut;

      auto const copy = const_queue_stats_reply;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, queue_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_stats_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, queue_stats_reply_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::queue_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      BOOST_TEST(
          (stats::queue_stats_reply{body, flags, xid}
        == stats::queue_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats_reply{body_type{ stats1 }, flags, xid}
        != stats::queue_stats_reply{body_type{ stats2 }, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats_reply{body, 0, xid}
        != stats::queue_stats_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::queue_stats_reply{body, flags, 1}
        != stats::queue_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, queue_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, queue_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_queue_stats_reply_from_binary)
    {
      auto it = bin.begin();

      auto const queue_stats_reply
        = stats::queue_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // queue_stats_reply

BOOST_AUTO_TEST_SUITE_END() // stats_test
