#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/meter_band/drop.hpp>
#include <boost/test/unit_test.hpp>
#include <utility>

#include <canard/net/ofp/v13/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace bands = v13::meter_bands;

namespace {
  struct drop_fixture
  {
    bands::drop sut{0x01020304, 0x05060708};
    std::vector<unsigned char> bin
      = "\x00\x01\x00\x10\x01\x02\x03\x04""\x05\x06\x07\x08\x00\x00\x00\x00"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(meter_band_test)
BOOST_AUTO_TEST_SUITE(drop)
  BOOST_AUTO_TEST_SUITE(type_definition_test)
    BOOST_AUTO_TEST_CASE(type)
    {
      using sut = bands::drop;

      constexpr auto type = sut::type();

      BOOST_TEST(type == v13::protocol::OFPMBT_DROP);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition_test

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_rate)
    {
      auto const rate = std::uint32_t{3};

      bands::drop const sut{rate};

      BOOST_TEST(sut.length() == sizeof(v13::protocol::ofp_meter_band_drop));
      BOOST_TEST(sut.rate() == rate);
      BOOST_TEST(sut.burst_size() == 0);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_rate_and_burst_size)
    {
      auto const rate = std::uint32_t{1};
      auto const burst_size = std::uint32_t{322};

      bands::drop const sut{rate, burst_size};

      BOOST_TEST(sut.rate() == rate);
      BOOST_TEST(sut.burst_size() == burst_size);
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, drop_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, drop_fixture)
    {
      auto const& const_drop = sut;

      auto const copy = const_drop;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, drop_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, drop_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, drop_fixture)
    {
      auto it = bin.begin();

      auto const drop = bands::drop::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((drop == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // drop
BOOST_AUTO_TEST_SUITE_END() // meter_band_test
