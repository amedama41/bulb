#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/message/hello.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <type_traits>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace msg = v13::messages;
namespace helems = v13::hello_elements;
namespace detail = v13::v13_detail;

using proto = v13::protocol;

namespace {
struct parameters {
  helems::versionbitmap versionbitmap{{0x00000012}};
  helems::unknown_element unknown_element{2};
  std::uint8_t version = versionbitmap.max_support_version();
  std::vector<v13::any_hello_element> elements{versionbitmap, unknown_element};
  std::uint32_t xid = 0x12345678;
};
struct hello_fixture : parameters {
  msg::hello sut{version, elements, xid};
  std::vector<unsigned char> bin
    = "\x04\x00\x00\x18\x12\x34\x56\x78"
      "\x00\x01\x00\x08\x00\x00\x00\x12"
      "\x00\x02\x00\x04\x00\x00\x00\x00"
      ""_bin;
};
struct no_elements_fixture {
  std::uint8_t version = 0x12;
  std::uint32_t xid = 0x01020304;
  msg::hello sut{version, xid};
  std::vector<unsigned char> bin = "\x12\x00\x00\x08\x01\x02\x03\x04"_bin;
};
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(hello)

  BOOST_AUTO_TEST_CASE(type_test)
  {
    using sut = msg::hello;

    using msg_type = std::integral_constant<std::uint8_t, sut::type()>;

    BOOST_TEST(msg_type::value == proto::OFPT_HELLO);
  }

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_version_and_xid)
    {
      auto const version = 1;
      auto const xid = 0x12345678;

      msg::hello sut{version, xid};

      BOOST_TEST(sut.version() == version);
      BOOST_TEST(sut.length() == sizeof(detail::ofp_hello));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.elements().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_version)
    {
      auto const version = 13;

      msg::hello sut{version};

      BOOST_TEST(sut.version() == version);
      BOOST_TEST(sut.length() == sizeof(detail::ofp_hello));
      BOOST_TEST(sut.elements().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_versionbitmap_and_xid)
    {
      auto const versionbitmap = helems::versionbitmap{{0x00000012}};
      auto const xid = 0x01020304;

      msg::hello sut{versionbitmap, xid};

      BOOST_TEST(sut.version() == versionbitmap.max_support_version());
      BOOST_TEST(sut.length() == 16);
      BOOST_TEST(sut.elements().size() == 1);
      BOOST_TEST_REQUIRE(bool(sut.find<helems::versionbitmap>()));
      BOOST_TEST((*sut.find<helems::versionbitmap>() == versionbitmap));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_version_and_hello_elements_and_xid, parameters)
    {
      msg::hello sut{version, elements, xid};

      BOOST_TEST(sut.version() == version);
      BOOST_TEST(sut.length() == 8 + 8 + 8);
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.elements() == elements));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_version_and_hello_elements_and, parameters)
    {
      msg::hello sut{version, elements};

      BOOST_TEST(sut.version() == version);
      BOOST_TEST(sut.length() == 8 + 8 + 8);
      BOOST_TEST((sut.elements() == elements));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_empty_hello_elements)
    {
      auto const version = std::uint8_t{1};
      auto const elements = std::vector<v13::any_hello_element>{};
      auto const xid = 0x11223344;

      msg::hello sut{version, elements, xid};

      BOOST_TEST(sut.version() == version);
      BOOST_TEST(sut.length() == sizeof(detail::ofp_hello));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST((sut.elements() == elements));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, hello_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, hello_fixture)
    {
      auto const& const_hello = sut;

      auto const copy = const_hello;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, hello_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(detail::ofp_hello));
      BOOST_TEST(moved.elements().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generates_binary, hello_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
          generates_no_elements_binary_from_no_elements_hello
        , no_elements_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructs_hello_from_binary, hello_fixture)
    {
      auto it = bin.begin();

      auto const hello = msg::hello::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((hello == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructs_hello_from_too_long_binary, hello_fixture)
    {
      auto const size = bin.size();
      bin.resize(size + 10, 0);
      auto it = bin.begin();

      auto const hello = msg::hello::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), size)));
      BOOST_TEST((hello == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          constructs_no_elements_hello_from_no_elements_binary
        , no_elements_fixture)
    {
      auto it = bin.begin();

      auto const hello = msg::hello::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((hello == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // hello
BOOST_AUTO_TEST_SUITE_END() // message_test
