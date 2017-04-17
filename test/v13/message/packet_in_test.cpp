#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/packet_in.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <utility>
#include <vector>
#include <canard/net/ofp/v13/openflow.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace match = v13::oxm_match_fields;
namespace protocol = v13::protocol;

namespace {

  static constexpr std::size_t data_padding_size = 2;
  static constexpr std::size_t base_packet_in_size
    = sizeof(protocol::ofp_packet_in)
    + sizeof(protocol::ofp_match) + data_padding_size;

  struct packet_in_parameter {
    std::uint32_t buffer_id = 0x01234567;
    std::uint16_t total_len = 512;
    protocol::packet_in_reason reason = protocol::OFPR_NO_MATCH;
    std::uint8_t table_id = 2;
    std::uint64_t cookie = 0x0001020304050607;
    match::in_port in_port{1};
    match::eth_dst eth_dst{"\x01\x02\x03\x04\x05\x06"_mac};
    match::eth_src eth_src{"\x11\x12\x13\x14\x15\x16"_mac};
    match::eth_type eth_type{0x0800, 0x0800};
    v13::oxm_match match{in_port, eth_dst, eth_src, eth_type};
    v13::messages::packet_in::data_type data
      = "\x00\xff\x01\xff\x02\xff\x03\xff\x04\xff\x05\xff\x06\xff\x07\xff"_bbin;
    std::uint32_t xid = 0x00010203;
  };
  struct packet_in_fixture : packet_in_parameter {
    v13::messages::packet_in sut{
      buffer_id, total_len, reason, table_id, cookie, match, data, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x0a\x00\x52\x00\x01\x02\x03" "\x01\x23\x45\x67\x02\x00\x00\x02"
        "\x00\x01\x02\x03\x04\x05\x06\x07"
        "\x00\x01\x00\x28\x80\x00\x00\x04"
        "\x00\x00\x00\x01\x80\x00\x06\x06" "\x01\x02\x03\x04\x05\x06\x80\x00"
        "\x08\x06\x11\x12\x13\x14\x15\x16" "\x80\x00\x0b\x04\x08\x00\x08\x00"

        "\x00\x00\x00\xff\x01\xff\x02\xff" "\x03\xff\x04\xff\x05\xff\x06\xff"
        "\x07\xff"_bin
        ;
  };
  struct no_data_packet_in : packet_in_parameter {
    v13::oxm_match match{in_port};
    v13::messages::packet_in sut{
      buffer_id, total_len, reason, table_id, cookie, match, ""_bbin, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x0a\x00\x2a\x00\x01\x02\x03" "\x01\x23\x45\x67\x02\x00\x00\x02"
        "\x00\x01\x02\x03\x04\x05\x06\x07"
        "\x00\x01\x00\x0c\x80\x00\x00\x04" "\x00\x00\x00\x01\x00\x00\x00\x00"
        "\x00\x00"_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(packet_in)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_binary)
    {
      auto const buffer_id = protocol::OFP_NO_BUFFER;
      auto const total_len = std::uint16_t{1514};
      auto const reason = protocol::OFPR_NO_MATCH;
      auto const table_id = std::uint8_t{0};
      auto const cookie = std::uint64_t{0xffffffffffffffff};
      auto const match = v13::oxm_match{
          match::in_port{1}
        , match::eth_dst{"\x01\x02\x03\x04\x05\x06"_mac}
        , match::eth_src{"\x11\x12\x13\x14\x15\x16"_mac}
        , match::eth_type{0x0800, 0x0800}
      };
      auto const data = "\x00\x01\x02\x03\x04"_bbin;

      v13::messages::packet_in const sut{
        buffer_id, total_len, reason, table_id, cookie, match, std::move(data)
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_PACKET_IN);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_packet_in)
          + of::detail::v13::exact_length(match.length())
          + data_padding_size + data.size());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.total_length() == total_len);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST(sut.frame() == data, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_from_range)
    {
      auto const buffer_id = 0;
      auto const total_len = std::uint16_t{0};
      auto const reason = protocol::OFPR_ACTION;
      auto const table_id = std::uint8_t{0xff};
      auto const cookie = std::uint64_t{0};
      auto const match = v13::oxm_match{};
      auto const data = "\x09\x08\x07\x06\x05\x04"_bbin;

      v13::messages::packet_in const sut{
        buffer_id, total_len, reason, table_id, cookie, match, data
      };

      BOOST_TEST(sut.length() == base_packet_in_size + data.size());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.total_length() == total_len);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST(sut.frame() == data, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(constructible_without_data)
    {
      auto const buffer_id = 0;
      auto const total_len = std::uint16_t{0};
      auto const reason = protocol::OFPR_INVALID_TTL;
      auto const table_id = std::uint8_t{0xff};
      auto const cookie = std::uint64_t{0};
      auto const match = v13::oxm_match{
        match::eth_src{"\x11\x12\x13\x14\x15\x16"_mac}
      };
      auto const data = v13::messages::packet_in::data_type{};

      v13::messages::packet_in const sut{
        buffer_id, total_len, reason, table_id, cookie, match, data
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_packet_in)
          + of::detail::v13::exact_length(match.length())
          + data_padding_size);
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST(sut.total_length() == total_len);
      BOOST_TEST(sut.reason() == reason);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.frame_length() == data.size());
      BOOST_TEST(sut.frame() == data, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, packet_in_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, packet_in_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == base_packet_in_size);
      BOOST_TEST((moved.match() == v13::oxm_match{}));
      BOOST_TEST(moved.frame().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, packet_in_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = v13::messages::packet_in{
        0, 0, protocol::OFPR_ACTION, 0, 0, {}, ""
      };

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = v13::messages::packet_in{
        0, 0, protocol::OFPR_INVALID_TTL, 0, 0, {}, ""
      };
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == base_packet_in_size);
      BOOST_TEST(moved.frame().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, packet_in_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::packet_in{
        buffer_id, total_len, reason, table_id, cookie, match, data, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
             buffer_id, total_len, reason, table_id, cookie, match, data, xid
           }
        == v13::messages::packet_in{
             buffer_id, total_len, reason, table_id, cookie, match, data, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_buffer_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
             1, total_len, reason, table_id, cookie, match, data, xid
           }
        != v13::messages::packet_in{
             2, total_len, reason, table_id, cookie, match, data, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_total_len_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
             buffer_id, 1, reason, table_id, cookie, match, data, xid
           }
        != v13::messages::packet_in{
             buffer_id, 2, reason, table_id, cookie, match, data, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_reason_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
               buffer_id, total_len, protocol::packet_in_reason::action
             , table_id, cookie, match, data, xid
           }
        != v13::messages::packet_in{
               buffer_id, total_len, protocol::packet_in_reason::invalid_ttl
             , table_id, cookie, match, data, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
             buffer_id, total_len, reason, 1, cookie, match, data, xid
           }
        != v13::messages::packet_in{
             buffer_id, total_len, reason, 2, cookie, match, data, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
             buffer_id, total_len, reason, table_id, 1, match, data, xid
           }
        != v13::messages::packet_in{
             buffer_id, total_len, reason, table_id, 2, match, data, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
               buffer_id, total_len, reason, table_id, cookie
             , v13::oxm_match{eth_dst}, data, xid
           }
        != v13::messages::packet_in{
               buffer_id, total_len, reason, table_id, cookie
             , v13::oxm_match{eth_src}, data, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_data_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
             buffer_id, total_len, reason, table_id, cookie, match, "1", xid
           }
        != v13::messages::packet_in{
             buffer_id, total_len, reason, table_id, cookie, match, "2", xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::packet_in{
             buffer_id, total_len, reason, table_id, cookie, match, data, 1
           }
        != v13::messages::packet_in{
             buffer_id, total_len, reason, table_id, cookie, match, data, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, packet_in_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, packet_in_fixture)
    {
      auto it = bin.begin();

      auto const packet_in = v13::messages::packet_in::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((packet_in == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_no_data_binary, no_data_packet_in)
    {
      auto it = bin.begin();

      auto const packet_in = v13::messages::packet_in::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((packet_in == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

  BOOST_AUTO_TEST_SUITE(extract_frame)
    BOOST_FIXTURE_TEST_CASE(move_frame_ownership, packet_in_fixture)
    {
      auto const org_packet_in = sut;

      auto const frame = sut.extract_frame();

      BOOST_TEST(sut.length() == org_packet_in.length() - frame.size());
      BOOST_TEST(sut.frame_length() == 0);
      BOOST_TEST(sut.frame().empty());
      BOOST_TEST((frame == org_packet_in.frame()));
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_frame
BOOST_AUTO_TEST_SUITE_END() // packet_in
BOOST_AUTO_TEST_SUITE_END() // message_test

