#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/message/barrier.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;

namespace proto = v10::protocol;

namespace {
struct barrier_request_fixture {
  std::uint32_t xid = 0x12345678;
  msg::barrier_request sut{xid};
  std::vector<unsigned char> bin = "\x01\x12\x00\x08\x12\x34\x56\x78"_bin;
};
struct barrier_reply_fixture {
  std::uint32_t xid = 0x87654321;
  msg::barrier_reply sut{xid};
  std::vector<unsigned char> bin = "\x01\x13\x00\x08\x87\x65\x43\x21"_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(barrier_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      msg::barrier_request sut;

      BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = 0x12345678;

      msg::barrier_request sut{xid};

      BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, barrier_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, barrier_request_fixture)
    {
      auto const& const_barrier_request = sut;

      auto const copy = const_barrier_request;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::barrier_request{0x1234};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_xid_is_equal)
    {
      auto const xid = 0x12345678;

      BOOST_TEST((msg::barrier_request{xid} == msg::barrier_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST((msg::barrier_request{1} != msg::barrier_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, barrier_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, barrier_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_barrier_request_from_binary)
    {
      auto it = bin.begin();

      auto const barrier_request = msg::barrier_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((barrier_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // barrier_request


BOOST_AUTO_TEST_SUITE(barrier_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      msg::barrier_reply sut;

      BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = 0x12345678;

      msg::barrier_reply sut{xid};

      BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_barrier_request)
    {
      msg::barrier_request barrier_request{12345};

      msg::barrier_reply sut{barrier_request};

      BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_header));
      BOOST_TEST(sut.xid() == barrier_request.xid());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::barrier_reply{0x1234};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_xid_is_equal)
    {
      auto const xid = 0x12345678;

      BOOST_TEST((msg::barrier_reply{xid} == msg::barrier_reply{xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST((msg::barrier_reply{1} != msg::barrier_reply{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, barrier_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, barrier_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_barrier_reply_from_binary)
    {
      auto it = bin.begin();

      auto const barrier_reply = msg::barrier_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((barrier_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // barrier_reply

BOOST_AUTO_TEST_SUITE_END() // message_test
