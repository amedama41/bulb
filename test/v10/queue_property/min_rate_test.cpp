#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/queue_property/min_rate.hpp>
#include <boost/test/unit_test.hpp>

#include <canard/network/openflow/v10/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace queue_properties = ofp::v10::queue_properties;
namespace protocol = ofp::v10::protocol;

namespace {

struct binary_min_rate {
  queue_properties::min_rate sut{0x1234};
  std::vector<unsigned char> bin
    = "\x00\x01\x00\x10\x00\x00\x00\x00""\x12\x34\x00\x00\x00\x00\x00\x00"_bin;
  std::vector<unsigned char> non_zero_padding_bin
    = "\x00\x01\x00\x10\x01\x23\x45\x67""\x12\x34\x01\x23\x45\x67\x89\xab"_bin;
};

}

BOOST_AUTO_TEST_SUITE(queue_property_test)
BOOST_AUTO_TEST_SUITE(min_rate_test)

  BOOST_AUTO_TEST_CASE(constructible_from_rate_parameter)
  {
    auto const rate = std::uint16_t{12};

    auto const sut = queue_properties::min_rate{rate};

    BOOST_TEST(sut.property() == protocol::OFPQT_MIN_RATE);
    BOOST_TEST(sut.length() == sizeof(protocol::ofp_queue_prop_min_rate));
    BOOST_TEST(sut.rate() == sut.rate());
  }

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = queue_properties::min_rate{300};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_rate_is_equal)
    {
      auto const rate = std::uint16_t{1000};

      BOOST_TEST(
          (queue_properties::min_rate{rate}
        == queue_properties::min_rate{rate}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rate_is_not_equal)
    {
      BOOST_TEST(
          (queue_properties::min_rate{0} != queue_properties::min_rate{1}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rate_is_not_equal_and_both_are_over_1000)
    {
      BOOST_TEST(
          (queue_properties::min_rate{1001}
        != queue_properties::min_rate{1002}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_lhs_rate_is_over_1000)
    {
      BOOST_TEST(
          (queue_properties::min_rate{1000}
        != queue_properties::min_rate{1001}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rhs_rate_is_over_1000)
    {
      BOOST_TEST(
          (queue_properties::min_rate{1001}
        != queue_properties::min_rate{1000}));
    }
    BOOST_FIXTURE_TEST_CASE(is_false_if_padding_is_not_equal, binary_min_rate)
    {
      bin.back() = 0x01;
      auto it = bin.begin();
      auto const non_zero_padding
        = queue_properties::min_rate::decode(it, bin.end());

      BOOST_TEST((sut != non_zero_padding));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(function_equivalent)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = queue_properties::min_rate{300};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_rate_is_equal)
    {
      auto const rate = std::uint16_t{1000};

      BOOST_TEST(
          equivalent(
              queue_properties::min_rate{rate}
            , queue_properties::min_rate{rate}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rate_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
            queue_properties::min_rate{0}, queue_properties::min_rate{1}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_rate_is_not_equal_but_both_are_over_1000)
    {
      BOOST_TEST(
          equivalent(
              queue_properties::min_rate{1001}
            , queue_properties::min_rate{1002}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_lhs_rate_is_over_1000)
    {
      BOOST_TEST(
          !equivalent(
              queue_properties::min_rate{1000}
            , queue_properties::min_rate{1001}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_rhs_rate_is_over_1000)
    {
      BOOST_TEST(
          !equivalent(
              queue_properties::min_rate{1001}
            , queue_properties::min_rate{1000}));
    }
    BOOST_FIXTURE_TEST_CASE(is_true_if_padding_is_not_equal, binary_min_rate)
    {
      bin.back() = 0x01;
      auto it = bin.begin();
      auto const non_zero_padding
        = queue_properties::min_rate::decode(it, bin.end());

      BOOST_TEST(equivalent(sut, non_zero_padding));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

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
      BOOST_TEST((min_rate == sut));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_non_zero_padding_binary)
    {
      auto it = non_zero_padding_bin.begin();

      auto const min_rate
        = queue_properties::min_rate::decode(it, non_zero_padding_bin.end());

      BOOST_TEST((it == non_zero_padding_bin.end()));
      BOOST_TEST((min_rate != sut));
      BOOST_TEST(equivalent(min_rate, sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // min_rate_test
BOOST_AUTO_TEST_SUITE_END() // queue_property_test
