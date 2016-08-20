#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/any_queue_property.hpp>
#include <boost/test/unit_test.hpp>

#include <utility>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace queue_props = ofp::v10::queue_properties;

namespace {
struct any_queue_property_fixiture
{
  queue_props::min_rate min_rate{0x1234};
  std::vector<unsigned char> min_rate_binary
    = "\x00\x01\x00\x10\x00\x00\x00\x00""\x12\x34\x00\x00\x00\x00\x00\x00"_bin;
};
}

BOOST_AUTO_TEST_SUITE(utility_test)
BOOST_AUTO_TEST_SUITE(any_queue_property_test)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_min_rate)
    {
      auto const min_rate = queue_props::min_rate{10};

      auto const sut = ofp::v10::any_queue_property{min_rate};

      BOOST_TEST(sut.property() == min_rate.property());
      BOOST_TEST(sut.length() == min_rate.length());
      // TODO
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = ofp::v10::any_queue_property{queue_props::min_rate{10}};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_contained_property_is_equal)
    {
      auto const min_rate = queue_props::min_rate{200};

      BOOST_TEST(
          (ofp::v10::any_queue_property{min_rate}
        == ofp::v10::any_queue_property{min_rate}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_contained_property_is_not_equal)
    {
      BOOST_TEST(
          (ofp::v10::any_queue_property{queue_props::min_rate{200}}
        != ofp::v10::any_queue_property{queue_props::min_rate{300}}));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_contained_property_is_not_equal_but_equivalent)
    {
      BOOST_TEST(
          (ofp::v10::any_queue_property{queue_props::min_rate{1001}}
        != ofp::v10::any_queue_property{queue_props::min_rate{1002}}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_right_operand_is_equal_value_property)
    {
      auto const min_rate = queue_props::min_rate{200};

      BOOST_TEST((ofp::v10::any_queue_property{min_rate} == min_rate));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_right_operand_is_not_equal_value_property)
    {
      BOOST_TEST(
          (ofp::v10::any_queue_property{queue_props::min_rate{200}}
        != queue_props::min_rate{201}));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_right_operand_is_not_equal_value_property_but_equivalent)
    {
      BOOST_TEST(
          (ofp::v10::any_queue_property{queue_props::min_rate{1200}}
        != queue_props::min_rate{1201}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_left_operand_is_equal_value_property)
    {
      auto const min_rate = queue_props::min_rate{201};

      BOOST_TEST((min_rate == ofp::v10::any_queue_property{min_rate}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_left_operand_is_not_equal_value_property)
    {
      BOOST_TEST(
          (queue_props::min_rate{201}
        != ofp::v10::any_queue_property{queue_props::min_rate{200}}));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_left_operand_is_not_equal_value_property_but_equivalent)
    {
      BOOST_TEST(
          (queue_props::min_rate{1201}
        != ofp::v10::any_queue_property{queue_props::min_rate{1200}}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(function_equivalent)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = ofp::v10::any_queue_property{queue_props::min_rate{10}};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_contained_property_is_equal)
    {
      auto const min_rate = queue_props::min_rate{200};

      BOOST_TEST(
          equivalent(
              ofp::v10::any_queue_property{min_rate}
            , ofp::v10::any_queue_property{min_rate}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_contained_property_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              ofp::v10::any_queue_property{queue_props::min_rate{200}}
            , ofp::v10::any_queue_property{queue_props::min_rate{300}}));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_contained_property_is_not_equal_but_equivalent)
    {
      BOOST_TEST(
          equivalent(
              ofp::v10::any_queue_property{queue_props::min_rate{1001}}
            , ofp::v10::any_queue_property{queue_props::min_rate{1002}}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_right_operand_is_equal_value_property)
    {
      auto const min_rate = queue_props::min_rate{200};

      BOOST_TEST(equivalent(ofp::v10::any_queue_property{min_rate}, min_rate));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_right_operand_is_not_equal_value_property)
    {
      BOOST_TEST(
          !equivalent(
              ofp::v10::any_queue_property{queue_props::min_rate{200}}
            , queue_props::min_rate{201}));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_right_operand_is_not_equal_value_property_but_equivalent)
    {
      BOOST_TEST(
          equivalent(
              ofp::v10::any_queue_property{queue_props::min_rate{1200}}
            , queue_props::min_rate{1201}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_left_operand_is_equal_value_property)
    {
      auto const min_rate = queue_props::min_rate{201};

      BOOST_TEST(equivalent(min_rate, ofp::v10::any_queue_property{min_rate}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_left_operand_is_not_equal_value_property)
    {
      BOOST_TEST(
          !equivalent(
              queue_props::min_rate{201}
            , ofp::v10::any_queue_property{queue_props::min_rate{200}}));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_left_operand_is_not_equal_value_property_but_equivalent)
    {
      BOOST_TEST(
          equivalent(
              queue_props::min_rate{1201}
            , ofp::v10::any_queue_property{queue_props::min_rate{1200}}));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_FIXTURE_TEST_SUITE(encode, any_queue_property_fixiture)
    BOOST_AUTO_TEST_CASE(generate_min_rate_binary_if_containing_min_rate)
    {
      auto const sut = ofp::v10::any_queue_property{min_rate};
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == min_rate_binary, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, any_queue_property_fixiture)
    BOOST_AUTO_TEST_CASE(constructible_from_min_rate_binary)
    {
      auto it = min_rate_binary.begin();

      auto const prop
        = ofp::v10::any_queue_property::decode(it, min_rate_binary.end());

      BOOST_TEST((it == min_rate_binary.end()));
      BOOST_TEST((prop == min_rate));
    }
    BOOST_AUTO_TEST_CASE(throw_exception_if_queue_property_is_unknown)
    {
      auto const binary = "\x00\xff\x00\x08\x00\x00\x00\x00"_bin;
      auto it = binary.begin();

      BOOST_CHECK_THROW(
            ofp::v10::any_queue_property::decode(it, binary.end())
          , std::runtime_error);
    }
    BOOST_AUTO_TEST_CASE(
        throw_exception_if_length_field_is_larger_than_binary_size)
    {
      min_rate_binary.resize(min_rate_binary.size() - 1);
      auto it = min_rate_binary.begin();

      BOOST_CHECK_THROW(
            ofp::v10::any_queue_property::decode(it, min_rate_binary.end())
          , std::runtime_error);
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

  BOOST_AUTO_TEST_SUITE(reference_style_any_cast)
    BOOST_AUTO_TEST_CASE(return_contained_property_if_specified_type_is_same)
    {
      using prop_type = queue_props::min_rate;
      auto const prop = prop_type{1};
      auto const sut = ofp::v10::any_queue_property{prop};

      BOOST_TEST((ofp::v10::any_cast<prop_type>(sut) == prop));
    }
  BOOST_AUTO_TEST_SUITE_END() // refstyle_any_cast

  BOOST_AUTO_TEST_SUITE(pointer_style_any_cast)
    BOOST_AUTO_TEST_CASE(
        return_pointer_to_contained_property_if_specified_type_is_same)
    {
      using prop_type = queue_props::min_rate;
      auto const prop = prop_type{1};
      auto const sut = ofp::v10::any_queue_property{prop};

      auto const value = ofp::v10::any_cast<prop_type>(&sut);

      BOOST_TEST_REQUIRE((value != nullptr));
      BOOST_TEST((*value == prop));
    }
  BOOST_AUTO_TEST_SUITE_END() // pointer_style_any_cast

BOOST_AUTO_TEST_SUITE_END() // any_queue_property_test
BOOST_AUTO_TEST_SUITE_END() // utility_test
