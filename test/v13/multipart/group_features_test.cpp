#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/group_features.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace multipart = v13::messages::multipart;

namespace {
  using max_groups_type = multipart::group_features_reply::max_groups_type;
  using actions_type = multipart::group_features_reply::actions_type;

  struct group_features_request_fixture {
    multipart::group_features_request sut{0x12345678};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x10\x12\x34\x56\x78""\x00\x08\x00\x00\x00\x00\x00\x00"
        ""_bin;
  };

  struct group_features_reply_parameter {
    std::uint32_t types
      = (1 << protocol::group_type::all)
      | (1 << protocol::group_type::select)
      | (1 << protocol::group_type::indirect)
      | (1 << protocol::group_type::ff);
    std::uint32_t capabilities
      = protocol::group_capabilities::select_weight
      | protocol::group_capabilities::select_liveness
      | protocol::group_capabilities::chaining
      | protocol::group_capabilities::chaining_checks;
    max_groups_type max_groups{{0x12345678, 0x01020304, 0x11223344, 0x10203040}};
    actions_type actions{{0x11111001, 0x1fff3001, 0x00000000, 0x01010001}};
    std::uint32_t xid = 0x1f2f3f4f;
  };
  struct group_features_reply_fixture : group_features_reply_parameter {
    multipart::group_features_reply sut{
      types, capabilities, max_groups, actions, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\x38\x1f\x2f\x3f\x4f""\x00\x08\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x0f\x00\x00\x00\x0f""\x12\x34\x56\x78\x01\x02\x03\x04"
        "\x11\x22\x33\x44\x10\x20\x30\x40""\x11\x11\x10\x01\x1f\xff\x30\x01"
        "\x00\x00\x00\x00\x01\x01\x00\x01"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(group_features_request)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::group_features_request;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_request);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::group_features_request;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::group_features);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      multipart::group_features_request const sut{};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.flags() == 0);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_xid)
    {
      auto const xid = std::uint32_t{123};

      multipart::group_features_request const sut{xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::group_features_request{};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = std::uint32_t{1234};

      BOOST_TEST(
          (multipart::group_features_request{xid}
        == multipart::group_features_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_features_request{1}
        != multipart::group_features_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, group_features_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        construct_from_binary, group_features_request_fixture)
    {
      auto it = bin.begin();

      auto const group_features_request
        = multipart::group_features_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_features_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // group_features_request

BOOST_AUTO_TEST_SUITE(group_features_reply)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::group_features_reply;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_reply);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::group_features_reply;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::group_features);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const types = (protocol::group_type::indirect << 1);
      auto const capabilities
        = protocol::group_capabilities::select_liveness
        | protocol::group_capabilities::chaining;
      auto const max_groups = max_groups_type{{1, 2, 3, 4}};
      auto const actions = actions_type{{5, 6, 7, 8}};
      auto const xid = std::uint32_t{1234};

      multipart::group_features_reply const sut{
        types, capabilities, max_groups, actions, xid
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_reply)
        + sizeof(protocol::ofp_group_features));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.types() == types);
      BOOST_TEST(sut.capabilities() == capabilities);
      BOOST_TEST(sut.max_groups() == max_groups);
      BOOST_TEST(sut.actions() == actions);
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      auto const types = (protocol::group_type::indirect << 1);
      auto const capabilities
        = protocol::group_capabilities::select_liveness
        | protocol::group_capabilities::chaining;
      auto const max_groups = max_groups_type{{1, 2, 3, 4}};
      auto const actions = actions_type{{5, 6, 7, 8}};

      multipart::group_features_reply const sut{
        types, capabilities, max_groups, actions
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_multipart_reply)
        + sizeof(protocol::ofp_group_features));
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.types() == types);
      BOOST_TEST(sut.capabilities() == capabilities);
      BOOST_TEST(sut.max_groups() == max_groups);
      BOOST_TEST(sut.actions() == actions);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, group_features_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::group_features_reply{
        types, capabilities, max_groups, actions, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::group_features_reply{
            types, capabilities, max_groups, actions, xid}
        == multipart::group_features_reply{
            types, capabilities, max_groups, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_types_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_features_reply{
            1, capabilities, max_groups, actions, xid}
        != multipart::group_features_reply{
            2, capabilities, max_groups, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_capabilities_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_features_reply{
            types, 1, max_groups, actions, xid}
        != multipart::group_features_reply{
            types, 2, max_groups, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_max_groups_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_features_reply{
            types, capabilities, {{0, 0, 0, 1}}, actions, xid}
        != multipart::group_features_reply{
            types, capabilities, {{0, 0, 0, 2}}, actions, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_actions_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_features_reply{
            types, capabilities, max_groups, {{0, 0, 0, 1}}, xid}
        != multipart::group_features_reply{
            types, capabilities, max_groups, {{0, 0, 0, 2}}, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_features_reply{
            types, capabilities, max_groups, actions, 1}
        != multipart::group_features_reply{
            types, capabilities, max_groups, actions, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, group_features_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, group_features_reply_fixture)
    {
      auto it = bin.begin();

      auto const group_features_reply
        = multipart::group_features_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_features_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // group_features_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
