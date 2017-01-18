#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/action/set_nw_ttl.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <vector>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace actions = v13::actions;
namespace protocol = v13::protocol;

namespace bdata = boost::unit_test::data;

namespace {

struct set_nw_ttl_fixture
{
    actions::set_nw_ttl sut{0x12};
    std::vector<std::uint8_t> binary = "\x00\x17\x00\x08\x12\x00\x00\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(action_test)
BOOST_AUTO_TEST_SUITE(set_nw_ttl_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = actions::set_nw_ttl;

        BOOST_TEST(sut::type() == protocol::OFPAT_SET_NW_TTL);
        BOOST_TEST(sut::length() == sizeof(protocol::ofp_action_nw_ttl));
    }

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const ttl = std::uint8_t{1};

        auto const sut = actions::set_nw_ttl{ttl};

        BOOST_TEST(sut.ttl() == ttl);
    }

    BOOST_DATA_TEST_CASE(
              create_test
            , bdata::make(std::vector<std::uint8_t>{0x00, 0xff})
            , ttl)
    {
        auto const sut = actions::set_nw_ttl::create(ttl);

        BOOST_TEST(sut.ttl() == ttl);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::set_nw_ttl{0};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_nw_ttl_and_pad_are_equal)
      {
        auto const sut1 = actions::set_nw_ttl{1};
        auto const sut2 = actions::set_nw_ttl{1};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_nw_ttl_is_not_equal)
      {
        auto const sut1 = actions::set_nw_ttl{2};
        auto const sut2 = actions::set_nw_ttl{3};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x17\x00\x08\x12\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::set_nw_ttl{0x12};
        auto const sut2 = actions::set_nw_ttl::decode(it, binary.end());

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::set_nw_ttl{0};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_nw_ttl_and_pad_are_equal)
      {
        auto const sut1 = actions::set_nw_ttl{1};
        auto const sut2 = actions::set_nw_ttl{1};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_nw_ttl_is_not_equal)
      {
        auto const sut1 = actions::set_nw_ttl{2};
        auto const sut2 = actions::set_nw_ttl{3};

        BOOST_TEST(!equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x17\x00\x08\x12\xab\xcd\xef"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::set_nw_ttl{0x12};
        auto const sut2 = actions::set_nw_ttl::decode(it, binary.end());

        BOOST_TEST(equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, set_nw_ttl_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, set_nw_ttl_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const action = actions::set_nw_ttl::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((action == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // set_nw_ttl_test
BOOST_AUTO_TEST_SUITE_END() // action_test
