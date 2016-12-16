#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/message/meter_mod.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"
#include <iostream>
#include <boost/format.hpp>

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace msg = v13::messages;

namespace {
  using v13::protocol;

  struct meter_delete_fixture
  {
    std::uint32_t meter_id = 0x01020304;
    std::uint32_t xid = 0x1a2b3c4d;
    msg::meter_delete sut{meter_id, xid};
    std::vector<unsigned char> bin
      = "\x04\x1d\x00\x10\x1a\x2b\x3c\x4d""\x00\x01\x00\x00\x01\x02\x03\x04"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(meter_delete)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_with_xid)
    {
      auto const meter_id = protocol::OFPM_ALL;
      auto const xid = std::uint32_t{321};

      msg::meter_delete const sut{meter_id, xid};

      BOOST_TEST(sut.length() == sizeof(v13::v13_detail::ofp_meter_mod));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.meter_id() == meter_id);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.bands().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_without_xid)
    {
      auto const meter_id = protocol::OFPM_MAX;

      msg::meter_delete const sut{meter_id};

      BOOST_TEST(sut.length() == sizeof(v13::v13_detail::ofp_meter_mod));
      BOOST_TEST(sut.meter_id() == meter_id);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.bands().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(function_equivalent)
    BOOST_AUTO_TEST_CASE(is_true_if_meter_id_is_equal)
    {
      auto const meter_id = 1;
      auto const xid = 3;

      BOOST_TEST(
          equivalent(
              msg::meter_delete{meter_id, xid}
            , msg::meter_delete{meter_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_meter_id_is_not_equal)
    {
      auto const xid = 3;

      BOOST_TEST(
          !equivalent(
            msg::meter_delete{1, xid}, msg::meter_delete{2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      auto const meter_id = 1;

      BOOST_TEST(
          !equivalent(
            msg::meter_delete{meter_id, 3}, msg::meter_delete{meter_id, 4}));
    }
    BOOST_FIXTURE_TEST_CASE(is_true_if_flags_is_not_equal, meter_delete_fixture)
    {
      bin[11] = protocol::OFPMF_PKTPS;
      auto it = bin.begin();
      auto const has_flags = msg::meter_delete::decode(it, bin.end());

      BOOST_TEST(equivalent(msg::meter_delete{meter_id, xid}, has_flags));
    }
    BOOST_FIXTURE_TEST_CASE(is_true_if_bands_is_not_equal, meter_delete_fixture)
    {
      auto const meter_band
        = "\x00\x01\x00\x10\x00\x01\x00\x01""\x55\x66\x77\x88\x00\x00\x00\x00"
          ""_bin;
      bin.insert(bin.end(), meter_band.begin(), meter_band.end());
      bin[3] = bin.size();
      auto it = bin.begin();
      auto const has_bands = msg::meter_delete::decode(it, bin.end());

      BOOST_TEST(equivalent(msg::meter_delete{meter_id, xid}, has_bands));
    }
  BOOST_AUTO_TEST_SUITE_END() // equivalent

BOOST_AUTO_TEST_SUITE_END() // meter_delete
BOOST_AUTO_TEST_SUITE_END() // message_test
