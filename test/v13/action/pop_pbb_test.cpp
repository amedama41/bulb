#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/action/pop_pbb.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace actions = v13::actions;
namespace protocol = v13::protocol;

namespace bdata = boost::unit_test::data;

namespace {

struct pop_pbb_fixture
{
    actions::pop_pbb sut{};
    std::vector<std::uint8_t> binary = "\x00\x1b\x00\x08\x00\x00\x00\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(action_test)
BOOST_AUTO_TEST_SUITE(pop_pbb_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = actions::pop_pbb;

        BOOST_TEST(sut::type() == protocol::OFPAT_POP_PBB);
    }

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const sut = actions::pop_pbb{};

        BOOST_TEST(sut.length() == sizeof(protocol::ofp_action_header));
    }

    BOOST_AUTO_TEST_CASE(create_test)
    {
        auto const sut = actions::pop_pbb::create();

        BOOST_TEST(sut.length() == sizeof(protocol::ofp_action_header));
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::pop_pbb{};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_equal)
      {
        auto const sut1 = actions::pop_pbb{};
        auto const sut2 = actions::pop_pbb{};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x1b\x00\x08\x00\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::pop_pbb{};
        auto const sut2 = actions::pop_pbb::decode(it, binary.end());

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::pop_pbb{};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_equal)
      {
        auto const sut1 = actions::pop_pbb{};
        auto const sut2 = actions::pop_pbb{};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x1b\x00\x08\x01\x02\x03\x04"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::pop_pbb{};
        auto const sut2 = actions::pop_pbb::decode(it, binary.end());

        BOOST_TEST(equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, pop_pbb_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, pop_pbb_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const action = actions::pop_pbb::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((action == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // pop_pbb_test
BOOST_AUTO_TEST_SUITE_END() // action_test
