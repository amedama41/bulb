#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/packet_out.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace match = v13::oxm_match_fields;
namespace protocol = v13::protocol;

namespace {

  constexpr auto ofp_pkt_out_size = sizeof(protocol::ofp_packet_out);

  struct packet_out_parameter {
    v13::messages::packet_out::data_type data = "\x10\x11\x12\x13\x14\x15"_bbin;
    std::uint32_t in_port = protocol::OFPP_CONTROLLER;
    v13::actions::push_vlan push_vlan{0x8100};
    v13::actions::set_vlan_vid set_vlan_vid{protocol::OFPVID_PRESENT | 3};
    v13::actions::output output{3};
    v13::action_list actions{push_vlan, set_vlan_vid, output};
    std::uint32_t xid = 0x1234;
  };
  struct packet_out_fixture : packet_out_parameter {
    v13::messages::packet_out sut{data, in_port, actions, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x0d\x00\x46\x00\x00\x12\x34" "\xff\xff\xff\xff\xff\xff\xff\xfd"
        "\x00\x28\x00\x00\x00\x00\x00\x00"
        "\x00\x11\x00\x08\x81\x00\x00\x00" "\x00\x19\x00\x10\x80\x00\x0c\x02"
        "\x10\x03\x00\x00\x00\x00\x00\x00" "\x00\x00\x00\x10\x00\x00\x00\x03"
        "\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x10\x11\x12\x13\x14\x15"_bin
        ;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(packet_out)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_buffer_id)
    {
      auto const buffer_id = std::uint32_t{0xffffff};
      auto const in_port = 1;
      auto const actions = v13::action_list{
        v13::actions::decrement_nw_ttl{}, v13::actions::output{3}
      };
      auto const xid = std::uint32_t{0xff00ff};

      v13::messages::packet_out const sut{buffer_id, in_port, actions, xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_PACKET_OUT);
      BOOST_TEST(sut.length() == ofp_pkt_out_size + actions.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == 24);
      BOOST_TEST(sut.frame_length() == 0);
      BOOST_TEST(sut.frame().empty());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_binary)
    {
      auto const data = "\x00\x01\x02\x03\x04\x05"_bbin;
      auto const in_port = protocol::OFPP_CONTROLLER;
      auto const actions = v13::action_list{
          v13::actions::push_vlan{0x8100}
        , v13::actions::set_vlan_vid{3}
        , v13::actions::output{3}
      };
      auto const xid = std::uint32_t{0x000001};

      v13::messages::packet_out const sut{data, in_port, actions, xid};

      BOOST_TEST(
          sut.length() == ofp_pkt_out_size + actions.length() + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == 40);
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST(sut.frame() == data, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_empty_actions)
    {
      auto const data = "\x10\x11\x12\x13\x14\x15"_bbin;
      auto const in_port = 2;
      auto const xid = std::uint32_t{0x000001};

      v13::messages::packet_out const sut{data, in_port, {}, xid};

      BOOST_TEST(sut.length() == ofp_pkt_out_size + data.size());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == 0);
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST(sut.frame() == data, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_empty_binary)
    {
      auto const in_port = 2;

      v13::messages::packet_out const sut{
        v13::messages::packet_out::data_type{}, in_port, {}
      };

      BOOST_TEST(sut.length() == ofp_pkt_out_size);
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
      BOOST_TEST(sut.in_port() == in_port);
      BOOST_TEST(sut.actions_length() == 0);
      BOOST_TEST(sut.frame_length() == 0);
      BOOST_TEST(sut.frame().empty());
    }
    BOOST_AUTO_TEST_CASE(copy_constructible_from_no_data_packet_out)
    {
      auto const sut = v13::messages::packet_out{
        0, 0, {v13::actions::output::to_controller()}
      };

      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, packet_out_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, packet_out_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == ofp_pkt_out_size);
      BOOST_TEST(moved.actions_length() == 0);
      BOOST_TEST(moved.frame_length() == 0);
      BOOST_TEST(moved.frame().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, packet_out_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = v13::messages::packet_out{0, 0, {}};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto moved = sut;
      auto copy = v13::messages::packet_out{0, 0, {}};

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == ofp_pkt_out_size);
      BOOST_TEST(moved.actions_length() == 0);
      BOOST_TEST(moved.frame_length() == 0);
      BOOST_TEST(moved.frame().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, packet_out_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::packet_out{data, in_port, actions, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_out{data, in_port, actions, xid}
        == v13::messages::packet_out{data, in_port, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_out{"1"_bbin, in_port, actions, xid}
        != v13::messages::packet_out{"2"_bbin, in_port, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_buffer_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_out{1, in_port, actions, xid}
        != v13::messages::packet_out{2, in_port, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_in_port_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_out{data, 1, actions, xid}
        != v13::messages::packet_out{data, 2, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_actions_is_not_equal)
    {
      auto set_tcp_src1 = v13::actions::set_tcp_src{1};
      auto set_tcp_src2 = v13::actions::set_tcp_src{2};

      BOOST_TEST(
          (v13::messages::packet_out{data, in_port, {set_tcp_src1}, xid}
        != v13::messages::packet_out{data, in_port, {set_tcp_src2}, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_out{data, in_port, actions, 1}
        != v13::messages::packet_out{data, in_port, actions, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, packet_out_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, packet_out_fixture)
    {
      auto it = bin.begin();

      auto pkt_out = v13::messages::packet_out::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((sut == pkt_out));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // packet_out
BOOST_AUTO_TEST_SUITE_END() // message_test

