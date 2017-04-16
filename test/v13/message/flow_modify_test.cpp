#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/flow_modify.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/instructions.hpp>
#include <canard/net/ofp/v13/common/oxm_match.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace match = v13::oxm_match_fields;
namespace protocol = v13::protocol;

namespace {

  struct flow_entry_fixture {
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
    v13::actions::output output{4};
    v13::instructions::apply_actions apply_actions{
      set_eth_dst, set_eth_src, output
    }; // 8 + 16 + 16 + 16 = 56
    v13::instructions::clear_actions clear_actions{}; // 8
    v13::instructions::write_actions write_actions{
      set_eth_dst, set_eth_src
    }; // 8 + 16 + 16 = 40
    v13::instruction_set instructions{
      apply_actions, clear_actions, write_actions
    }; // 56 + 8 + 40 = 104
    v13::flow_entry entry{match, priority, cookie, instructions};
  };

  struct flow_modify_parameter : flow_entry_fixture {
    std::uint8_t table_id = 254;
    v13::cookie_mask cookie_mask{0xf1f2f3f4f5f6f7f8, 0x0f0f0f0f0f0f0f0f};
    bool reset_counter = true;
    std::uint32_t buffer_id = protocol::OFP_NO_BUFFER;
    std::uint32_t xid = 0x12345678;
  };
  struct flow_modify_fixutre : flow_modify_parameter {
    v13::messages::flow_modify sut = {
        entry.match(), table_id, entry.instructions(), cookie_mask
      , reset_counter, buffer_id, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x0e\x00\xb8\x12\x34\x56\x78" "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f" "\xfe\x01\x00\x00\x00\x00\x00\x00"
        "\xff\xff\xff\xff\x00\x00\x00\x00" "\x00\x00\x00\x00\x00\x04\x00\x00"

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

  struct flow_modify_strict_parameter : flow_entry_fixture {
    std::uint8_t table_id = 254;
    v13::cookie_mask cookie_mask{0xf1f2f3f4f5f6f7f8, 0x0f0f0f0f0f0f0f0f};
    bool reset_counter = true;
    std::uint32_t buffer_id = protocol::OFP_NO_BUFFER;
    std::uint32_t xid = 0x12345678;
  };
  struct flow_modify_strict_fixture : flow_modify_strict_parameter {
    v13::messages::flow_modify_strict sut = {
      entry, table_id, entry.instructions(), reset_counter, buffer_id, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x0e\x00\xb8\x12\x34\x56\x78" "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\xff\xff\xff\xff\xff\xff\xff\xff" "\xfe\x02\x00\x00\x00\x00\x80\x00"
        "\xff\xff\xff\xff\x00\x00\x00\x00" "\x00\x00\x00\x00\x00\x04\x00\x00"

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

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(flow_modify)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_table_id, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{1};

      v13::messages::flow_modify const sut{
        entry.match(), table_id, entry.instructions()
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(!sut.reset_counter());
      BOOST_TEST(sut.buffer_id() == protocol::OFP_NO_BUFFER);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(constructible_without_reset, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{0};
      auto const buffer_id = std::uint32_t{0xff1234ff};

      v13::messages::flow_modify const sut{
        entry.match(), table_id, entry.instructions(), false, buffer_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(!sut.reset_counter());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(constructible_with_reset_count, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{254};
      auto const buffer_id = std::uint32_t{0xff1234ff};

      v13::messages::flow_modify const sut{
        entry.match(), table_id, entry.instructions(), true, buffer_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.reset_counter());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(constructible_from_cookie_mask, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{128};
      auto const cookie_mask
        = v13::cookie_mask{0x12ff34ff56ff78ff, 0xf0f0f0f0f0f0f0f0};
      auto const buffer_id = std::uint32_t{0xff1234ff};

      v13::messages::flow_modify const sut{
          entry.match(), table_id, entry.instructions()
        , cookie_mask, false, buffer_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.cookie() == cookie_mask.value());
      BOOST_TEST(sut.cookie_mask() == cookie_mask.mask());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(!sut.reset_counter());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_cookie_mask_with_reset_count, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{253};
      auto const cookie_mask
        = v13::cookie_mask{0x12ff34ff56ff78ff, 0xf0f0f0f0f0f0f0f0};
      auto const buffer_id = std::uint32_t{0xff1234ff};

      v13::messages::flow_modify const sut{
          entry.match(), table_id, entry.instructions()
        , cookie_mask, true, buffer_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_flow_mod) + 32 + 56 + 8 + 40);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.cookie() == cookie_mask.value());
      BOOST_TEST(sut.cookie_mask() == cookie_mask.mask());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.reset_counter());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, flow_modify_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::flow_modify{
          match, table_id, instructions, cookie_mask
        , reset_counter, buffer_id, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify{
               match, table_id, instructions, cookie_mask
             , reset_counter, buffer_id, xid
           }
        == v13::messages::flow_modify{
               match, table_id, instructions, cookie_mask
             , reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify{
               v13::oxm_match{eth_src}, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify{
               v13::oxm_match{eth_dst}, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify{
               match, 1, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify{
               match, 2, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_instructions_is_not_equal)
    {
      using v13::instructions::apply_actions;

      BOOST_TEST(
          (v13::messages::flow_modify{
               match, table_id, v13::instruction_set{apply_actions{set_eth_src}}
             , cookie_mask, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify{
               match, table_id, v13::instruction_set{apply_actions{set_eth_dst}}
             , cookie_mask, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify{
               match, table_id, instructions
             , v13::cookie_mask{1, 0}, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify{
               match, table_id, instructions
             , v13::cookie_mask{2, 0}, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_mask_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify{
               match, table_id, instructions
             , v13::cookie_mask{0, 1}, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify{
               match, table_id, instructions
             , v13::cookie_mask{0, 2}, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_reset_counter_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify{
               match, table_id, instructions, cookie_mask, false, buffer_id, xid
           }
        != v13::messages::flow_modify{
               match, table_id, instructions, cookie_mask, true, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_buffer_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify{
               match, table_id, instructions, cookie_mask, reset_counter, 1, xid
           }
        != v13::messages::flow_modify{
               match, table_id, instructions, cookie_mask, reset_counter, 2, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify{
               match, table_id, instructions, cookie_mask
             , reset_counter, buffer_id, 1
           }
        != v13::messages::flow_modify{
               match, table_id, instructions, cookie_mask
             , reset_counter, buffer_id, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_modify_fixutre)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, flow_modify_fixutre)
    {
      auto it = bin.begin();

      auto const flow_modify
        = v13::messages::flow_modify::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_modify == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // flow_modify


BOOST_AUTO_TEST_SUITE(flow_modify_strict)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_entry, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{1};
      auto const instructions
        = v13::instruction_set{v13::instructions::goto_table{3}};
      auto const buffer_id = protocol::OFP_NO_BUFFER;

      v13::messages::flow_modify_strict const sut{
        entry, table_id, instructions, true, buffer_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 32 + 8);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.cookie_mask() == 0xffffffffffffffff);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.reset_counter());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == instructions.to_list()));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_match)
    {
      auto const match = v13::oxm_match{
        match::eth_type{0x0800}, match::ip_proto{17}, match::udp_src{52}
      }; // 4 + 6 + 5 + 6 = 21
      auto const priority = std::uint16_t{32};
      auto const table_id = std::uint8_t{1};
      auto const instructions
        = v13::instruction_set{v13::instructions::write_metadata{0xff00}};
      auto const buffer_id = 0xff000;

      v13::messages::flow_modify_strict const sut{
        match, priority, table_id, instructions, false, buffer_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 24 + 24);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(!sut.reset_counter());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == instructions.to_list()));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_cookie_mask)
    {
      auto const match = v13::oxm_match{};
      auto const priority = std::uint16_t{0};
      auto const table_id = std::uint8_t{254};
      auto const instructions = v13::instruction_set{};
      auto const buffer_id = 0;
      auto const cookie_mask
        = v13::cookie_mask{0x123456780000, 0xffffffff00000000};

      v13::messages::flow_modify_strict const sut{
        match, priority, table_id, instructions, cookie_mask, true, buffer_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.cookie() == cookie_mask.value());
      BOOST_TEST(sut.cookie_mask() == cookie_mask.mask());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.reset_counter());
      BOOST_TEST(sut.buffer_id() == buffer_id);
      BOOST_TEST((sut.instructions() == instructions.to_list()));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, flow_modify_strict_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::flow_modify_strict{
          match, priority, table_id, instructions
        , cookie_mask, reset_counter, buffer_id, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }
        == v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               v13::oxm_match{eth_src}, priority, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify_strict{
               v13::oxm_match{eth_dst}, priority, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_priority_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, 1, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify_strict{
               match, 2, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, priority, 1, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify_strict{
               match, priority, 2, instructions
             , cookie_mask, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_instructions_is_not_equal)
    {
      using set = v13::instruction_set;
      using v13::instructions::apply_actions;

      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, priority, table_id, set{apply_actions{set_eth_src}}
             , cookie_mask, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify_strict{
               match, priority, table_id, set{apply_actions{set_eth_dst}}
             , cookie_mask, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , v13::cookie_mask{1, 0}, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , v13::cookie_mask{2, 0}, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_mask_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , v13::cookie_mask{0, 1}, reset_counter, buffer_id, xid
           }
        != v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , v13::cookie_mask{0, 2}, reset_counter, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_reset_counter_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , cookie_mask, true, buffer_id, xid
           }
        != v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , cookie_mask, false, buffer_id, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_buffer_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , cookie_mask, reset_counter, 1, xid
           }
        != v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , cookie_mask, reset_counter, 2, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, 1
           }
        != v13::messages::flow_modify_strict{
               match, priority, table_id, instructions
             , cookie_mask, reset_counter, buffer_id, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_modify_strict_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_binary, flow_modify_strict_fixture)
    {
      auto it = bin.begin();

      auto const flow_modify_strict
        = v13::messages::flow_modify_strict::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_modify_strict == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // flow_modify_strict
BOOST_AUTO_TEST_SUITE_END() // message
