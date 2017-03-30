#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/instruction/clear_actions.hpp>
#include <boost/test/unit_test.hpp>

#include <vector>
#include <canard/network/openflow/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace instructions = v13::instructions;

namespace {

struct clear_actions_fixture
{
    instructions::clear_actions sut{};
    std::vector<unsigned char> binary = "\x00\x05\x00\x08\x00\x00\x00\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(instruction_test)
BOOST_AUTO_TEST_SUITE(clear_actions_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = instructions::clear_actions;

        BOOST_TEST(sut::type() == v13::protocol::OFPIT_CLEAR_ACTIONS);
    }

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const sut = instructions::clear_actions{};

        BOOST_TEST(sut.length() == 8);
    }

    BOOST_AUTO_TEST_CASE(create_test)
    {
        BOOST_CHECK_NO_THROW(instructions::clear_actions::create());
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = instructions::clear_actions{};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_equal)
      {
        BOOST_TEST(
            (instructions::clear_actions{} == instructions::clear_actions{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x05\x00\x08\x00\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const nonzero_pad = instructions::clear_actions::decode(it, binary.end());

        BOOST_TEST((instructions::clear_actions{} != nonzero_pad));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = instructions::clear_actions{};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_equal)
      {
        BOOST_TEST(
            equivalent(instructions::clear_actions{}, instructions::clear_actions{}));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x05\x00\x08\x12\x34\x56\x78"_bin;
        auto it = binary.begin();
        auto const nonzero_pad = instructions::clear_actions::decode(it, binary.end());

        BOOST_TEST(equivalent(instructions::clear_actions{}, nonzero_pad));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, clear_actions_fixture)
    {
        auto buffer = std::vector<unsigned char>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, clear_actions_fixture)
    {
        auto it = binary.begin();
        auto it_end = binary.end();

        auto const clear_actions
            = instructions::clear_actions::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((clear_actions == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // clear_actions_test
BOOST_AUTO_TEST_SUITE_END() // instruction_test
