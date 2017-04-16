#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/error.hpp>
#include <boost/test/unit_test.hpp>
#include <canard/net/ofp/v13/message/switch_config.hpp>
#include <cstdint>
#include <vector>
#include <boost/endian/conversion.hpp>

#include <canard/net/ofp/v13/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;

constexpr auto error_size = sizeof(protocol::ofp_error_msg);

namespace {
  struct error_parameter {
    protocol::error_type type = protocol::error_type::switch_config_failed;
    protocol::switch_config_failed_code code = protocol::OFPSCFC_BAD_FLAGS;
    std::uint32_t xid = 0x12345678;
    v13::messages::set_config const msg{0xff01, 0xfffe, xid};
    v13::messages::error::data_type data;

    error_parameter() { msg.encode(data); }
  };
  struct error_fixture : error_parameter {
    v13::messages::error sut{type, code, data, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x01\x00\x18\x12\x34\x56\x78" "\x00\x0a\x00\x00\x04\x09\x00\x0c"
        "\x12\x34\x56\x78\xff\x01\xff\xfe"_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(error)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_binary_data)
    {
      auto const etype = protocol::OFPET_HELLO_FAILED;
      auto const ecode = protocol::OFPHFC_INCOMPATIBLE;
      auto const edata = "incompatible version"_bbin;

      v13::messages::error const sut{etype, ecode, edata};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ERROR);
      BOOST_TEST(sut.length() == error_size + edata.size());
      BOOST_TEST(sut.error_type() == etype);
      BOOST_TEST(sut.error_code() == ecode);
      BOOST_TEST(sut.data() == edata, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_code_and_message)
    {
      auto const etype = protocol::OFPET_SWITCH_CONFIG_FAILED;
      auto const ecode = protocol::OFPSCFC_BAD_FLAGS;
      auto const msg = v13::messages::set_config{0xffff, 0xffff};

      v13::messages::error const sut{ecode, msg};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_ERROR);
      BOOST_TEST(sut.length() == error_size + msg.length());
      BOOST_TEST(sut.xid() == msg.xid());
      BOOST_TEST(sut.error_type() == etype);
      BOOST_TEST(sut.error_code() == ecode);
      auto buffer = std::vector<std::uint8_t>{};
      BOOST_TEST(
          sut.data() == msg.encode(buffer), boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, error_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, error_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == error_size);
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, error_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = v13::messages::error{
        protocol::OFPET_FLOW_MOD_FAILED, protocol::OFPFMFC_TABLE_FULL, ""_bbin
      };

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = v13::messages::error{
        protocol::OFPET_FLOW_MOD_FAILED, protocol::OFPFMFC_TABLE_FULL, ""_bbin
      };
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == error_size);
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, error_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::error{code, msg};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::error{type, code, data, xid}
        == v13::messages::error{type, code, data, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_type_is_not_equal)
    {
      using protocol::error_type;

      BOOST_TEST(
          (v13::messages::error{error_type::bad_action, code, data, xid}
        != v13::messages::error{error_type::bad_match, code, data, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_code_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::error{type, 1, data, xid}
        != v13::messages::error{type, 2, data, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::error{type, code, "1"_bbin, xid}
        != v13::messages::error{type, code, "2"_bbin, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::error{type, code, data, 1}
        != v13::messages::error{type, code, data, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(extract_data)
    BOOST_FIXTURE_TEST_CASE(move_data_ownership, error_fixture)
    {
      auto const org_data = sut.data();

      auto const data = sut.extract_data();

      BOOST_TEST(sut.length() == error_size);
      BOOST_TEST(sut.data().empty());
      BOOST_TEST(data == org_data, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_data

  BOOST_AUTO_TEST_SUITE(failed_request_header)
    BOOST_FIXTURE_TEST_CASE(return_msg_header, error_fixture)
    {
      auto const header = sut.failed_request_header();

      BOOST_TEST(header.version == msg.version());
      BOOST_TEST(header.type == msg.type());
      BOOST_TEST(header.length == msg.length());
      BOOST_TEST(header.xid == msg.xid());
    }
  BOOST_AUTO_TEST_SUITE_END() // failed_request_header

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, error_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin);
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, error_fixture)
    {
      auto it = bin.begin();

      auto const error = v13::messages::error::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((error == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // error
BOOST_AUTO_TEST_SUITE_END() // message_test

