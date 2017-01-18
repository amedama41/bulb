#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/message/switch_config.hpp>
#include <boost/test/unit_test.hpp>

#include <numeric>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace protocol = v10::protocol;

namespace {

struct get_config_request_fixture {
  std::uint32_t xid = 0x12345678;
  msg::get_config_request sut{xid};
  std::vector<unsigned char> bin = "\x01\x07\x00\x08\x12\x34\x56\x78"_bin;
};

struct get_config_reply_fixture {
  std::uint32_t xid = 0x12345678;
  std::uint16_t flags = protocol::OFPC_FRAG_DROP;
  std::uint16_t miss_send_len = 0x4321;
  msg::get_config_reply sut{flags, miss_send_len, xid};
  std::vector<unsigned char> bin
    = "\x01\x08\x00\x0c\x12\x34\x56\x78""\x00\x01\x43\x21"_bin;
};

struct set_config_fixture {
  std::uint32_t xid = 0x12345678;
  std::uint16_t flags = protocol::OFPC_FRAG_DROP;
  std::uint16_t miss_send_len = 0x1010;
  msg::set_config sut{flags, miss_send_len, xid};
  std::vector<unsigned char> bin
    = "\x01\x09\x00\x0c\x12\x34\x56\x78""\x00\x01\x10\x10"_bin;
};

}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(get_config_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      auto const sut = msg::get_config_request{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_GET_CONFIG_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = std::uint32_t{1234};

      auto const sut = msg::get_config_request{xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = msg::get_config_request{0x01020304};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_xid_is_equal)
    {
      auto const xid = 0x12345678;

      BOOST_TEST(
          (msg::get_config_request{xid} == msg::get_config_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::get_config_request{1} != msg::get_config_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, get_config_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, get_config_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_get_config_request_from_binary)
    {
      auto it = bin.begin();

      auto const get_config_request
        = msg::get_config_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((get_config_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // get_config_request


BOOST_AUTO_TEST_SUITE(get_config_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_flags_and_length)
    {
      auto const flags = protocol::OFPC_FRAG_REASM;
      auto const miss_send_len = 64;

      auto const sut = msg::get_config_reply{flags, miss_send_len};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_GET_CONFIG_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_config));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.miss_send_length() == miss_send_len);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = std::uint32_t{5678};
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const miss_send_len = 1024;

      auto const sut = msg::get_config_reply{flags, miss_send_len, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_config));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.miss_send_length() == miss_send_len);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_get_config_request)
    {
      auto const request = msg::get_config_request{6535};
      auto const flags = protocol::OFPC_FRAG_DROP;
      auto const miss_send_len = 512;

      auto const sut = msg::get_config_reply{request, flags, miss_send_len};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_config));
      BOOST_TEST(sut.xid() == request.xid());
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.miss_send_length() == miss_send_len);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut
        = msg::get_config_reply{protocol::OFPC_FRAG_REASM, 0x1111, 0x12345678};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const miss_send_len = 0x2211;
      auto const xid = 0x01020304;

      BOOST_TEST(
          (msg::get_config_reply{flags, miss_send_len, xid}
        == msg::get_config_reply{flags, miss_send_len, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flags_is_not_equal)
    {
      auto const miss_send_len = 0x2211;
      auto const xid = 0x01020304;

      BOOST_TEST(
          (msg::get_config_reply{protocol::OFPC_FRAG_REASM, miss_send_len, xid}
        != msg::get_config_reply{protocol::OFPC_FRAG_DROP, miss_send_len, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_miss_send_len_is_not_equal)
    {
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const xid = 0x01020304;

      BOOST_TEST(
          (msg::get_config_reply{flags, 1, xid}
        != msg::get_config_reply{flags, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const miss_send_len = 0x2211;

      BOOST_TEST(
          (msg::get_config_reply{flags, miss_send_len, 1}
        != msg::get_config_reply{flags, miss_send_len, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, get_config_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, get_config_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_get_config_reply_from_binary)
    {
      auto it = bin.begin();

      auto const get_config_reply
        = msg::get_config_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((get_config_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // get_config_reply


BOOST_AUTO_TEST_SUITE(set_config)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_flags_and_length)
    {
      auto const flags = protocol::OFPC_FRAG_DROP;
      auto const miss_send_len = std::numeric_limits<std::uint16_t>::max();

      auto const sut = msg::set_config{flags, miss_send_len};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_SET_CONFIG);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_config));
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.miss_send_length() == miss_send_len);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = std::uint32_t{0x12345678};
      auto const flags = protocol::OFPC_FRAG_DROP;
      auto const miss_send_len = std::numeric_limits<std::uint16_t>::max();

      auto const sut = msg::set_config{flags, miss_send_len, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_config));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.miss_send_length() == miss_send_len);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut
        = msg::set_config{protocol::OFPC_FRAG_REASM, 0x1111, 0x12345678};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const miss_send_len = 0x2211;
      auto const xid = 0x01020304;

      BOOST_TEST(
          (msg::set_config{flags, miss_send_len, xid}
        == msg::set_config{flags, miss_send_len, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flags_is_not_equal)
    {
      auto const miss_send_len = 0x2211;
      auto const xid = 0x01020304;

      BOOST_TEST(
          (msg::set_config{protocol::OFPC_FRAG_REASM, miss_send_len, xid}
        != msg::set_config{protocol::OFPC_FRAG_DROP, miss_send_len, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_miss_send_len_is_not_equal)
    {
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const xid = 0x01020304;

      BOOST_TEST(
          (msg::set_config{flags, 1, xid}
        != msg::set_config{flags, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      auto const flags = protocol::OFPC_FRAG_NORMAL;
      auto const miss_send_len = 0x2211;

      BOOST_TEST(
          (msg::set_config{flags, miss_send_len, 1}
        != msg::set_config{flags, miss_send_len, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, set_config_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, set_config_fixture)
    BOOST_AUTO_TEST_CASE(constructs_set_config_from_binary)
    {
      auto it = bin.begin();

      auto const set_config = msg::set_config::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((set_config == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // set_config

BOOST_AUTO_TEST_SUITE_END() // message_test
