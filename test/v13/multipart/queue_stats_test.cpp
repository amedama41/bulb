#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/queue_stats.hpp>
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

  using body_type = multipart::queue_stats_reply::body_type;

  struct queue_stats_parameter {
    std::uint32_t queue_id = 0x12345678;
    std::uint32_t port_no = protocol::OFPP_MAX;
    std::uint64_t tx_packets = 0x0102030405060708;
    std::uint64_t tx_bytes = 0xf1f2f3f4f5f6f7f8;
    std::uint64_t tx_errors = 0xff00ff00ff00ff00;
    v13::elapsed_time elapsed_time{0x1234, 0x5678};
  };
  struct queue_stats_fixture : queue_stats_parameter {
    multipart::queue_stats sut{
      queue_id, port_no, tx_packets, tx_bytes, tx_errors, elapsed_time
    };
    std::vector<std::uint8_t> bin
      = "\xff\xff\xff\x00\x12\x34\x56\x78""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x01\x02\x03\x04\x05\x06\x07\x08""\xff\x00\xff\x00\xff\x00\xff\x00"
        "\x00\x00\x12\x34\x00\x00\x56\x78"
        ""_bin
        ;
  };

  struct queue_stats_request_parameter {
    std::uint32_t queue_id = 0x12345678;
    std::uint32_t port_no = protocol::OFPP_MAX;
    std::uint32_t xid = 0x12345678;
  };
  struct queue_stats_request_fixture : queue_stats_request_parameter {
    multipart::queue_stats_request sut{queue_id, port_no, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x18\x12\x34\x56\x78""\x00\x05\x00\x00\x00\x00\x00\x00"
        "\xff\xff\xff\x00\x12\x34\x56\x78"
        ""_bin
        ;
  };

  struct body_fixture {
    multipart::queue_stats queue_stats0{
        0x1, 0x1
      , 0x0102030405060708, 0xf1f2f3f4f5f6f7f8, 0xff00ff00ff00ff00
      , v13::elapsed_time{0x10001234, 0x10005678}
    };
    multipart::queue_stats queue_stats1{
        0xfffffffe, 0x1
      , 0x1112131415161718, 0xe1e2e3e4e5e6e7e8, 0xee00ee00ee00ee00
      , v13::elapsed_time{0x20001234, 0x20005678}
    };
    multipart::queue_stats queue_stats2{
        0x2, protocol::OFPP_MAX
      , 0x8182838485868788, 0xa1a2a3a4a5a6a7a8, 0xbf00bf00bf00bf00
      , v13::elapsed_time{0x30001234, 0x30005678}
    };
  };
  struct queue_stats_reply_parameter : body_fixture {
    body_type body{queue_stats0, queue_stats1, queue_stats2};
    std::uint16_t flags = protocol::OFPMPF_REPLY_MORE;
    std::uint32_t xid = 0x12345678;
  };
  struct queue_stats_reply_fixture : queue_stats_reply_parameter {
    multipart::queue_stats_reply sut{body, flags, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\x88\x12\x34\x56\x78""\x00\x05\x00\x01\x00\x00\x00\x00"

        "\x00\x00\x00\x01\x00\x00\x00\x01""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x01\x02\x03\x04\x05\x06\x07\x08""\xff\x00\xff\x00\xff\x00\xff\x00"
        "\x10\x00\x12\x34\x10\x00\x56\x78"

        "\x00\x00\x00\x01\xff\xff\xff\xfe""\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8"
        "\x11\x12\x13\x14\x15\x16\x17\x18""\xee\x00\xee\x00\xee\x00\xee\x00"
        "\x20\x00\x12\x34\x20\x00\x56\x78"

        "\xff\xff\xff\x00\x00\x00\x00\x02""\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8"
        "\x81\x82\x83\x84\x85\x86\x87\x88""\xbf\x00\xbf\x00\xbf\x00\xbf\x00"
        "\x30\x00\x12\x34\x30\x00\x56\x78"
        ""_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(queue_stats)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const queue_id = std::uint32_t{3};
      auto const port_no = std::uint32_t{1};
      auto const tx_packets = std::uint64_t{1234};
      auto const tx_bytes = std::uint64_t{5678};
      auto const tx_errors = std::uint64_t{987654};
      auto const elapsed_time = v13::elapsed_time{32, 45};

      multipart::queue_stats const sut{
        queue_id, port_no, tx_packets, tx_bytes, tx_errors, elapsed_time
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_queue_stats));
      BOOST_TEST(sut.queue_id() == queue_id);
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST(sut.tx_packets() == tx_packets);
      BOOST_TEST(sut.tx_bytes() == tx_bytes);
      BOOST_TEST(sut.tx_errors() == tx_errors);
      BOOST_TEST(sut.duration_sec() == elapsed_time.duration_sec());
      BOOST_TEST(sut.duration_nsec() == elapsed_time.duration_nsec());
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, queue_stats_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, queue_stats_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::queue_stats{
        queue_id, port_no, tx_packets, tx_bytes, tx_errors, elapsed_time
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats{
             queue_id, port_no, tx_packets, tx_bytes, tx_errors, elapsed_time
           }
        == multipart::queue_stats{
             queue_id, port_no, tx_packets, tx_bytes, tx_errors, elapsed_time
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_queue_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats{
             1, port_no, tx_packets, tx_bytes, tx_errors, elapsed_time
           }
        != multipart::queue_stats{
             2, port_no, tx_packets, tx_bytes, tx_errors, elapsed_time
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats{
             queue_id, 1, tx_packets, tx_bytes, tx_errors, elapsed_time
           }
        != multipart::queue_stats{
             queue_id, 2, tx_packets, tx_bytes, tx_errors, elapsed_time
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_tx_packets_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats{
             queue_id, port_no, 1, tx_bytes, tx_errors, elapsed_time
           }
        != multipart::queue_stats{
             queue_id, port_no, 2, tx_bytes, tx_errors, elapsed_time
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_tx_bytes_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats{
             queue_id, port_no, tx_packets, 1, tx_errors, elapsed_time
           }
        != multipart::queue_stats{
             queue_id, port_no, tx_packets, 2, tx_errors, elapsed_time
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_tx_errors_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats{
             queue_id, port_no, tx_packets, tx_bytes, 1, elapsed_time
           }
        != multipart::queue_stats{
             queue_id, port_no, tx_packets, tx_bytes, 2, elapsed_time
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_sec_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats{
               queue_id, port_no, tx_packets, tx_bytes, tx_errors
             , v13::elapsed_time{1, 0}
           }
        != multipart::queue_stats{
               queue_id, port_no, tx_packets, tx_bytes, tx_errors
             , v13::elapsed_time{2, 0}
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_nsec_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats{
               queue_id, port_no, tx_packets, tx_bytes, tx_errors
             , v13::elapsed_time{0, 1}
           }
        != multipart::queue_stats{
               queue_id, port_no, tx_packets, tx_bytes, tx_errors
             , v13::elapsed_time{0, 2}
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, queue_stats_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, queue_stats_fixture)
    {
      auto it = bin.begin();

      auto const queue_stats = multipart::queue_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // queue_stats


BOOST_AUTO_TEST_SUITE(queue_stats_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const queue_id = std::uint32_t{93283};
      auto const port_no = std::uint32_t{7833};

      multipart::queue_stats_request const sut{queue_id, port_no};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_queue_stats_request));
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_QUEUE);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.queue_id() == queue_id);
      BOOST_TEST(sut.port_no() == port_no);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, queue_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, queue_stats_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::queue_stats_request{queue_id, port_no, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats_request{queue_id, port_no, xid}
        == multipart::queue_stats_request{queue_id, port_no, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_queue_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats_request{1, port_no, xid}
        != multipart::queue_stats_request{2, port_no, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats_request{queue_id, 1, xid}
        != multipart::queue_stats_request{queue_id, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats_request{queue_id, port_no, 1}
        != multipart::queue_stats_request{queue_id, port_no, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, queue_stats_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, queue_stats_request_fixture)
    {
      auto it = bin.begin();

      auto const queue_stats_request
        = multipart::queue_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // queue_stats_request


BOOST_AUTO_TEST_SUITE(queue_stats_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible, body_fixture)
    {
      auto const body = ::body_type{queue_stats0, queue_stats1};

      multipart::queue_stats_reply const sut{body};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request)
          + body.length());
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_QUEUE);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, queue_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, queue_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, queue_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = multipart::queue_stats_reply{{}};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = multipart::queue_stats_reply{{}};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, queue_stats_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::queue_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats_reply{body, flags, xid}
        == multipart::queue_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats_reply{body_type{queue_stats1}, flags, xid}
        != multipart::queue_stats_reply{body_type{queue_stats2}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats_reply{body, 0, xid}
        != multipart::queue_stats_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::queue_stats_reply{body, flags, 1}
        != multipart::queue_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, queue_stats_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, queue_stats_reply_fixture)
    {
      auto it = bin.begin();

      auto const queue_stats_reply
        = multipart::queue_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // queue_stats_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
