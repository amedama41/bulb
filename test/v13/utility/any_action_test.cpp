#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/any_action.hpp>
#include <boost/test/unit_test.hpp>

#include <type_traits>
#include <utility>
#include <vector>
#include <canard/net/ofp/v13/actions.hpp>
#include <canard/net/ofp/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace actions = v13::actions;

BOOST_AUTO_TEST_SUITE(common_type_test)
BOOST_AUTO_TEST_SUITE(any_action_test)

  BOOST_AUTO_TEST_SUITE(type_definition_test)
    BOOST_AUTO_TEST_CASE(min_length)
    {
      using sut = v13::any_action;

      using min_length
        = std::integral_constant<std::uint16_t, sut::min_length()>;

      BOOST_TEST(min_length::value == 8);
    }
    BOOST_AUTO_TEST_CASE(min_byte_length)
    {
      using sut = v13::any_action;

      using min_byte_length
        = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

      BOOST_TEST(min_byte_length::value == 8);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_test

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_output_action)
    {
      auto const output = actions::output{1, 1};

      auto const sut = v13::any_action{output};

      BOOST_TEST(sut.type() == output.type());
      BOOST_TEST(sut.length() == output.length());
      BOOST_TEST(sut.byte_length() == output.byte_length());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_set_field)
    {
      auto const set_ipv6_src = actions::set_ipv6_src{"::1"_ipv6};

      auto const sut = v13::any_action{set_ipv6_src};

      BOOST_TEST(sut.type() == set_ipv6_src.type());
      BOOST_TEST(sut.length() == set_ipv6_src.length());
      BOOST_TEST(sut.byte_length() == set_ipv6_src.byte_length());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(copy_constructor)
    BOOST_AUTO_TEST_CASE(copyable_from_non_const_lvalue)
    {
      auto lvalue = v13::any_action{actions::decrement_nw_ttl{}};

      v13::any_action const sut{lvalue};

      BOOST_TEST((sut == lvalue));
    }
    BOOST_AUTO_TEST_CASE(copyable_from_const_lvalue)
    {
      auto const clvalue = v13::any_action{actions::decrement_nw_ttl{}};

      v13::any_action const sut{clvalue};

      BOOST_TEST((sut == clvalue));
    }
    BOOST_AUTO_TEST_CASE(copyable_from_const_xvalue)
    {
      auto const cxvalue = v13::any_action{actions::set_ipv6_dst{"::"_ipv6}};

      v13::any_action const sut{std::move(cxvalue)};

      BOOST_TEST((sut == cxvalue));
    }
  BOOST_AUTO_TEST_SUITE_END() // copy_constructor

  BOOST_AUTO_TEST_SUITE(move_constructor)
    BOOST_AUTO_TEST_CASE(copyable_from_non_const_xvalue)
    {
      auto const lvalue = v13::any_action{
        actions::set_ipv6_nd_sll{"\x01\x02\x03\x04\x05\x06"_mac}
      };
      auto xvalue = lvalue;

      v13::any_action const sut{std::move(xvalue)};

      BOOST_TEST((sut == lvalue));
    }
    BOOST_AUTO_TEST_CASE(copyable_from_prvalue)
    {
      v13::any_action const sut{actions::set_ipv6_src{"::1"_ipv6}};

      BOOST_TEST((sut.type() == actions::set_ipv6_src::type()));
    }
  BOOST_AUTO_TEST_SUITE_END() // move_constructor

  BOOST_AUTO_TEST_SUITE(assignment)
    BOOST_AUTO_TEST_CASE(assignable_with_non_const_lvalue_action)
    {
      auto sut = v13::any_action{actions::set_nw_ttl{2}};
      auto action = actions::decrement_nw_ttl{};

      sut = action;

      BOOST_TEST((sut == v13::any_action{action}));
    }
    BOOST_AUTO_TEST_CASE(assignable_with_const_lvalue_action)
    {
      auto sut = v13::any_action{actions::set_nw_ttl{2}};
      auto const action = actions::decrement_nw_ttl{};

      sut = action;

      BOOST_TEST((sut == v13::any_action{action}));
    }
    BOOST_AUTO_TEST_CASE(assignable_with_const_xvalue_action)
    {
      auto sut = v13::any_action{actions::set_nw_ttl{2}};
      auto const action = actions::decrement_nw_ttl{};

      sut = std::move(action);

      BOOST_TEST((sut == v13::any_action{action}));
    }
    BOOST_AUTO_TEST_CASE(assignable_with_non_const_xvalue_action)
    {
      auto sut = v13::any_action{actions::set_nw_ttl{2}};
      auto const action = actions::decrement_nw_ttl{};
      auto xvalue = action;

      sut = std::move(xvalue);

      BOOST_TEST((sut == v13::any_action{action}));
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_AUTO_TEST_SUITE(copy_assignmennt)
    BOOST_AUTO_TEST_CASE(assignable_with_non_const_lvalue)
    {
      auto sut = v13::any_action{actions::output{1, 2}};
      auto lvalue = v13::any_action{actions::output{2, 3}};

      sut = lvalue;

      BOOST_TEST((sut == lvalue));
    }
    BOOST_AUTO_TEST_CASE(assignable_with_const_lvalue)
    {
      auto sut = v13::any_action{actions::push_mpls::unicast()};
      auto const clvalue = v13::any_action{actions::push_mpls::multicast()};

      sut = clvalue;

      BOOST_TEST((sut == clvalue));
    }
    BOOST_AUTO_TEST_CASE(assignable_with_const_xvalue)
    {
      auto sut = v13::any_action{actions::set_eth_type{0x0800}};
      auto const cxvalue = v13::any_action{actions::set_eth_type{0x8100}};

      sut = std::move(cxvalue);

      BOOST_TEST((sut == cxvalue));
    }
    BOOST_AUTO_TEST_CASE(assignable_with_object_containing_different_action_type)
    {
      auto sut = v13::any_action{actions::output{1, 2}};
      auto const diff_type_action = v13::any_action{actions::group{3}};

      sut = diff_type_action;

      BOOST_TEST((sut == diff_type_action));
    }
  BOOST_AUTO_TEST_SUITE_END() // copy_assignmennt

  BOOST_AUTO_TEST_SUITE(move_assignment)
    BOOST_AUTO_TEST_CASE(assignable_with_non_const_xvalue)
    {
      auto sut = v13::any_action{actions::group{1}};
      auto const lvalue = v13::any_action{
        actions::set_ipv6_nd_sll{"\x01\x02\x03\x04\x05\x06"_mac}
      };
      auto xvalue = lvalue;

      sut = std::move(xvalue);

      BOOST_TEST((sut == lvalue));
    }
    BOOST_AUTO_TEST_CASE(assignable_with_prvalue)
    {
      auto sut = v13::any_action{actions::group{1}};

      sut = actions::set_ipv6_src{"::1"_ipv6};

      BOOST_TEST((sut.type() == actions::set_ipv6_src::type()));
    }
  BOOST_AUTO_TEST_SUITE_END() // move_assignment

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::any_action{actions::group{1}};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_contained_action_value_is_equal)
    {
      auto const sut1 = v13::any_action{actions::set_queue{1}};
      auto const sut2 = v13::any_action{actions::set_queue{1}};

      BOOST_TEST((sut1 == sut2));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_action_value_is_not_equal)
    {
      auto const sut1 = v13::any_action{actions::push_vlan::ieee802_1Q()};
      auto const sut2 = v13::any_action{actions::push_vlan::ieee802_1ad()};

      BOOST_TEST((sut1 != sut2));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_action_padd_is_not_equal)
    {
      auto const binary = "\x00\x0c\x00\x08\x00\x00\x00\x01"_bin;
      auto it = binary.begin();
      auto const sut1 = v13::any_action{actions::copy_ttl_in{}};
      auto const sut2
        = v13::any_action{actions::copy_ttl_in::decode(it, binary.end())};

      BOOST_TEST((sut1 != sut2));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_action_type_is_not_equal)
    {
      auto const sut1 = v13::any_action{actions::pop_vlan{}};
      auto const sut2 = v13::any_action{actions::pop_pbb{}};

      BOOST_TEST((sut1 != sut2));
    }
    BOOST_AUTO_TEST_CASE(true_if_right_operand_is_same_values_action)
    {
      auto const output = actions::output{1, 1};
      auto const sut = v13::any_action{actions::output{1, 1}};

      BOOST_TEST((sut == output));
    }
    BOOST_AUTO_TEST_CASE(false_if_right_operand_is_different_values_action)
    {
      auto const output = actions::output{2, 1};
      auto const sut = v13::any_action{actions::output{1, 1}};

      BOOST_TEST((sut != output));
    }
    BOOST_AUTO_TEST_CASE(false_if_right_operand_is_different_types_action)
    {
      auto const group = actions::group{1};
      auto const sut = v13::any_action{actions::output{1, 1}};

      BOOST_TEST((sut != group));
    }
    BOOST_AUTO_TEST_CASE(true_if_left_operand_is_same_values_action)
    {
      auto const set_ipv4_src = actions::set_ipv4_src{"127.0.0.1"_ipv4};
      auto const sut = v13::any_action{actions::set_ipv4_src{"127.0.0.1"_ipv4}};

      BOOST_TEST((set_ipv4_src == sut));
    }
    BOOST_AUTO_TEST_CASE(false_if_left_operand_is_different_values_action)
    {
      auto const set_ipv4_src = actions::set_ipv4_src{"127.0.0.2"_ipv4};
      auto const sut = v13::any_action{actions::set_ipv4_src{"127.0.0.1"_ipv4}};

      BOOST_TEST((set_ipv4_src != sut));
    }
    BOOST_AUTO_TEST_CASE(false_if_left_operand_is_different_types_action)
    {
      auto const set_eth_dst = actions::set_eth_dst{"\x01\x02\x03\x04\x05\x06"_mac};
      auto const sut = v13::any_action{actions::set_ipv4_src{"127.0.0.1"_ipv4}};

      BOOST_TEST((set_eth_dst != sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(function_equivalent)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::any_action{actions::group{1}};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_contained_action_value_is_equal)
    {
      auto const sut1 = v13::any_action{actions::set_queue{1}};
      auto const sut2 = v13::any_action{actions::set_queue{1}};

      BOOST_TEST(equivalent(sut1, sut2));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_action_value_is_not_equal)
    {
      auto const sut1 = v13::any_action{actions::push_vlan::ieee802_1Q()};
      auto const sut2 = v13::any_action{actions::push_vlan::ieee802_1ad()};

      BOOST_TEST(!equivalent(sut1, sut2));
    }
    BOOST_AUTO_TEST_CASE(true_if_contained_action_padd_is_not_equal)
    {
      auto const binary = "\x00\x0c\x00\x08\x00\x00\x00\x01"_bin;
      auto it = binary.begin();
      auto const sut1 = v13::any_action{actions::copy_ttl_in{}};
      auto const sut2
        = v13::any_action{actions::copy_ttl_in::decode(it, binary.end())};

      BOOST_TEST(equivalent(sut1, sut2));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_action_type_is_not_equal)
    {
      auto const sut1 = v13::any_action{actions::pop_vlan{}};
      auto const sut2 = v13::any_action{actions::pop_pbb{}};

      BOOST_TEST(!equivalent(sut1, sut2));
    }
    BOOST_AUTO_TEST_CASE(true_if_right_operand_is_same_values_action)
    {
      auto const output = actions::output{1, 1};
      auto const sut = v13::any_action{actions::output{1, 1}};

      BOOST_TEST(equivalent(sut, output));
    }
    BOOST_AUTO_TEST_CASE(false_if_right_operand_is_different_values_action)
    {
      auto const output = actions::output{2, 1};
      auto const sut = v13::any_action{actions::output{1, 1}};

      BOOST_TEST(!equivalent(sut, output));
    }
    BOOST_AUTO_TEST_CASE(false_if_right_operand_is_different_types_action)
    {
      auto const group = actions::group{1};
      auto const sut = v13::any_action{actions::output{1, 1}};

      BOOST_TEST(!equivalent(sut, group));
    }
    BOOST_AUTO_TEST_CASE(true_if_left_operand_is_same_values_action)
    {
      auto const set_ipv4_src = actions::set_ipv4_src{"127.0.0.1"_ipv4};
      auto const sut = v13::any_action{actions::set_ipv4_src{"127.0.0.1"_ipv4}};

      BOOST_TEST(equivalent(set_ipv4_src, sut));
    }
    BOOST_AUTO_TEST_CASE(false_if_left_operand_is_different_values_action)
    {
      auto const set_ipv4_src = actions::set_ipv4_src{"127.0.0.2"_ipv4};
      auto const sut = v13::any_action{actions::set_ipv4_src{"127.0.0.1"_ipv4}};

      BOOST_TEST(!equivalent(set_ipv4_src, sut));
    }
    BOOST_AUTO_TEST_CASE(false_if_left_operand_is_different_types_action)
    {
      auto const set_eth_dst = actions::set_eth_dst{"\x01\x02\x03\x04\x05\x06"_mac};
      auto const sut = v13::any_action{actions::set_ipv4_src{"127.0.0.1"_ipv4}};

      BOOST_TEST(!equivalent(set_eth_dst, sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_AUTO_TEST_SUITE(encode)
    // TODO
  BOOST_AUTO_TEST_SUITE_END() // function_encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_AUTO_TEST_CASE(throw_exception_if_type_is_unknown)
    {
      auto const binary = "\xff\x00\x00\x08\x00\x00\x00\x00"_bin;
      auto it = binary.begin();

      BOOST_CHECK_EXCEPTION(
            v13::any_action::decode(it, binary.end())
          , v13::exception
          , [](v13::exception const& e) {
              return e.error_type() == v13::protocol::error_type::bad_action
                  && e.error_code() == v13::protocol::bad_action_code::bad_type;
            });
    }
    // TODO
  BOOST_AUTO_TEST_SUITE_END() // function_decode

  BOOST_AUTO_TEST_SUITE(function_any_cast)
    BOOST_AUTO_TEST_CASE(success_if_matching_type)
    {
      auto const action = actions::set_tcp_src{1};
      auto const sut = v13::any_action{action};

      auto const& value = v13::any_cast<actions::set_tcp_src>(sut);

      BOOST_TEST((value == action));
    }
    BOOST_AUTO_TEST_CASE(throw_exception_if_not_matching_type)
    {
      auto const sut = v13::any_action{actions::set_tcp_src{1}};

      BOOST_CHECK_THROW(
          v13::any_cast<actions::set_udp_src>(sut), ofp::bad_get);
    }
    BOOST_AUTO_TEST_CASE(return_pointer_if_argument_is_pointer)
    {
      auto const action = actions::pop_mpls{1};
      auto const sut = v13::any_action{action};

      auto const value = v13::any_cast<actions::pop_mpls>(&sut);

      BOOST_TEST((*value == action));
    }
    BOOST_AUTO_TEST_CASE(return_nullptr_if_arugment_is_pointer_and_not_matching_type)
    {
      auto const sut = v13::any_action{actions::pop_vlan{}};

      auto const value = v13::any_cast<actions::pop_pbb>(&sut);

      BOOST_TEST((value == nullptr));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_any_cast

BOOST_AUTO_TEST_SUITE_END() // any_action_test
BOOST_AUTO_TEST_SUITE_END() // common_type_test

