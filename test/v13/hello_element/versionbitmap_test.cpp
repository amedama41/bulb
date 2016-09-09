#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/hello_element/versionbitmap.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <boost/range/algorithm/max_element.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace helems = v13::hello_elements;

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
      helems::versionbitmap const sut{std::vector<std::uint32_t>{0x00000012}};

      BOOST_TEST(sut.type() == proto::OFPHET_VERSIONBITMAP);
      BOOST_TEST(sut.length() == 8);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_multiple_bitmaps)
    {
      helems::versionbitmap const sut{{0x00000012, 0x00010001}};

      BOOST_TEST(sut.type() == proto::OFPHET_VERSIONBITMAP);
      BOOST_TEST(sut.length() == 12);
    }
    BOOST_AUTO_TEST_CASE(throws_if_all_bitmap_are_zero)
    {
      BOOST_CHECK_THROW(
            (helems::versionbitmap{{0x00000000, 0x00000000}})
          , std::runtime_error);
    }
    BOOST_AUTO_TEST_CASE(throws_if_no_bitmap)
    {
      BOOST_CHECK_THROW(helems::versionbitmap{{}}, std::runtime_error);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

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

      BOOST_TEST(versionbitmap.length() == sut.length());
      BOOST_TEST(
          versionbitmap.max_support_version() == sut.max_support_version());
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // versionbitmap
BOOST_AUTO_TEST_SUITE_END() // hello_element_test
