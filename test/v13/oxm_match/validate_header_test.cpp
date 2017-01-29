#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/common/oxm_match_field.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;

using all_oxm_match_field_types = v13::default_oxm_match_field_list;

static auto& random_hasmask = ::random<bool>;

BOOST_AUTO_TEST_SUITE(oxm_match_field_test)
  BOOST_AUTO_TEST_SUITE(validate_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_no_hasmask_and_header_is_valid
        , T, all_oxm_match_field_types)
    {
      constexpr auto payload_length = T::min_length() - sizeof(std::uint32_t);
      auto const header
        = std::uint32_t((T::oxm_type() << 9) | 0x0000 | payload_length);

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_hasmask_and_header_is_valid
        , T, all_oxm_match_field_types)
    {
      constexpr auto payload_length = T::min_length() - sizeof(std::uint32_t);
      auto const header
        = std::uint32_t((T::oxm_type() << 9) | 0x0100 | (payload_length * 2));

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_oxm_class_is_incorrect
        , T, all_oxm_match_field_types)
    {
      constexpr auto payload_length = T::min_length() - sizeof(std::uint32_t);
      auto const hasmask = random_hasmask();
      auto const header = std::uint32_t(
            (std::uint32_t(T::oxm_class() + 1) << 16)
          | (std::uint32_t(T::oxm_field()) << 9)
          | (hasmask ? 0x0100 : 0x0000)
          | (hasmask ? payload_length * 2 : payload_length));

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm class"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_oxm_field_is_incorrect
        , T, all_oxm_match_field_types)
    {
      constexpr auto payload_length = T::min_length() - sizeof(std::uint32_t);
      auto const hasmask = random_hasmask();
      auto const header = std::uint32_t(
            (std::uint32_t(T::oxm_class()) << 16)
          | (std::uint32_t(T::oxm_field() + 1) << 9)
          | (hasmask ? 0x0100 : 0x0000)
          | (hasmask ? payload_length * 2 : payload_length));

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm field"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_no_hasmask_and_oxm_length_is_less_than_oxm_value_size
        , T, all_oxm_match_field_types)
    {
      constexpr auto payload_length = T::min_length() - sizeof(std::uint32_t);
      auto const header
        = std::uint32_t((T::oxm_type() << 9) | 0x0000 | payload_length - 1);

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_no_hasmask_and_oxm_length_is_greater_than_oxm_value_size
        , T, all_oxm_match_field_types)
    {
      constexpr auto payload_length = T::min_length() - sizeof(std::uint32_t);
      auto const header
        = std::uint32_t((T::oxm_type() << 9) | 0x0000 | payload_length + 1);

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_hasmask_and_oxm_length_is_less_than_oxm_value_size
        , T, all_oxm_match_field_types)
    {
      constexpr auto payload_length = T::min_length() - sizeof(std::uint32_t);
      auto const header
        = std::uint32_t((T::oxm_type() << 9) | 0x0100 | payload_length * 2 - 1);

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_hasmask_and_oxm_length_is_greater_than_oxm_value_size
        , T, all_oxm_match_field_types)
    {
      constexpr auto payload_length = T::min_length() - sizeof(std::uint32_t);
      auto const header
        = std::uint32_t((T::oxm_type() << 9) | 0x0100 | payload_length * 2 + 1);

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid oxm length"_sr);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header
BOOST_AUTO_TEST_SUITE_END() // oxm_match_field_test
