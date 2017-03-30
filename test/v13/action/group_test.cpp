#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/action/group.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <vector>
#include <canard/network/openflow/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace actions = v13::actions;
namespace protocol = v13::protocol;

namespace bdata = boost::unit_test::data;

namespace {

struct group_fixture
{
    actions::group sut{0x12345678};
    std::vector<std::uint8_t> binary = "\x00\x16\x00\x08\x12\x34\x56\x78"_bin;
};

}

BOOST_AUTO_TEST_SUITE(action_test)
BOOST_AUTO_TEST_SUITE(group_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = actions::group;

        BOOST_TEST(sut::type() == protocol::OFPAT_GROUP);
        BOOST_TEST(sut::length() == sizeof(protocol::ofp_action_group));
    }

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const group_id = std::uint32_t{1};

        auto const sut = actions::group{group_id};

        BOOST_TEST(sut.group_id() == group_id);
    }

    BOOST_DATA_TEST_CASE(
            create_success_test
            , bdata::make(std::vector<std::uint32_t>{0, protocol::OFPG_MAX})
            , group_id)
    {
        auto const sut = actions::group::create(group_id);

        BOOST_TEST(sut.group_id() == group_id);
    }

    BOOST_DATA_TEST_CASE(
            create_failure_test
            , bdata::make(std::vector<std::uint32_t>{
                protocol::OFPG_MAX + 1, protocol::OFPG_ALL, protocol::OFPG_ANY
              })
            , group_id)
    {
        BOOST_CHECK_THROW(actions::group::create(group_id), std::runtime_error);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::group{protocol::OFPG_MAX};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_group_id_is_equal)
      {
        auto const sut1 = actions::group{1};
        auto const sut2 = actions::group{1};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_group_id_not_equal)
      {
        auto const sut1 = actions::group{protocol::OFPG_MAX};
        auto const sut2 = actions::group{0};

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::group{protocol::OFPG_MAX};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_group_id_is_equal)
      {
        auto const sut1 = actions::group{1};
        auto const sut2 = actions::group{1};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_group_id_not_equal)
      {
        auto const sut1 = actions::group{protocol::OFPG_MAX};
        auto const sut2 = actions::group{0};

        BOOST_TEST(!equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, group_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, group_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const action = actions::group::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((action == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // group_test
BOOST_AUTO_TEST_SUITE_END() // action_test
