#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/flow_add.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/instructions.hpp>
#include <canard/net/ofp/v13/common/oxm_match.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace match = v13::oxm_match_fields;
namespace msg = v13::messages;
namespace protocol = v13::protocol;

namespace {

  struct flow_entry_parameter {
    canard::mac_address eth1 = "\x01\x02\x03\x04\x05\x06"_mac;
    canard::mac_address eth2 = "\x11\x12\x13\x14\x15\x16"_mac;
    match::in_port in_port{4};
    match::eth_dst eth_dst{eth1};
    match::eth_src eth_src{eth2};
    v13::oxm_match match{in_port, eth_dst, eth_src}; // 4 + 8 + 10 + 10 = 32
    std::uint16_t priority = protocol::OFP_DEFAULT_PRIORITY;
    std::uint64_t cookie = 0xf1f2f3f4f5f6f7f8;
    v13::actions::set_eth_dst set_eth_dst{eth2};
    v13::actions::set_eth_src set_eth_src{eth1};
    v13::instructions::apply_actions apply_actions{
      set_eth_dst, set_eth_src, v13::actions::output{4}
    }; // 8 + 16 + 16 + 16 = 56
    v13::instructions::clear_actions clear_actions{}; // 8
    v13::instructions::write_actions write_actions{
      set_eth_dst, set_eth_src
    }; // 8 + 16 + 16 = 40
    v13::instruction_set instructions{
      apply_actions, clear_actions, write_actions
    }; // 56 + 8 + 40 = 104
  };
  struct flow_entry_fixture : flow_entry_parameter {
    v13::flow_entry entry = {match, priority, cookie, instructions};
  };
  struct flow_add_parameter : flow_entry_fixture {
    std::uint8_t table_id = 0;
    v13::timeouts timeouts{0x1234, 0x5678};
    std::uint16_t flags
      = protocol::OFPFF_SEND_FLOW_REM | protocol::OFPFF_NO_BYT_COUNTS;
    std::uint32_t buffer_id = 0xff12ff34;
    std::uint32_t xid = 0xff56ff78;
  };
  struct flow_add_fixture : flow_add_parameter {
    msg::flow_add sut{entry, table_id, timeouts, flags, buffer_id, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x0e\x00\xb8\xff\x56\xff\x78" "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x00\x00\x00\x00\x00" "\x00\x00\x12\x34\x56\x78\x80\x00"
        "\xff\x12\xff\x34\x00\x00\x00\x00" "\x00\x00\x00\x00\x00\x11\x00\x00"

        "\x00\x01\x00\x20\x80\x00\x00\x04" "\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00" "\x08\x06\x11\x12\x13\x14\x15\x16"

        "\x00\x04\x00\x38\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06" "\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06" "\x01\x02\x03\x04\x05\x06\x00\x00"
        "\x00\x00\x00\x10\x00\x00\x00\x04" "\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x05\x00\x08\x00\x00\x00\x00"
        "\x00\x03\x00\x28\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06" "\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06" "\x01\x02\x03\x04\x05\x06\x00\x00"_bin
        ;
  };

  static auto const base_flow_mod_size
    = sizeof(protocol::ofp_flow_mod) + sizeof(protocol::ofp_match);

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(flow_add)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_lvalue_entry, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{1};
      auto const flags = std::uint16_t{
        protocol::OFPFF_SEND_FLOW_REM | protocol::OFPFF_CHECK_OVERLAP
      };
      auto const timeouts = v13::timeouts{32, 24};
      auto const buffer_id = 3;

      msg::flow_add const sut{entry, table_id, timeouts, flags, buffer_id};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == 48 + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.idle_timeout() == timeouts.idle_timeout());
      BOOST_TEST(sut.hard_timeout() == timeouts.hard_timeout());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(constructible_from_rvalue_entry, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{1};
      auto const flags = std::uint16_t{
        protocol::OFPFF_SEND_FLOW_REM | protocol::OFPFF_CHECK_OVERLAP
      };
      auto const timeouts = v13::timeouts{32, 24};
      auto const buffer_id = 3;
      auto moved_entry = entry;

      msg::flow_add const sut{
        std::move(moved_entry), table_id, timeouts, flags, buffer_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == 48 + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.idle_timeout() == timeouts.idle_timeout());
      BOOST_TEST(sut.hard_timeout() == timeouts.hard_timeout());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == entry.instructions()));

      BOOST_TEST(moved_entry.match().length() == 4);
      BOOST_TEST(moved_entry.instructions().empty());
    }
    BOOST_FIXTURE_TEST_CASE(constructible_from_table_id, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{127};

      msg::flow_add const sut{entry, table_id};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == 48 + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.idle_timeout() == 0);
      BOOST_TEST(sut.hard_timeout() == 0);
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(constructible_from_timeouts, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{254};
      auto const timeouts = v13::timeouts{128, 64};

      msg::flow_add const sut{entry, table_id, timeouts};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == 48 + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST(sut.idle_timeout() == timeouts.idle_timeout());
      BOOST_TEST(sut.hard_timeout() == timeouts.hard_timeout());
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(constructible_from_flags, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{0};
      auto const flags = std::uint16_t{protocol::OFPFF_CHECK_OVERLAP};

      msg::flow_add const sut{entry, table_id, flags};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == 48 + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.idle_timeout() == 0);
      BOOST_TEST(sut.hard_timeout() == 0);
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, flow_add_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, flow_add_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == base_flow_mod_size);
      BOOST_TEST(moved.match().length() == 4);
      BOOST_TEST(moved.instructions().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, flow_add_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto const empty_entry
        = v13::flow_entry{v13::oxm_match{}, 0, 0, v13::instruction_set{}};
      auto copy = msg::flow_add{empty_entry, 0, 0};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto const empty_entry
        = v13::flow_entry{v13::oxm_match{}, 0, 0, v13::instruction_set{}};
      auto copy = msg::flow_add{empty_entry, 0, 0};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == base_flow_mod_size);
      BOOST_TEST(moved.match().length() == 4);
      BOOST_TEST(moved.instructions().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, flow_add_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut
        = msg::flow_add{entry, table_id, timeouts, flags, buffer_id, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (msg::flow_add{entry, table_id, timeouts, flags, buffer_id, xid}
        == msg::flow_add{entry, table_id, timeouts, flags, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{v13::flow_entry{
            v13::oxm_match{eth_src}, priority, cookie, instructions
           }, table_id, timeouts, flags, buffer_id, xid}
        != msg::flow_add{v13::flow_entry{
            v13::oxm_match{eth_dst}, priority, cookie, instructions
           }, table_id, timeouts, flags, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_priority_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{v13::flow_entry{
            v13::oxm_match{eth_src}, 1, cookie, instructions
           }, table_id, timeouts, flags, buffer_id, xid}
        != msg::flow_add{v13::flow_entry{
            v13::oxm_match{eth_src}, 2, cookie, instructions
           }, table_id, timeouts, flags, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{v13::flow_entry{
            v13::oxm_match{eth_src}, priority, 1, instructions
           }, table_id, timeouts, flags, buffer_id, xid}
        != msg::flow_add{v13::flow_entry{
            v13::oxm_match{eth_src}, priority, 2, instructions
           }, table_id, timeouts, flags, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_instructions_is_not_equal)
    {
      using v13::instructions::apply_actions;

      BOOST_TEST(
          (msg::flow_add{v13::flow_entry{
              v13::oxm_match{eth_src}, priority, cookie
            , v13::instruction_set{apply_actions{set_eth_src}}
           }, table_id, timeouts, flags, buffer_id, xid}
        != msg::flow_add{v13::flow_entry{
              v13::oxm_match{eth_src}, priority, cookie
            , v13::instruction_set{apply_actions{set_eth_dst}}
           }, table_id, timeouts, flags, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{entry, 1, timeouts, flags, buffer_id, xid}
        != msg::flow_add{entry, 2, timeouts, flags, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_idle_timeout_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{
           entry, table_id, v13::timeouts{1, 0}, flags, buffer_id, xid}
        != msg::flow_add{
           entry, table_id, v13::timeouts{2, 0}, flags, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_hard_timeout_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{
           entry, table_id, v13::timeouts{0, 1}, flags, buffer_id, xid}
        != msg::flow_add{
           entry, table_id, v13::timeouts{0, 2}, flags, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{entry, table_id, timeouts, 1, buffer_id, xid}
        != msg::flow_add{entry, table_id, timeouts, 2, buffer_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_buffer_id_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{entry, table_id, timeouts, flags, 1, xid}
        != msg::flow_add{entry, table_id, timeouts, flags, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::flow_add{entry, table_id, timeouts, flags, buffer_id, 1}
        != msg::flow_add{entry, table_id, timeouts, flags, buffer_id, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_add_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, flow_add_fixture)
    {
      auto it = bin.begin();

      auto const flow_add = msg::flow_add::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_add == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // flow_add
BOOST_AUTO_TEST_SUITE_END() // message_test

