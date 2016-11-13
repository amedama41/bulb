#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/common/oxm_match_field.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <stdexcept>
#include <boost/endian/arithmetic.hpp>
#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>

#include "../../test_utility.hpp"
#include "../oxm_headers.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace match = v13::oxm_match_fields;

using proto = v13::protocol;

using boost::endian::big_uint24_t;

namespace bdata = boost::unit_test::data;

namespace {

struct no_mask_pbb_isid_fixture
{
    match::pbb_isid sut{0x123456};
    std::vector<std::uint8_t> binary = "\x80\x00\x4a\x03\x12\x34\x56"_bin;
};

struct has_mask_pbb_isid_fixture
{
    match::pbb_isid sut{0x123456, 0xf2f4f6};
    std::vector<std::uint8_t> binary
        = "\x80\x00\x4b\x06\x12\x34\x56\xf2""\xf4\xf6"_bin;
};

}

BOOST_AUTO_TEST_SUITE(oxm_match_test)
BOOST_AUTO_TEST_SUITE(pbb_isid_test)

    BOOST_AUTO_TEST_CASE(construct_from_value_test)
    {
        auto const value = big_uint24_t{0x000001};

        auto const sut = match::pbb_isid{value};

        BOOST_TEST(sut.oxm_length() == 3);
        BOOST_TEST(sut.oxm_header() == OXM_OF_PBB_ISID);
        BOOST_TEST(sut.oxm_value() == value);
        BOOST_TEST(!sut.oxm_has_mask());
        BOOST_TEST(sut.oxm_mask() == boost::none);
        BOOST_TEST(sut.length() == sizeof(std::uint32_t) + 3);
        BOOST_TEST(!sut.is_wildcard());
        BOOST_TEST(sut.is_exact());
    }

    BOOST_AUTO_TEST_CASE(construct_from_value_and_mask_test)
    {
        auto const value = big_uint24_t{0x010203};
        auto const mask = big_uint24_t{0xf1f2f3};

        auto const sut = match::pbb_isid{value, mask};

        BOOST_TEST(sut.oxm_length() == 2 * 3);
        BOOST_TEST(sut.oxm_header() == OXM_OF_PBB_ISID_W);
        BOOST_TEST(sut.oxm_value() == value);
        BOOST_TEST_REQUIRE(sut.oxm_has_mask());
        BOOST_TEST(*sut.oxm_mask() == mask);
        BOOST_TEST(sut.length() == sizeof(std::uint32_t) + 2 * 3);
        BOOST_TEST(!sut.is_wildcard());
        BOOST_TEST(!sut.is_exact());
    }

    BOOST_AUTO_TEST_CASE(construct_from_all_zero_mask_test)
    {
        auto const value = big_uint24_t{0};
        auto const mask = big_uint24_t{0};

        auto const sut = match::pbb_isid{value, mask};

        BOOST_TEST(sut.oxm_length() == 2 * 3);
        BOOST_TEST(sut.oxm_value() == value);
        BOOST_TEST_REQUIRE(sut.oxm_has_mask());
        BOOST_TEST(*sut.oxm_mask() == mask);
        BOOST_TEST(sut.length() == sizeof(std::uint32_t) + 2 * 3);
        BOOST_TEST(sut.is_wildcard());
        BOOST_TEST(!sut.is_exact());
    }

    BOOST_AUTO_TEST_CASE(construct_from_all_one_mask_test)
    {
        auto const value = big_uint24_t{0x123456};
        auto const mask = big_uint24_t{0xffffff};

        auto const sut = match::pbb_isid{value, mask};

        BOOST_TEST(sut.oxm_length() == 2 * 3);
        BOOST_TEST(sut.oxm_value() == value);
        BOOST_TEST_REQUIRE(sut.oxm_has_mask());
        BOOST_TEST(*sut.oxm_mask() == mask);
        BOOST_TEST(sut.length() == sizeof(std::uint32_t) + 2 * 3);
        BOOST_TEST(!sut.is_wildcard());
        BOOST_TEST(sut.is_exact());
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_value_is_same_and_no_mask)
      {
        auto const sut1 = match::pbb_isid{0x010203};
        auto const sut2 = match::pbb_isid{0x010203};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(true_if_value_and_mask_are_same)
      {
        auto const sut1 = match::pbb_isid{1, 0x000fff};
        auto const sut2 = match::pbb_isid{1, 0x000fff};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_value_is_diff_and_no_mask)
      {
        auto const sut1 = match::pbb_isid{0x010203};
        auto const sut2 = match::pbb_isid{0xf1f2f3};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_value_is_diff_and_mask_is_same)
      {
        auto const sut1 = match::pbb_isid{1, 0x000fff};
        auto const sut2 = match::pbb_isid{2, 0x000fff};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_value_is_same_and_mask_is_diff)
      {
        auto const sut1 = match::pbb_isid{1, 0x000fff};
        auto const sut2 = match::pbb_isid{1, 0xfff000};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_value_and_mask_are_diff)
      {
        auto const sut1 = match::pbb_isid{1, 0x000fff};
        auto const sut2 = match::pbb_isid{2, 0xfff000};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_equivalent_but_one_has_no_mask_and_another_has_mask)
      {
        auto const sut1 = match::pbb_isid{3};
        auto const sut2 = match::pbb_isid{3, 0xffffff};

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_CASE(no_mask_equivalent_test)
    {
        auto const sut = match::pbb_isid{0x010203};
        auto const same_value = match::pbb_isid{0x010203};
        auto const diff_value = match::pbb_isid{0xf1f2f3};

        BOOST_TEST(equivalent(sut, sut));
        BOOST_TEST(equivalent(sut, same_value));
        BOOST_TEST(!equivalent(sut, diff_value));
    }

    BOOST_AUTO_TEST_CASE(has_mask_equivalent_test)
    {
        auto const sut = match::pbb_isid{1, 0x000fff};
        auto const same_value_and_same_mask = match::pbb_isid{1, 0x000fff};
        auto const same_value_and_diff_mask = match::pbb_isid{1, 0xfff000};
        auto const diff_value_and_same_mask = match::pbb_isid{2, 0x000fff};
        auto const diff_value_and_diff_mask = match::pbb_isid{2, 0xfff000};

        BOOST_TEST(equivalent(sut, sut));
        BOOST_TEST(equivalent(sut, same_value_and_same_mask));
        BOOST_TEST(!equivalent(sut, same_value_and_diff_mask));
        BOOST_TEST(!equivalent(sut, diff_value_and_same_mask));
        BOOST_TEST(!equivalent(sut, diff_value_and_diff_mask));
    }

    BOOST_AUTO_TEST_CASE(no_mask_and_has_mask_equivalent_test)
    {
        auto const sut = match::pbb_isid{3};
        auto const same_value_and_mask = match::pbb_isid{3, 0x0f0f0f};
        auto const diff_value_and_mask = match::pbb_isid{4, 0x0f0f0f};

        BOOST_TEST(!equivalent(sut, same_value_and_mask));
        BOOST_TEST(!equivalent(sut, diff_value_and_mask));
    }

    BOOST_AUTO_TEST_CASE(exact_and_wildcard_equivalent_test)
    {
        auto const sut = match::pbb_isid{3};
        auto const same_value_and_all_zero_mask = match::pbb_isid{3, 0x000000};
        auto const same_value_and_all_one_mask = match::pbb_isid{3, 0xffffff};
        auto const diff_value_and_all_zero_mask = match::pbb_isid{4, 0x000000};
        auto const diff_value_and_all_one_mask = match::pbb_isid{4, 0xffffff};

        BOOST_TEST(!equivalent(sut, same_value_and_all_zero_mask));
        BOOST_TEST(equivalent(sut, same_value_and_all_one_mask));
        BOOST_TEST(!equivalent(sut, diff_value_and_all_zero_mask));
        BOOST_TEST(!equivalent(sut, diff_value_and_all_one_mask));
    }

    BOOST_DATA_TEST_CASE(
              no_mask_create_success_test
            , bdata::make(std::vector<big_uint24_t>{
                0x000000, 0xffffff / 2, 0xffffff
              })
            , value)
    {
        auto const sut = match::pbb_isid::create(value);

        BOOST_TEST(sut.oxm_length() == 3);
        BOOST_TEST(sut.oxm_value() == value);
        BOOST_TEST(!sut.oxm_has_mask());
        BOOST_TEST(sut.oxm_mask() == boost::none);
        BOOST_TEST(sut.length() == sizeof(std::uint32_t) + 3);
        BOOST_TEST(!sut.is_wildcard());
        BOOST_TEST(sut.is_exact());
    }

    BOOST_DATA_TEST_CASE(
              has_mask_create_success_test
            , bdata::make(std::vector<big_uint24_t>{
                0x000000, 0x103050, 0xfffffe
              })
            ^ bdata::make(std::vector<big_uint24_t>{
                0x000001, 0x123456, 0xfffffe
              })
            , value, mask)
    {
        auto const sut = match::pbb_isid::create(value, mask);

        BOOST_TEST(sut.oxm_length() == 2 * 3);
        BOOST_TEST(sut.oxm_value() == value);
        BOOST_TEST_REQUIRE(sut.oxm_has_mask());
        BOOST_TEST(*sut.oxm_mask() == mask);
        BOOST_TEST(sut.length() == sizeof(std::uint32_t) + 2 * 3);
        BOOST_TEST(!sut.is_wildcard());
        BOOST_TEST(!sut.is_exact());
    }

    BOOST_AUTO_TEST_CASE(has_mask_create_failure_test)
    {
        BOOST_CHECK_THROW(
                  match::pbb_isid::create(0x010101, 0x101010)
                , std::runtime_error);
    }

    BOOST_FIXTURE_TEST_CASE(no_mask_encode_test, no_mask_pbb_isid_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(no_mask_decode_test, no_mask_pbb_isid_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const field = match::pbb_isid::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((field == sut));
        BOOST_TEST(field.oxm_has_mask() == sut.oxm_has_mask());
        BOOST_TEST(field.oxm_length() == sut.oxm_length());
        BOOST_TEST(field.oxm_value() == sut.oxm_value());
        BOOST_TEST(field.oxm_mask() == sut.oxm_mask());
    }

    BOOST_FIXTURE_TEST_CASE(has_mask_encode_test, has_mask_pbb_isid_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(has_mask_decode_test, has_mask_pbb_isid_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const field = match::pbb_isid::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((field == sut));
        BOOST_TEST(field.oxm_has_mask() == sut.oxm_has_mask());
        BOOST_TEST(field.oxm_length() == sut.oxm_length());
        BOOST_TEST(field.oxm_value() == sut.oxm_value());
        BOOST_TEST(field.oxm_mask() == sut.oxm_mask());
    }

BOOST_AUTO_TEST_SUITE_END() // pbb_isid_test
BOOST_AUTO_TEST_SUITE_END() // oxm_match_test
