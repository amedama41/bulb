#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/any_hello_element.hpp>
#include <boost/test/unit_test.hpp>

#include <type_traits>

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;

BOOST_AUTO_TEST_SUITE(utility_test)
BOOST_AUTO_TEST_SUITE(any_hello_element)

  BOOST_AUTO_TEST_SUITE(type_definition_test)
    BOOST_AUTO_TEST_CASE(min_length)
    {
      using sut = v13::any_hello_element;

      using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

      BOOST_TEST(min_length::value == 4);
    }
    BOOST_AUTO_TEST_CASE(min_byte_length)
    {
      using sut = v13::any_hello_element;

      using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

      BOOST_TEST(min_byte_length::value == 8);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition_test

BOOST_AUTO_TEST_SUITE_END() // any_hello_element
BOOST_AUTO_TEST_SUITE_END() // utility_test
