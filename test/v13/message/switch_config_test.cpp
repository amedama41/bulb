#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/switch_config.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/openflow.hpp>
#include <canard/net/ofp/v13/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;

namespace {
  struct get_config_request_fixture {
    v13::messages::get_config_request sut{0x40};
    std::vector<std::uint8_t> bin = "\x04\x07\00\x08\x00\x00\x00\x40"_bin;
  };
  struct get_config_reply_parameter {
    std::uint16_t flags = protocol::OFPC_FRAG_DROP;
    std::uint16_t miss_send_len = 0xff07;
    std::uint32_t xid = 0x8;
  };
  struct get_config_reply_fixture : get_config_reply_parameter {
    v13::messages::get_config_reply sut{flags, miss_send_len, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x08\x00\x0c\x00\x00\x00\x08""\x00\x01\xff\x07"_bin;
  };
  struct set_config_parameter {
    std::uint16_t flags = protocol::OFPC_FRAG_DROP;
    std::uint16_t miss_send_len = 0xff07;
    std::uint32_t xid = 0x8;
  };
  struct set_config_fixture : set_config_parameter {
    v13::messages::set_config sut{flags, miss_send_len, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x09\x00\x0c\x00\x00\x00\x08""\x00\x01\xff\x07"_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(get_config_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      v13::messages::get_config_request const sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_GET_CONFIG_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const xid = std::uint32_t{79};

      v13::messages::get_config_request const sut{xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_GET_CONFIG_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::get_config_request{1};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = std::uint32_t{1};

      BOOST_TEST(
          (v13::messages::get_config_request{xid}
        == v13::messages::get_config_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::get_config_request{1}
        != v13::messages::get_config_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, get_config_request_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_binary, get_config_request_fixture)
    {
      auto it = bin.begin();

      auto const get_config_request
        = v13::messages::get_config_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((get_config_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // get_config_request


BOOST_AUTO_TEST_SUITE(get_config_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const miss_send_len = std::uint16_t{protocol::OFPCML_MAX};
      auto const xid = std::uint32_t{0x79};

      v13::messages::get_config_reply const sut{flags, miss_send_len, xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_GET_CONFIG_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_config));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.miss_send_length() == miss_send_len);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_request)
    {
      auto const request = v13::messages::get_config_request{0x3382};
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const miss_send_len = std::uint16_t{0x3456};

      v13::messages::get_config_reply const sut{request, flags, miss_send_len};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_GET_CONFIG_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_config));
      BOOST_TEST(sut.xid() == request.xid());
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.miss_send_length() == miss_send_len);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, get_config_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut
        = v13::messages::get_config_reply{flags, miss_send_len, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::get_config_reply{flags, miss_send_len, xid}
        == v13::messages::get_config_reply{flags, miss_send_len, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::get_config_reply{1, miss_send_len, xid}
        != v13::messages::get_config_reply{2, miss_send_len, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_miss_send_len_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::get_config_reply{flags, 1, xid}
        != v13::messages::get_config_reply{flags, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::get_config_reply{flags, miss_send_len, 1}
        != v13::messages::get_config_reply{flags, miss_send_len, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, get_config_reply_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, get_config_reply_fixture)
    {
      auto it = bin.begin();

      auto const get_config_reply
        = v13::messages::get_config_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((get_config_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // get_config_reply


BOOST_AUTO_TEST_SUITE(set_config)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
        auto const flags = protocol::OFPC_FRAG_NORMAL;
        auto const miss_send_len = std::uint16_t{protocol::OFPCML_MAX};
        auto const xid = std::uint32_t{0x79};

        v13::messages::set_config const sut{flags, miss_send_len, xid};

        BOOST_TEST(sut.version() == protocol::OFP_VERSION);
        BOOST_TEST(sut.type() == protocol::OFPT_SET_CONFIG);
        BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_config));
        BOOST_TEST(sut.xid() == xid);
        BOOST_TEST(sut.flags() == flags);
        BOOST_TEST(sut.miss_send_length() == miss_send_len);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, set_config_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::set_config{flags, miss_send_len, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::set_config{flags, miss_send_len, xid}
        == v13::messages::set_config{flags, miss_send_len, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::set_config{1, miss_send_len, xid}
        != v13::messages::set_config{2, miss_send_len, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_miss_send_len_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::set_config{flags, 1, xid}
        != v13::messages::set_config{flags, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::set_config{flags, miss_send_len, 1}
        != v13::messages::set_config{flags, miss_send_len, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, set_config_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, set_config_fixture)
    {
      auto it = bin.begin();

      auto const set_config = v13::messages::set_config::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((set_config == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // set_config
BOOST_AUTO_TEST_SUITE_END() // message_test
