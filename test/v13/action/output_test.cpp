#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/action/output.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <vector>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace actions = v13::actions;
namespace v13_detail = v13::v13_detail;

using protocol = v13::protocol;

namespace bdata = boost::unit_test::data;

namespace {

struct output_fixture
{
    actions::output sut{0x12345678, 0xabcd};
    std::vector<std::uint8_t> binary
        = "\x00\x00\x00\x10\x12\x34\x56\x78""\xab\xcd\x00\x00\x00\x00\x00\x00"
          ""_bin;
};

}

BOOST_AUTO_TEST_SUITE(action_test)
BOOST_AUTO_TEST_SUITE(output_test)

    BOOST_AUTO_TEST_CASE(type_definition_test)
    {
        using sut = actions::output;

        BOOST_TEST(sut::type() == protocol::OFPAT_OUTPUT);
        BOOST_TEST(sut::length() == sizeof(v13_detail::ofp_action_output));
    }

    BOOST_AUTO_TEST_CASE(construct_from_port_test)
    {
        auto const port = std::uint32_t{1};

        auto const sut = actions::output{port};

        BOOST_TEST(sut.port_no() == port);
        BOOST_TEST(sut.max_length() == protocol::OFPCML_NO_BUFFER);
    }

    BOOST_AUTO_TEST_CASE(construct_from_port_and_length_test)
    {
        auto const port = std::uint32_t{protocol::OFPP_MAX};
        auto const length = std::uint16_t{1};

        auto const sut = actions::output{port, length};

        BOOST_TEST(sut.port_no() == port);
        BOOST_TEST(sut.max_length() == length);
    }

    BOOST_DATA_TEST_CASE(
              create_success_test
            , bdata::make(std::vector<std::uint32_t>{
                1, protocol::OFPP_MAX, protocol::OFPP_ANY - 1
              })
            ^ bdata::make(std::vector<std::uint16_t>{
                0, protocol::OFPCML_MAX, protocol::OFPCML_NO_BUFFER
              })
            , port, max_length)
    {
        auto const sut = actions::output::create(port, max_length);

        BOOST_TEST(sut.port_no() == port);
        BOOST_TEST(sut.max_length() == max_length);
    }

    BOOST_DATA_TEST_CASE(
              create_failure_by_port_no_test
            , bdata::make(std::vector<std::uint32_t>{0, protocol::OFPP_ANY})
            , port)
    {
        BOOST_CHECK_THROW(actions::output::create(port), std::runtime_error);
    }

    BOOST_DATA_TEST_CASE(
              create_failure_by_max_length_test
            , bdata::make(std::vector<std::uint16_t>{
                protocol::OFPCML_MAX + 1, protocol::OFPCML_NO_BUFFER - 1
              })
            , max_length)
    {
        BOOST_CHECK_THROW(
                actions::output::create(1, max_length), std::runtime_error);
    }

    BOOST_DATA_TEST_CASE(
              to_controller_factory_test
            , bdata::make(std::vector<std::uint16_t>{
                0, protocol::OFPCML_MAX, protocol::OFPCML_NO_BUFFER
              })
            , length)
    {
        auto const sut = actions::output::to_controller(length);

        BOOST_TEST(sut.port_no() == protocol::OFPP_CONTROLLER);
        BOOST_TEST(sut.max_length() == length);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::output{1, 0};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_port_and_len_are_equal)
      {
        auto const sut1 = actions::output{1, 0};
        auto const sut2 = actions::output{1, 0};

        BOOST_TEST((sut1 == sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_port_is_not_equal)
      {
        auto const sut1 = actions::output{1, 0};
        auto const sut2 = actions::output{2, 0};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_len_is_not_equal)
      {
        auto const sut1 = actions::output{1, 0};
        auto const sut2 = actions::output{1, 9};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_port_and_len_are_not_equal)
      {
        auto const sut1 = actions::output{1, 0};
        auto const sut2 = actions::output{2, 9};

        BOOST_TEST((sut1 != sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_pad_is_not_equal)
      {
        auto const binary
          = "\x00\x00\x00\x10\x12\x34\x56\x78""\x12\x34\x00\x00\x00\x00\x00\x01"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::output{0x12345678, 0x1234};
        auto const sut2 = actions::output::decode(it, binary.end());

        BOOST_TEST((sut1 != sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = actions::output{1, 0};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_port_and_len_are_equal)
      {
        auto const sut1 = actions::output{1, 0};
        auto const sut2 = actions::output{1, 0};

        BOOST_TEST(equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_port_is_not_equal)
      {
        auto const sut1 = actions::output{1, 0};
        auto const sut2 = actions::output{2, 0};

        BOOST_TEST(!equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_len_is_not_equal)
      {
        auto const sut1 = actions::output{1, 0};
        auto const sut2 = actions::output{1, 9};

        BOOST_TEST(!equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(false_if_port_and_len_are_not_equal)
      {
        auto const sut1 = actions::output{1, 0};
        auto const sut2 = actions::output{2, 9};

        BOOST_TEST(!equivalent(sut1, sut2));
      }
      BOOST_AUTO_TEST_CASE(true_if_pad_is_not_equal)
      {
        auto const binary
          = "\x00\x00\x00\x10\x12\x34\x56\x78""\x12\x34\x12\x34\x56\x78\x9a\xbc"_bin;
        auto it = binary.begin();
        auto const sut1 = actions::output{0x12345678, 0x1234};
        auto const sut2 = actions::output::decode(it, binary.end());

        BOOST_TEST(equivalent(sut1, sut2));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, output_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, output_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const action = actions::output::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((action == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // output_test
BOOST_AUTO_TEST_SUITE_END() // action_test
