#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/hello_element/versionbitmap.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace helems = v13::hello_elements;
namespace detail = v13::v13_detail;

using proto = v13::protocol;

namespace bdata = boost::unit_test::data;

namespace {
constexpr std::uint8_t versions[] = { 1, 3, 4, 5, 8, 13, 64, 95, 96 };
template <std::size_t N>
auto create_bitmaps(std::uint8_t const (&versions)[N])
  -> std::vector<std::uint32_t>
{
  auto bitmaps = std::vector<std::uint32_t>{};
  for (auto const v : versions) {
    auto const index = v / 32U;
    auto const shift = v % 32U;
    bitmaps.resize(index + 1);
    bitmaps[index] |= std::uint32_t{1} << shift;
  }
  return bitmaps;
}
struct versionbitmap_fixture
{
  std::vector<std::uint32_t> bitmaps = create_bitmaps(versions);
  helems::versionbitmap sut{bitmaps};
  std::vector<unsigned char> bin
    = "\x00\x01\x00\x14"
      "\x00\x00\x21\x3a""\x00\x00\x00\x00"
      "\x80\x00\x00\x01""\x00\x00\x00\x01"
      "\x00\x00\x00\x00"
      ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(hello_element_test)
BOOST_AUTO_TEST_SUITE(versionbitmap)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_single_bitmap)
    {
      auto const bitmaps
        = helems::versionbitmap::bitmaps_type{ 0x00000012 };

      helems::versionbitmap const sut{bitmaps};

      BOOST_TEST(sut.length() == 8);
      BOOST_TEST(sut.bitmaps() == bitmaps, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_multiple_bitmaps)
    {
      auto const bitmaps
        = helems::versionbitmap::bitmaps_type{ 0x00000012, 0x00010001 };

      helems::versionbitmap const sut{bitmaps};

      BOOST_TEST(sut.length() == 12);
      BOOST_TEST(sut.bitmaps() == bitmaps, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, versionbitmap_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(
          moved.length() == sizeof(detail::ofp_hello_elem_versionbitmap));
      BOOST_TEST(moved.bitmaps().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = helems::versionbitmap{{ 1, 2, 3 }};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_bitmaps_are_equal)
    {
      auto const bitmaps = helems::versionbitmap::bitmaps_type{ 1, 2, 3 };

      BOOST_TEST(
          (helems::versionbitmap{bitmaps} == helems::versionbitmap{bitmaps}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_bitmap_is_not_equal)
    {
      BOOST_TEST(
          (helems::versionbitmap{{ 1, 2 }} != helems::versionbitmap{{ 1, 3 }}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(support)
    BOOST_DATA_TEST_CASE(
          returns_true_if_corresponding_bit_is_one
        , bdata::make(versions)
        , version)
    {
      auto const sut = helems::versionbitmap{create_bitmaps(versions)};

      BOOST_TEST(sut.support(version));
    }
    BOOST_DATA_TEST_CASE(
          returns_false_if_corresponding_bit_is_zero
        , bdata::make(versions)
        , version)
    {
      auto bitmaps = create_bitmaps(versions);
      for (auto& b : bitmaps) {
        b = ~b;
      }
      auto const sut = helems::versionbitmap{bitmaps};

      BOOST_TEST(!sut.support(version));
    }
  BOOST_AUTO_TEST_SUITE_END() // support

  BOOST_AUTO_TEST_SUITE(max_support_version)
    BOOST_FIXTURE_TEST_CASE(returns_max_version, versionbitmap_fixture)
    {
      auto const max_version = *boost::max_element(versions);

      BOOST_TEST(sut.max_support_version() == max_version);
    }
    BOOST_AUTO_TEST_CASE(returns_max_version_when_single_bitmap)
    {
      auto bitmaps = create_bitmaps(versions);
      bitmaps.resize(1);
      auto const sut = helems::versionbitmap{bitmaps};
      using boost::adaptors::filtered;
      auto const max_version = *boost::max_element(
          versions | filtered(
            (bool(*)(std::uint8_t))([](std::uint8_t v) { return v < 32; })));

      BOOST_TEST(sut.max_support_version() == max_version);
    }
    BOOST_AUTO_TEST_CASE(throws_exception_if_all_bitmaps_are_zero)
    {
      auto const sut = helems::versionbitmap{{0, 0, 0}};

      BOOST_CHECK_THROW(sut.max_support_version(), std::runtime_error);
    }
  BOOST_AUTO_TEST_SUITE_END() // max_support_version

  BOOST_FIXTURE_TEST_SUITE(encode, versionbitmap_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == (sut.length() + 7) / 8 * 8);
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, versionbitmap_fixture)
    BOOST_AUTO_TEST_CASE(constructs_versionbitmap_from_binary)
    {
      auto it = bin.begin();

      auto const versionbitmap = helems::versionbitmap::decode(it, bin.end());

      BOOST_TEST((versionbitmap == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // versionbitmap
BOOST_AUTO_TEST_SUITE_END() // hello_element_test
