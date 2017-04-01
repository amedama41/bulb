#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/decoder/flow_mod_decoder.hpp>
#include <boost/test/unit_test.hpp>

#include <type_traits>
#include "../../test_utility.hpp"
#include "../fixture.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace protocol = v10::protocol;
namespace msg = v10::messages;

namespace {
  struct parameters : match_fixture, action_fixture
  {
    v10::match match{
      in_port, eth_src, vlan_vid, eth_type, ip_proto, ipv4_dst, tcp_src
    };
    std::uint16_t priority = 0x4321;
    std::uint64_t cookie = 0x0102030405060708;
    v10::action_list actions{
      set_vlan_vid, output, strip_vlan, enqueue, set_eth_src, set_ipv4_src, output
    };
    v10::timeouts timeouts{0x3232, 0x8989};
    std::uint16_t flags
      = protocol::OFPFF_SEND_FLOW_REM | protocol::OFPFF_CHECK_OVERLAP;
    std::uint32_t buffer_id = 0x01020304;
    std::uint16_t out_port = 0x1234;
    std::uint32_t xid = 0x12345678;
  };

  struct flow_add_fixture : parameters
  {
    msg::flow_add flow_add{
      match, priority, cookie, actions, timeouts, flags, buffer_id, xid
    };
    std::vector<unsigned char> flow_add_bin
      = "\x01\x0e\x00\x90\x12\x34\x56\x78"
        "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
        "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
        "\xc0\xa8\x01\x00\x18\x23\x00\x00"
        "\x01\x02\x03\x04\x05\x06\x07\x08""\x00\x00\x32\x32\x89\x89\x43\x21"
        "\x01\x02\x03\x04\x00\x00\x00\x03"
        "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"
        "\x00\x03\x00\x08\x00\x00\x00\x00""\x00\x0b\x00\x10\x00\x01\x00\x00"
        "\x00\x00\x00\x00\x12\x34\x56\x78""\x00\x04\x00\x10\x01\x02\x03\x04"
        "\x05\x06\x00\x00\x00\x00\x00\x00""\x00\x06\x00\x08\xc0\xa8\x01\x03"
        "\x00\x00\x00\x08\xff\xfd\x00\x40"
        ""_bin;
  };

  struct flow_modify_fixture : parameters
  {
    msg::flow_modify flow_modify{match, cookie, actions, buffer_id, xid};
    std::vector<unsigned char> flow_modify_bin
      = "\x01\x0e\x00\x90\x12\x34\x56\x78"
        "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
        "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
        "\xc0\xa8\x01\x00\x18\x23\x00\x00"
        "\x01\x02\x03\x04\x05\x06\x07\x08""\x00\x01\x00\x00\x00\x00\x00\x00"
        "\x01\x02\x03\x04\x00\x00\x00\x00"
        "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"
        "\x00\x03\x00\x08\x00\x00\x00\x00""\x00\x0b\x00\x10\x00\x01\x00\x00"
        "\x00\x00\x00\x00\x12\x34\x56\x78""\x00\x04\x00\x10\x01\x02\x03\x04"
        "\x05\x06\x00\x00\x00\x00\x00\x00""\x00\x06\x00\x08\xc0\xa8\x01\x03"
        "\x00\x00\x00\x08\xff\xfd\x00\x40"
        ""_bin;
  };

  struct flow_modify_strict_fixture : parameters
  {
    msg::flow_modify_strict flow_modify_strict{
      match, priority, cookie, actions, buffer_id, xid
    };
    std::vector<unsigned char> flow_modify_strict_bin
      = "\x01\x0e\x00\x90\x12\x34\x56\x78"
        "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
        "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
        "\xc0\xa8\x01\x00\x18\x23\x00\x00"
        "\x01\x02\x03\x04\x05\x06\x07\x08""\x00\x02\x00\x00\x00\x00\x43\x21"
        "\x01\x02\x03\x04\x00\x00\x00\x00"
        "\x00\x01\x00\x08\x01\x23\x00\x00""\x00\x00\x00\x08\xff\xfd\x00\x40"
        "\x00\x03\x00\x08\x00\x00\x00\x00""\x00\x0b\x00\x10\x00\x01\x00\x00"
        "\x00\x00\x00\x00\x12\x34\x56\x78""\x00\x04\x00\x10\x01\x02\x03\x04"
        "\x05\x06\x00\x00\x00\x00\x00\x00""\x00\x06\x00\x08\xc0\xa8\x01\x03"
        "\x00\x00\x00\x08\xff\xfd\x00\x40"
        ""_bin;
  };

  struct flow_delete_fixture : parameters
  {
    msg::flow_delete flow_delete{match, out_port, xid};
    std::vector<unsigned char> flow_delete_bin
      = "\x01\x0e\x00\x48\x12\x34\x56\x78"
        "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
        "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
        "\xc0\xa8\x01\x00\x18\x23\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x03\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x12\x34\x00\x00"
        ""_bin;
  };

  struct flow_delete_strict_fixture : parameters
  {
    msg::flow_delete_strict flow_delete_strict{match, priority, out_port, xid};
    std::vector<unsigned char> flow_delete_strict_bin
      = "\x01\x0e\x00\x48\x12\x34\x56\x78"
        "\x00\x31\x3f\x88\x00\x01\x01\x02""\x03\x04\x05\x06\x00\x00\x00\x00"
        "\x00\x00\x01\x23\x00\x00\x08\x00""\x00\x07\x00\x00\x00\x00\x00\x00"
        "\xc0\xa8\x01\x00\x18\x23\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x00\x04\x00\x00\x00\x00\x43\x21"
        "\x00\x00\x00\x00\x12\x34\x00\x00"
        ""_bin;
  };

  struct discard
  {
    template <class T>
    void operator()(T&&) const {}
  };

  template <class T = char>
  struct to_optional
  {
    using result_type = boost::optional<T>;
    auto operator()(T const& t) const -> result_type
    { return result_type{t}; }
    template <class U>
    auto operator()(U const&) const -> result_type
    { return boost::none; }
  };
}

BOOST_AUTO_TEST_SUITE(decoder_test)
BOOST_AUTO_TEST_SUITE(flow_mod_decoder)

  BOOST_AUTO_TEST_SUITE(type_definition_test)
    BOOST_AUTO_TEST_CASE(header_type)
    {
      using sut = v10::flow_mod_decoder;

      using is_same = std::is_same<
          sut::header_type
        , protocol::ofp_flow_mod
      >;

      BOOST_TEST(is_same::value);
    }
    BOOST_AUTO_TEST_CASE(type_id)
    {
      using sut = v10::flow_mod_decoder;

      using is_same = std::is_same<
          sut::type_id
        , decltype(protocol::ofp_flow_mod::command)
      >;

      BOOST_TEST(is_same::value);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition_test

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
          construct_flow_add_if_binary_represent_flow_add
        , flow_add_fixture)
    {
      auto it = flow_add_bin.begin();
      using f = to_optional<msg::flow_add>;

      auto const flow_mod = v10::flow_mod_decoder::decode<f::result_type>(
          it, flow_add_bin.end(), f{});

      BOOST_TEST_REQUIRE(bool(flow_mod));
      BOOST_TEST((*flow_mod == flow_add));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_flow_modify_if_binary_represent_flow_modify
        , flow_modify_fixture)
    {
      auto it = flow_modify_bin.begin();
      using f = to_optional<msg::flow_modify>;

      auto const flow_mod = v10::flow_mod_decoder::decode<f::result_type>(
          it, flow_modify_bin.end(), f{});

      BOOST_TEST_REQUIRE(bool(flow_mod));
      BOOST_TEST((*flow_mod == flow_modify));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_flow_modify_strict_if_binary_represent_flow_modify_strict
        , flow_modify_strict_fixture)
    {
      auto it = flow_modify_strict_bin.begin();
      using f = to_optional<msg::flow_modify_strict>;

      auto const flow_mod = v10::flow_mod_decoder::decode<f::result_type>(
          it, flow_modify_strict_bin.end(), f{});

      BOOST_TEST_REQUIRE(bool(flow_mod));
      BOOST_TEST((*flow_mod == flow_modify_strict));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_flow_delete_if_binary_represent_flow_delete
        , flow_delete_fixture)
    {
      auto it = flow_delete_bin.begin();
      using f = to_optional<msg::flow_delete>;

      auto const flow_mod = v10::flow_mod_decoder::decode<f::result_type>(
          it, flow_delete_bin.end(), f{});

      BOOST_TEST_REQUIRE(bool(flow_mod));
      BOOST_TEST((*flow_mod == flow_delete));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_flow_delete_strict_if_binary_represent_flow_delete_strict
        , flow_delete_strict_fixture)
    {
      auto it = flow_delete_strict_bin.begin();
      using f = to_optional<msg::flow_delete_strict>;

      auto const flow_mod = v10::flow_mod_decoder::decode<f::result_type>(
          it, flow_delete_strict_bin.end(), f{});

      BOOST_TEST_REQUIRE(bool(flow_mod));
      BOOST_TEST((*flow_mod == flow_delete_strict));
    }
    BOOST_FIXTURE_TEST_CASE(
        throw_exception_if_command_is_unknown, flow_add_fixture)
    {
      flow_add_bin[56] = 0xff;

      BOOST_CHECK_EXCEPTION(
            v10::flow_mod_decoder::decode_without_consumption<void>(
              flow_add_bin.begin(), flow_add_bin.end(), discard{})
          , v10::exception
          , [](v10::exception const& e) {
              return e.error_type() == protocol::error_type::flow_mod_failed
                  && e.error_code() == protocol::flow_mod_failed_code::bad_command;
            });
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_mod_decoder
BOOST_AUTO_TEST_SUITE_END() // decoder_test
