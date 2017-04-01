#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/any_meter_band.hpp>
#include <boost/test/unit_test.hpp>

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace bands = v13::meter_bands;

BOOST_AUTO_TEST_SUITE(utility_test)
  BOOST_AUTO_TEST_SUITE(any_meter_band)

    BOOST_AUTO_TEST_SUITE(constructor)
      BOOST_AUTO_TEST_CASE(is_direct_initialization)
      {
        auto const drop = bands::drop{1, 2};

        v13::any_meter_band const sut{drop};

        BOOST_TEST(sut.rate() == drop.rate());
        BOOST_TEST(sut.burst_size() == drop.burst_size());
      }
      BOOST_AUTO_TEST_CASE(is_copy_initialization)
      {
        auto const dscp_remark = bands::dscp_remark{1, 2, 3};

        v13::any_meter_band const sut = dscp_remark;

        BOOST_TEST(sut.rate() == dscp_remark.rate());
        BOOST_TEST(sut.burst_size() == dscp_remark.burst_size());
      }
    BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE_END() // any_meter_band
BOOST_AUTO_TEST_SUITE_END() // utility_test
