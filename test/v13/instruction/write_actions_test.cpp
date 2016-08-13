#define BOOST_TEST_DYN_LINK
#include <canard/network/protocol/openflow/v13/instruction/write_actions.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <utility>
#include <vector>
#include <canard/network/protocol/openflow/v13/actions.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace actions = v13::actions;
namespace instructions = v13::instructions;
namespace detail = v13::v13_detail;

namespace {

struct write_actions_fixture
{
    instructions::write_actions sut{
          actions::set_ipv4_src{"192.168.10.1"_ipv4} // 16
        , actions::set_ipv4_dst{"127.0.0.1"_ipv4} // 16
        , actions::set_ip_proto{6} // 16
        , actions::copy_ttl_in{} // 8
        , actions::copy_ttl_out{} // 8
        , actions::output{0x12345678} // 16
    }; // 8 + 16 + 16 + 16 + 8 + 8 + 16 = 88
    std::vector<unsigned char> binary
        = "\x00\x03\x00\x58\x00\x00\x00\x00""\x00\x0c\x00\x08\x00\x00\x00\x00"
          "\x00\x0b\x00\x08\x00\x00\x00\x00""\x00\x19\x00\x10\x80\x00\x14\x01"
          "\x06\x00\x00\x00\x00\x00\x00\x00""\x00\x19\x00\x10\x80\x00\x16\x04"
          "\xc0\xa8\x0a\x01\x00\x00\x00\x00""\x00\x19\x00\x10\x80\x00\x18\x04"
          "\x7f\x00\x00\x01\x00\x00\x00\x00""\x00\x00\x00\x10\x12\x34\x56\x78"
          "\xff\xff\x00\x00\x00\x00\x00\x00"
          ""_bin;
};

struct has_same_action_write_actions_fixture
{
    instructions::write_actions sut{v13::action_list{
          actions::output{0x12345678}
        , actions::output{0x10305070}
    }};
    std::vector<unsigned char> binary
        = "\x00\x03\x00\x28\x00\x00\x00\x00""\x00\x00\x00\x10\x12\x34\x56\x78"
          "\xff\xff\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x10\x10\x30\x50\x70"
          "\xff\xff\x00\x00\x00\x00\x00\x00"
          ""_bin;
};

}

BOOST_AUTO_TEST_SUITE(instruction_test)
BOOST_AUTO_TEST_SUITE(write_actions_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = instructions::write_actions;

        BOOST_TEST(sut::type() == v13::protocol::OFPIT_WRITE_ACTIONS);
    }

    BOOST_AUTO_TEST_CASE(default_construct_test)
    {
        auto const sut = instructions::write_actions{};

        BOOST_TEST(sut.length() == sizeof(detail::ofp_instruction_actions));
        BOOST_TEST(sut.actions().empty());
    }

    BOOST_AUTO_TEST_CASE(construct_from_action_list_test)
    {
        auto const action_list = v13::action_list{
            actions::output{1}, actions::set_eth_type{0x0800}
        };

        auto const sut = instructions::write_actions{action_list};

        BOOST_TEST(sut.length()
                == sizeof(detail::ofp_instruction_actions) + 32);
        BOOST_TEST(sut.actions() == action_list);
    }

    BOOST_AUTO_TEST_CASE(construct_from_action_set_test)
    {
        auto const action_set = v13::action_set{
            actions::output{1}, actions::set_eth_type{0x0800}
        };

        auto const sut = instructions::write_actions{action_set};

        BOOST_TEST(sut.length()
                == sizeof(detail::ofp_instruction_actions) + 32);
        BOOST_TEST(sut.actions() == action_set.to_list());
    }

    BOOST_AUTO_TEST_CASE(construct_from_single_action_test)
    {
        auto const group = actions::group{1};
        auto const action_list = v13::action_list{group};

        auto const sut = instructions::write_actions{group};

        BOOST_TEST(sut.length()
                == sizeof(detail::ofp_instruction_actions) + 8);
        BOOST_TEST(sut.actions() == action_list);
    }

    BOOST_AUTO_TEST_CASE(construct_from_multiple_actions_test)
    {
        auto const copy_ttl_in = actions::copy_ttl_in{};
        auto const set_ipv4_src1 = actions::set_ipv4_src{"127.0.0.1"_ipv4};
        auto const set_ipv4_dst = actions::set_ipv4_dst{"127.0.0.2"_ipv4};
        auto const copy_ttl_out = actions::copy_ttl_out{};
        auto const set_ipv4_src2 = actions::set_ipv4_src{"172.16.1.1"_ipv4};
        auto const action_set = v13::action_set{
              copy_ttl_in, set_ipv4_src1, set_ipv4_dst
            , copy_ttl_in, copy_ttl_out, set_ipv4_src2
        };

        auto const sut = instructions::write_actions{
              copy_ttl_in, set_ipv4_src1, set_ipv4_dst
            , copy_ttl_in, copy_ttl_out, set_ipv4_src2
        };

        BOOST_TEST(sut.length()
                == sizeof(detail::ofp_instruction_actions) + 48);
        BOOST_TEST(sut.actions() == action_set.to_list());
    }

    BOOST_AUTO_TEST_CASE(create_success_test)
    {
        auto const sut = instructions::write_actions::create(
                actions::output{1}, actions::group{2});

        BOOST_TEST(sut.length()
                == sizeof(detail::ofp_instruction_actions) + 16 + 8);
    }

    BOOST_AUTO_TEST_CASE(create_failure_test)
    {
        auto const action_list = v13::action_list{
            actions::output{1}, actions::output{2}
        };

        BOOST_CHECK_THROW(
                  instructions::write_actions::create(action_list)
                , std::runtime_error);
    }

    BOOST_FIXTURE_TEST_CASE(copy_construct_test, write_actions_fixture)
    {
        auto const copy = sut;

        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST((copy.actions() == sut.actions()));
    }

    BOOST_FIXTURE_TEST_CASE(move_construct_test, write_actions_fixture)
    {
        auto src = sut;

        auto const copy = std::move(src);

        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST((copy.actions() == sut.actions()));
        BOOST_TEST(src.length() == sizeof(detail::ofp_instruction_actions));
        BOOST_TEST(src.actions().empty());
    }

    BOOST_FIXTURE_TEST_CASE(copy_assign_test, write_actions_fixture)
    {
        auto copy = instructions::write_actions{};

        copy = sut;

        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST((copy.actions() == sut.actions()));
    }

    BOOST_FIXTURE_TEST_CASE(move_assign_test, write_actions_fixture)
    {
        auto copy = instructions::write_actions{};
        auto src = sut;

        copy = std::move(src);

        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST((copy.actions() == sut.actions()));
        BOOST_TEST(src.length() == sizeof(detail::ofp_instruction_actions));
        BOOST_TEST(src.actions().empty());
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = instructions::write_actions{
          actions::output{1}, actions::set_queue{2}
        };

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_actinos_are_equal)
      {
        BOOST_TEST(
            (instructions::write_actions{ actions::output{1}, actions::set_queue{2} }
             == instructions::write_actions{ actions::output{1}, actions::set_queue{2} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_actions_are_empty)
      {
        BOOST_TEST(
            (instructions::write_actions{} == instructions::write_actions{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_actions_size_is_not_equal)
      {
        BOOST_TEST(
            (instructions::write_actions{ actions::output{1} }
             != instructions::write_actions{ actions::output{1}, actions::group{2} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_actions_order_is_not_equal)
      {
        BOOST_TEST(
            (instructions::write_actions{
              v13::action_list{ actions::pop_pbb{}, actions::pop_vlan{} } }
             != instructions::write_actions{
              v13::action_list{ actions::pop_vlan{}, actions::pop_pbb{} } }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_value_is_not_equal)
      {
        BOOST_TEST(
            (instructions::write_actions{ actions::pop_pbb{}, actions::set_nw_ttl{2} }
             != instructions::write_actions{ actions::pop_pbb{}, actions::set_nw_ttl{3} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_type_is_not_equal)
      {
        BOOST_TEST(
            (instructions::write_actions{ actions::pop_pbb{}, actions::set_ip_ecn{1} }
             != instructions::write_actions{ actions::pop_pbb{}, actions::set_arp_op{1} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_one_contains_duplicated_action)
      {
        BOOST_TEST(
            (instructions::write_actions{
               v13::action_list{ actions::pop_pbb{}, actions::pop_pbb{} } }
             != instructions::write_actions{ actions::pop_pbb{} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x03\x00\x08\x00\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const nonzero_pad
          = instructions::write_actions::decode(it, binary.end());

        BOOST_TEST((instructions::write_actions{} != nonzero_pad));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_actions_are_not_equal_but_equivalent)
      {
        auto const binary = "\x00\x12\x00\x08\x10\x20\x30\x40"_bin;
        auto it = binary.begin();
        auto const nonzero_pad_pop_vlan
          = actions::pop_vlan::decode(it, binary.end());

        BOOST_TEST(
            (instructions::write_actions{ actions::pop_vlan{} }
             != instructions::write_actions{ nonzero_pad_pop_vlan }));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = instructions::write_actions{
          actions::output{1}, actions::set_queue{2}
        };

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_actinos_are_equal)
      {
        BOOST_TEST(
            equivalent(
                instructions::write_actions{
                  actions::output{1}, actions::set_queue{2} }
              , instructions::write_actions{
                  actions::output{1}, actions::set_queue{2} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_actions_are_empty)
      {
        BOOST_TEST(
            equivalent(
              instructions::write_actions{}, instructions::write_actions{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_actions_size_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                instructions::write_actions{ actions::output{1} }
              , instructions::write_actions{ actions::output{1}, actions::group{2} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_actions_order_is_not_equal)
      {
        BOOST_TEST(
            equivalent(
                instructions::write_actions{
                  v13::action_list{ actions::pop_pbb{}, actions::pop_vlan{} } }
              , instructions::write_actions{
                  v13::action_list{ actions::pop_vlan{}, actions::pop_pbb{} } }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_value_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                instructions::write_actions{
                  actions::pop_pbb{}, actions::set_nw_ttl{2} }
              , instructions::write_actions{
                  actions::pop_pbb{}, actions::set_nw_ttl{3} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_type_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                instructions::write_actions{
                  actions::pop_pbb{}, actions::set_ip_ecn{1} }
              , instructions::write_actions{
                  actions::pop_pbb{}, actions::set_arp_op{1} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_one_contains_duplicated_action)
      {
        BOOST_TEST(
            !equivalent(
                instructions::write_actions{
                  v13::action_list{ actions::pop_pbb{}, actions::pop_pbb{} } }
              , instructions::write_actions{ actions::pop_pbb{} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_contain_duplicated_action)
      {
        BOOST_TEST(
            equivalent(
                instructions::write_actions{
                  v13::action_list{
                    actions::pop_pbb{}, actions::pop_pbb{}, actions::pop_vlan{} } }
              , instructions::write_actions{
                  v13::action_list{
                    actions::pop_pbb{}, actions::pop_vlan{}, actions::pop_pbb{} } }));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x03\x00\x08\x00\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const nonzero_pad
          = instructions::write_actions::decode(it, binary.end());

        BOOST_TEST(equivalent(instructions::write_actions{}, nonzero_pad));
      }
      BOOST_AUTO_TEST_CASE(true_if_contained_actions_are_not_equal_but_equivalent)
      {
        auto const binary = "\x00\x12\x00\x08\x10\x20\x30\x40"_bin;
        auto it = binary.begin();
        auto const nonzero_pad_pop_vlan
          = actions::pop_vlan::decode(it, binary.end());

        BOOST_TEST(
            equivalent(
                instructions::write_actions{ actions::pop_vlan{} }
              , instructions::write_actions{ nonzero_pad_pop_vlan }));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, write_actions_fixture)
    {
        auto buffer = std::vector<unsigned char>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, write_actions_fixture)
    {
        auto it = binary.begin();
        auto it_end = binary.end();

        auto const write_actions
            = instructions::write_actions::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((write_actions == sut));
    }

    BOOST_FIXTURE_TEST_CASE(
            decode_has_same_action_test, has_same_action_write_actions_fixture)
    {
        auto it = binary.begin();
        auto it_end = binary.end();

        auto const write_actions
            = instructions::write_actions::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((write_actions == sut));
        BOOST_CHECK_THROW(
                  instructions::write_actions::validate(write_actions)
                , std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END() // write_actions_test
BOOST_AUTO_TEST_SUITE_END() // instruction_test
