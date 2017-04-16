#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/echo.hpp>
#include <boost/test/unit_test.hpp>
#include <cstring>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;

namespace {
  struct echo_request_parameter {
    v13::messages::echo_request::data_type data = "\x00\xff\x01\xfe"_bbin;
    std::uint32_t xid = 0x0013fffe;
  };
  struct echo_request_fixture : echo_request_parameter
  {
    v13::messages::echo_request sut{data, xid};
    std::vector<std::uint8_t> const bin
      = "\x04\x02\x00\x0c\x00\x13\xff\xfe" "\x00\xff\x01\xfe"_bin;
  };
  struct no_data_echo_request_fixture
  {
    v13::messages::echo_request sut{0x0013fffe};
    std::vector<std::uint8_t> const bin
      = "\x04\x02\x00\x08\x00\x13\xff\xfe"_bin;
  };

  struct echo_reply_parameter {
    v13::messages::echo_reply::data_type data = "\x00\xff\x01\xfe"_bbin;
    std::uint32_t xid = 0x0013fffe;
  };
  struct echo_reply_fixture : echo_reply_parameter
  {
    v13::messages::echo_reply sut{data, 0x0013fffe};
    std::vector<std::uint8_t> const bin
      = "\x04\x03\x00\x0c\x00\x13\xff\xfe" "\x00\xff\x01\xfe"_bin;
  };
  struct no_data_echo_reply_fixture
  {
    v13::messages::echo_reply sut{0x0013fffe};
    std::vector<std::uint8_t> const bin
      = "\x04\x03\x00\x08\x00\x13\xff\xfe"_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(echo_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      v13::messages::echo_request const sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_xid)
    {
      auto const xid = 0xffffffff;

      v13::messages::echo_request const sut{xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_data)
    {
      auto const data = "\x01\x02\x03\x04"_bbin;

      v13::messages::echo_request const sut{data};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header) + data.size());
      BOOST_TEST(sut.data() == data, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_data_and_xid)
    {
      auto const data = "\xff\xff\xff\xff"_bbin;
      auto const xid = 0xfefefefe;

      v13::messages::echo_request const sut{data, xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header) + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.data() == data, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, echo_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, echo_request_fixture)
    {
      auto moved = sut;

      auto copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, echo_request_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = v13::messages::echo_request{};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto moved = sut;
      auto copy = v13::messages::echo_request{};

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, echo_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::echo_request{data, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::echo_request{data, xid}
        == v13::messages::echo_request{data, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::echo_request{"1"_bbin, xid}
        != v13::messages::echo_request{"2"_bbin, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::echo_request{data, 1}
        != v13::messages::echo_request{data, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(extract_data)
    BOOST_FIXTURE_TEST_CASE(move_data_ownership, echo_request_fixture)
    {
      auto const org_data = sut.data();

      auto const extracted_data = sut.extract_data();

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.data().empty());
      BOOST_TEST(extracted_data == org_data, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_data

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, echo_request_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin);
    }
    BOOST_FIXTURE_TEST_CASE(
        generate_binary_from_no_data_echo, no_data_echo_request_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin);
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, echo_request_fixture)
    {
      auto it = bin.begin();

      auto const echo = v13::messages::echo_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((echo == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_no_data_binary, no_data_echo_request_fixture)
    {
      auto it = bin.begin();

      auto const echo = v13::messages::echo_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((echo == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // echo_request

BOOST_AUTO_TEST_SUITE(echo_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      v13::messages::echo_reply const sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_xid)
    {
      auto const xid = 0xffffffff;

      v13::messages::echo_reply const sut{xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_data)
    {
      auto const data = "\x01\x02\x03\x04"_bbin;

      v13::messages::echo_reply const sut{data};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header) + data.size());
      BOOST_TEST(sut.data() == data, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_data_and_xid)
    {
      auto const data = "\xff\xff\xff\xff"_bbin;
      auto const xid = 0xfefefefe;

      v13::messages::echo_reply const sut{data, xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header) + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.data() == data, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_lvalue_echo_request)
    {
      auto const data = "\xff\xff\xff\xff"_bbin;
      auto const xid = 0xfefefefe;
      auto const request = v13::messages::echo_request{data, xid};

      v13::messages::echo_reply const sut{request};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header) + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.data() == data, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_rvalue_echo_request)
    {
      auto const data = "\xff\xff\xff\xff"_bbin;
      auto const xid = 0xfefefefe;
      auto request = v13::messages::echo_request{data, xid};

      v13::messages::echo_reply const sut{std::move(request)};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ECHO_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header) + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.data() == data, boost::test_tools::per_element{});
      BOOST_TEST(request.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(request.data().empty());
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, echo_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, echo_reply_fixture)
    {
      auto moved = sut;

      auto copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, echo_reply_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = v13::messages::echo_reply{};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto moved = sut;
      auto copy = v13::messages::echo_reply{};

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, echo_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::echo_reply{data, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::echo_reply{data, xid}
        == v13::messages::echo_reply{data, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::echo_reply{"1"_bbin, xid}
        != v13::messages::echo_reply{"2"_bbin, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::echo_reply{data, 1}
        != v13::messages::echo_reply{data, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(extract_data)
    BOOST_FIXTURE_TEST_CASE(move_data_ownership, echo_reply_fixture)
    {
      auto const org_data = sut.data();

      auto const extracted_data = sut.extract_data();

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.data().empty());
      BOOST_TEST(extracted_data == org_data, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_data

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, echo_reply_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin);
    }
    BOOST_FIXTURE_TEST_CASE(
        generate_binary_from_no_data_echo, no_data_echo_reply_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin);
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, echo_reply_fixture)
    {
      auto it = bin.begin();

      auto const echo = v13::messages::echo_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((echo == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_no_data_binary, no_data_echo_reply_fixture)
    {
      auto it = bin.begin();

      auto const echo = v13::messages::echo_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((echo == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // echo_reply
BOOST_AUTO_TEST_SUITE_END() // message_test

