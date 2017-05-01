#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/instruction/write_metadata.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace instructions = v13::instructions;

namespace bdata = boost::unit_test::data;

namespace {

struct write_metadata_fixture
{
    instructions::write_metadata sut{
        0x0102030405060708, 0x1f2f3f4f5f6f7f8f
    };
    std::vector<unsigned char> binary
        = "\x00\x02\x00\x18\x00\x00\x00\x00""\x01\x02\x03\x04\x05\x06\x07\x08"
          "\x1f\x2f\x3f\x4f\x5f\x6f\x7f\x8f"_bin;
};

}

BOOST_AUTO_TEST_SUITE(instruction_test)
BOOST_AUTO_TEST_SUITE(write_metadata_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = instructions::write_metadata;

        BOOST_TEST(sut::type() == v13::protocol::OFPIT_WRITE_METADATA);
    }

    BOOST_AUTO_TEST_CASE(construct_from_metadata_test)
    {
        auto const metadata = std::uint64_t{0x12345678};

        auto const sut = instructions::write_metadata{metadata};

        BOOST_TEST(sut.length()
                == sizeof(v13::protocol::ofp_instruction_write_metadata));
        BOOST_TEST(sut.metadata() == metadata);
        BOOST_TEST(sut.metadata_mask()
                == std::numeric_limits<std::uint64_t>::max());
    }

    BOOST_AUTO_TEST_CASE(construct_from_metadata_mask_test)
    {
        auto const metadata = std::uint64_t{0x12345678};
        auto const metadata_mask = std::uint64_t{0xf2f4f6f8};

        auto const sut = instructions::write_metadata{metadata, metadata_mask};

        BOOST_TEST(sut.metadata() == metadata);
        BOOST_TEST(sut.metadata_mask() == metadata_mask);
    }

    BOOST_DATA_TEST_CASE(
              create_test
            , bdata::make(std::vector<std::uint64_t>{0, 1, 0xffffffffffffffff})
            * bdata::make(std::vector<std::uint64_t>{0, 1, 0xffffffffffffffff})
            , metadata, metadata_mask)
    {
        BOOST_CHECK_NO_THROW(
                instructions::write_metadata::create(metadata, metadata_mask));
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = instructions::write_metadata{1, 0xffffffffffffffff};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_metadata_and_mask_are_equal)
      {
        BOOST_TEST(
            (instructions::write_metadata{0x12, 0xff} == instructions::write_metadata{0x12, 0xff}));
      }
      BOOST_AUTO_TEST_CASE(false_if_metadata_is_not_equal)
      {
        BOOST_TEST(
            (instructions::write_metadata{0} != instructions::write_metadata{1}));
      }
      BOOST_AUTO_TEST_CASE(false_if_mask_is_not_equal)
      {
        BOOST_TEST(
            (instructions::write_metadata{0, 0x1} != instructions::write_metadata{0, 0x2}));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary
          = "\x00\x02\x00\x18\x00\x00\x00\x01""\x00\x00\x00\x00\x00\x00\x00\xff"
            "\x00\x00\x00\x00\x00\x00\x00\xff"_bin;
        auto it = binary.begin();
        auto const nonzero_pad
          = instructions::write_metadata::decode(it, binary.end());

        BOOST_TEST((instructions::write_metadata{0xff, 0xff} != nonzero_pad));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = instructions::write_metadata{1, 0xffffffffffffffff};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_metadata_and_mask_are_equal)
      {
        BOOST_TEST(
            equivalent(
                instructions::write_metadata{0x12, 0xff}
              , instructions::write_metadata{0x12, 0xff}));
      }
      BOOST_AUTO_TEST_CASE(true_if_metadata_is_not_equal_but_write_value_is_equal)
      {
        BOOST_TEST(
            equivalent(
                instructions::write_metadata{0x0101, 0x00ff}
              , instructions::write_metadata{0x1001, 0x00ff}));
      }
      BOOST_AUTO_TEST_CASE(false_if_metadata_is_not_equal_and_write_value_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
              instructions::write_metadata{0}, instructions::write_metadata{1}));
      }
      BOOST_AUTO_TEST_CASE(false_if_mask_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                instructions::write_metadata{0, 0x1}
              , instructions::write_metadata{0, 0x2}));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary
          = "\x00\x02\x00\x18\x00\x00\x00\x01""\x00\x00\x00\x00\x00\x00\x00\xff"
            "\x00\x00\x00\x00\x00\x00\x00\xff"_bin;
        auto it = binary.begin();
        auto const nonzero_pad
          = instructions::write_metadata::decode(it, binary.end());

        BOOST_TEST(
            equivalent(instructions::write_metadata{0xff, 0xff}, nonzero_pad));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, write_metadata_fixture)
    {
        auto buffer = std::vector<unsigned char>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, write_metadata_fixture)
    {
        auto it = binary.begin();
        auto it_end = binary.end();

        auto const write_metadata
            = instructions::write_metadata::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((write_metadata == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // write_metadata_test
BOOST_AUTO_TEST_SUITE_END() // instruction_test
