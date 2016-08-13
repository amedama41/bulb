#define BOOST_TEST_DYN_LINK
#include <canard/network/protocol/openflow/v13/oxm_match_field.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <stdexcept>
#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace match = v13::oxm_match;

using proto = v13::protocol;

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(oxm_match_test)
BOOST_AUTO_TEST_SUITE(mpls_label_test)

    BOOST_DATA_TEST_CASE(
              no_mask_create_success_test
            , bdata::make(std::vector<std::uint32_t>{0x00000000, 0x000fffff})
            , value)
    {
        auto const sut = match::mpls_label::create(value);

        BOOST_TEST(sut.oxm_length() == sizeof(std::uint32_t));
        BOOST_TEST(sut.oxm_value() == value);
        BOOST_TEST(!sut.oxm_has_mask());
        BOOST_TEST(sut.oxm_mask() == boost::none);
        BOOST_TEST(sut.length()
                == sizeof(std::uint32_t) + sizeof(std::uint32_t));
        BOOST_TEST(!sut.is_wildcard());
        BOOST_TEST(sut.is_exact());
    }

    BOOST_DATA_TEST_CASE(
              has_mask_create_success_test
            , bdata::make(std::vector<std::uint32_t>{0x00000000, 0x000fffff})
            ^ bdata::make(std::vector<std::uint32_t>{0x00000001, 0x000fffff})
            , value, mask)
    {
        auto const sut = match::mpls_label::create(value, mask);

        BOOST_TEST(sut.oxm_length() == 2 * sizeof(std::uint32_t));
        BOOST_TEST(sut.oxm_value() == value);
        BOOST_TEST_REQUIRE(sut.oxm_has_mask());
        BOOST_TEST(*sut.oxm_mask() == mask);
        BOOST_TEST(sut.length()
                == sizeof(std::uint32_t) + 2 * sizeof(std::uint32_t));
        BOOST_TEST(!sut.is_wildcard());
        BOOST_TEST(!sut.is_exact());
    }

    BOOST_DATA_TEST_CASE(
              no_mask_create_failure_test
            , bdata::make(std::vector<std::uint32_t>{0x00100000, 0xffffffff})
            , value)
    {
        BOOST_CHECK_THROW(
                match::mpls_label::create(value), std::runtime_error);
    }

    BOOST_AUTO_TEST_CASE(has_mask_create_failure_test)
    {
        BOOST_CHECK_THROW(
                match::mpls_label::create(0x01, 0x00), std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END() // mpls_label_test
BOOST_AUTO_TEST_SUITE_END() // oxm_match_test
