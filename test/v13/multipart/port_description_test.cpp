#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/port_description.hpp>
#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/io/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace multipart = v13::messages::multipart;
namespace protocol = v13::protocol;

namespace {

  using body_type = multipart::port_description_reply::body_type;

  struct port_description_request_fixture {
    multipart::port_description_request sut{0x12345678};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x10\x12\x34\x56\x78""\x00\x0d\x00\x00\x00\x00\x00\x00"
        ""_bin
        ;
  };

  struct port_fixture {
    v13::port port1 = v13::port::from_ofp_port({
          0x1
        , {0}, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}, {0}, "eth1"
        , protocol::OFPPC_PORT_DOWN
        , protocol::OFPPS_LINK_DOWN
        , protocol::OFPPF_100GB_FD | protocol::OFPPF_FIBER
        , protocol::OFPPF_10MB_HD | protocol::OFPPF_100MB_HD
        | protocol::OFPPF_100GB_FD | protocol::OFPPF_AUTONEG
        , protocol::OFPPF_10MB_HD | protocol::OFPPF_100MB_HD
        | protocol::OFPPF_100GB_FD | protocol::OFPPF_AUTONEG
        , protocol::OFPPF_10MB_FD | protocol::OFPPF_100MB_FD
        | protocol::OFPPF_100GB_FD | protocol::OFPPF_AUTONEG
        , 0xf1f2f3f4, 0xf5f6f7f8
    });
    v13::port port2 = v13::port::from_ofp_port({
          0x2
        , {0}, {0x11, 0x12, 0x13, 0x14, 0x15, 0x16}, {0}, "eth2"
        , 0
        , 0
        , protocol::OFPPF_100MB_FD | protocol::OFPPF_COPPER
        , protocol::OFPPF_10MB_FD | protocol::OFPPF_100MB_FD
        | protocol::OFPPF_AUTONEG
        , protocol::OFPPF_10MB_FD | protocol::OFPPF_100MB_FD
        | protocol::OFPPF_AUTONEG
        , protocol::OFPPF_100MB_FD | protocol::OFPPF_100GB_FD
        | protocol::OFPPF_AUTONEG
            , 0xe1e2e3e4, 0xe5e6e7e8
    });
    v13::port port3 = v13::port::from_ofp_port({
          protocol::OFPP_MAX
        , {0}, {0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6}, {0}, "eth100"
        , protocol::OFPPC_NO_RECV | protocol::OFPPC_NO_PACKET_IN
        , protocol::OFPPS_BLOCKED | protocol::OFPPS_LIVE
        , protocol::OFPPF_1TB_FD | protocol::OFPPF_FIBER
        , protocol::OFPPF_100GB_FD | protocol::OFPPF_1TB_FD
        | protocol::OFPPF_AUTONEG
        , protocol::OFPPF_100GB_FD | protocol::OFPPF_1TB_FD
        | protocol::OFPPF_AUTONEG
        , protocol::OFPPF_40GB_FD | protocol::OFPPF_1TB_FD
        | protocol::OFPPF_AUTONEG
        , 0x01020304, 0x05060708
    });
  };

  struct port_description_reply_parameter : port_fixture {
    body_type body{port1, port2, port3};
    std::uint16_t flags = protocol::OFPMPF_REPLY_MORE;
    std::uint32_t xid = 0x12345678;
  };
  struct port_description_reply_fixture : port_description_reply_parameter {
    multipart::port_description_reply sut{body, flags, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\xd0\x12\x34\x56\x78""\x00\x0d\x00\x01\x00\x00\x00\x00"
        "\x00\x00\x00\x01\x00\x00\x00\x00""\x01\x02\x03\x04\x05\x06\x00\x00"
        "e" "t" "h" "1" "\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x01\x00\x00\x00\x01""\x00\x00\x11\x00\x00\x00\x21\x05"
        "\x00\x00\x21\x05\x00\x00\x21\x0a""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x02\x00\x00\x00\x00""\x11\x12\x13\x14\x15\x16\x00\x00"
        "e" "t" "h" "2" "\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x08\x08\x00\x00\x20\x0a"
        "\x00\x00\x20\x0a\x00\x00\x21\x08""\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8"
        "\xff\xff\xff\x00\x00\x00\x00\x00""\xf1\xf2\xf3\xf4\xf5\xf6\x00\x00"
        "e" "t" "h" "1" "0" "0" "\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x44\x00\x00\x00\x06""\x00\x00\x12\x00\x00\x00\x23\x00"
        "\x00\x00\x23\x00\x00\x00\x22\x80""\x01\x02\x03\x04\x05\x06\x07\x08"
        ""_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(port_description_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      multipart::port_description_request const sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_PORT_DESC);
      BOOST_TEST(sut.flags() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::port_description_request{1};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = 234;

      BOOST_TEST(
          (multipart::port_description_request{xid}
        == multipart::port_description_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_description_request{1}
        != multipart::port_description_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, port_description_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        construct_from_binary, port_description_request_fixture)
    {
      auto it = bin.begin();

      auto const port_description_request
        = multipart::port_description_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_description_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // port_description_request


BOOST_AUTO_TEST_SUITE(port_description_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible, port_fixture)
    {
      auto const ports = ::body_type{port1, port2};

      multipart::port_description_reply const sut{ports};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_reply)
          + v13::port::length() * ports.size());
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_PORT_DESC);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.body() == ports));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, port_description_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, port_description_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, port_description_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::port_description_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::port_description_reply{body, flags, xid}
        == multipart::port_description_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_description_reply{body_type{port1}, flags, xid}
        != multipart::port_description_reply{body_type{port2}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_description_reply{body, 0, xid}
        != multipart::port_description_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::port_description_reply{body, flags, 1}
        != multipart::port_description_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, port_description_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        construct_from_binary, port_description_reply_fixture)
    {
      auto it = bin.begin();

      auto const port_description_reply
        = multipart::port_description_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port_description_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // port_description_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
