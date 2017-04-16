#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/barrier.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <vector>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;

namespace {
  struct barrier_request_fixture {
    std::uint32_t xid = 0x12345678;
    v13::messages::barrier_request sut{xid};
    std::vector<std::uint8_t> bin = "\x04\x14\x00\x08\x12\x34\x56\x78"_bin;
  };
  struct barrier_reply_fixture {
    std::uint32_t xid = 0x51627384;
    v13::messages::barrier_reply sut{xid};
    std::vector<std::uint8_t> bin = "\x04\x15\x00\x08\x51\x62\x73\x84"_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(barrier_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      v13::messages::barrier_request const sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_BARRIER_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_xid)
    {
      auto const xid = std::uint32_t{0xffffffff};

      v13::messages::barrier_request const sut{xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_BARRIER_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, barrier_request_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::barrier_request{0x12345678};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = 0x01020304;

      BOOST_TEST(
          (v13::messages::barrier_request{xid}
        == v13::messages::barrier_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::barrier_request{1}
        != v13::messages::barrier_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, barrier_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, barrier_request_fixture)
    {
      auto it = bin.begin();

      auto const barrier_request
        = v13::messages::barrier_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((barrier_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // barrier_request


BOOST_AUTO_TEST_SUITE(barrier_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      v13::messages::barrier_reply sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_BARRIER_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_xid)
    {
      auto const xid = std::uint32_t{0x00110022};

      v13::messages::barrier_reply const sut{xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_BARRIER_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_barrier_request)
    {
      auto const request = v13::messages::barrier_request{0x0abcdef0};

      v13::messages::barrier_reply const sut{request};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_BARRIER_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.xid() == request.xid());
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, barrier_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::barrier_reply{0x12345678};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = 0x01020304;

      BOOST_TEST(
          (v13::messages::barrier_reply{xid}
        == v13::messages::barrier_reply{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::barrier_reply{1}
        != v13::messages::barrier_reply{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, barrier_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, barrier_reply_fixture)
    {
      auto it = bin.begin();
      auto const barrier_reply
        = v13::messages::barrier_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((barrier_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // barrier_reply

BOOST_AUTO_TEST_SUITE_END() // message_test

