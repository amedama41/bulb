#define BOOST_TEST_DYN_LINK
#include <canard/network/protocol/openflow/v13/action_list.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <utility>
#include <vector>
#include <canard/network/protocol/openflow/v13/actions.hpp>

#include "../../test_utility.hpp"

namespace of = canard::network::openflow;
namespace v13 = of::v13;
namespace actions = v13::actions;

using boost::asio::ip::address_v4;
using canard::mac_address;

namespace {

struct action_list_fixture
{
    v13::action_list sut{
          actions::group{0x12345678}
        , actions::copy_ttl_in{}
        , actions::copy_ttl_out{}
        , actions::set_ipv4_src{"127.0.0.1"_ipv4}
        , actions::set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac}
        , actions::output{0x87654321}
    };
    std::vector<std::uint8_t> binary
        = "\x00\x16\x00\x08\x12\x34\x56\x78""\x00\x0c\x00\x08\x00\x00\x00\x00"
          "\x00\x0b\x00\x08\x00\x00\x00\x00""\x00\x19\x00\x10\x80\x00\x16\x04"
          "\x7f\x00\x00\x01\x00\x00\x00\x00""\x00\x19\x00\x10\x80\x00\x08\x06"
          "\x01\x02\x03\x04\x05\x06\x00\x00""\x00\x00\x00\x10\x87\x65\x43\x21"
          "\xff\xff\x00\x00\x00\x00\x00\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(common_type_test)
BOOST_AUTO_TEST_SUITE(action_list_test)

    BOOST_AUTO_TEST_CASE(default_construct_test)
    {
        auto const sut = v13::action_list{};

        BOOST_TEST(sut.length() == 0);
        BOOST_TEST(sut.empty());
        BOOST_TEST(sut.size() == 0);
        BOOST_TEST((sut.begin() == sut.end()));
    }

    BOOST_AUTO_TEST_CASE(construct_from_single_action_test)
    {
        auto const output = actions::output{1};

        auto const sut = v13::action_list{output};

        BOOST_TEST(sut.length() == output.length());
        BOOST_TEST(!sut.empty());
        BOOST_TEST(sut.size() == 1);
        BOOST_TEST((sut.begin() != sut.end()));

        BOOST_TEST((sut.at(0) == output));
        BOOST_TEST((sut[0] == output));
    }

    BOOST_AUTO_TEST_CASE(construct_from_multiple_actions_test)
    {
        auto const output = actions::output{4}; // 16
        auto const set_ipv4_src = actions::set_ipv4_src{"127.0.0.1"_ipv4}; // 16
        auto const copy_ttl_in = actions::copy_ttl_in{}; // 8
        auto const push_vlan = actions::push_vlan{0x8100}; // 8
        auto const set_tcp_dst = actions::set_tcp_dst{6653}; // 16
        auto const set_queue = actions::set_queue{4}; // 8

        auto const sut = v13::action_list{
            output, set_ipv4_src, copy_ttl_in, push_vlan, set_tcp_dst, set_queue
        };

        BOOST_TEST(sut.length() == 72);
        BOOST_TEST(!sut.empty());
        BOOST_TEST(sut.size() == 6);
        BOOST_TEST((sut.begin() != sut.end()));
        BOOST_TEST((sut.at(0) == output));
        BOOST_TEST((sut[0] == output));
        BOOST_TEST((sut.at(1) == set_ipv4_src));
        BOOST_TEST((sut[1] == set_ipv4_src));
        BOOST_TEST((sut.at(2) == copy_ttl_in));
        BOOST_TEST((sut[2] == copy_ttl_in));
        BOOST_TEST((sut.at(3) == push_vlan));
        BOOST_TEST((sut[3] == push_vlan));
        BOOST_TEST((sut.at(4) == set_tcp_dst));
        BOOST_TEST((sut[4] == set_tcp_dst));
        BOOST_TEST((sut.at(5) == set_queue));
        BOOST_TEST((sut[5] == set_queue));
    }

    BOOST_AUTO_TEST_CASE(construct_from_same_actions_test)
    {
        auto const output1 = actions::output{4}; // 16
        auto const group = actions::group{1}; // 8
        auto const output2 = actions::output{5}; // 16

        auto const sut = v13::action_list{output1, group, output2};

        BOOST_TEST(sut.length() == 40);
        BOOST_TEST(!sut.empty());
        BOOST_TEST(sut.size() == 3);
        BOOST_TEST((sut.at(0) == output1));
        BOOST_TEST((sut.at(1) == group));
        BOOST_TEST((sut.at(2) == output2));
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut
            = v13::action_list{ actions::output{1}, actions::set_queue{2} };

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_action_list_is_equal)
      {
        BOOST_TEST(
            (v13::action_list{ actions::output{1}, actions::set_queue{2} }
             == v13::action_list{ actions::output{1}, actions::set_queue{2} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_action_lists_are_empty)
      {
        BOOST_TEST((v13::action_list{} == v13::action_list{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_action_list_size_is_not_equal)
      {
        BOOST_TEST(
            (v13::action_list{ actions::output{1} }
             != v13::action_list{ actions::output{1}, actions::group{2} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_action_list_order_is_not_equal)
      {
        BOOST_TEST(
            (v13::action_list{ actions::pop_pbb{}, actions::pop_vlan{} }
             != v13::action_list{ actions::pop_vlan{}, actions::pop_pbb{} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_value_is_not_equal)
      {
        BOOST_TEST(
            (v13::action_list{ actions::pop_pbb{}, actions::set_nw_ttl{2} }
             != v13::action_list{ actions::pop_pbb{}, actions::set_nw_ttl{3} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_type_is_not_equal)
      {
        BOOST_TEST(
            (v13::action_list{ actions::pop_pbb{}, actions::copy_ttl_out{} }
             != v13::action_list{ actions::pop_pbb{}, actions::copy_ttl_in{} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_actions_are_not_equal_but_equivalent)
      {
        auto const binary = "\x00\x12\x00\x08\x10\x20\x30\x40"_bin;
        auto it = binary.begin();
        auto const nonzero_pad_pop_vlan
          = actions::pop_vlan::decode(it, binary.end());

        BOOST_TEST(
            (v13::action_list{ actions::pop_vlan{} }
             != v13::action_list{ nonzero_pad_pop_vlan }));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut
            = v13::action_list{ actions::output{1}, actions::set_queue{2} };

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_action_list_is_equal)
      {
        BOOST_TEST(
            equivalent(
                v13::action_list{ actions::output{1}, actions::set_queue{2} }
              , v13::action_list{ actions::output{1}, actions::set_queue{2} }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_action_lists_are_empty)
      {
        BOOST_TEST(equivalent(v13::action_list{}, v13::action_list{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_action_list_size_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                v13::action_list{ actions::output{1} }
              , v13::action_list{ actions::output{1}, actions::group{2} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_action_list_order_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                v13::action_list{ actions::pop_pbb{}, actions::pop_vlan{} }
              , v13::action_list{ actions::pop_vlan{}, actions::pop_pbb{} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_value_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                v13::action_list{ actions::pop_pbb{}, actions::set_nw_ttl{2} }
              , v13::action_list{ actions::pop_pbb{}, actions::set_nw_ttl{3} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_action_type_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                v13::action_list{ actions::pop_pbb{}, actions::copy_ttl_out{} }
              , v13::action_list{ actions::pop_pbb{}, actions::copy_ttl_in{} }));
      }
      BOOST_AUTO_TEST_CASE(false_if_contained_actions_are_not_equal_but_equivalent)
      {
        auto const binary = "\x00\x12\x00\x08\x10\x20\x30\x40"_bin;
        auto it = binary.begin();
        auto const nonzero_pad_pop_vlan
          = actions::pop_vlan::decode(it, binary.end());

        BOOST_TEST(
            equivalent(
                v13::action_list{ actions::pop_vlan{} }
              , v13::action_list{ nonzero_pad_pop_vlan }));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, action_list_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, action_list_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const match_set = v13::action_list::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((match_set == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // action_list_test
BOOST_AUTO_TEST_SUITE_END() // common_type_test
