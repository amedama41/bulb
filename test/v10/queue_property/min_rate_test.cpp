#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/queue_property/min_rate.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace queue_properties = ofp::v10::queue_properties;
namespace v10_detail = ofp::v10::v10_detail;

namespace {

struct binary_min_rate {
  queue_properties::min_rate const sut{0x1234};
  std::vector<unsigned char> const bin
    = "\x00\x01\x00\x10\x00\x00\x00\x00""\x12\x34\x00\x00\x00\x00\x00\x00"_bin;
  std::vector<unsigned char> const non_zero_padding_bin
    = "\x00\x01\x00\x10\x01\x23\x45\x67""\x12\x34\x01\x23\x45\x67\x89\xab"_bin;
};

}

BOOST_AUTO_TEST_SUITE(queue_property_test)
BOOST_AUTO_TEST_SUITE(min_rate_test)

  BOOST_AUTO_TEST_CASE(constructible_from_rate_parameter)
  {
    auto const rate = std::uint16_t{12};

    auto const sut = queue_properties::min_rate{rate};

    BOOST_TEST(sut.property() == ofp::v10::protocol::OFPQT_MIN_RATE);
    BOOST_TEST(sut.length() == sizeof(v10_detail::ofp_queue_prop_min_rate));
    BOOST_TEST(sut.rate() == sut.rate());
  }

  BOOST_FIXTURE_TEST_SUITE(encode, binary_min_rate)
    BOOST_AUTO_TEST_CASE(generate_binary)
    {
      auto buf = std::vector<unsigned char>();

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, binary_min_rate)
    BOOST_AUTO_TEST_CASE(constructible_from_binary)
    {
      auto it = bin.begin();

      auto const min_rate = queue_properties::min_rate::decode(it, bin.end());
      BOOST_TEST((it == bin.end()));
      // TODO BOOST_TEST((min_rate == sut));
      BOOST_TEST(min_rate.rate() == sut.rate());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_non_zero_padding_binary)
    {
      auto it = non_zero_padding_bin.begin();

      auto const min_rate
        = queue_properties::min_rate::decode(it, non_zero_padding_bin.end());

      BOOST_TEST((it == non_zero_padding_bin.end()));
      // TODO BOOST_TEST((min_rate != sut));
      // TODO BOOST_TEST(equivalnet(min_rate, sut));
      BOOST_TEST(min_rate.rate() == sut.rate());
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // min_rate_test
BOOST_AUTO_TEST_SUITE_END() // queue_property_test
