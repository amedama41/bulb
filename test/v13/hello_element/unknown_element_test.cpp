#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/hello_element/unknown_element.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace helems = v13::hello_elements;
namespace detail = v13::v13_detail;

namespace {
struct parameters {
  std::uint16_t type = 3;
  helems::unknown_element::data_type data = "\x01\x02\x03\x04\x05\x06"_bin;
};
struct unknown_element_fixture : parameters {
  helems::unknown_element sut{type, data};
  std::vector<unsigned char> bin
    = "\x00\x03\x00\x0a\x01\x02\x03\x04""\x05\x06\x00\x00\x00\x00\x00\x00"_bin;
};
struct no_data_fixture {
  helems::unknown_element sut{5};
  std::vector<unsigned char> bin = "\x00\x05\x00\x04\x00\x00\x00\x00"_bin;
};
}

BOOST_AUTO_TEST_SUITE(hello_element_test)
BOOST_AUTO_TEST_SUITE(unknown_element)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_type)
    {
      auto const type = std::uint16_t{2};

      helems::unknown_element const sut{type};

      BOOST_TEST(sut.type() == type);
      BOOST_TEST(sut.length() == sizeof(detail::ofp_hello_elem_header));
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_type_and_data)
    {
      auto const type = std::uint16_t{2};
      auto const data = helems::unknown_element::data_type{
        'A', 'B', 'C', 'D', 'E', 'F', 'G'
      };

      helems::unknown_element const sut{type, data};

      BOOST_TEST(sut.type() == type);
      BOOST_TEST(
          sut.length() == sizeof(detail::ofp_hello_elem_header) + data.size());
      BOOST_TEST(sut.data() == data, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, unknown_element_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, unknown_element_fixture)
    {
      auto const& const_unknown_element = sut;

      auto const copy = const_unknown_element;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, unknown_element_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_hello_elem_header));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = helems::unknown_element{type, data};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_type_and_data_are_equal)
    {
      BOOST_TEST(
          (helems::unknown_element{type, data}
        == helems::unknown_element{type, data}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_type_is_not_equal)
    {
      BOOST_TEST(
          (helems::unknown_element{1, data}
        != helems::unknown_element{2, data}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (helems::unknown_element{type, { 1, 2, 3 }}
        != helems::unknown_element{type, { 1, 2, 4 }}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(function_equivalent, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = helems::unknown_element{type, data};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_type_and_data_are_equal)
    {
      BOOST_TEST(
          equivalent(
                helems::unknown_element{type, data}
              , helems::unknown_element{type, data}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_type_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              helems::unknown_element{1, data}
            , helems::unknown_element{2, data}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_data_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              helems::unknown_element{type, { 1, 2, 3 }}
            , helems::unknown_element{type, { 1, 2, 4 }}));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generates_binary, unknown_element_fixture)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == (sut.length() + 7) / 8 * 8);
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
        generates_no_data_binary_from_no_data_unknown_element, no_data_fixture)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == (sut.length() + 7) / 8 * 8);
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        constructs_unknown_element_from_binary, unknown_element_fixture)
    {
      auto it = bin.begin();

      auto const unknown_element
        = helems::unknown_element::decode(it, bin.end());

      BOOST_TEST((unknown_element == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructs_no_data_unknown_element_from_no_data_binary, no_data_fixture)
    {
      auto it = bin.begin();

      auto const unknown_element
        = helems::unknown_element::decode(it, bin.end());

      BOOST_TEST((unknown_element == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // unknown_element
BOOST_AUTO_TEST_SUITE_END() // hello_element_test
