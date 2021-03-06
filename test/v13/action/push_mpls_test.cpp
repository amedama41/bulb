#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/action/push_mpls.hpp>
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

struct push_mpls_fixture
{
    actions::push_mpls sut{0x8847};
    std::vector<std::uint8_t> binary = "\x00\x13\x00\x08\x88\x47\x00\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(action_test)
BOOST_AUTO_TEST_SUITE(push_mpls_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = actions::push_mpls;

        BOOST_TEST(sut::type() == protocol::OFPAT_PUSH_MPLS);
    }

    BOOST_DATA_TEST_CASE(
              construct_test
            , bdata::make(std::vector<std::uint16_t>{0x8847, 0x8848})
            , ethertype)
    {
        auto const sut = actions::push_mpls{ethertype};

        BOOST_TEST(sut.length() == sizeof(protocol::ofp_action_push));
        BOOST_TEST(sut.ethertype() == ethertype);
    }

    BOOST_AUTO_TEST_CASE(unicast_test)
    {
        auto const sut = actions::push_mpls::unicast();

        BOOST_TEST(sut.ethertype() == 0x8847);
    }

    BOOST_AUTO_TEST_CASE(multicast)
    {
        auto const sut = actions::push_mpls::multicast();

        BOOST_TEST(sut.ethertype() == 0x8848);
    }

    BOOST_DATA_TEST_CASE(
              create_success_test
            , bdata::make(std::vector<std::uint16_t>{0x8847, 0x8848})
            , ethertype)
    {
        auto const sut = actions::push_mpls::create(ethertype);

        BOOST_TEST(sut.ethertype() == ethertype);
    }

    BOOST_DATA_TEST_CASE(
              create_failure_test
            , bdata::make(std::vector<std::uint16_t>{0x8846, 0x8849})
            , ethertype)
    {
        BOOST_CHECK_THROW(
                actions::push_mpls::create(ethertype), std::runtime_error);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::push_mpls::unicast();

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_ethertype_and_pad_are_equal)
      {
        auto const sut1 = actions::push_mpls::multicast();
        auto const sut2 = actions::push_mpls{0x8848};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_ethertype_is_not_equal)
      {
        auto const sut1 = actions::push_mpls::unicast();
        auto const sut2 = actions::push_mpls::multicast();

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x13\x00\x08\x88\x47\x00\x01"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::push_mpls{0x8847};
        auto const sut2 = actions::push_mpls::decode(it, binary.end());

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::push_mpls::unicast();

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_ethertype_and_pad_are_equal)
      {
        auto const sut1 = actions::push_mpls::multicast();
        auto const sut2 = actions::push_mpls{0x8848};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_ethertype_is_not_equal)
      {
        auto const sut1 = actions::push_mpls::unicast();
        auto const sut2 = actions::push_mpls::multicast();

        BOOST_TEST(!equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x13\x00\x08\x88\x47\xab\xcd"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::push_mpls{0x8847};
        auto const sut2 = actions::push_mpls::decode(it, binary.end());

        BOOST_TEST(equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, push_mpls_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, push_mpls_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const action = actions::push_mpls::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((action == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // push_mpls_test
BOOST_AUTO_TEST_SUITE_END() // action_test
