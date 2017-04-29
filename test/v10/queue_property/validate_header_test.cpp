#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/queue_properties.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace protocol = v10::protocol;

using all_queue_property_types = v10::default_queue_property_list;

static auto& random_pad = ::random<std::uint8_t>;

BOOST_AUTO_TEST_SUITE(queue_property_test)
BOOST_AUTO_TEST_SUITE(validate_header)
  BOOST_AUTO_TEST_CASE_TEMPLATE(
      return_nullptr_if_header_is_valid, T, all_queue_property_types)
  {
    auto const header = protocol::ofp_queue_prop_header{
        T::type(), sizeof(typename T::ofp_type)
      , { random_pad(), random_pad(), random_pad(), random_pad() }
    };

    auto const error_msg = T::validate_header(header);

    BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
  }
  BOOST_AUTO_TEST_CASE_TEMPLATE(
      return_error_message_if_type_is_incorrect, T, all_queue_property_types)
  {
    auto const header = protocol::ofp_queue_prop_header{
        T::type() + 1, sizeof(typename T::ofp_type)
      , { random_pad(), random_pad(), random_pad(), random_pad() }
    };

    auto const error_msg = T::validate_header(header);

    BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
    BOOST_TEST(error_msg == "invalid queue property type"_sr);
  }
  BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_length_is_less_than_ofp_struct_size
      , T, all_queue_property_types)
  {
    auto const header = protocol::ofp_queue_prop_header{
        T::type(), sizeof(typename T::ofp_type) - 1
      , { random_pad(), random_pad(), random_pad(), random_pad() }
    };

    auto const error_msg = T::validate_header(header);

    BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
    BOOST_TEST(error_msg == "invalid queue property length"_sr);
  }
  BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_length_is_greater_than_ofp_struct_size
      , T, all_queue_property_types)
  {
    auto const header = protocol::ofp_queue_prop_header{
        T::type(), sizeof(typename T::ofp_type) + 1
      , { random_pad(), random_pad(), random_pad(), random_pad() }
    };

    auto const error_msg = T::validate_header(header);

    BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
    BOOST_TEST(error_msg == "invalid queue property length"_sr);
  }
BOOST_AUTO_TEST_SUITE_END() // validate_header
BOOST_AUTO_TEST_SUITE_END() // queue_property_test
