#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/hello_elements.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;

namespace bdata = boost::unit_test::data;

using all_hello_element_types = v13::default_hello_element_list;

BOOST_AUTO_TEST_SUITE(hello_element_test)
  BOOST_AUTO_TEST_SUITE(validate_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_header_is_valid
        , HelloElement, all_hello_element_types)
    {
      auto const header = v13::protocol::ofp_hello_elem_header{
        HelloElement::type(), HelloElement::min_length()
      };

      auto const error_msg = HelloElement::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_type_is_incorrect
        , HelloElement, all_hello_element_types)
    {
      auto const header = v13::protocol::ofp_hello_elem_header{
        HelloElement::type() + 1, HelloElement::min_length()
      };

      auto const error_msg = HelloElement::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid hello element type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , HelloElement, all_hello_element_types)
    {
      auto const header = v13::protocol::ofp_hello_elem_header{
        HelloElement::type(), HelloElement::min_length() - 1
      };

      auto const error_msg = HelloElement::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid hello element length"_sr);
    }
    BOOST_DATA_TEST_CASE(
          return_nullptr_if_versionbitmap_length_is_adjust_to_element_size
        , bdata::make(std::vector<std::size_t>{0, 1, 2, 4, 8, 16, 32})
        , num_elem)
    {
      using sut = v13::hello_elements::versionbitmap;
      constexpr auto elem_length = sizeof(sut::bitmaps_type::value_type);
      auto const header = v13::protocol::ofp_hello_elem_header{
          sut::type()
        , std::uint16_t(sut::min_length() + elem_length * num_elem)
      };

      auto const error_msg = sut::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_DATA_TEST_CASE(
          return_error_message_if_versionbitmap_length_is_not_adjust_to_element_size
        , bdata::make(std::vector<std::size_t>{0, 1, 2, 4, 8, 16, 32})
        , num_elem)
    {
      using sut = v13::hello_elements::versionbitmap;
      constexpr auto elem_length = sizeof(sut::bitmaps_type::value_type);
      auto const header = v13::protocol::ofp_hello_elem_header{
          sut::type()
        , std::uint16_t(sut::min_length() + elem_length * num_elem - 1)
      };

      auto const error_msg = sut::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid hello element length"_sr);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header
BOOST_AUTO_TEST_SUITE_END() // hello_element_test
