#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/description.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>

#include <canard/net/ofp/v13/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace multipart = v13::messages::multipart;
namespace protocol = v13::protocol;

namespace {

  struct description_request_fixture {
    multipart::description_request sut{0x12345678};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x10\x12\x34\x56\x78""\x00\x00\x00\x00\x00\x00\x00\x00"_bin
      ;
  };

  struct description_reply_parameter {
    char const* mfr_desc = "manufacture_desc";
    char const* hw_desc = "hardware_desc";
    char const* sw_desc = "software_desc";
    char const* serial_num = "serial_number";
    char const* dp_desc = "datapath_desc";
    std::uint32_t xid = 0x12345678;
  };
  struct description_reply_fixture {
    multipart::description_reply sut{
        "manufacture_desc"
      , "hardware_desc"
      , "software_desc"
      , "serial_number"
      , "datapath_desc"
      , 0x12345678
    };
    std::vector<std::uint8_t> bin
      = "\x04\x13\x04\x30\x12\x34\x56\x78""\x00\x00\x00\x00\x00\x00\x00\x00"
        "m" "a" "n" "u" "f" "a" "c" "t"   "u" "r" "e" "_" "d" "e" "s" "c"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "h" "a" "r" "d" "w" "a" "r" "e"   "_" "d" "e" "s" "c" "\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "s" "o" "f" "t" "w" "a" "r" "e"   "_" "d" "e" "s" "c" "\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "s" "e" "r" "i" "a" "l" "_" "n"   "u" "m" "b" "e" "r" "\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "d" "a" "t" "a" "p" "a" "t" "h"   "_" "d" "e" "s" "c" "\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(description_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      multipart::description_request const sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_DESC);
      BOOST_TEST(sut.flags() == 0);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, description_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::description_request{1};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = 32;

      BOOST_TEST(
          (multipart::description_request{xid}
        == multipart::description_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::description_request{1}
        != multipart::description_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, description_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, description_request_fixture)
    {
      auto it = bin.begin();

      auto const description_request
        = multipart::description_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((description_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // description_request


BOOST_AUTO_TEST_SUITE(description_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      multipart::description_reply const sut{
          "manufacture_desc"
        , "hardware_desc"
        , "software_desc"
        , "serial_number"
        , "datapath_desc"
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_reply)
          + sizeof(protocol::ofp_desc));
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_DESC);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.manufacture_desc() == "manufacture_desc");
      BOOST_TEST(sut.hardware_desc() == "hardware_desc");
      BOOST_TEST(sut.software_desc() == "software_desc");
      BOOST_TEST(sut.serial_number() == "serial_number");
      BOOST_TEST(sut.datapath_desc() == "datapath_desc");
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, description_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, description_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::description_reply{
        mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::description_reply{
             mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid
           }
        == multipart::description_reply{
             mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_manufacture_desc_is_not_equal)
    {
      BOOST_TEST(
          (multipart::description_reply{
             "1", hw_desc, sw_desc, serial_num, dp_desc, xid
           }
        != multipart::description_reply{
             "2", hw_desc, sw_desc, serial_num, dp_desc, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_hardware_desc_is_not_equal)
    {
      BOOST_TEST(
          (multipart::description_reply{
             mfr_desc, "1", sw_desc, serial_num, dp_desc, xid
           }
        != multipart::description_reply{
             mfr_desc, "2", sw_desc, serial_num, dp_desc, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_software_desc_is_not_equal)
    {
      BOOST_TEST(
          (multipart::description_reply{
             mfr_desc, hw_desc, "1", serial_num, dp_desc, xid
           }
        != multipart::description_reply{
             mfr_desc, hw_desc, "2", serial_num, dp_desc, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_serial_number_is_not_equal)
    {
      BOOST_TEST(
          (multipart::description_reply{
             mfr_desc, hw_desc, sw_desc, "1", dp_desc, xid
           }
        != multipart::description_reply{
             mfr_desc, hw_desc, sw_desc, "2", dp_desc, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_datapath_desc_is_not_equal)
    {
      BOOST_TEST(
          (multipart::description_reply{
             mfr_desc, hw_desc, sw_desc, serial_num, "1", xid
           }
        != multipart::description_reply{
             mfr_desc, hw_desc, sw_desc, serial_num, "2", xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::description_reply{
             mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, 1
           }
        != multipart::description_reply{
             mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, description_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, description_reply_fixture)
    {
      auto it = bin.begin();

      auto const description_reply
        = multipart::description_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((description_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // description_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
