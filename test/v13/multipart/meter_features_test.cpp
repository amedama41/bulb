#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/meter_features.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace multipart = v13::messages::multipart;

namespace {

  struct meter_features_request_fixture {
    multipart::meter_features_request sut{0x12345678};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x10\x12\x34\x56\x78""\x00\x0b\x00\x00\x00\x00\x00\x00"
        ""_bin;
  };

  struct meter_features_reply_parameter {
    std::uint32_t max_meter = 0x01010101;
    std::uint32_t band_types
        = (1 << protocol::meter_band_type::drop)
        | (1 << protocol::meter_band_type::dscp_remark);
    std::uint32_t capabilities
        = protocol::meter_flags::kbps | protocol::meter_flags::pktps
        | protocol::meter_flags::burst | protocol::meter_flags::stats;
    std::uint8_t max_bands = 0x22;
    std::uint8_t max_color = 0x33;
    std::uint32_t xid = 0x12345678;
  };
  struct meter_features_reply_fixture : meter_features_reply_parameter {
    multipart::meter_features_reply sut{
      max_meter, band_types, capabilities, max_bands, max_color, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\x20\x12\x34\x56\x78""\x00\x0b\x00\x00\x00\x00\x00\x00"
        "\x01\x01\x01\x01\x00\x00\x00\x06""\x00\x00\x00\x0f\x22\x33\x00\x00"
        ""_bin;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(meter_features_request)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::meter_features_request;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_request);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::meter_features_request;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::meter_features);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      multipart::meter_features_request const sut{};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.flags() == 0);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_xid)
    {
      auto const xid = std::uint32_t{12};

      multipart::meter_features_request const sut{xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::meter_features_request{0x01010101};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = std::uint32_t{8761};

      BOOST_TEST(
          (multipart::meter_features_request{xid}
        == multipart::meter_features_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_features_request{1}
        != multipart::meter_features_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_features_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_features_request_fixture)
    {
      auto it = bin.begin();

      auto const meter_features_request
        = multipart::meter_features_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_features_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_features_request

BOOST_AUTO_TEST_SUITE(meter_features_reply)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::meter_features_reply;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_reply);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::meter_features_reply;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::meter_features);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const max_meter = std::uint32_t{1234};
      auto const band_types
        = (1 << protocol::meter_band_type::drop)
        | (1 << protocol::meter_band_type::dscp_remark);
      auto const capabilities
        = protocol::meter_flags::kbps | protocol::meter_flags::pktps
        | protocol::meter_flags::burst | protocol::meter_flags::stats;
      auto const max_bands = std::uint8_t{32};
      auto const max_color = std::uint8_t{11};
      auto const xid = std::uint32_t{1122};

      multipart::meter_features_reply const sut{
        max_meter, band_types, capabilities, max_bands, max_color, xid
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_reply)
        + sizeof(protocol::ofp_meter_features));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.max_meter() == max_meter);
      BOOST_TEST(sut.band_types() == band_types);
      BOOST_TEST(sut.capabilities() == capabilities);
      BOOST_TEST(sut.max_bands() == max_bands);
      BOOST_TEST(sut.max_color() == max_color);
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      auto const max_meter = std::uint32_t{14};
      auto const band_types = (1 << protocol::meter_band_type::drop);
      auto const capabilities
        = protocol::meter_flags::kbps | protocol::meter_flags::burst;
      auto const max_bands = std::uint8_t{2};
      auto const max_color = std::uint8_t{1};

      multipart::meter_features_reply const sut{
        max_meter, band_types, capabilities, max_bands, max_color
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_reply)
        + sizeof(protocol::ofp_meter_features));
      BOOST_TEST(sut.max_meter() == max_meter);
      BOOST_TEST(sut.band_types() == band_types);
      BOOST_TEST(sut.capabilities() == capabilities);
      BOOST_TEST(sut.max_bands() == max_bands);
      BOOST_TEST(sut.max_color() == max_color);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, meter_features_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::meter_features_reply{
        max_meter, band_types, capabilities, max_bands, max_color
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::meter_features_reply{
             max_meter, band_types, capabilities, max_bands, max_color, xid
           }
        == multipart::meter_features_reply{
             max_meter, band_types, capabilities, max_bands, max_color, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_max_meter_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_features_reply{
             1, band_types, capabilities, max_bands, max_color, xid
           }
        != multipart::meter_features_reply{
             2, band_types, capabilities, max_bands, max_color, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_band_types_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_features_reply{
             max_meter, 1, capabilities, max_bands, max_color, xid
           }
        != multipart::meter_features_reply{
             max_meter, 2, capabilities, max_bands, max_color, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_capabilities_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_features_reply{
             max_meter, band_types, 1, max_bands, max_color, xid
           }
        != multipart::meter_features_reply{
             max_meter, band_types, 2, max_bands, max_color, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_max_bands_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_features_reply{
             max_meter, band_types, capabilities, 1, max_color, xid
           }
        != multipart::meter_features_reply{
             max_meter, band_types, capabilities, 2, max_color, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_max_color_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_features_reply{
             max_meter, band_types, capabilities, max_bands, 1, xid
           }
        != multipart::meter_features_reply{
             max_meter, band_types, capabilities, max_bands, 2, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_features_reply{
             max_meter, band_types, capabilities, max_bands, max_color, 1
           }
        != multipart::meter_features_reply{
             max_meter, band_types, capabilities, max_bands, max_color, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_features_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_features_reply_fixture)
    {
      auto it = bin.begin();

      auto const meter_features_reply
        = multipart::meter_features_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_features_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_features_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
