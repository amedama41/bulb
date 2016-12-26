#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/queue_config.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace props = v10::queue_properties;
namespace detail = v10::v10_detail;

namespace proto = v10::protocol;

namespace {
struct queue_get_config_request_fixture {
  std::uint16_t port_no = 0x1234;
  std::uint32_t xid = 0x12345678;

  msg::queue_get_config_request sut{port_no, xid};
  std::vector<unsigned char> bin
    = "\x01\x14\x00\x0c\x12\x34\x56\x78""\x12\x34\x00\x00"_bin;
};
struct parameters {
  std::uint16_t port_no = 0x1234;
  using props_t = v10::packet_queue::properties_type;
  v10::packet_queue queue1{0x00010001, props_t{ props::min_rate{300} }};
  v10::packet_queue queue2{
    0x00010002, props_t{ props::min_rate{1200}, props::min_rate{0} }
  };
  v10::packet_queue queue3{0x00020001, props_t{}};
  using queues_t = msg::queue_get_config_reply::queues_type;
  std::size_t queues_length
    = queue1.length() + queue2.length() + queue3.length();
  queues_t queues{queue1, queue2, queue3};
  std::uint32_t xid = 0x12345678;
};
struct queue_get_config_reply_fixture : parameters {
  msg::queue_get_config_reply sut{port_no, queues, xid};
  std::vector<unsigned char> bin
    = "\x01\x15\x00\x58\x12\x34\x56\x78""\x12\x34\x00\x00\x00\x00\x00\x00"

      "\x00\x01\x00\x01\x00\x18\x00\x00"
      "\x00\x01\x00\x10\x00\x00\x00\x00""\x01\x2c\x00\x00\x00\x00\x00\x00"

      "\x00\x01\x00\x02\x00\x28\x00\x00"
      "\x00\x01\x00\x10\x00\x00\x00\x00""\x04\xb0\x00\x00\x00\x00\x00\x00"
      "\x00\x01\x00\x10\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"

      "\x00\x02\x00\x01\x00\x08\x00\x00"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(queue_get_config_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_port_no)
    {
      auto const port_no = proto::OFPP_MAX;
      auto const xid = 0x12345678;

      msg::queue_get_config_request sut{port_no, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_queue_get_config_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.port_no() == port_no);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_port_no_without_xid)
    {
      auto const port_no = proto::OFPP_MAX;

      msg::queue_get_config_request sut{port_no};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_queue_get_config_request));
      BOOST_TEST(sut.port_no() == port_no);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::queue_get_config_request{1};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_port_no_and_xid_are_equal)
    {
      auto const port_no = 42;
      auto const xid = 0x12345678;

      BOOST_TEST(
          (msg::queue_get_config_request{port_no, xid}
        == msg::queue_get_config_request{port_no, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      auto const xid = 0x12345678;

      BOOST_TEST(
          (msg::queue_get_config_request{1, xid}
        != msg::queue_get_config_request{2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      auto const port_no = 42;

      BOOST_TEST(
          (msg::queue_get_config_request{port_no, 0x12345678}
        != msg::queue_get_config_request{port_no, 0x87654321}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, queue_get_config_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, queue_get_config_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_queue_get_config_request_from_binary)
    {
      auto it = bin.begin();

      auto const queue_get_config_request
        = msg::queue_get_config_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_get_config_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // queue_get_config_request


BOOST_AUTO_TEST_SUITE(queue_get_config_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_port_no_and_queues, parameters)
    {
      msg::queue_get_config_reply sut{port_no, queues, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_queue_get_config_reply) + queues_length);
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST((sut.queues() == queues));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_port_no_and_queues_without_xid, parameters)
    {
      msg::queue_get_config_reply sut{port_no, queues};

      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_queue_get_config_reply) + queues_length);
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST((sut.queues() == queues));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_empty_queues)
    {
      auto const port_no = 1;
      auto const empty_queues = msg::queue_get_config_reply::queues_type{};

      msg::queue_get_config_reply sut{port_no, empty_queues};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_queue_get_config_reply));
      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST((sut.queues() == empty_queues));
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_non_const_lvalue
        , queue_get_config_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          is_copy_constructible_from_const_lvalue
        , queue_get_config_reply_fixture)
    {
      auto const& const_queue_get_config_reply = sut;

      auto const copy = const_queue_get_config_reply;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_move_constructible, queue_get_config_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_queue_get_config_reply));
      BOOST_TEST((moved.queues() == queues_t{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::queue_get_config_reply{port_no, queues, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_port_no_and_queues_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::queue_get_config_reply{port_no, queues, xid}
        == msg::queue_get_config_reply{port_no, queues, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (msg::queue_get_config_reply{1, queues, xid}
        != msg::queue_get_config_reply{2, queues, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_queues_is_not_equal)
    {
      BOOST_TEST(
          (msg::queue_get_config_reply{port_no, queues_t{ queue1 }, xid}
        != msg::queue_get_config_reply{port_no, queues_t{ queue2 }, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::queue_get_config_reply{port_no, queues, 1}
        != msg::queue_get_config_reply{port_no, queues, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, queue_get_config_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_empty_queues_binary)
    {
      auto const sut = msg::queue_get_config_reply{port_no, queues_t{}, xid};
      bin.resize(sizeof(detail::ofp_queue_get_config_reply));
      bin[3] = sizeof(detail::ofp_queue_get_config_reply);
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, queue_get_config_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_queue_get_config_reply_from_binary)
    {
      auto it = bin.begin();

      auto const queue_get_config_reply
        = msg::queue_get_config_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_get_config_reply == sut));
    }
    BOOST_AUTO_TEST_CASE(constructs_empty_queues_queue_get_config_reply)
    {
      auto const sut = msg::queue_get_config_reply{port_no, queues_t{}, xid};
      bin.resize(sizeof(detail::ofp_queue_get_config_reply));
      bin[3] = sizeof(detail::ofp_queue_get_config_reply);
      auto it = bin.begin();

      auto const queue_get_config_reply
        = msg::queue_get_config_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((queue_get_config_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // queue_get_config_reply

BOOST_AUTO_TEST_SUITE_END() // message_test
