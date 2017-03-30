#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/action/strip_vlan.hpp>
#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>
#include <canard/network/openflow/v10/io/openflow.hpp>

namespace of = canard::net::ofp;
namespace v10 = of::v10;
namespace actions = v10::actions;

namespace {

auto operator""_bin(char const* const str, std::size_t const size)
    -> std::vector<std::uint8_t>
{
    return std::vector<std::uint8_t>(str, str + size);
}

struct strip_vlan_fixture
{
    actions::strip_vlan sut{};
    std::vector<std::uint8_t> binary
        = "\x00\x03\x00\x08\x00\x00\x00\x00"_bin;
    std::vector<std::uint8_t> non_zero_padding_binary
        = "\x00\x03\x00\x08\x00\x00\x00\x01"_bin;
};

}

BOOST_AUTO_TEST_SUITE(actions_test)
BOOST_AUTO_TEST_SUITE(strip_vlan_test)

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const sut = actions::strip_vlan{};

        BOOST_TEST(sut.type() == v10::protocol::OFPAT_STRIP_VLAN);
        BOOST_TEST(sut.length() == sizeof(v10::protocol::ofp_action_header));
    }

    BOOST_AUTO_TEST_CASE(create_test)
    {
        auto const sut = actions::strip_vlan::create();

        BOOST_TEST(sut.type() == v10::protocol::OFPAT_STRIP_VLAN);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::strip_vlan{};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_object_value_is_equal)
      {
        BOOST_TEST((actions::strip_vlan{} == actions::strip_vlan{}));
      }
      BOOST_FIXTURE_TEST_CASE(
          is_false_if_padding_is_not_equal, strip_vlan_fixture)
      {
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::strip_vlan::decode(it, non_zero_padding_binary.end());

        BOOST_TEST((actions::strip_vlan{} != non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
      {
        auto const sut = actions::strip_vlan{};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(is_true_if_object_value_is_equal)
      {
        BOOST_TEST(equivalent(actions::strip_vlan{}, actions::strip_vlan{}));
      }
      BOOST_FIXTURE_TEST_CASE(
          is_true_if_padding_is_not_equal, strip_vlan_fixture)
      {
        auto it = non_zero_padding_binary.begin();
        auto const non_zero_padding
          = actions::strip_vlan::decode(it, non_zero_padding_binary.end());

        BOOST_TEST(equivalent(actions::strip_vlan{}, non_zero_padding));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, strip_vlan_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, strip_vlan_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const strip_vlan = actions::strip_vlan::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((strip_vlan == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // strip_vlan_test
BOOST_AUTO_TEST_SUITE_END() // actions_test


