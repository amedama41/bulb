#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/utility/meter_band_set.hpp>
#include <boost/test/unit_test.hpp>

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace bands = v13::meter_bands;

namespace {
  template <class T>
  auto calc_key(T const& t) noexcept
    -> std::uint64_t
  {
    return v13::meter_band_set::calc_key(t.rate(), t.burst_size());
  }
}

BOOST_AUTO_TEST_SUITE(utility_test)
BOOST_AUTO_TEST_SUITE(meter_band_set)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(
        is_constructible_from_multiple_same_band_types_but_rates_are_different)
    {
      auto const drop1 = bands::drop{1};
      auto const drop2 = bands::drop{3};
      auto const dscp_remark = bands::dscp_remark{0, 2};

      v13::meter_band_set const sut{ drop1, drop2, dscp_remark };

      BOOST_TEST(sut.size() == 3);
      BOOST_TEST_REQUIRE((sut.find(calc_key(drop1)) != sut.end()));
      BOOST_TEST((sut.at(calc_key(drop1)) == drop1));
      BOOST_TEST_REQUIRE((sut.find(calc_key(drop2)) != sut.end()));
      BOOST_TEST((sut.at(calc_key(drop2)) == drop2));
      BOOST_TEST_REQUIRE((sut.find(calc_key(dscp_remark)) != sut.end()));
      BOOST_TEST((sut.at(calc_key(dscp_remark)) == dscp_remark));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(is_valid_set)
    BOOST_AUTO_TEST_CASE(is_true_if_list_contains_different_rate_meter_bands)
    {
      auto const drop1 = bands::drop{1};
      auto const drop2 = bands::drop{3};
      auto const dscp_remark = bands::dscp_remark{0, 2};

      auto const list
        = v13::meter_band_set::list_type{ drop1, drop2, dscp_remark };

      BOOST_TEST(v13::meter_band_set::is_valid_set(list));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_list_contains_different_burst_size_meter_bands)
    {
      auto const rate = 1;
      auto const drop1 = bands::drop{rate, 1};
      auto const drop2 = bands::drop{rate, 3};
      auto const dscp_remark = bands::dscp_remark{0, rate, 2};

      auto const list
        = v13::meter_band_set::list_type{ drop1, drop2, dscp_remark };

      BOOST_TEST(v13::meter_band_set::is_valid_set(list));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_list_contains_same_rate_and_burst_size_bands)
    {
      auto const rate = 1;
      auto const burst_size = 2;
      auto const drop = bands::drop{rate, burst_size};
      auto const dscp_remark = bands::dscp_remark{1, rate, burst_size};

      auto const list = v13::meter_band_set::list_type{ drop, dscp_remark };

      BOOST_TEST(!v13::meter_band_set::is_valid_set(list));
    }
  BOOST_AUTO_TEST_SUITE_END() // is_valid_set

  BOOST_AUTO_TEST_SUITE(equivalent_as_set)
    BOOST_AUTO_TEST_CASE(
        is_true_if_both_meter_bands_are_equal_but_order_is_not_equal)
    {
      auto const drop = bands::drop{1};
      auto const dscp_remark = bands::dscp_remark{1, 2};
      auto const list1 = v13::meter_band_set::list_type{ drop, dscp_remark };
      auto const list2 = v13::meter_band_set::list_type{ dscp_remark, drop };

      BOOST_TEST(v13::meter_band_set::equivalent_as_set(list1, list2));
    }
  BOOST_AUTO_TEST_SUITE_END() // equivalent_as_set

BOOST_AUTO_TEST_SUITE_END() // meter_band_set
BOOST_AUTO_TEST_SUITE_END() // utility_test
