#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/action/set_queue.hpp>
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

struct set_queue_fixture
{
    actions::set_queue sut{0x12345678};
    std::vector<std::uint8_t> binary = "\x00\x15\x00\x08\x12\x34\x56\x78"_bin;
};

}

BOOST_AUTO_TEST_SUITE(action_test)
BOOST_AUTO_TEST_SUITE(set_queue_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = actions::set_queue;

        BOOST_TEST(sut::type() == protocol::OFPAT_SET_QUEUE);
    }

    BOOST_AUTO_TEST_CASE(constructor_test)
    {
        auto const queue_id = std::uint32_t{1};

        auto const sut = actions::set_queue{queue_id};

        BOOST_TEST(sut.length() == sizeof(protocol::ofp_action_set_queue));
        BOOST_TEST(sut.queue_id() == queue_id);
    }

    BOOST_DATA_TEST_CASE(
              create_success_test
            , bdata::make(std::vector<std::uint32_t>{0, protocol::OFPQ_ALL - 1})
            , queue_id)
    {
        auto const sut = actions::set_queue::create(queue_id);

        BOOST_TEST(sut.queue_id() == queue_id);
    }

    BOOST_DATA_TEST_CASE(
              create_failure_test
            , bdata::make(protocol::OFPQ_ALL)
            , queue_id)
    {
        BOOST_CHECK_THROW(
                actions::set_queue::create(queue_id), std::runtime_error);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::set_queue{0};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_queue_id_is_equal)
      {
        auto const sut1 = actions::set_queue{1};
        auto const sut2 = actions::set_queue{1};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_queue_id_is_not_equal)
      {
        auto const sut1 = actions::set_queue{2};
        auto const sut2 = actions::set_queue{3};

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::set_queue{0};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_queue_id_is_equal)
      {
        auto const sut1 = actions::set_queue{1};
        auto const sut2 = actions::set_queue{1};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_queue_id_is_not_equal)
      {
        auto const sut1 = actions::set_queue{2};
        auto const sut2 = actions::set_queue{3};

        BOOST_TEST(!equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, set_queue_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, set_queue_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const action = actions::set_queue::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((action == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // set_queue_test
BOOST_AUTO_TEST_SUITE_END() // action_test
