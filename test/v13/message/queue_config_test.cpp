#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/queue_config.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace queue_props = v13::queue_properties;
namespace protocol = v13::protocol;

namespace {

  struct queue_get_config_request_parameter {
    std::uint32_t port_no = protocol::OFPP_MAX;
    std::uint32_t xid = 0x12345678;
  };
  struct queue_get_config_request_fixture : queue_get_config_request_parameter {
    v13::messages::queue_get_config_request sut{port_no, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x16\x00\x10\x12\x34\x56\x78""\xff\xff\xff\x00\x00\x00\x00\x00"
        ""_bin
        ;
  };

  struct packet_queue_fixture {
    v13::packet_queue queue1{
        0x1, 0x1, {
          v13::any_queue_property{queue_props::min_rate{0x1234}}
        , v13::any_queue_property{queue_props::max_rate{0x5432}}
        }
    };
    v13::packet_queue queue2{
        0x2, 0x1, {
          v13::any_queue_property{queue_props::max_rate{0x3212}}
        }
    };
    v13::packet_queue queue3{
        0xfffffffe, 0x1, {
          v13::any_queue_property{queue_props::max_rate{0xf0f0}}
        , v13::any_queue_property{queue_props::min_rate{0x0f0f}}
        }
    };
  };

  struct queue_get_config_reply_parameter : packet_queue_fixture {
    std::uint32_t port_no = 0x1;
    v13::messages::queue_get_config_reply::queues_type queues{
      queue1, queue2, queue3
    };
    std::uint32_t xid = 0x12345678;
  };
  struct queue_get_config_reply_fixture : queue_get_config_reply_parameter
  {
    v13::messages::queue_get_config_reply sut{port_no, queues, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x17\x00\x90\x12\x34\x56\x78""\x00\x00\x00\x01\x00\x00\x00\x00"

        "\x00\x00\x00\x01\x00\x00\x00\x01""\x00\x30\x00\x00\x00\x00\x00\x00"
        "\x00\x01\x00\x10\x00\x00\x00\x00""\x12\x34\x00\x00\x00\x00\x00\x00"
        "\x00\x02\x00\x10\x00\x00\x00\x00""\x54\x32\x00\x00\x00\x00\x00\x00"

        "\x00\x00\x00\x02\x00\x00\x00\x01""\x00\x20\x00\x00\x00\x00\x00\x00"
        "\x00\x02\x00\x10\x00\x00\x00\x00""\x32\x12\x00\x00\x00\x00\x00\x00"

        "\xff\xff\xff\xfe\x00\x00\x00\x01""\x00\x30\x00\x00\x00\x00\x00\x00"
        "\x00\x02\x00\x10\x00\x00\x00\x00""\xf0\xf0\x00\x00\x00\x00\x00\x00"
        "\x00\x01\x00\x10\x00\x00\x00\x00""\x0f\x0f\x00\x00\x00\x00\x00\x00"
        ""_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(queue_get_config_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const port_no = std::uint32_t{1};

      v13::messages::queue_get_config_request const sut{port_no};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_QUEUE_GET_CONFIG_REQUEST);
      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_queue_get_config_request));
      BOOST_TEST(sut.port_no() == port_no);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, queue_get_config_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, queue_get_config_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::queue_get_config_request{port_no, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::queue_get_config_request{port_no, xid}
        == v13::messages::queue_get_config_request{port_no, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::queue_get_config_request{1, xid}
        != v13::messages::queue_get_config_request{2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::queue_get_config_request{port_no, 1}
        != v13::messages::queue_get_config_request{port_no, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, queue_get_config_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_binary, queue_get_config_request_fixture)
    {
      auto it = bin.begin();

      auto const queue_get_config_request
        = v13::messages::queue_get_config_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_get_config_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // queue_get_config_request


BOOST_AUTO_TEST_SUITE(queue_get_config_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible, packet_queue_fixture)
    {
      auto const port_no = std::uint32_t{protocol::OFPP_MAX};

      v13::messages::queue_get_config_reply const sut{
        port_no, {queue1, queue2}
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_QUEUE_GET_CONFIG_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_queue_get_config_reply)
          + queue1.length() + queue2.length());
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST(sut.queues().size() == 2);
      BOOST_TEST(sut.queues()[0].queue_id() == queue1.queue_id());
      BOOST_TEST(sut.queues()[1].queue_id() == queue2.queue_id());
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, queue_get_config_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, queue_get_config_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_queue_get_config_reply));
      BOOST_TEST(moved.queues().size() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, queue_get_config_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut
        = v13::messages::queue_get_config_reply{port_no, queues, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::queue_get_config_reply{port_no, queues, xid}
        == v13::messages::queue_get_config_reply{port_no, queues, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::queue_get_config_reply{1, queues, xid}
        != v13::messages::queue_get_config_reply{2, queues, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_queues_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::queue_get_config_reply{port_no, {queue1, queue2}, xid}
        != v13::messages::queue_get_config_reply{port_no, {queue2, queue1}, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::queue_get_config_reply{port_no, queues, 1}
        != v13::messages::queue_get_config_reply{port_no, queues, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(assignment, queue_get_config_reply_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignment)
    {
      auto copy = v13::messages::queue_get_config_reply{0, {}};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignment)
    {
      auto copy = v13::messages::queue_get_config_reply{0, {}};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_queue_get_config_reply));
      BOOST_TEST(moved.queues().size() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, queue_get_config_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(decode, queue_get_config_reply_fixture)
    {
      auto it = bin.begin();

      auto const queue_get_config_reply
        = v13::messages::queue_get_config_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_get_config_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // queue_get_config_reply
BOOST_AUTO_TEST_SUITE_END() // message_test
