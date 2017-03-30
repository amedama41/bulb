#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/action/pop_mpls.hpp>
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

struct pop_mpls_fixture
{
    actions::pop_mpls sut{0x0800};
    std::vector<std::uint8_t> binary = "\x00\x14\x00\x08\x08\x00\x00\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(action_test)
BOOST_AUTO_TEST_SUITE(pop_mpls_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = actions::pop_mpls;

        BOOST_TEST(sut::type() == protocol::OFPAT_POP_MPLS);
        BOOST_TEST(sut::length() == sizeof(protocol::ofp_action_pop_mpls));
    }

    BOOST_AUTO_TEST_CASE(construct_test)
    {
        auto const ethertype = std::uint16_t{0x8847};

        auto const sut = actions::pop_mpls{ethertype};

        BOOST_TEST(sut.ethertype() == ethertype);
    }

    BOOST_AUTO_TEST_CASE(ipv4_test)
    {
        auto const sut = actions::pop_mpls::ipv4();

        BOOST_TEST(sut.ethertype() == 0x0800);
    }

    BOOST_AUTO_TEST_CASE(unicast_test)
    {
        auto const sut = actions::pop_mpls::unicast();

        BOOST_TEST(sut.ethertype() == 0x8847);
    }

    BOOST_AUTO_TEST_CASE(multicast_test)
    {
        auto const sut = actions::pop_mpls::multicast();

        BOOST_TEST(sut.ethertype() == 0x8848);
    }

    BOOST_DATA_TEST_CASE(
            create_test
            , bdata::make(std::vector<std::uint16_t>{0x0800, 0x8847, 0x8848})
            , ethertype)
    {
        auto const sut = actions::pop_mpls::create(ethertype);

        BOOST_TEST(sut.ethertype() == ethertype);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::pop_mpls{0x0800};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_ethertype_and_pad_are_equal)
      {
        auto const sut1 = actions::pop_mpls{0x8847};
        auto const sut2 = actions::pop_mpls{0x8847};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_ethertype_is_not_equal)
      {
        auto const sut1 = actions::pop_mpls{0x8847};
        auto const sut2 = actions::pop_mpls{0x8848};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x14\x00\x08\x08\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::pop_mpls{0x0800};
        auto const sut2 = actions::pop_mpls::decode(it, binary.end());

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::pop_mpls{0x0800};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_ethertype_and_pad_are_equal)
      {
        auto const sut1 = actions::pop_mpls{0x8847};
        auto const sut2 = actions::pop_mpls{0x8847};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_ethertype_is_not_equal)
      {
        auto const sut1 = actions::pop_mpls{0x8847};
        auto const sut2 = actions::pop_mpls{0x8848};

        BOOST_TEST(!equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary = "\x00\x14\x00\x08\x08\x00\xab\xcd"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::pop_mpls{0x0800};
        auto const sut2 = actions::pop_mpls::decode(it, binary.end());

        BOOST_TEST(equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, pop_mpls_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, pop_mpls_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const action = actions::pop_mpls::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((action == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // pop_mpls_test
BOOST_AUTO_TEST_SUITE_END() // action_test
