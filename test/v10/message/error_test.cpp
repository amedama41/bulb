#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/error.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"
#include <canard/network/openflow/v10/message/flow_add.hpp>
#include <canard/network/openflow/v10/message/packet_out.hpp>

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace detail = v10::v10_detail;

using proto = v10::protocol;

namespace {
struct parameters : action_fixture {
  proto::ofp_error_type type = proto::OFPET_BAD_REQUEST;
  std::uint16_t code = proto::OFPBRC_BAD_LEN;
  std::uint32_t xid = 0x12345678;
  ofp::binary_data data{
    "\x01\x02\x03\x04\x05\x06\xa1\xa2""\xa3\xa4\xa5\xa6\x08\x00"
    "\x45\x00\x00\x42\x12\x34\x00\x00""\x64\x07\xab\xcd\xc0\xa8\x0a\x01"
    "\xc0\xa8\x10\x02"
    "\xf0\x12\x80\x80\x12\x34\x56\x78""\x87\x65\x43\x21\x05\x00\x01\x80"
    "\xab\xcd\x00\x00"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bin
  };
  msg::packet_out packet_out{
      data, proto::OFPP_CONTROLLER
    , v10::action_list{ set_vlan_vid, output, strip_vlan, enqueue }
    , 0x11223344
  };
};
struct error_fixture : parameters {
  msg::error sut{type, code, packet_out};
  std::vector<unsigned char> bin
    = "\x01\x01\x00\x94\x11\x22\x33\x44""\x00\x01\x00\x06"
      "\x01\x0d\x00\x88\x11\x22\x33\x44""\xff\xff\xff\xff\xff\xfd\x00\x28"
      "\x00\x01\x00\x08\x01\x23\x00\x00"
      "\x00\x00\x00\x08\xff\xfd\x00\x40"
      "\x00\x03\x00\x08\x00\x00\x00\x00"
      "\x00\x0b\x00\x10\x00\x01\x00\x00""\x00\x00\x00\x00\x12\x34\x56\x78"
      "\x01\x02\x03\x04\x05\x06\xa1\xa2""\xa3\xa4\xa5\xa6\x08\x00"
      "\x45\x00\x00\x42\x12\x34\x00\x00""\x64\x07\xab\xcd\xc0\xa8\x0a\x01"
      "\xc0\xa8\x10\x02"
      "\xf0\x12\x80\x80\x12\x34\x56\x78""\x87\x65\x43\x21\x05\x00\x01\x80"
      "\xab\xcd\x00\x00"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bin;
  msg::error no_data_sut{type, code, ofp::binary_data{}, xid};
  std::vector<unsigned char> no_data_bin
    = "\x01\x01\x00\x0c\x12\x34\x56\x78""\x00\x01\x00\x06"_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(error)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_data)
    {
      auto const type = proto::OFPET_HELLO_FAILED;
      auto const code = proto::OFPHFC_INCOMPATIBLE;
      auto const data = ofp::binary_data{"incompatible version"};
      auto const xid = 0x01010202;

      msg::error sut{type, code, data, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_error_msg) + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.error_type() == type);
      BOOST_TEST(sut.error_code() == code);
      BOOST_TEST((sut.data() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_data_without_xid)
    {
      auto const type = proto::OFPET_HELLO_FAILED;
      auto const code = proto::OFPHFC_EPERM;
      auto const data = ofp::binary_data{"permission error"};

      msg::error sut{type, code, data};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_error_msg) + data.size());
      BOOST_TEST(sut.error_type() == type);
      BOOST_TEST(sut.error_code() == code);
      BOOST_TEST((sut.data() == data));
    }
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_message, action_fixture)
    {
      auto const type = proto::OFPET_FLOW_MOD_FAILED;
      auto const code = proto::OFPFMFC_OVERLAP;
      auto const req = msg::flow_add{
        v10::flow_entry{{v10::match{}, 1}, 1, {}}
      };

      msg::error sut{type, code, req};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_error_msg) + req.length());
      BOOST_TEST(sut.xid() == req.xid());
      BOOST_TEST(sut.error_type() == type);
      BOOST_TEST(sut.error_code() == code);
      auto buf = std::vector<unsigned char>{};
      BOOST_TEST((sut.data() == req.encode(buf)));

      auto const header = sut.failed_request_header();
      BOOST_TEST(header.version == req.version());
      BOOST_TEST(header.type == req.type());
      BOOST_TEST(header.length == req.length());
      BOOST_TEST(header.xid == req.xid());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_message_and_data_size, parameters)
    {
      auto const data_size = 64;

      msg::error sut{type, code, packet_out, data_size};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_error_msg) + data_size);
      BOOST_TEST(sut.xid() == packet_out.xid());
      BOOST_TEST(sut.error_type() == type);
      BOOST_TEST(sut.error_code() == code);

      auto const header = sut.failed_request_header();
      BOOST_TEST(header.version == packet_out.version());
      BOOST_TEST(header.type == packet_out.type());
      BOOST_TEST(header.length == packet_out.length());
      BOOST_TEST(header.xid == packet_out.xid());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, error_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, error_fixture)
    {
      auto const& const_error = sut;

      auto const copy = const_error;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, error_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_error_msg));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::error{type, code, packet_out};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_type_and_code_and_data_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::error{type, code, data, xid}
        == msg::error{type, code, data, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_type_is_not_equal)
    {
      BOOST_TEST(
          (msg::error{proto::OFPET_BAD_ACTION, code, data, xid}
        != msg::error{proto::OFPET_QUEUE_OP_FAILED, code, data, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_code_is_not_equal)
    {
      BOOST_TEST(
          (msg::error{type, proto::OFPBRC_BAD_TYPE, data, xid}
        != msg::error{type, proto::OFPBRC_BAD_STAT, data, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (msg::error{type, code, ofp::binary_data{"1"}, xid}
        != msg::error{type, code, ofp::binary_data{"2"}, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::error{type, code, data, 1}
        != msg::error{type, code, data, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, error_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_no_data_binary_from_no_data_error)
    {
      auto buf = std::vector<unsigned char>{};

      no_data_sut.encode(buf);

      BOOST_TEST(buf.size() == sizeof(detail::ofp_error_msg));
      BOOST_TEST(buf == no_data_bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, error_fixture)
    BOOST_AUTO_TEST_CASE(constructs_error_from_binary)
    {
      auto it = bin.begin();

      auto const error = msg::error::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((error == sut));
    }
    BOOST_AUTO_TEST_CASE(constructs_no_data_error_from_no_data_binary)
    {
      auto it = no_data_bin.begin();

      auto const error = msg::error::decode(it, no_data_bin.end());

      BOOST_TEST((it == no_data_bin.end()));
      BOOST_TEST((error == no_data_sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // error
BOOST_AUTO_TEST_SUITE_END() // message_test
