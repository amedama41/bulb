#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/meter_config.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace multipart = v13::messages::multipart;
namespace meter_bands = v13::meter_bands;

namespace {

  struct meter_bands_fixture {
    meter_bands::drop drop{0x11111111, 0x12121212};
    meter_bands::dscp_remark dscp_remark{0x23, 0x21212121, 0x22222222};
  };
  struct meter_config_parameter : meter_bands_fixture {
    std::uint32_t meter_id = 0x01020304;
    std::uint16_t flags
      = protocol::meter_flags::kbps | protocol::meter_flags::pktps
      | protocol::meter_flags::burst | protocol::meter_flags::stats;
    using bands_type = multipart::meter_config::bands_type;
    bands_type bands{drop, dscp_remark};
  };
  struct meter_config_fixture : meter_config_parameter {
    multipart::meter_config sut{meter_id, flags, bands};
    std::vector<std::uint8_t> bin
      = "\x00\x28\x00\x0f\x01\x02\x03\x04"
        "\x00\x01\x00\x10\x11\x11\x11\x11""\x12\x12\x12\x12\x00\x00\x00\x00"
        "\x00\x02\x00\x10\x21\x21\x21\x21""\x22\x22\x22\x22\x23\x00\x00\x00"
        ""_bin;
  };

  struct meter_config_request_parameter {
    std::uint32_t meter_id = 0x12345678;
    std::uint32_t xid = 0x87654321;
  };
  struct meter_config_request_fixture : meter_config_request_parameter {
    multipart::meter_config_request sut{meter_id, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x18\x87\x65\x43\x21""\x00\x0a\x00\x00\x00\x00\x00\x00"
        "\x12\x34\x56\x78\x00\x00\x00\x00"
        ""_bin;
  };

  struct meter_config_reply_parameter : meter_bands_fixture {
    multipart::meter_config config1{
      0x01010101, 0x000f, multipart::meter_config::bands_type{drop, dscp_remark}
    };
    multipart::meter_config config2{
      0x02020202, 0x0001, multipart::meter_config::bands_type{dscp_remark, drop}
    };
    multipart::meter_config config3{
      0x03030303, 0x0000, multipart::meter_config::bands_type{}
    };
    using body_type = multipart::meter_config_reply::body_type;
    body_type body{config1, config2, config3};
    std::uint16_t flags = 0x0001;
    std::uint32_t xid = 0x12345678;
  };
  struct meter_config_reply_fixture : meter_config_reply_parameter {
    multipart::meter_config_reply sut{body, flags, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\x68\x12\x34\x56\x78""\x00\x0a\x00\x01\x00\x00\x00\x00"
        "\x00\x28\x00\x0f\x01\x01\x01\x01"
        "\x00\x01\x00\x10\x11\x11\x11\x11""\x12\x12\x12\x12\x00\x00\x00\x00"
        "\x00\x02\x00\x10\x21\x21\x21\x21""\x22\x22\x22\x22\x23\x00\x00\x00"
        "\x00\x28\x00\x01\x02\x02\x02\x02"
        "\x00\x02\x00\x10\x21\x21\x21\x21""\x22\x22\x22\x22\x23\x00\x00\x00"
        "\x00\x01\x00\x10\x11\x11\x11\x11""\x12\x12\x12\x12\x00\x00\x00\x00"
        "\x00\x08\x00\x00\x03\x03\x03\x03"
        ""_bin;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(meter_config)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_parameters, meter_config_fixture)
    {
      auto const meter_id = 3;
      auto const flags = 1;
      auto const bands = multipart::meter_config::bands_type{dscp_remark};

      multipart::meter_config const sut{meter_id, flags, bands};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_meter_config) + bands.length());
      BOOST_TEST(sut.meter_id() == meter_id);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.bands() == bands));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, meter_config_fixture)
    {
      auto const copy = add_const(sut);

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, meter_config_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_meter_config));
      BOOST_TEST(moved.bands().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, meter_config_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = multipart::meter_config{0, 0, {}};

      copy = add_const(sut);

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = multipart::meter_config{0, 0, {}};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_meter_config));
      BOOST_TEST(moved.bands().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, meter_config_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::meter_config{meter_id, flags, bands};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::meter_config{meter_id, flags, bands}
        == multipart::meter_config{meter_id, flags, bands}));
    }
    BOOST_AUTO_TEST_CASE(false_if_meter_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_config{1, flags, bands}
        != multipart::meter_config{2, flags, bands}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_config{meter_id, 1, bands}
        != multipart::meter_config{meter_id, 2, bands}));
    }
    BOOST_AUTO_TEST_CASE(false_if_bands_is_not_equal)
    {
      using multipart::meter_config;

      BOOST_TEST(
          (meter_config{meter_id, flags, bands_type{drop, dscp_remark}}
        != meter_config{meter_id, flags, bands_type{dscp_remark, drop}}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(extract_bands)
    BOOST_FIXTURE_TEST_CASE(move_bands_ownership, meter_config_fixture)
    {
      auto const original_bands = sut.bands();

      auto const bands = sut.extract_bands();

      BOOST_TEST((bands == original_bands));
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_meter_config));
      BOOST_TEST(sut.bands().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_bands

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_config_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_config_fixture)
    {
      auto it = bin.begin();

      auto const meter_config = multipart::meter_config::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_config == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_config

BOOST_AUTO_TEST_SUITE(meter_config_request)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::meter_config_request;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_request);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::meter_config_request;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::meter_config);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const meter_id = std::uint32_t{12};
      auto const xid = std::uint32_t{10};

      multipart::meter_config_request const sut{meter_id, xid};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_request)
        + sizeof(protocol::ofp_meter_multipart_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.meter_id() == meter_id);
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      auto const meter_id = std::uint32_t{1};

      multipart::meter_config_request const sut{meter_id};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_request)
        + sizeof(protocol::ofp_meter_multipart_request));
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.meter_id() == meter_id);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, meter_config_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::meter_config_request{meter_id, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::meter_config_request{meter_id, xid}
        == multipart::meter_config_request{meter_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_meter_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_config_request{1, xid}
        != multipart::meter_config_request{2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_config_request{meter_id, 1}
        != multipart::meter_config_request{meter_id, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_config_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_config_request_fixture)
    {
      auto it = bin.begin();

      auto const meter_config_request
        = multipart::meter_config_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_config_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_config_request

BOOST_AUTO_TEST_SUITE(meter_config_reply)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::meter_config_reply;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_reply);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::meter_config_reply;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::meter_config);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_FIXTURE_TEST_SUITE(constructor, meter_config_reply_parameter)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      body_type const body{config1, config2};
      auto const flags = 0;
      auto const xid = 32;

      multipart::meter_config_reply const sut{body, flags, xid};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      body_type const body{config3};
      auto const flags = protocol::multipart_reply_flags::reply_more;

      multipart::meter_config_reply const sut{body, flags};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_AUTO_TEST_CASE(constructible_without_flags)
    {
      body_type const body{config1};

      multipart::meter_config_reply const sut{body};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.body() == body));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, meter_config_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::meter_config_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::meter_config_reply{body,flags, xid}
        == multipart::meter_config_reply{body,flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      using multipart::meter_config_reply;

      BOOST_TEST(
          (meter_config_reply{body_type{config1, config2}, flags, xid}
        != meter_config_reply{body_type{config2, config1}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_config_reply{body, 1, xid}
        != multipart::meter_config_reply{body, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::meter_config_reply{body, flags, 1}
        != multipart::meter_config_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_config_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      add_const(sut).encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_config_reply_fixture)
    {
      auto it = bin.begin();

      auto const meter_config_reply
        = multipart::meter_config_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_config_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_config_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
