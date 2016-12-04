#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/message/group_mod.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <utility>

#include "../../test_utility.hpp"

using boost::asio::ip::address_v4;

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace msg = v13::messages;
namespace actions = v13::actions;
namespace v13_detail = v13::v13_detail;
using v13::protocol;

namespace {
struct group_add_parameters {
  std::uint32_t group_id = 0x01020304;
  std::uint8_t group_type = protocol::OFPGT_SELECT;
  std::uint16_t weight1 = 0x1122;
  std::uint16_t weight2 = 0x3344;
  actions::push_vlan push_vlan{0x8100};
  actions::set_vlan_vid vlan_vid{4095};
  actions::output output1{0x11223344};
  actions::set_ipv4_src set_ipv4_src{address_v4{0x7f000001}};
  actions::set_ipv4_dst set_ipv4_dst{address_v4{0x7f000002}};
  actions::output output2{0x87654321};
  std::uint32_t xid = 0x12345678;
};
struct group_add_fixture : group_add_parameters {
  msg::group_add sut{
      group_id, group_type, {
          v13::bucket{weight1, { push_vlan, vlan_vid, output1 }}
        , v13::bucket{weight2, { set_ipv4_src, set_ipv4_dst, output2 }}
      }
    , xid
  };
  std::vector<unsigned char> bin
    = "\x04\x0f\x00\x88\x12\x34\x56\x78""\x00\x00\x01\x00\x01\x02\x03\x04"
      "\x00\x38\x11\x22\xff\xff\xff\xff""\xff\xff\xff\xff\x00\x00\x00\x00"
      "\x00\x11\x00\x08\x81\x00\x00\x00""\x00\x19\x00\x10\x80\x00\x0c\x02"
      "\x0f\xff\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x10\x11\x22\x33\x44"
      "\xff\xff\x00\x00\x00\x00\x00\x00"
      "\x00\x40\x33\x44\xff\xff\xff\xff""\xff\xff\xff\xff\x00\x00\x00\x00"
      "\x00\x19\x00\x10\x80\x00\x16\x04""\x7f\x00\x00\x01\x00\x00\x00\x00"
      "\x00\x19\x00\x10\x80\x00\x18\x04""\x7f\x00\x00\x02\x00\x00\x00\x00"
      "\x00\x00\x00\x10\x87\x65\x43\x21""\xff\xff\x00\x00\x00\x00\x00\x00"
      ""_bin;
};
struct group_delete_fixture {
  msg::group_delete sut{
    protocol::OFPG_MAX, 0x12345678
  };
  std::vector<unsigned char> bin
    = "\x04\x0f\x00\x10\x12\x34\x56\x78""\x00\x02\x00\x00\xff\xff\xff\x00"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(group_add_test)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(
        is_constructible_from_group_id_and_type_and_single_bucket)
    {
      auto const group_id = 0;
      auto const sut = msg::group_add{
          group_id, protocol::OFPGT_INDIRECT
        , {v13::bucket{actions::output{4}}}
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_GROUP_MOD);
      BOOST_TEST(sut.length() == sizeof(v13_detail::ofp_group_mod) + sizeof(v13_detail::ofp_bucket) + sizeof(v13_detail::ofp_action_output));
      BOOST_TEST(sut.command() == protocol::OFPGC_ADD);
      BOOST_TEST(sut.group_id() == group_id);
      BOOST_TEST(sut.group_type() == protocol::OFPGT_INDIRECT);
      BOOST_TEST(sut.buckets().size() == 1);
    }
    BOOST_AUTO_TEST_CASE(is_move_constructible)
    {
      auto sut = msg::group_add{
          protocol::OFPG_MAX, protocol::OFPGT_SELECT
        , {
              v13::bucket{2, {actions::pop_vlan{}, actions::output{1}}} // 16 + 8 + 16 = 40
            , v13::bucket{1, {actions::set_vlan_vid{4}, actions::output{2}}} // 16 + 16 + 16 = 48
          }
      };

      auto const copy = std::move(sut);

      BOOST_TEST(copy.version() == sut.version());
      BOOST_TEST(copy.type() == sut.type());
      BOOST_TEST(copy.length() == sizeof(v13_detail::ofp_group_mod) + 40 + 48);
      BOOST_TEST(copy.command() == sut.command());
      BOOST_TEST(copy.group_id() == sut.group_id());
      BOOST_TEST(copy.group_type() == sut.group_type());
      BOOST_TEST(copy.buckets().size() == 2);

      BOOST_TEST(sut.buckets().size() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, group_add_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, group_add_fixture)
    {
      auto it = bin.begin();

      auto const decoded_message = msg::group_add::decode(it, bin.end());
      BOOST_TEST((it == bin.end()));
      BOOST_TEST(decoded_message.version() == sut.version());
      BOOST_TEST(decoded_message.type() == sut.type());
      BOOST_TEST(decoded_message.length() == sut.length());
      BOOST_TEST(decoded_message.xid() == sut.xid());
      BOOST_TEST(decoded_message.command() == sut.command());
      BOOST_TEST(decoded_message.group_id() == sut.group_id());
      BOOST_TEST(decoded_message.group_type() == sut.group_type());
      BOOST_TEST(decoded_message.buckets().size() == sut.buckets().size());
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // group_add_test

BOOST_AUTO_TEST_SUITE(group_delete_test)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_group_id)
    {
      auto const group_id = protocol::OFPG_ANY;
      auto const sut = msg::group_delete{group_id};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_GROUP_MOD);
      BOOST_TEST(sut.length() == sizeof(v13_detail::ofp_group_mod));
      BOOST_TEST(sut.command() == protocol::OFPGC_DELETE);
      BOOST_TEST(sut.group_id() == group_id);
    }

    BOOST_AUTO_TEST_CASE(is_move_constructible)
    {
      auto sut = msg::group_delete{0};

      auto const copy = std::move(sut);

      BOOST_TEST(copy.version() == sut.version());
      BOOST_TEST(copy.type() == sut.type());
      BOOST_TEST(copy.length() == sizeof(v13_detail::ofp_group_mod));
      BOOST_TEST(copy.command() == sut.command());
      BOOST_TEST(copy.group_id() == sut.group_id());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(encode_test, group_delete_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(decode_test, group_delete_fixture)
    {
      auto it = bin.begin();

      auto const decoded_message = msg::group_delete::decode(it, bin.end());
      BOOST_TEST((it == bin.end()));
      BOOST_TEST(decoded_message.version() == sut.version());
      BOOST_TEST(decoded_message.type() == sut.type());
      BOOST_TEST(decoded_message.length() == sut.length());
      BOOST_TEST(decoded_message.xid() == sut.xid());
      BOOST_TEST(decoded_message.command() == sut.command());
      BOOST_TEST(decoded_message.group_id() == sut.group_id());
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // group_delete_test

BOOST_AUTO_TEST_SUITE_END() // message_test
