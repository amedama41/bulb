#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/meter_band/dscp_remark.hpp>
#include <boost/test/unit_test.hpp>
#include <utility>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace bands = v13::meter_bands;

namespace {
  struct dscp_remark_fixture
  {
    bands::dscp_remark sut{0xab, 0x01020304, 0x05060708};
    std::vector<unsigned char> bin
      = "\x00\x02\x00\x10\x01\x02\x03\x04""\x05\x06\x07\x08\xab\x00\x00\x00"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(meter_band_test)
BOOST_AUTO_TEST_SUITE(dscp_remark)
  BOOST_AUTO_TEST_SUITE(type_definition_test)
    BOOST_AUTO_TEST_CASE(type)
    {
      using sut = bands::dscp_remark;

      constexpr auto type = sut::type();

      BOOST_TEST(type == v13::protocol::OFPMBT_DSCP_REMARK);
    }
    BOOST_AUTO_TEST_CASE(length)
    {
      using sut = bands::dscp_remark;

      constexpr auto length = sut::length();

      BOOST_TEST(length == sizeof(v13::v13_detail::ofp_meter_band_dscp_remark));
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition_test

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_prec_level_and_rate)
    {
      auto const prec_level = std::uint8_t{1};
      auto const rate = std::uint32_t{3};

      bands::dscp_remark const sut{prec_level, rate};

      BOOST_TEST(sut.precedence_level() == prec_level);
      BOOST_TEST(sut.rate() == rate);
      BOOST_TEST(sut.burst_size() == 0);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_rate_and_burst_size)
    {
      auto const prec_level = std::uint8_t{2};
      auto const rate = std::uint32_t{1};
      auto const burst_size = std::uint32_t{322};

      bands::dscp_remark const sut{prec_level, rate, burst_size};

      BOOST_TEST(sut.precedence_level() == prec_level);
      BOOST_TEST(sut.rate() == rate);
      BOOST_TEST(sut.burst_size() == burst_size);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, dscp_remark_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, dscp_remark_fixture)
    {
      auto const& const_dscp_remark = sut;

      auto const copy = const_dscp_remark;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, dscp_remark_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, dscp_remark_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, dscp_remark_fixture)
    {
      auto it = bin.begin();

      auto const dscp_remark = bands::dscp_remark::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((dscp_remark == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // dscp_remark
BOOST_AUTO_TEST_SUITE_END() // meter_band_test
