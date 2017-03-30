#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/instruction/apply_actions.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <utility>
#include <vector>
#include <canard/network/openflow/v13/actions.hpp>
#include <canard/network/openflow/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;
namespace actions = v13::actions;
namespace instructions = v13::instructions;

namespace {

struct apply_actions_fixture
{
    instructions::apply_actions sut{
          actions::push_vlan{0x8100}
        , actions::set_vlan_vid{protocol::OFPVID_PRESENT | 0x0123}
        , actions::output{0x12345678}
        , actions::set_eth_dst{"\x66\x55\x44\x33\x22\x11"_mac}
        , actions::output{protocol::OFPP_MAX}
    }; // 8 + 8 + 16 + 16 + 16 + 16
    std::vector<unsigned char> binary
        = "\x00\x04\x00\x50\x00\x00\x00\x00""\x00\x11\x00\x08\x81\x00\x00\x00"
          "\x00\x19\x00\x10\x80\x00\x0c\x02""\x11\x23\x00\x00\x00\x00\x00\x00"
          "\x00\x00\x00\x10\x12\x34\x56\x78""\xff\xff\x00\x00\x00\x00\x00\x00"
          "\x00\x19\x00\x10\x80\x00\x06\x06""\x66\x55\x44\x33\x22\x11\x00\x00"
          "\x00\x00\x00\x10\xff\xff\xff\x00""\xff\xff\x00\x00\x00\x00\x00\x00"
          ""_bin;
};

}

BOOST_AUTO_TEST_SUITE(instruction_test)
BOOST_AUTO_TEST_SUITE(apply_actions_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = instructions::apply_actions;

        BOOST_TEST(sut::type() == protocol::OFPIT_APPLY_ACTIONS);
    }

    BOOST_AUTO_TEST_CASE(default_construct_test)
    {
        auto const sut = instructions::apply_actions{};

        BOOST_TEST(sut.length() == sizeof(protocol::ofp_instruction_actions));
        BOOST_TEST(sut.actions().empty());
    }

    BOOST_AUTO_TEST_CASE(construct_from_action_list_test)
    {
        auto const action_list = v13::action_list{
            actions::set_queue{1}, actions::set_vlan_pcp{2}
        };

        auto const sut = instructions::apply_actions{action_list};

        BOOST_TEST(sut.length()
                == sizeof(protocol::ofp_instruction_actions) + 24);
        BOOST_TEST((sut.actions() == action_list));
    }

    BOOST_AUTO_TEST_CASE(construct_from_single_action_test)
    {
        auto const group = actions::group{1};
        auto const action_list = v13::action_list{group};

        auto const sut = instructions::apply_actions{group};

        BOOST_TEST(sut.length() == sizeof(protocol::ofp_instruction_actions) + 8);
        BOOST_TEST((sut.actions() == action_list));
    }

    BOOST_AUTO_TEST_CASE(construct_from_multiple_actions_test)
    {
        auto const copy_ttl_in = actions::copy_ttl_in{};
        auto const set_ipv4_src = actions::set_ipv4_src{"127.0.0.1"_ipv4};
        auto const set_vlan_vid1
            = actions::set_vlan_vid{protocol::OFPVID_PRESENT | 1};
        auto const push_vlan = actions::push_vlan{0x8100};
        auto const set_vlan_vid2
            = actions::set_vlan_vid{protocol::OFPVID_PRESENT | 1024};
        auto const output = actions::output{4};
        auto const action_list = v13::action_list{
              copy_ttl_in, set_ipv4_src, set_vlan_vid1
            , push_vlan, set_vlan_vid2, output
        };

        auto const sut = instructions::apply_actions{
              copy_ttl_in, set_ipv4_src, set_vlan_vid1
            , push_vlan, set_vlan_vid2, output
        };

        BOOST_TEST(sut.length()
                == sizeof(protocol::ofp_instruction_actions) + 80);
        BOOST_TEST((sut.actions() == action_list));
    }

    BOOST_AUTO_TEST_CASE(create_test)
    {
        auto const sut = instructions::apply_actions::create(
                  actions::set_vlan_vid{protocol::OFPVID_PRESENT | 0x0123}
                , actions::output{1});

        BOOST_TEST(sut.length()
                == sizeof(protocol::ofp_instruction_actions) + 16 + 16);
    }

    BOOST_FIXTURE_TEST_CASE(copy_construct_test, apply_actions_fixture)
    {
        auto const copy = sut;

        BOOST_TEST((copy == sut));
    }

    BOOST_FIXTURE_TEST_CASE(move_construct_test, apply_actions_fixture)
    {
        auto src = sut;

        auto const copy = std::move(src);

        BOOST_TEST((copy == sut));
        BOOST_TEST(src.length() == sizeof(protocol::ofp_instruction_actions));
        BOOST_TEST(src.actions().empty());
    }

    BOOST_FIXTURE_TEST_CASE(copy_assign_test, apply_actions_fixture)
    {
        auto copy = instructions::apply_actions{};

        copy = sut;

        BOOST_TEST((copy == sut));
    }

    BOOST_FIXTURE_TEST_CASE(move_assign_test, apply_actions_fixture)
    {
        auto copy = instructions::apply_actions{};
        auto src = sut;

        copy = std::move(src);

        BOOST_TEST((copy == sut));
        BOOST_TEST(src.length() == sizeof(protocol::ofp_instruction_actions));
        BOOST_TEST(src.actions().empty());
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = instructions::apply_actions{
          actions::output{1}, actions::set_queue{2}
        };

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_action_list_is_equal)
      {
        BOOST_TEST(
            (instructions::apply_actions{ actions::output{1}, actions::set_queue{2} }
             == instructions::apply_actions{ actions::output{1}, actions::set_queue{2} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_action_lists_are_empty)
      {
        BOOST_TEST(
            (instructions::apply_actions{} == instructions::apply_actions{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_action_list_size_is_not_equal)
      {
        BOOST_TEST(
            (instructions::apply_actions{ actions::output{1} }
             != instructions::apply_actions{ actions::output{1}, actions::group{2} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_action_list_order_is_not_equal)
      {
        BOOST_TEST(
            (instructions::apply_actions{ actions::pop_pbb{}, actions::pop_vlan{} }
             != instructions::apply_actions{ actions::pop_vlan{}, actions::pop_pbb{} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_value_is_not_equal)
      {
        BOOST_TEST(
            (instructions::apply_actions{ actions::pop_pbb{}, actions::set_nw_ttl{2} }
             != instructions::apply_actions{ actions::pop_pbb{}, actions::set_nw_ttl{3} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_type_is_not_equal)
      {
        BOOST_TEST(
            (instructions::apply_actions{ actions::pop_pbb{}, actions::set_ip_ecn{1} }
             != instructions::apply_actions{ actions::pop_pbb{}, actions::set_arp_op{1} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x04\x00\x08\x00\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const nonzero_pad
          = instructions::apply_actions::decode(it, binary.end());

        BOOST_TEST((instructions::apply_actions{} != nonzero_pad));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_actions_are_not_equal_but_equivalent)
      {
        auto const binary = "\x00\x12\x00\x08\x10\x20\x30\x40"_bin;
        auto it = binary.begin();
        auto const nonzero_pad_pop_vlan
          = actions::pop_vlan::decode(it, binary.end());

        BOOST_TEST(
            (instructions::apply_actions{ actions::pop_vlan{} }
             != instructions::apply_actions{ nonzero_pad_pop_vlan }));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = instructions::apply_actions{
            actions::output{1}, actions::set_queue{2}
        };

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_action_list_is_equal)
      {
        BOOST_TEST(
            equivalent(
                instructions::apply_actions{ actions::output{1}, actions::set_queue{2} }
              , instructions::apply_actions{ actions::output{1}, actions::set_queue{2} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_action_lists_are_empty)
      {
        BOOST_TEST(
            equivalent(
                instructions::apply_actions{}
              , instructions::apply_actions{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_action_list_size_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                instructions::apply_actions{ actions::output{1} }
              , instructions::apply_actions{ actions::output{1}, actions::group{2} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_action_list_order_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                instructions::apply_actions{ actions::pop_pbb{}, actions::pop_vlan{} }
              , instructions::apply_actions{ actions::pop_vlan{}, actions::pop_pbb{} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_value_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                instructions::apply_actions{ actions::pop_pbb{}, actions::set_nw_ttl{2} }
              , instructions::apply_actions{ actions::pop_pbb{}, actions::set_nw_ttl{3} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_type_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                instructions::apply_actions{ actions::pop_pbb{}, actions::set_ip_ecn{1} }
              , instructions::apply_actions{ actions::pop_pbb{}, actions::set_arp_op{1} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x04\x00\x08\x12\x34\x56\x78"_bin;
        auto it = binary.begin();
        auto const nonzero_pad
          = instructions::apply_actions::decode(it, binary.end());

        BOOST_TEST(equivalent(instructions::apply_actions{}, nonzero_pad));
      }
      BOOST_AUTO_TEST_CASE(true_if_contained_actions_are_not_equal_but_equivalent)
      {
        auto const binary = "\x00\x12\x00\x08\x10\x20\x30\x40"_bin;
        auto it = binary.begin();
        auto const nonzero_pad_pop_vlan
          = actions::pop_vlan::decode(it, binary.end());

        BOOST_TEST(
            equivalent(
                instructions::apply_actions{ actions::pop_vlan{} }
              , instructions::apply_actions{ nonzero_pad_pop_vlan }));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, apply_actions_fixture)
    {
        auto buffer = std::vector<unsigned char>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, apply_actions_fixture)
    {
        auto it = binary.begin();
        auto it_end = binary.end();

        auto const apply_actions
            = instructions::apply_actions::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((apply_actions == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // apply_actions_test
BOOST_AUTO_TEST_SUITE_END() // instruction_test
