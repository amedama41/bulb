#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/port.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10_detail = ofp::v10::v10_detail;

namespace {

struct binary_port {
  ofp::v10::port const sut = ofp::v10::port::from_ofp_port({
        0x1234, {0x01, 0x02, 0x03, 0x04, 0x05, 0x06}, "port_name"
      , 0x00000012, 0x00000301
      , 0x00000123, 0x00000456, 0x00000789, 0x00000abc
  });
  std::vector<unsigned char> const bin
    = "\x12\x34\x01\x02\x03\x04\x05\x06""port_nam"
      "e" "\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x12\x00\x00\x03\x01"
      "\x00\x00\x01\x23\x00\x00\x04\x56""\x00\x00\x07\x89\x00\x00\x0a\xbc"_bin;
  std::vector<unsigned char> const invalid_port_no_bin
    = "\x00\x00\x01\x02\x03\x04\x05\x06""port_nam"
      "e" "\x00\x00\x00\x00\x00\x00\x00""\x00\x00\x00\x12\x00\x00\x03\x01"
      "\x00\x00\x01\x23\x00\x00\x04\x56""\x00\x00\x07\x89\x00\x00\x0a\xbc"_bin;
  std::vector<unsigned char> const non_null_terminated_name_bin
    = "\x12\x34\x01\x02\x03\x04\x05\x06""01234567"
      "89abcdef"                        "\x00\x00\x00\x12\x00\x00\x03\x01"
      "\x00\x00\x01\x23\x00\x00\x04\x56""\x00\x00\x07\x89\x00\x00\x0a\xbc"_bin;
};

}

BOOST_AUTO_TEST_SUITE(common_type_test)
BOOST_AUTO_TEST_SUITE(port_test)

  BOOST_FIXTURE_TEST_SUITE(encode, binary_port)
    BOOST_AUTO_TEST_CASE(generate_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      // TODO BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, binary_port)
    BOOST_AUTO_TEST_CASE(constructible_from_binary)
    {
      auto it = bin.begin();

      auto const port = ofp::v10::port::decode(it, bin.end());

      // TODO
      BOOST_TEST(port.port_no() == sut.port_no());
      BOOST_TEST(port.hardware_address() == sut.hardware_address());
      BOOST_TEST(port.name() == sut.name());
      BOOST_TEST(port.config() == sut.config());
      BOOST_TEST(port.state() == sut.state());
      BOOST_TEST(port.current_features() == sut.current_features());
      BOOST_TEST(port.advertised_features() == sut.advertised_features());
      BOOST_TEST(port.supported_features() == sut.supported_features());
      BOOST_TEST(
          port.peer_advertised_features() == sut.peer_advertised_features());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_invalid_port_no_binary)
    {
      auto it = invalid_port_no_bin.begin();

      auto const port = ofp::v10::port::decode(it, invalid_port_no_bin.end());

      // TODO
      BOOST_TEST(port.port_no() == 0);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_non_null_terminated_name_binary)
    {
      auto it = non_null_terminated_name_bin.begin();

      auto const port
        = ofp::v10::port::decode(it, non_null_terminated_name_bin.end());

      // TODO
      (void)port;
      BOOST_TEST(true);
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // port_test
BOOST_AUTO_TEST_SUITE_END() // common_type_test
