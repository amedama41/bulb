#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/actions.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace v13 = canard::net::ofp::v13;
namespace protocol = v13::protocol;
namespace actions = v13::actions;

using all_action_types = v13::default_all_action_list;
using fixed_length_action_types = v13::default_action_list;
using variable_length_action_types = v13::default_set_field_list;

using all_set_field_types = v13::default_set_field_list;

static auto& random_pad = ::random<std::uint8_t>;

BOOST_AUTO_TEST_SUITE(actions_test)
  BOOST_AUTO_TEST_SUITE(validate_header_for_ofp_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, Action, all_action_types)
    {
      constexpr auto min_length = sizeof(typename Action::raw_ofp_type);
      auto const header = protocol::ofp_action_header{
          Action::type(), min_length
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = Action::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_type_is_incorrect, Action, all_action_types)
    {
      constexpr auto min_length = sizeof(typename Action::raw_ofp_type);
      auto const header = protocol::ofp_action_header{
          Action::type() + 1, min_length
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = Action::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid action type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , Action, all_action_types)
    {
      constexpr auto min_length = sizeof(typename Action::raw_ofp_type);
      auto const header = protocol::ofp_action_header{
          Action::type(), min_length - 1
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = Action::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid action length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size_for
        , FixedLengthAction, fixed_length_action_types)
    {
      constexpr auto min_length
        = sizeof(typename FixedLengthAction::raw_ofp_type);
      auto const header = protocol::ofp_action_header{
          FixedLengthAction::type(), min_length + 1
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = FixedLengthAction::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid action length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_length_is_greater_than_ofp_struct_size_for
        , VariableLengthAction, variable_length_action_types)
    {
      constexpr auto min_length
        = sizeof(typename VariableLengthAction::raw_ofp_type);
      auto const header = protocol::ofp_action_header{
          VariableLengthAction::type(), min_length + 1
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = VariableLengthAction::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header_for_ofp_header
  BOOST_AUTO_TEST_SUITE(validate_header_for_set_field)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, SetField, all_set_field_types)
    {
      constexpr auto oxm_field = SetField::oxm_match_field::oxm_field();
      constexpr auto oxm_value_length
        = SetField::oxm_match_field::min_length() - sizeof(std::uint32_t);
      auto const set_field = protocol::ofp_action_set_field{
          protocol::action_type::set_field, SetField::min_length()
        , { 0x80, 0x00, std::uint8_t(oxm_field << 1), oxm_value_length }
      };

      auto const error_msg = SetField::validate_set_field(set_field);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_even_if_with_oxm_hasmask, SetField, all_set_field_types)
    {
      constexpr auto oxm_field = SetField::oxm_match_field::oxm_field();
      constexpr auto oxm_value_length
        = SetField::oxm_match_field::min_length() - sizeof(std::uint32_t);
      constexpr auto oxm_length = oxm_value_length * 2;
      constexpr auto set_field_length
        = (sizeof(protocol::ofp_action_set_field) + oxm_length + 7) / 8 * 8;
      auto const set_field = protocol::ofp_action_set_field{
          protocol::action_type::set_field, set_field_length
        , { 0x80, 0x00, std::uint8_t((oxm_field << 1) | 0x01), oxm_length }
      };

      auto const error_msg = SetField::validate_set_field(set_field);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_oxm_class_is_incorrect
        , SetField, all_set_field_types)
    {
      constexpr auto oxm_field = SetField::oxm_match_field::oxm_field();
      constexpr auto oxm_value_length
        = SetField::oxm_match_field::min_length() - sizeof(std::uint32_t);
      auto const set_field = protocol::ofp_action_set_field{
          protocol::action_type::set_field, SetField::min_length()
        , { 0x80, 0x01, std::uint8_t(oxm_field << 1), oxm_value_length }
      };

      auto const error_msg = SetField::validate_set_field(set_field);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm class"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_oxm_field_is_incorrect
        , SetField, all_set_field_types)
    {
      constexpr auto oxm_field = SetField::oxm_match_field::oxm_field();
      constexpr auto oxm_value_length
        = SetField::oxm_match_field::min_length() - sizeof(std::uint32_t);
      auto const set_field = protocol::ofp_action_set_field{
          protocol::action_type::set_field, SetField::min_length()
        , { 0x80, 0x00, std::uint8_t((oxm_field + 1) << 1), oxm_value_length }
      };

      auto const error_msg = SetField::validate_set_field(set_field);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm field"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_oxm_length_is_incorrect
        , SetField, all_set_field_types)
    {
      constexpr auto oxm_field = SetField::oxm_match_field::oxm_field();
      constexpr auto oxm_value_length
        = SetField::oxm_match_field::min_length() - sizeof(std::uint32_t);
      auto const set_field = protocol::ofp_action_set_field{
          protocol::action_type::set_field, SetField::min_length()
        , { 0x80, 0x00, std::uint8_t(oxm_field << 1), oxm_value_length + 1 }
      };

      auto const error_msg = SetField::validate_set_field(set_field);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_valid_length
        , SetField, all_set_field_types)
    {
      constexpr auto oxm_field = SetField::oxm_match_field::oxm_field();
      constexpr auto oxm_value_length
        = SetField::oxm_match_field::min_length() - sizeof(std::uint32_t);
      auto const set_field = protocol::ofp_action_set_field{
          protocol::action_type::set_field, SetField::min_length() - 1
        , { 0x80, 0x00, std::uint8_t(oxm_field << 1), oxm_value_length }
      };

      auto const error_msg = SetField::validate_set_field(set_field);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid set_field length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_valid_length
        , SetField, all_set_field_types)
    {
      constexpr auto oxm_field = SetField::oxm_match_field::oxm_field();
      constexpr auto oxm_value_length
        = SetField::oxm_match_field::min_length() - sizeof(std::uint32_t);
      auto const set_field = protocol::ofp_action_set_field{
          protocol::action_type::set_field, SetField::min_length() + 1
        , { 0x80, 0x00, std::uint8_t(oxm_field << 1), oxm_value_length }
      };

      auto const error_msg = SetField::validate_set_field(set_field);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid set_field length"_sr);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header_for_set_field
BOOST_AUTO_TEST_SUITE_END() // actions_test

