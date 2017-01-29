#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/instructions.hpp>
#include <boost/test/unit_test.hpp>

#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;

using all_instruction_types = v13::default_instruction_list;
using fixed_length_instruction_types = std::tuple<
    v13::instructions::goto_table
  , v13::instructions::write_metadata
  , v13::instructions::meter
>;
using variable_length_instruction_types = std::tuple<
    v13::instructions::write_actions
  , v13::instructions::apply_actions
  , v13::instructions::clear_actions
>;

BOOST_AUTO_TEST_SUITE(instruction_test)
  BOOST_AUTO_TEST_SUITE(validate_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, Instruction, all_instruction_types)
    {
      auto const header = v13::protocol::ofp_instruction{
        Instruction::type(), Instruction::min_length()
      };

      auto const error_msg = Instruction::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_type_is_incorrect
        , Instruction, all_instruction_types)
    {
      auto const header = v13::protocol::ofp_instruction{
        Instruction::type() + 1, Instruction::min_length()
      };

      auto const error_msg = Instruction::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid instruction type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , Instruction, all_instruction_types)
    {
      auto const header = v13::protocol::ofp_instruction{
        Instruction::type(), Instruction::min_length() - 1
      };

      auto const error_msg = Instruction::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid instruction length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size_for
        , FixedLengthInstruction, fixed_length_instruction_types)
    {
      auto const header = v13::protocol::ofp_instruction{
        FixedLengthInstruction::type(), FixedLengthInstruction::min_length() + 1
      };

      auto const error_msg = FixedLengthInstruction::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid instruction length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_length_is_greater_than_ofp_struct_size_for
        , VariableLengthInstruction, variable_length_instruction_types)
    {
      auto const header = v13::protocol::ofp_instruction{
          VariableLengthInstruction::type()
        , VariableLengthInstruction::min_length() + 1
      };

      auto const error_msg = VariableLengthInstruction::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header
BOOST_AUTO_TEST_SUITE_END() // instruction_test
