#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/echo.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace detail = v10::v10_detail;

namespace proto = v10::protocol;

namespace {
struct parameters {
  msg::echo_request::data_type data = "\xAB\xCD\xEF\x01\x02\x03\x04"_bbin;
  std::uint32_t xid = 0x01020304;
};
struct echo_request_fixture : parameters {
  msg::echo_request sut{data, xid};
  std::vector<unsigned char> bin
    = "\x01\x02\x00\x0f\x01\x02\x03\x04""\xAB\xCD\xEF\x01\x02\x03\x04"_bin;
  msg::echo_request no_data_sut{xid};
  std::vector<unsigned char> no_data_bin
    = "\x01\x02\x00\x08\x01\x02\x03\x04""\xAB\xCD\xEF\x01\x02\x03\x04"_bin;
};
struct echo_reply_fixture : parameters {
  msg::echo_reply sut{data, xid};
  std::vector<unsigned char> bin
    = "\x01\x03\x00\x0f\x01\x02\x03\x04""\xAB\xCD\xEF\x01\x02\x03\x04"_bin;
  msg::echo_reply no_data_sut{xid};
  std::vector<unsigned char> no_data_bin
    = "\x01\x03\x00\x08\x01\x02\x03\x04""\xAB\xCD\xEF\x01\x02\x03\x04"_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(echo_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_data)
    {
      auto const data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bbin;
      auto const xid = 0x12345678;

      msg::echo_request sut{data, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_header) + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.data() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_data_without_xid)
    {
      auto const data = "abcdefghijklmnopqrstuvwxyz"_bbin;

      msg::echo_request sut{data};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_header) + data.size());
      BOOST_TEST((sut.data() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = 0x87654321;

      msg::echo_request sut{xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_header));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      msg::echo_request sut{};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_header));
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_echo_request)
    {
      auto const request = msg::echo_request{
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bbin, 0x12345678
      };

      msg::echo_reply sut{request};

      BOOST_TEST(sut.length() == request.length());
      BOOST_TEST(sut.xid() == request.xid());
      BOOST_TEST((sut.data() == request.data()));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_rvalue_echo_request)
    {
      auto const request = msg::echo_request{
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bbin, 0x12345678
      };
      auto rvalue_request = request;

      msg::echo_reply sut{std::move(rvalue_request)};

      BOOST_TEST(sut.length() == request.length());
      BOOST_TEST(sut.xid() == request.xid());
      BOOST_TEST((sut.data() == request.data()));
      BOOST_TEST(rvalue_request.length() == sizeof(detail::ofp_header));
      BOOST_TEST(rvalue_request.data().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, echo_request_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, echo_request_fixture)
    {
      auto const& const_echo_request = sut;

      auto const copy = const_echo_request;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, echo_request_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_header));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::echo_request{data, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_data_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::echo_request{data, xid} == msg::echo_request{data, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (msg::echo_request{"1"_bbin, xid}
        != msg::echo_request{"2"_bbin, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::echo_request{data, 1} != msg::echo_request{data, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, echo_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_no_data_binary_from_no_data_echo_request)
    {
      no_data_bin.resize(sizeof(detail::ofp_header));
      auto buf = std::vector<unsigned char>{};

      no_data_sut.encode(buf);

      BOOST_TEST(buf.size() == sizeof(detail::ofp_header));
      BOOST_TEST(buf == no_data_bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, echo_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_echo_request_from_binary)
    {
      auto it = bin.begin();

      auto const echo_request = msg::echo_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((echo_request == sut));
    }
    BOOST_AUTO_TEST_CASE(constructs_no_data_echo_request_from_no_data_binary)
    {
      auto it = no_data_bin.begin();

      auto const echo_request
        = msg::echo_request::decode(it, no_data_bin.end());

      BOOST_TEST(
          (it == std::next(no_data_bin.begin(), sizeof(detail::ofp_header))));
      BOOST_TEST((echo_request == no_data_sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // echo_request

BOOST_AUTO_TEST_SUITE(echo_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_data)
    {
      auto const data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bbin;
      auto const xid = 0x12345678;

      msg::echo_reply sut{data, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_header) + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.data() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_data_without_xid)
    {
      auto const data = "abcdefghijklmnopqrstuvwxyz"_bbin;

      msg::echo_reply sut{data};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_header) + data.size());
      BOOST_TEST((sut.data() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = 0x87654321;

      msg::echo_reply sut{xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_header));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      msg::echo_reply sut{};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_header));
      BOOST_TEST(sut.data().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, echo_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, echo_reply_fixture)
    {
      auto const& const_echo_reply = sut;

      auto const copy = const_echo_reply;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, echo_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_header));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::echo_reply{data, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_data_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::echo_reply{data, xid} == msg::echo_reply{data, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (msg::echo_reply{"1"_bbin, xid}
        != msg::echo_reply{"2"_bbin, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::echo_reply{data, 1} != msg::echo_reply{data, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, echo_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_no_data_binary_from_no_data_echo_reply)
    {
      no_data_bin.resize(sizeof(detail::ofp_header));
      auto buf = std::vector<unsigned char>{};

      no_data_sut.encode(buf);

      BOOST_TEST(buf.size() == sizeof(detail::ofp_header));
      BOOST_TEST(buf == no_data_bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, echo_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_echo_reply_from_binary)
    {
      auto it = bin.begin();

      auto const echo_reply = msg::echo_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((echo_reply == sut));
    }
    BOOST_AUTO_TEST_CASE(constructs_no_data_echo_reply_from_no_data_binary)
    {
      auto it = no_data_bin.begin();

      auto const echo_reply
        = msg::echo_reply::decode(it, no_data_bin.end());

      BOOST_TEST(
          (it == std::next(no_data_bin.begin(), sizeof(detail::ofp_header))));
      BOOST_TEST((echo_reply == no_data_sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // echo_reply

BOOST_AUTO_TEST_SUITE_END() // message_test
