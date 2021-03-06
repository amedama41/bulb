#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/any_queue_property.hpp>
#include <boost/test/unit_test.hpp>

#include <canard/net/ofp/v13/queue_properties.hpp>

#include <type_traits>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace queue_props = v13::queue_properties;

BOOST_AUTO_TEST_SUITE(common_type_test)
BOOST_AUTO_TEST_SUITE(any_queue_property_test)

  BOOST_AUTO_TEST_SUITE(type_definition_test)
    BOOST_AUTO_TEST_CASE(min_length)
    {
      using sut = v13::any_queue_property;

      using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

      BOOST_TEST(min_length::value == 16);
    }
    BOOST_AUTO_TEST_CASE(min_byte_length)
    {
      using sut = v13::any_queue_property;

      using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

      BOOST_TEST(min_byte_length::value == 16);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_test

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::any_queue_property{queue_props::min_rate{1}};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_contained_prop_value_is_equal)
    {
      BOOST_TEST(
          (v13::any_queue_property{queue_props::min_rate{1}}
           == v13::any_queue_property{queue_props::min_rate{1}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_prop_value_is_not_equal)
    {
      BOOST_TEST(
          (v13::any_queue_property{queue_props::min_rate{2}}
           != v13::any_queue_property{queue_props::min_rate{3}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_prop_value_is_not_equal_but_equivalent)
    {
      auto const binary
        = "\x00\x01\x00\x10\x00\x00\x00\x00""\x00\x12\x00\x00\x00\x00\x00\x01"
          ""_bin;
      auto it = binary.begin();
      auto const equivalent_min_rate
        = queue_props::min_rate::decode(it, binary.end());

      BOOST_TEST(
          (v13::any_queue_property{queue_props::min_rate{0x12}}
           != v13::any_queue_property{equivalent_min_rate}));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_prop_type_is_not_equal)
    {
      BOOST_TEST(
          (v13::any_queue_property{queue_props::min_rate{4}}
           != v13::any_queue_property{queue_props::max_rate{4}}));
    }
    BOOST_AUTO_TEST_CASE(true_if_right_operand_is_same_value_prop)
    {
      auto const max_rate = queue_props::max_rate{1};

      BOOST_TEST((v13::any_queue_property{max_rate} == max_rate));
    }
    BOOST_AUTO_TEST_CASE(false_if_right_operand_is_different_value_prop)
    {
      auto const max_rate1 = queue_props::max_rate{1};
      auto const max_rate2 = queue_props::max_rate{2};

      BOOST_TEST((v13::any_queue_property{max_rate1} != max_rate2));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_right_operand_is_not_equal_value_property_but_equivalent)
    {
      BOOST_TEST(
          (v13::any_queue_property{queue_props::max_rate{1200}}
        != queue_props::max_rate{1201}));
    }
    BOOST_AUTO_TEST_CASE(false_if_right_operand_is_different_type_prop)
    {
      auto const min_rate = queue_props::min_rate{1};
      auto const max_rate = queue_props::max_rate{1};

      BOOST_TEST((v13::any_queue_property{min_rate} != max_rate));
    }
    BOOST_AUTO_TEST_CASE(true_if_left_operand_is_same_value_prop)
    {
      auto const max_rate = queue_props::max_rate{1};

      BOOST_TEST((max_rate == v13::any_queue_property{max_rate}));
    }
    BOOST_AUTO_TEST_CASE(false_if_left_operand_is_different_value_prop)
    {
      auto const max_rate1 = queue_props::max_rate{1};
      auto const max_rate2 = queue_props::max_rate{2};

      BOOST_TEST((max_rate2 != v13::any_queue_property{max_rate1}));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_left_operand_is_not_equal_value_property_but_equivalent)
    {
      BOOST_TEST(
          (queue_props::min_rate{1201}
        != v13::any_queue_property{queue_props::min_rate{1200}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_left_operand_is_different_type_prop)
    {
      auto const min_rate = queue_props::min_rate{1};
      auto const max_rate = queue_props::max_rate{1};

      BOOST_TEST((max_rate != v13::any_queue_property{min_rate}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(function_equivalent)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::any_queue_property{queue_props::min_rate{1}};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_contained_prop_value_is_equal)
    {
      BOOST_TEST(
          equivalent(
              v13::any_queue_property{queue_props::min_rate{1}}
            , v13::any_queue_property{queue_props::min_rate{1}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_prop_value_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::any_queue_property{queue_props::min_rate{2}}
            , v13::any_queue_property{queue_props::min_rate{3}}));
    }
    BOOST_AUTO_TEST_CASE(true_if_contained_prop_value_is_not_equal_but_equivalent)
    {
      auto const binary
        = "\x00\x01\x00\x10\x00\x00\x00\x00""\x00\x12\x00\x00\x00\x00\x00\x01"
          ""_bin;
      auto it = binary.begin();
      auto const equivalent_min_rate
        = queue_props::min_rate::decode(it, binary.end());

      BOOST_TEST(
          equivalent(
              v13::any_queue_property{queue_props::min_rate{0x12}}
            , v13::any_queue_property{equivalent_min_rate}));
    }
    BOOST_AUTO_TEST_CASE(false_if_contained_prop_type_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::any_queue_property{queue_props::min_rate{4}}
            , v13::any_queue_property{queue_props::max_rate{4}}));
    }
    BOOST_AUTO_TEST_CASE(true_if_right_operand_is_same_value_prop)
    {
      auto const max_rate = queue_props::max_rate{1};

      BOOST_TEST(equivalent(v13::any_queue_property{max_rate}, max_rate));
    }
    BOOST_AUTO_TEST_CASE(false_if_right_operand_is_different_value_prop)
    {
      auto const max_rate1 = queue_props::max_rate{1};
      auto const max_rate2 = queue_props::max_rate{2};

      BOOST_TEST(!equivalent(v13::any_queue_property{max_rate1}, max_rate2));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_right_operand_is_not_equal_value_property_but_equivalent)
    {
      BOOST_TEST(
          equivalent(
              v13::any_queue_property{queue_props::max_rate{1200}}
            , queue_props::max_rate{1201}));
    }
    BOOST_AUTO_TEST_CASE(false_if_right_operand_is_different_type_prop)
    {
      auto const min_rate = queue_props::min_rate{1};
      auto const max_rate = queue_props::max_rate{1};

      BOOST_TEST(!equivalent(v13::any_queue_property{min_rate}, max_rate));
    }
    BOOST_AUTO_TEST_CASE(true_if_left_operand_is_same_value_prop)
    {
      auto const max_rate = queue_props::max_rate{1};

      BOOST_TEST(equivalent(max_rate, v13::any_queue_property{max_rate}));
    }
    BOOST_AUTO_TEST_CASE(false_if_left_operand_is_different_value_prop)
    {
      auto const max_rate1 = queue_props::max_rate{1};
      auto const max_rate2 = queue_props::max_rate{2};

      BOOST_TEST(!equivalent(max_rate2, v13::any_queue_property{max_rate1}));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_left_operand_is_not_equal_value_property_but_equivalent)
    {
      BOOST_TEST(
          equivalent(
              queue_props::min_rate{1201}
            , v13::any_queue_property{queue_props::min_rate{1200}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_left_operand_is_different_type_prop)
    {
      auto const min_rate = queue_props::min_rate{1};
      auto const max_rate = queue_props::max_rate{1};

      BOOST_TEST(!equivalent(max_rate, v13::any_queue_property{min_rate}));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_AUTO_TEST_SUITE(reference_style_any_cast)
    BOOST_AUTO_TEST_CASE(return_contained_property_if_specified_type_is_same)
    {
      using prop_type = queue_props::min_rate;
      auto const prop = prop_type{1};
      auto const sut = v13::any_queue_property{prop};

      BOOST_TEST((v13::any_cast<prop_type>(sut) == prop));
    }
    BOOST_AUTO_TEST_CASE(throw_exception_if_specified_type_is_not_same)
    {
      auto const sut = v13::any_queue_property{queue_props::max_rate{3}};

      BOOST_CHECK_THROW(
          v13::any_cast<queue_props::min_rate>(sut), ofp::bad_get);
    }
  BOOST_AUTO_TEST_SUITE_END() // reference_style_any_cast

  BOOST_AUTO_TEST_SUITE(pointer_style_any_cast)
    BOOST_AUTO_TEST_CASE(
        return_pointer_to_contained_property_if_specified_type_is_same)
    {
      using prop_type = queue_props::min_rate;
      auto const prop = prop_type{1};
      auto const sut = v13::any_queue_property{prop};

      auto const ptr = v13::any_cast<prop_type>(&sut);

      BOOST_TEST_REQUIRE((ptr != nullptr));
      BOOST_TEST((*ptr == prop));
    }
    BOOST_AUTO_TEST_CASE(return_null_pointer_if_specified_type_is_not_same)
    {
      auto const sut = v13::any_queue_property{queue_props::max_rate{3}};

      BOOST_TEST((v13::any_cast<queue_props::min_rate>(&sut) == nullptr));
    }
  BOOST_AUTO_TEST_SUITE_END() // pointer_style_any_cast

BOOST_AUTO_TEST_SUITE_END() // any_queue_property_test
BOOST_AUTO_TEST_SUITE_END() // common_type_test
