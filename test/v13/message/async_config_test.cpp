#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/async_config.hpp>
#include <boost/test/unit_test.hpp>

#include <utility>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace msg = v13::messages;

namespace {
  struct get_async_request_fixture {
    std::uint32_t xid = 0x12345678;
    msg::get_async_request sut{xid};
    std::vector<unsigned char> bin
      = "\x04\x1a\x00\x08\x12\x34\x56\x78"_bin;
  };
  using mask_type = msg::get_async_reply::mask_array_type;
  struct get_async_reply_parameter {
    std::uint32_t packet_in_master_mask = 0x01;
    std::uint32_t packet_in_slave_mask = 0x02;
    mask_type packet_in_mask{{
      packet_in_master_mask, packet_in_slave_mask
    }};
    std::uint32_t port_status_master_mask = 0x03;
    std::uint32_t port_status_slave_mask = 0x04;
    mask_type port_status_mask{{
      port_status_master_mask, port_status_slave_mask
    }};
    std::uint32_t flow_removed_master_mask = 0x05;
    std::uint32_t flow_removed_slave_mask = 0x06;
    mask_type flow_removed_mask{{
      flow_removed_master_mask, flow_removed_slave_mask
    }};
    std::uint32_t xid = 0x12345678;
  };
  struct get_async_reply_fixture : get_async_reply_parameter {
    msg::get_async_reply sut{
      packet_in_mask, port_status_mask, flow_removed_mask, xid
    };
    std::vector<unsigned char> bin
      = "\x04\x1b\x00\x20\x12\x34\x56\x78""\x00\x00\x00\x01\x00\x00\x00\x02"
        "\x00\x00\x00\x03\x00\x00\x00\x04""\x00\x00\x00\x05\x00\x00\x00\x06"
        ""_bin;
  };
  struct set_async_parameter {
    std::uint32_t packet_in_master_mask = 0x01020304;
    std::uint32_t packet_in_slave_mask = 0x10203040;
    mask_type packet_in_mask{{
      packet_in_master_mask, packet_in_slave_mask
    }};
    std::uint32_t port_status_master_mask = 0x05060708;
    std::uint32_t port_status_slave_mask = 0x50607080;
    mask_type port_status_mask{{
      port_status_master_mask, port_status_slave_mask
    }};
    std::uint32_t flow_removed_master_mask = 0x0a0b0c0d;
    std::uint32_t flow_removed_slave_mask = 0xa0b0c0d0;
    mask_type flow_removed_mask{{
      flow_removed_master_mask, flow_removed_slave_mask
    }};
    std::uint32_t xid = 0x12345678;
  };
  struct set_async_fixture : set_async_parameter {
    msg::set_async sut{
      packet_in_mask, port_status_mask, flow_removed_mask, xid
    };
    std::vector<unsigned char> bin
      = "\x04\x1c\x00\x20\x12\x34\x56\x78""\x01\x02\x03\x04\x10\x20\x30\x40"
        "\x05\x06\x07\x08\x50\x60\x70\x80""\x0a\x0b\x0c\x0d\xa0\xb0\xc0\xd0"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(get_async_request)

  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(type_is_get_async_request)
    {
      using sut = msg::get_async_request;

      constexpr auto type = sut::type();

      BOOST_TEST(type == protocol::OFPT_GET_ASYNC_REQUEST);
    }
    BOOST_AUTO_TEST_CASE(min_length_is_size_of_ofp_header)
    {
      using sut = msg::get_async_request;

      constexpr auto min_length = sut::min_length();

      BOOST_TEST(min_length == sizeof(protocol::ofp_header));
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_xid)
    {
      auto const xid = std::uint32_t{123};

      msg::get_async_request const sut{xid};

      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      msg::get_async_request const sut{};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, get_async_request_fixture)
    {
      auto const& const_get_async_request = sut;

      auto const copy = const_get_async_request;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, get_async_request_fixture)
    {
      auto const& moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = msg::get_async_request{0x1234};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = std::uint32_t{0x12121212};

      BOOST_TEST(
          (msg::get_async_request{xid}
        == msg::get_async_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_async_request{0x1234}
        != msg::get_async_request{0x1235}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, get_async_request_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, get_async_request_fixture)
    {
      auto it = bin.begin();

      auto const get_async_request
        = msg::get_async_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((get_async_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // get_async_request

BOOST_AUTO_TEST_SUITE(get_async_reply)

  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(type_is_get_async_reply)
    {
      using sut = msg::get_async_reply;

      constexpr auto type = sut::type();

      BOOST_TEST(type == protocol::OFPT_GET_ASYNC_REPLY);
    }
    BOOST_AUTO_TEST_CASE(min_length_is_size_of_ofp_async_config)
    {
      using sut = msg::get_async_reply;

      constexpr auto min_length = sut::min_length();

      BOOST_TEST(min_length == sizeof(protocol::ofp_async_config));
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_each_mask_and_xid)
    {
      auto const packet_in_master_mask = 0x01;
      auto const packet_in_slave_mask = 0x02;
      auto const port_status_master_mask = 0x03;
      auto const port_status_slave_mask = 0x04;
      auto const flow_removed_master_mask = 0x05;
      auto const flow_removed_slave_mask = 0x06;
      auto const xid = std::uint32_t{0x12345678};

      msg::get_async_reply const sut{
          packet_in_master_mask, packet_in_slave_mask
        , port_status_master_mask, port_status_slave_mask
        , flow_removed_master_mask ,flow_removed_slave_mask
        , xid
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_async_config));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.packet_in_master_mask() == packet_in_master_mask);
      BOOST_TEST(sut.packet_in_slave_mask() == packet_in_slave_mask);
      BOOST_TEST(
             sut.packet_in_mask()
          == (mask_type{{ packet_in_master_mask, packet_in_slave_mask }})
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.port_status_master_mask() == port_status_master_mask);
      BOOST_TEST(sut.port_status_slave_mask() == port_status_slave_mask);
      BOOST_TEST(
             sut.port_status_mask()
          == (mask_type{{ port_status_master_mask, port_status_slave_mask }})
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.flow_removed_master_mask() == flow_removed_master_mask);
      BOOST_TEST(sut.flow_removed_slave_mask() == flow_removed_slave_mask);
      BOOST_TEST(
             sut.flow_removed_mask()
          == (mask_type{{ flow_removed_master_mask, flow_removed_slave_mask }})
          , boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_each_mask_without_xid)
    {
      auto const packet_in_master_mask = 0x10;
      auto const packet_in_slave_mask = 0x20;
      auto const port_status_master_mask = 0x30;
      auto const port_status_slave_mask = 0x40;
      auto const flow_removed_master_mask = 0x50;
      auto const flow_removed_slave_mask = 0x60;

      msg::get_async_reply const sut{
          packet_in_master_mask, packet_in_slave_mask
        , port_status_master_mask, port_status_slave_mask
        , flow_removed_master_mask ,flow_removed_slave_mask
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_async_config));
      BOOST_TEST(sut.packet_in_master_mask() == packet_in_master_mask);
      BOOST_TEST(sut.packet_in_slave_mask() == packet_in_slave_mask);
      BOOST_TEST(
             sut.packet_in_mask()
          == (mask_type{{ packet_in_master_mask, packet_in_slave_mask }})
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.port_status_master_mask() == port_status_master_mask);
      BOOST_TEST(sut.port_status_slave_mask() == port_status_slave_mask);
      BOOST_TEST(
             sut.port_status_mask()
          == (mask_type{{ port_status_master_mask, port_status_slave_mask }})
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.flow_removed_master_mask() == flow_removed_master_mask);
      BOOST_TEST(sut.flow_removed_slave_mask() == flow_removed_slave_mask);
      BOOST_TEST(
             sut.flow_removed_mask()
          == (mask_type{{ flow_removed_master_mask, flow_removed_slave_mask }})
          , boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_mask_and_xid)
    {
      auto const packet_in_mask = mask_type{{0x12, 0x34}};
      auto const port_status_mask = mask_type{{0x56, 0x78}};
      auto const flow_removed_mask = mask_type{{0x11, 0x22}};
      auto const xid = std::uint32_t{0xa1b2c3d4};

      msg::get_async_reply const sut{
        packet_in_mask, port_status_mask, flow_removed_mask, xid
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_async_config));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.packet_in_master_mask() == packet_in_mask[0]);
      BOOST_TEST(sut.packet_in_slave_mask() == packet_in_mask[1]);
      BOOST_TEST(
            sut.packet_in_mask() == packet_in_mask
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.port_status_master_mask() == port_status_mask[0]);
      BOOST_TEST(sut.port_status_slave_mask() == port_status_mask[1]);
      BOOST_TEST(
            sut.port_status_mask() == port_status_mask
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.flow_removed_master_mask() == flow_removed_mask[0]);
      BOOST_TEST(sut.flow_removed_slave_mask() == flow_removed_mask[1]);
      BOOST_TEST(
            sut.flow_removed_mask() == flow_removed_mask
          , boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_mask_without_xid)
    {
      auto const packet_in_mask = mask_type{{0x21, 0x43}};
      auto const port_status_mask = mask_type{{0x65, 0x87}};
      auto const flow_removed_mask = mask_type{{0x33, 0x44}};

      msg::get_async_reply const sut{
        packet_in_mask, port_status_mask, flow_removed_mask
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_async_config));
      BOOST_TEST(sut.packet_in_master_mask() == packet_in_mask[0]);
      BOOST_TEST(sut.packet_in_slave_mask() == packet_in_mask[1]);
      BOOST_TEST(
            sut.packet_in_mask() == packet_in_mask
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.port_status_master_mask() == port_status_mask[0]);
      BOOST_TEST(sut.port_status_slave_mask() == port_status_mask[1]);
      BOOST_TEST(
            sut.port_status_mask() == port_status_mask
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.flow_removed_master_mask() == flow_removed_mask[0]);
      BOOST_TEST(sut.flow_removed_slave_mask() == flow_removed_mask[1]);
      BOOST_TEST(
            sut.flow_removed_mask() == flow_removed_mask
          , boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, get_async_reply_fixture)
    {
      auto const& const_get_async_reply = sut;

      auto const copy = const_get_async_reply;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, get_async_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, get_async_reply_fixture)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = msg::get_async_reply{
        packet_in_mask, port_status_mask, flow_removed_mask, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_all_masks_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::get_async_reply{
              packet_in_mask, port_status_mask, flow_removed_mask, xid}
        == msg::get_async_reply{
              packet_in_mask, port_status_mask, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_in_master_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_async_reply{
              {{1, 2}}, port_status_mask, flow_removed_mask, xid}
        != msg::get_async_reply{
              {{2, 2}}, port_status_mask, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_in_slave_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_async_reply{
              {{1, 1}}, port_status_mask, flow_removed_mask, xid}
        != msg::get_async_reply{
              {{1, 2}}, port_status_mask, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_status_master_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_async_reply{
              packet_in_mask, {{1, 2}}, flow_removed_mask, xid}
        != msg::get_async_reply{
              packet_in_mask, {{2, 2}}, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_status_slave_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_async_reply{
              packet_in_mask, {{1, 1}}, flow_removed_mask, xid}
        != msg::get_async_reply{
              packet_in_mask, {{1, 2}}, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flow_removed_master_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_async_reply{
              packet_in_mask, port_status_mask, {{1, 2}}, xid}
        != msg::get_async_reply{
              packet_in_mask, port_status_mask, {{2, 2}}, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flow_removed_slave_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_async_reply{
              packet_in_mask, port_status_mask, {{1, 1}}, xid}
        != msg::get_async_reply{
              packet_in_mask, port_status_mask, {{1, 2}}, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_async_reply{
              packet_in_mask, port_status_mask, flow_removed_mask, 1}
        != msg::get_async_reply{
              packet_in_mask, port_status_mask, flow_removed_mask, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, get_async_reply_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, get_async_reply_fixture)
    {
      auto it = bin.begin();

      auto const get_async_reply = msg::get_async_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((get_async_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // get_async_reply

BOOST_AUTO_TEST_SUITE(set_async)

  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(type_is_set_async)
    {
      using sut = msg::set_async;

      constexpr auto type = sut::type();

      BOOST_TEST(type == protocol::OFPT_SET_ASYNC);
    }
    BOOST_AUTO_TEST_CASE(min_length_is_size_of_ofp_async_config)
    {
      using sut = msg::set_async;

      constexpr auto min_length = sut::min_length();

      BOOST_TEST(min_length == sizeof(protocol::ofp_async_config));
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_each_mask_and_xid)
    {
      auto const packet_in_master_mask = 0x01;
      auto const packet_in_slave_mask = 0x02;
      auto const port_status_master_mask = 0x03;
      auto const port_status_slave_mask = 0x04;
      auto const flow_removed_master_mask = 0x05;
      auto const flow_removed_slave_mask = 0x06;
      auto const xid = std::uint32_t{0x12345678};

      msg::set_async const sut{
          packet_in_master_mask, packet_in_slave_mask
        , port_status_master_mask, port_status_slave_mask
        , flow_removed_master_mask ,flow_removed_slave_mask
        , xid
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_async_config));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.packet_in_master_mask() == packet_in_master_mask);
      BOOST_TEST(sut.packet_in_slave_mask() == packet_in_slave_mask);
      BOOST_TEST(
             sut.packet_in_mask()
          == (mask_type{{ packet_in_master_mask, packet_in_slave_mask }})
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.port_status_master_mask() == port_status_master_mask);
      BOOST_TEST(sut.port_status_slave_mask() == port_status_slave_mask);
      BOOST_TEST(
             sut.port_status_mask()
          == (mask_type{{ port_status_master_mask, port_status_slave_mask }})
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.flow_removed_master_mask() == flow_removed_master_mask);
      BOOST_TEST(sut.flow_removed_slave_mask() == flow_removed_slave_mask);
      BOOST_TEST(
             sut.flow_removed_mask()
          == (mask_type{{ flow_removed_master_mask, flow_removed_slave_mask }})
          , boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_each_mask_without_xid)
    {
      auto const packet_in_master_mask = 0x10;
      auto const packet_in_slave_mask = 0x20;
      auto const port_status_master_mask = 0x30;
      auto const port_status_slave_mask = 0x40;
      auto const flow_removed_master_mask = 0x50;
      auto const flow_removed_slave_mask = 0x60;

      msg::set_async const sut{
          packet_in_master_mask, packet_in_slave_mask
        , port_status_master_mask, port_status_slave_mask
        , flow_removed_master_mask ,flow_removed_slave_mask
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_async_config));
      BOOST_TEST(sut.packet_in_master_mask() == packet_in_master_mask);
      BOOST_TEST(sut.packet_in_slave_mask() == packet_in_slave_mask);
      BOOST_TEST(
             sut.packet_in_mask()
          == (mask_type{{ packet_in_master_mask, packet_in_slave_mask }})
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.port_status_master_mask() == port_status_master_mask);
      BOOST_TEST(sut.port_status_slave_mask() == port_status_slave_mask);
      BOOST_TEST(
             sut.port_status_mask()
          == (mask_type{{ port_status_master_mask, port_status_slave_mask }})
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.flow_removed_master_mask() == flow_removed_master_mask);
      BOOST_TEST(sut.flow_removed_slave_mask() == flow_removed_slave_mask);
      BOOST_TEST(
             sut.flow_removed_mask()
          == (mask_type{{ flow_removed_master_mask, flow_removed_slave_mask }})
          , boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_mask_and_xid)
    {
      auto const packet_in_mask = mask_type{{0x12, 0x34}};
      auto const port_status_mask = mask_type{{0x56, 0x78}};
      auto const flow_removed_mask = mask_type{{0x11, 0x22}};
      auto const xid = std::uint32_t{0xa1b2c3d4};

      msg::set_async const sut{
        packet_in_mask, port_status_mask, flow_removed_mask, xid
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_async_config));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.packet_in_master_mask() == packet_in_mask[0]);
      BOOST_TEST(sut.packet_in_slave_mask() == packet_in_mask[1]);
      BOOST_TEST(
            sut.packet_in_mask() == packet_in_mask
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.port_status_master_mask() == port_status_mask[0]);
      BOOST_TEST(sut.port_status_slave_mask() == port_status_mask[1]);
      BOOST_TEST(
            sut.port_status_mask() == port_status_mask
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.flow_removed_master_mask() == flow_removed_mask[0]);
      BOOST_TEST(sut.flow_removed_slave_mask() == flow_removed_mask[1]);
      BOOST_TEST(
            sut.flow_removed_mask() == flow_removed_mask
          , boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_mask_without_xid)
    {
      auto const packet_in_mask = mask_type{{0x21, 0x43}};
      auto const port_status_mask = mask_type{{0x65, 0x87}};
      auto const flow_removed_mask = mask_type{{0x33, 0x44}};

      msg::set_async const sut{
        packet_in_mask, port_status_mask, flow_removed_mask
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_async_config));
      BOOST_TEST(sut.packet_in_master_mask() == packet_in_mask[0]);
      BOOST_TEST(sut.packet_in_slave_mask() == packet_in_mask[1]);
      BOOST_TEST(
            sut.packet_in_mask() == packet_in_mask
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.port_status_master_mask() == port_status_mask[0]);
      BOOST_TEST(sut.port_status_slave_mask() == port_status_mask[1]);
      BOOST_TEST(
            sut.port_status_mask() == port_status_mask
          , boost::test_tools::per_element{});
      BOOST_TEST(sut.flow_removed_master_mask() == flow_removed_mask[0]);
      BOOST_TEST(sut.flow_removed_slave_mask() == flow_removed_mask[1]);
      BOOST_TEST(
            sut.flow_removed_mask() == flow_removed_mask
          , boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, set_async_fixture)
    {
      auto const& const_set_async = sut;

      auto const copy = const_set_async;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, set_async_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, set_async_fixture)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = msg::set_async{
        packet_in_mask, port_status_mask, flow_removed_mask, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_all_masks_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::set_async{
              packet_in_mask, port_status_mask, flow_removed_mask, xid}
        == msg::set_async{
              packet_in_mask, port_status_mask, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_in_master_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::set_async{
              {{1, 2}}, port_status_mask, flow_removed_mask, xid}
        != msg::set_async{
              {{2, 2}}, port_status_mask, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_in_slave_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::set_async{
              {{1, 1}}, port_status_mask, flow_removed_mask, xid}
        != msg::set_async{
              {{1, 2}}, port_status_mask, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_status_master_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::set_async{
              packet_in_mask, {{1, 2}}, flow_removed_mask, xid}
        != msg::set_async{
              packet_in_mask, {{2, 2}}, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_port_status_slave_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::set_async{
              packet_in_mask, {{1, 1}}, flow_removed_mask, xid}
        != msg::set_async{
              packet_in_mask, {{1, 2}}, flow_removed_mask, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flow_removed_master_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::set_async{
              packet_in_mask, port_status_mask, {{1, 2}}, xid}
        != msg::set_async{
              packet_in_mask, port_status_mask, {{2, 2}}, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flow_removed_slave_mask_is_not_equal)
    {
      BOOST_TEST(
          (msg::set_async{
              packet_in_mask, port_status_mask, {{1, 1}}, xid}
        != msg::set_async{
              packet_in_mask, port_status_mask, {{1, 2}}, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::set_async{
              packet_in_mask, port_status_mask, flow_removed_mask, 1}
        != msg::set_async{
              packet_in_mask, port_status_mask, flow_removed_mask, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, set_async_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, set_async_fixture)
    {
      auto it = bin.begin();

      auto const set_async = msg::set_async::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((set_async == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // set_async

BOOST_AUTO_TEST_SUITE_END() // message_test
