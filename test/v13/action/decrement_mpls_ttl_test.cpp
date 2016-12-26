#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/action/decrement_mpls_ttl.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <vector>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace actions = v13::actions;
namespace v13_detail = v13::v13_detail;

namespace protocol = v13::protocol;

namespace bdata = boost::unit_test::data;

namespace {

struct decrement_mpls_ttl_fixture
{
    actions::decrement_mpls_ttl sut{};
    std::vector<std::uint8_t> binary = "\x00\x10\x00\x08\x00\x00\x00\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(action_test)
BOOST_AUTO_TEST_SUITE(decrement_mpls_ttl_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = actions::decrement_mpls_ttl;

        BOOST_TEST(sut::type() == protocol::OFPAT_DEC_MPLS_TTL);
        BOOST_TEST(sut::length() == sizeof(v13_detail::ofp_action_header));
    }

    BOOST_AUTO_TEST_CASE(constructor_test)
    {
        auto const sut = actions::decrement_mpls_ttl{};

        BOOST_TEST(((void)sut, true));
    }

    BOOST_AUTO_TEST_CASE(create_test)
    {
        auto const sut = actions::decrement_mpls_ttl::create();

        BOOST_TEST(((void)sut, true));
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::decrement_mpls_ttl{};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_equal)
      {
        auto const sut1 = actions::decrement_mpls_ttl{};
        auto const sut2 = actions::decrement_mpls_ttl{};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x10\x00\x08\x00\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::decrement_mpls_ttl{};
        auto const sut2 = actions::decrement_mpls_ttl::decode(it, binary.end());

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::decrement_mpls_ttl{};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_equal)
      {
        auto const sut1 = actions::decrement_mpls_ttl{};
        auto const sut2 = actions::decrement_mpls_ttl{};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x10\x00\x08\x12\x34\x57\x78"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::decrement_mpls_ttl{};
        auto const sut2 = actions::decrement_mpls_ttl::decode(it, binary.end());

        BOOST_TEST(equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, decrement_mpls_ttl_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, decrement_mpls_ttl_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const action = actions::decrement_mpls_ttl::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((action == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // decrement_mpls_ttl_test
BOOST_AUTO_TEST_SUITE_END() // action_test
