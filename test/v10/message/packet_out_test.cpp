#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/packet_out.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace detail = v10::v10_detail;

namespace proto = v10::protocol;

namespace {
struct parameters : action_fixture {
  std::uint32_t buffer_id = 0xf2f4f6f8;
  std::uint16_t in_port = proto::OFPP_CONTROLLER;
  v10::action_list actions{ set_vlan_vid, output, strip_vlan, enqueue };
  std::uint32_t xid = 0x12345678;
  ofp::binary_data data{
    "\x01\x02\x03\x04\x05\x06\xa1\xa2""\xa3\xa4\xa5\xa6\x08\x00"
    "\x45\x00\x00\x42\x12\x34\x00\x00""\x64\x07\xab\xcd\xc0\xa8\x0a\x01"
    "\xc0\xa8\x10\x02"
    "\xf0\x12\x80\x80\x12\x34\x56\x78""\x87\x65\x43\x21\x05\x00\x01\x80"
    "\xab\xcd\x00\x00"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_bin
  };
};
struct packet_out_fixture : parameters {
  msg::packet_out sut{data, in_port, actions, xid};
  std::vector<unsigned char> bin
    = "\x01\x0d\x00\x88\x12\x34\x56\x78""\xff\xff\xff\xff\xff\xfd\x00\x28"

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
  msg::packet_out no_data_sut{buffer_id, in_port, actions, xid};
  std::vector<unsigned char> no_data_bin
    = "\x01\x0d\x00\x38\x12\x34\x56\x78""\xf2\xf4\xf6\xf8\xff\xfd\x00\x28"
      "\x00\x01\x00\x08\x01\x23\x00\x00"
      "\x00\x00\x00\x08\xff\xfd\x00\x40"
      "\x00\x03\x00\x08\x00\x00\x00\x00"
      "\x00\x0b\x00\x10\x00\x01\x00\x00""\x00\x00\x00\x00\x12\x34\x56\x78"_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(packet_out)

  BOOST_FIXTURE_TEST_SUITE(constructor, parameters)
    BOOST_AUTO_TEST_CASE(is_constructible_from_buffer_id)
    {
      msg::packet_out sut{buffer_id, in_port, actions, xid};

      BOOST_TEST(
          sut.length() == sizeof(detail::ofp_packet_out) + actions.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == actions.length());
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.frame_length() == 0);
      BOOST_TEST(sut.frame().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_buffer_id_without_xid)
    {
      msg::packet_out sut{buffer_id, in_port, actions};

      BOOST_TEST(
          sut.length() == sizeof(detail::ofp_packet_out) + actions.length());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == actions.length());
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.frame_length() == 0);
      BOOST_TEST(sut.frame().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_buffer_id_without_in_port)
    {
      msg::packet_out sut{buffer_id, actions, xid};

      BOOST_TEST(
          sut.length() == sizeof(detail::ofp_packet_out) + actions.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.in_port() == proto::OFPP_NONE);
      BOOST_TEST(sut.actions_length() == actions.length());
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.frame_length() == 0);
      BOOST_TEST(sut.frame().empty());
    }
    BOOST_AUTO_TEST_CASE(
        is_constructible_from_buffer_id_without_in_port_and_xid)
    {
      msg::packet_out sut{buffer_id, actions};

      BOOST_TEST(
          sut.length() == sizeof(detail::ofp_packet_out) + actions.length());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.in_port() == proto::OFPP_NONE);
      BOOST_TEST(sut.actions_length() == actions.length());
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.frame_length() == 0);
      BOOST_TEST(sut.frame().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_buffer_id_and_empty_actions)
    {
      auto const empty_actions = v10::action_list{};

      msg::packet_out sut{buffer_id, in_port, empty_actions, xid};

      BOOST_TEST(sut.length() == sizeof(detail::ofp_packet_out));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == 0);
      BOOST_TEST((sut.actions() == empty_actions));
      BOOST_TEST(sut.frame_length() == 0);
      BOOST_TEST(sut.frame().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_data)
    {
      msg::packet_out sut{data, in_port, actions, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_packet_out) + actions.length() + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == proto::OFP_NO_BUFFER);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == actions.length());
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST((sut.frame() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_data_without_xid)
    {
      msg::packet_out sut{data, in_port, actions};

      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_packet_out) + actions.length() + data.size());
      BOOST_TEST(sut.buffer_id() == proto::OFP_NO_BUFFER);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == actions.length());
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST((sut.frame() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_data_without_in_port)
    {
      msg::packet_out sut{data, actions, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_packet_out) + actions.length() + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == proto::OFP_NO_BUFFER);
      BOOST_TEST(sut.in_port() == proto::OFPP_NONE);
      BOOST_TEST(sut.actions_length() == actions.length());
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST((sut.frame() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_data_without_in_port_and_xid)
    {
      msg::packet_out sut{data, actions};

      BOOST_TEST(
          sut.length()
       == sizeof(detail::ofp_packet_out) + actions.length() + data.size());
      BOOST_TEST(sut.buffer_id() == proto::OFP_NO_BUFFER);
      BOOST_TEST(sut.in_port() == proto::OFPP_NONE);
      BOOST_TEST(sut.actions_length() == actions.length());
      BOOST_TEST((sut.actions() == actions));
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST((sut.frame() == data));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_data_and_empty_actions)
    {
      auto const empty_actions = v10::action_list{};

      msg::packet_out sut{data, in_port, empty_actions, xid};

      BOOST_TEST(
          sut.length() == sizeof(detail::ofp_packet_out) + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == proto::OFP_NO_BUFFER);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == 0);
      BOOST_TEST((sut.actions() == empty_actions));
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST((sut.frame() == data));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, packet_out_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, packet_out_fixture)
    {
      auto const& const_packet_out = sut;

      auto const copy = const_packet_out;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, packet_out_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.actions_length() == 0);
      BOOST_TEST((moved.actions() == v10::action_list{}));
      BOOST_TEST(moved.frame_length() == 0);
      BOOST_TEST(moved.frame().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::packet_out{data, in_port, actions, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_data_and_in_port_and_actions_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::packet_out{data, in_port, actions, xid}
        == msg::packet_out{data, in_port, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_buffer_id_and_in_port_and_actions_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::packet_out{buffer_id, in_port, actions, xid}
        == msg::packet_out{buffer_id, in_port, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_out{
              ofp::binary_data{"\x01"_bin}, in_port, actions, xid}
        != msg::packet_out{
              ofp::binary_data{"\x02"_bin}, in_port, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_buffer_id_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_out{0x1, in_port, actions, xid}
        != msg::packet_out{0x2, in_port, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_in_port_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_out{buffer_id, 1, actions, xid}
        != msg::packet_out{buffer_id, 2, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_actions_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_out{
              buffer_id, in_port, v10::action_list{set_vlan_pcp}, xid}
        != msg::packet_out{
              buffer_id, in_port, v10::action_list{set_vlan_vid}, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::packet_out{buffer_id, in_port, actions, 1}
        != msg::packet_out{buffer_id, in_port, actions, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(extract_frame, packet_out_fixture)
    BOOST_AUTO_TEST_CASE(returns_frame_and_make_source_data_empty)
    {
      auto const original = ofp::binary_data{sut.frame()};
      auto const original_length = sut.length();

      auto const frame = sut.extract_frame();

      BOOST_TEST(boost::equal(frame, original));
      BOOST_TEST(sut.length() == original_length - frame.size());
      BOOST_TEST(sut.frame().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_frame

  BOOST_FIXTURE_TEST_SUITE(encode, packet_out_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_no_data_binary_from_no_data_packet_out)
    {
      auto buf = std::vector<unsigned char>{};

      no_data_sut.encode(buf);

      BOOST_TEST(buf.size() == no_data_sut.length());
      BOOST_TEST(buf == no_data_bin, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generates_no_actions_binary_from_no_actions_packet_out)
    {
      no_data_bin.resize(sizeof(detail::ofp_packet_out));
      no_data_bin[3] = no_data_bin.size();
      no_data_bin[offsetof(detail::ofp_packet_out, actions_len) + 1] = 0;
      auto const sut
        = msg::packet_out{buffer_id, in_port, v10::action_list{}, xid};

      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == no_data_bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, packet_out_fixture)
    BOOST_AUTO_TEST_CASE(constructs_packet_out_from_binary)
    {
      auto it = bin.begin();

      auto const packet_out = msg::packet_out::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((packet_out == sut));
    }
    BOOST_AUTO_TEST_CASE(constructs_no_data_packet_out_from_no_data_binary)
    {
      auto it = no_data_bin.begin();

      auto const packet_out = msg::packet_out::decode(it, no_data_bin.end());

      BOOST_TEST((it == no_data_bin.end()));
      BOOST_TEST((packet_out == no_data_sut));
    }
    BOOST_AUTO_TEST_CASE(generates_no_actions_binary_from_no_actions_packet_out)
    {
      constexpr auto length = sizeof(detail::ofp_packet_out);
      no_data_bin[3] = length;
      no_data_bin[offsetof(detail::ofp_packet_out, actions_len) + 1] = 0;
      auto it = no_data_bin.begin();

      auto const packet_out = msg::packet_out::decode(it, no_data_bin.end());

      BOOST_TEST((it == std::next(no_data_bin.begin(), length)));
      BOOST_TEST(
          (packet_out
        == msg::packet_out{buffer_id, in_port, v10::action_list{}, xid}));
    }
    BOOST_AUTO_TEST_CASE(
        throws_exception_if_actions_len_is_larger_than_binary_size)
    {
      no_data_bin[offsetof(detail::ofp_packet_out, actions_len)] = 0xff;
      auto it = no_data_bin.begin();

      BOOST_CHECK_THROW(
          msg::packet_out::decode(it, no_data_bin.end()), std::runtime_error);
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // packet_out
BOOST_AUTO_TEST_SUITE_END() // message_test
