#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/packet_in.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace detail = v10::v10_detail;

using proto = v10::protocol;

namespace {
struct parameters {
  std::uint32_t buffer_id = 0xf2f4f6f8;
  std::uint16_t total_len = 1024;
  std::uint16_t in_port = 0x1234;
  proto::ofp_packet_in_reason reason = proto::OFPR_ACTION;
  ofp::binary_data data{
    "\x01\x02\x03\x04\x05\x06\xa1\xa2""\xa3\xa4\xa5\xa6\x08\x00"
    "\x45\x00\x00\x42\x12\x34\x00\x00""\x64\x07\xab\xcd\xc0\xa8\x0a\x01"
    "\xc0\xa8\x10\x02"
    "\xf0\x12\x80\x80\x12\x34\x56\x78""\x87\x65\x43\x21\x05\x00\x01\x80"
    "\xab\xcd\x00\x00"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bin
  };
  std::uint32_t xid = 0x12345678;
};
struct packet_in_fixture : parameters {
  msg::packet_in sut{data, total_len, in_port, reason, buffer_id, xid};
  std::vector<unsigned char> bin
    = "\x01\x0a\x00\x62\x12\x34\x56\x78""\xf2\xf4\xf6\xf8\x04\x00\x12\x34"
      "\x01\x00"
      "\x01\x02\x03\x04\x05\x06\xa1\xa2""\xa3\xa4\xa5\xa6\x08\x00"
      "\x45\x00\x00\x42\x12\x34\x00\x00""\x64\x07\xab\xcd\xc0\xa8\x0a\x01"
      "\xc0\xa8\x10\x02"
      "\xf0\x12\x80\x80\x12\x34\x56\x78""\x87\x65\x43\x21\x05\x00\x01\x80"
      "\xab\xcd\x00\x00"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bin;
  msg::packet_in no_data_sut{{}, total_len, in_port, reason, buffer_id, xid};
  std::vector<unsigned char> no_data_bin
    = "\x01\x0a\x00\x12\x12\x34\x56\x78""\xf2\xf4\xf6\xf8\x04\x00\x12\x34"
      "\x01\x00"_bin;
};
}

constexpr auto ofp_packet_in_size = 18;

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(packet_in)

  BOOST_FIXTURE_TEST_SUITE(constructor, parameters)
    BOOST_AUTO_TEST_CASE(is_constructible_from_all_parameters)
    {
      msg::packet_in sut{data, total_len, in_port, reason, buffer_id, xid};

      BOOST_TEST(sut.length() == ofp_packet_in_size + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.total_length() == total_len);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST((sut.frame() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_without_xid)
    {
      auto const reason = proto::OFPR_NO_MATCH;

      msg::packet_in sut{data, total_len, in_port, reason, buffer_id};

      BOOST_TEST(sut.length() == ofp_packet_in_size + data.size());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.total_length() == total_len);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST((sut.frame() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_empty_data)
    {
      msg::packet_in sut{
        ofp::binary_data{}, total_len, in_port, reason, buffer_id
      };

      BOOST_TEST(sut.length() == ofp_packet_in_size);
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.total_length() == total_len);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST(sut.frame().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, packet_in_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, packet_in_fixture)
    {
      auto const& const_packet_in = sut;

      auto const copy = const_packet_in;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, packet_in_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == ofp_packet_in_size);
      BOOST_TEST(moved.frame().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut
        = msg::packet_in{data, total_len, in_port, reason, buffer_id, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_parameters_are_equal)
    {
      BOOST_TEST(
          (msg::packet_in{data, total_len, in_port, reason, buffer_id, xid}
        == msg::packet_in{data, total_len, in_port, reason, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_in{
              ofp::binary_data{"\x01"_bin}, total_len, in_port, reason
            , buffer_id, xid}
        != msg::packet_in{
              ofp::binary_data{"\x02"_bin}, total_len, in_port, reason
            , buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_total_len_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_in{data, 512, in_port, reason, buffer_id, xid}
        != msg::packet_in{data, 256, in_port, reason, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_in_port_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_in{data, total_len, 1, reason, buffer_id, xid}
        != msg::packet_in{data, total_len, 2, reason, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_reason_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_in{
            data, total_len, in_port, proto::OFPR_NO_MATCH, buffer_id, xid}
        != msg::packet_in{
            data, total_len, in_port, proto::OFPR_ACTION, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_buffer_id_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_in{data, total_len, in_port, reason, 1, xid}
        != msg::packet_in{data, total_len, in_port, reason, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_in{data, total_len, in_port, reason, buffer_id, 1}
        != msg::packet_in{data, total_len, in_port, reason, buffer_id, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, packet_in_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_no_data_binary_from_no_data_packet_in)
    {
      auto buf = std::vector<unsigned char>{};

      no_data_sut.encode(buf);

      BOOST_TEST(buf.size() == ofp_packet_in_size);
      BOOST_TEST(buf == no_data_bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, packet_in_fixture)
    BOOST_AUTO_TEST_CASE(constructs_packet_in_from_binary)
    {
      auto it = bin.begin();

      auto const packet_in = msg::packet_in::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((packet_in == sut));
    }
    BOOST_AUTO_TEST_CASE(constructs_no_data_packet_in_from_no_data_binary)
    {
      auto it = no_data_bin.begin();

      auto const packet_in = msg::packet_in::decode(it, no_data_bin.end());

      BOOST_TEST((it == no_data_bin.end()));
      BOOST_TEST((packet_in == no_data_sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // packet_in
BOOST_AUTO_TEST_SUITE_END() // message_test
