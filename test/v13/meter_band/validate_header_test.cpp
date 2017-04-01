#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/meter_bands.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;

using all_meter_band_types = v13::default_meter_band_list;

static auto& random_rate = ::random<std::uint32_t>;
static auto& random_burst_size = ::random<std::uint32_t>;

BOOST_AUTO_TEST_SUITE(meter_band_test)
  BOOST_AUTO_TEST_SUITE(validate_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, MeterBand, all_meter_band_types)
    {
      auto const header = v13::protocol::ofp_meter_band_header{
          MeterBand::type(), MeterBand::min_length()
        , random_rate(), random_burst_size()
      };

      auto const error_msg = MeterBand::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_type_is_incorrect
        , MeterBand, all_meter_band_types)
    {
      auto const header = v13::protocol::ofp_meter_band_header{
          MeterBand::type() + 1, MeterBand::min_length()
        , random_rate(), random_burst_size()
      };

      auto const error_msg = MeterBand::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid meter band type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , MeterBand, all_meter_band_types)
    {
      auto const header = v13::protocol::ofp_meter_band_header{
          MeterBand::type(), MeterBand::min_length() - 1
        , random_rate(), random_burst_size()
      };

      auto const error_msg = MeterBand::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid meter band length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size
        , MeterBand, all_meter_band_types)
    {
      auto const header = v13::protocol::ofp_meter_band_header{
          MeterBand::type(), MeterBand::min_length() + 1
        , random_rate(), random_burst_size()
      };

      auto const error_msg = MeterBand::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid meter band length"_sr);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header
BOOST_AUTO_TEST_SUITE_END() // meter_band_test
