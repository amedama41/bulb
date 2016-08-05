#define BOOST_TEST_DYN_LINK
#include <canard/network/protocol/openflow/v13/queue_property/min_rate.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>

namespace of = canard::network::openflow;
namespace v13 = of::v13;
namespace v13_detail = v13::v13_detail;
namespace queue_props = v13::queue_properties;
using proto = v13::protocol;

namespace {

auto operator ""_bin(char const* const str, std::size_t const size)
    -> std::vector<std::uint8_t>
{
    return std::vector<std::uint8_t>(str, str + size);
}

struct min_rate_fixture
{
    queue_props::min_rate sut{0x1234};
    std::vector<std::uint8_t> binary
        = "\x00\x01\x00\x10\x00\x00\x00\x00""\x12\x34\x00\x00\x00\x00\x00\x00"
          ""_bin
        ;
};

}

BOOST_AUTO_TEST_SUITE(queue_property_test)

BOOST_AUTO_TEST_SUITE(min_rate_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const rate = std::uint16_t{34};

        auto const sut = queue_props::min_rate{rate};

        BOOST_TEST(sut.property() == proto::OFPQT_MIN_RATE);
        BOOST_TEST(sut.length() == sizeof(v13_detail::ofp_queue_prop_min_rate));
        BOOST_TEST(sut.rate() == rate);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = queue_props::min_rate{1};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_rate_is_equal)
      {
        BOOST_TEST((queue_props::min_rate{2} == queue_props::min_rate{2}));
      }
      BOOST_AUTO_TEST_CASE(false_if_rate_is_not_equal)
      {
        BOOST_TEST((queue_props::min_rate{3} != queue_props::min_rate{4}));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary
          = "\x00\x01\x00\x10\x00\x00\x00\x00""\x02\x34\x00\x00\x00\x00\x00\x01"
            ""_bin;
        auto it = binary.begin();
        auto const nonzero_pad
          = queue_props::min_rate::decode(it, binary.end());

        BOOST_TEST((queue_props::min_rate{0x234} != nonzero_pad));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = queue_props::min_rate{1};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_rate_is_equal)
      {
        BOOST_TEST(
            equivalent(queue_props::min_rate{2}, queue_props::min_rate{2}));
      }
      BOOST_AUTO_TEST_CASE(false_if_rate_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(queue_props::min_rate{3}, queue_props::min_rate{4}));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary
          = "\x00\x01\x00\x10\x00\x00\x00\x00""\x02\x34\x00\x00\x00\x00\x00\x01"
            ""_bin;
        auto it = binary.begin();
        auto const nonzero_pad
          = queue_props::min_rate::decode(it, binary.end());

        BOOST_TEST(equivalent(queue_props::min_rate{0x234}, nonzero_pad));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, min_rate_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, min_rate_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const min_rate = queue_props::min_rate::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((min_rate == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // min_rate_test

BOOST_AUTO_TEST_SUITE_END() // queue_property_test