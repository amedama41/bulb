#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/action_list.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace actions = ofp::v10::actions;

namespace {
struct action_list_fixture
{
  actions::output output1 = actions::output::to_controller(0x1234);
  actions::enqueue enqueue{0x12345678, ofp::v10::protocol::OFPP_MAX};
  actions::strip_vlan strip_vlan{};
  actions::set_eth_src set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac};
  actions::set_ipv4_dst set_ipv4_dst{"192.168.1.254"_ipv4};
  actions::set_tcp_src set_tcp_src{0x1234};
  actions::output output2{0x1234, 0x5678};

  ofp::v10::action_list sut{
      output1, enqueue, strip_vlan
    , set_eth_src, set_ipv4_dst, set_tcp_src
    , output2
  };
  std::vector<unsigned char> binary
    = "\x00\x00\x00\x08\xff\xfd\x12\x34"
      "\x00\x0b\x00\x10\xff\x00\x00\x00""\x00\x00\x00\x00\x12\x34\x56\x78"
      "\x00\x03\x00\x08\x00\x00\x00\x00"
      "\x00\x04\x00\x10\x01\x02\x03\x04""\x05\x06\x00\x00\x00\x00\x00\x00"
      "\x00\x07\x00\x08\xc0\xa8\x01\xfe"
      "\x00\x09\x00\x08\x12\x34\x00\x00"
      "\x00\x00\x00\x08\x12\x34\x56\x78"_bin
    ;

  ofp::v10::action_list empty_action_list{};
  std::vector<unsigned char> const zero_size_binary{};

  std::vector<unsigned char> buf;
};
}

BOOST_AUTO_TEST_SUITE(utility_test)
BOOST_AUTO_TEST_SUITE(action_list_test)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_no_action)
    {
      auto const sut = ofp::v10::action_list{};

      BOOST_TEST(sut.length() == 0);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_single_action)
    {
      auto const output = actions::output{1};

      auto const sut = ofp::v10::action_list{output};

      BOOST_TEST(sut.length() == output.length());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_multiple_actions)
    {
      auto const set_vlan_vid = actions::set_vlan_vid{2};
      auto const output = actions::output{1};

      auto const sut = ofp::v10::action_list{set_vlan_vid, output};

      BOOST_TEST(sut.length() == set_vlan_vid.length() + output.length());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_any_action)
    {
      auto const action = ofp::v10::any_action{actions::strip_vlan{}};

      auto const sut = ofp::v10::action_list{action};

      BOOST_TEST(sut.length() == action.length());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_any_action_and_concrete_action)
    {
      auto const output = actions::output{1};
      auto const action = ofp::v10::any_action{actions::strip_vlan{}};
      auto const set_vlan_vid = actions::set_vlan_vid{2};

      auto const sut = ofp::v10::action_list{output, action, set_vlan_vid};

      BOOST_TEST(
          sut.length()
       == output.length() + action.length() + set_vlan_vid.length());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_multiple_same_actions)
    {
      auto const output1 = actions::output{1};
      auto const output2 = ofp::v10::any_action{actions::output{2}};
      auto const output3 = actions::output{3};

      auto const sut = ofp::v10::action_list{output1, output2, output3};

      BOOST_TEST(
          sut.length()
       == output1.length() + output2.length() + output3.length());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(encode, action_list_fixture)
    BOOST_AUTO_TEST_CASE(generate_binary)
    {
      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == binary, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generate_zero_size_binary_from_empty_action_list)
    {
      empty_action_list.encode(buf);

      BOOST_TEST(buf.size() == 0);
      BOOST_TEST(buf == zero_size_binary, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, action_list_fixture)
    BOOST_AUTO_TEST_CASE(constructible_from_binary)
    {
      auto it = binary.begin();

      auto const action_list = ofp::v10::action_list::decode(it, binary.end());

      BOOST_TEST((it == binary.end()));
      BOOST_TEST(action_list == sut);
    }
    BOOST_AUTO_TEST_CASE(construct_empty_list_from_zero_size_binary)
    {
      auto it = zero_size_binary.begin();

      auto const action_list
        = ofp::v10::action_list::decode(it, zero_size_binary.end());

      BOOST_TEST((it == zero_size_binary.end()));
      BOOST_TEST(action_list == ofp::v10::action_list{});
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // action_list_test
BOOST_AUTO_TEST_SUITE_END() // utility_test
