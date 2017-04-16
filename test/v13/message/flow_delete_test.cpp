#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/flow_delete.hpp>
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

namespace  {

  struct flow_entry_fixture {
    match::in_port in_port{4};
    match::eth_dst eth_dst{"\x01\x02\x03\x04\x05\x06"_mac};
    match::eth_src eth_src{"\x11\x12\x13\x14\x15\x16"_mac};
    v13::oxm_match match{in_port, eth_dst, eth_src}; // 4 + 8 + 10 + 10 = 32
    std::uint16_t priority = protocol::OFP_DEFAULT_PRIORITY;
    std::uint64_t cookie = 0xf1f2f3f4f5f6f7f8;
    v13::instruction_set instructions{};
    v13::flow_entry entry{match, priority, cookie, instructions};
  };

  struct flow_delete_parameter : flow_entry_fixture {
    std::uint8_t table_id = protocol::OFPTT_ALL;
    v13::cookie_mask cookie_mask{0xf1f2f3f4f5f6f7f8, 0x0f0f0f0f0f0f0f0f};
    std::uint32_t out_port = protocol::OFPP_ANY;
    std::uint32_t out_group = 33;
    std::uint32_t xid = 0xff56ff78;
  };
  struct flow_delete_fixture : flow_delete_parameter
  {
    v13::messages::flow_delete sut{
      entry.match(), table_id, cookie_mask, out_port, out_group, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x0e\x00\x50\xff\x56\xff\x78" "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f" "\xff\x03\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\xff\xff\xff\xff" "\x00\x00\x00\x21\x00\x00\x00\x00"

        "\x00\x01\x00\x20\x80\x00\x00\x04" "\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00" "\x08\x06\x11\x12\x13\x14\x15\x16"_bin
        ;
  };

  struct flow_delete_strict_parameter : flow_entry_fixture {
    std::uint8_t table_id = 0x8f;
    std::uint32_t out_port = protocol::OFPP_CONTROLLER;
    std::uint32_t out_group = protocol::OFPG_MAX;
    v13::cookie_mask cookie_mask{0xf1f2f3f4f5f6f7f8, 0x0f0f0f0f0f0f0f0f};
    std::uint32_t xid = 0xff56ff78;
  };
  struct flow_delete_strict_fixture : flow_delete_strict_parameter {
    v13::messages::flow_delete_strict sut{
      entry, table_id, out_port, out_group, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x0e\x00\x50\xff\x56\xff\x78" "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\xff\xff\xff\xff\xff\xff\xff\xff" "\x8f\x04\x00\x00\x00\x00\x80\x00"
        "\x00\x00\x00\x00\xff\xff\xff\xfd" "\xff\xff\xff\x00\x00\x00\x00\x00"

        "\x00\x01\x00\x20\x80\x00\x00\x04" "\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00" "\x08\x06\x11\x12\x13\x14\x15\x16"_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(flow_delete)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_table_id, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{1};

      v13::messages::flow_delete const sut{entry.match(), table_id};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 32);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.out_port() == protocol::OFPP_ANY);
      BOOST_TEST(sut.out_group() == protocol::OFPG_ANY);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_out_port)
    {
      auto const empty_match = v13::oxm_match{};
      auto const table_id = std::uint8_t{protocol::OFPTT_ALL};
      auto const out_port = std::uint32_t{1};
      auto const out_group = std::uint32_t{0};

      v13::messages::flow_delete const sut{
        empty_match, table_id, out_port, out_group
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST((sut.match() == empty_match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.out_port() == out_port);
      BOOST_TEST(sut.out_group() == out_group);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_cookie_mask)
    {
      auto const empty_match = v13::oxm_match{};
      auto const table_id = std::uint8_t{254};
      auto const cookie_mask = v13::cookie_mask{0xff, 0xffff};

      v13::messages::flow_delete const sut{empty_match, table_id, cookie_mask};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST((sut.match() == empty_match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == cookie_mask.value());
      BOOST_TEST(sut.cookie_mask() == cookie_mask.mask());
      BOOST_TEST(sut.out_port() == protocol::OFPP_ANY);
      BOOST_TEST(sut.out_group() == protocol::OFPG_ANY);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_cookie_mask_and_port)
    {
      auto const empty_match = v13::oxm_match{};
      auto const table_id = std::uint8_t{0};
      auto const cookie_mask = v13::cookie_mask{0xff, 0xffff};
      auto const out_port = std::uint32_t{protocol::OFPP_MAX};
      auto const out_group = std::uint32_t{protocol::OFPG_MAX};

      v13::messages::flow_delete const sut{
        empty_match, table_id, cookie_mask, out_port, out_group
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST((sut.match() == empty_match));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == cookie_mask.value());
      BOOST_TEST(sut.cookie_mask() == cookie_mask.mask());
      BOOST_TEST(sut.out_port() == out_port);
      BOOST_TEST(sut.out_group() == out_group);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, flow_delete_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, flow_delete_fixture)
    {
        auto moved = sut;

        auto const copy = std::move(moved);

        BOOST_TEST((copy == sut));
        BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_mod) + 8);
        BOOST_TEST(moved.match().length() == 4);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, flow_delete_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = v13::messages::flow_delete{v13::oxm_match{}, 0};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = v13::messages::flow_delete{v13::oxm_match{}, 0};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST(moved.match().length() == 4);
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, flow_delete_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::flow_delete{
        match, table_id, cookie_mask, out_port, out_group, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete{
             match, table_id, cookie_mask, out_port, out_group, xid
           }
        == v13::messages::flow_delete{
             match, table_id, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete{
               v13::oxm_match{eth_src}
             , table_id, cookie_mask, out_port, out_group, xid
           }
        != v13::messages::flow_delete{
               v13::oxm_match{eth_dst}
             , table_id, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete{
             match, 1, cookie_mask, out_port, out_group, xid
           }
        != v13::messages::flow_delete{
             match, 2, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete{
             match, table_id, v13::cookie_mask{1, 0}, out_port, out_group, xid
           }
        != v13::messages::flow_delete{
             match, table_id, v13::cookie_mask{2, 0}, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_mask_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete{
             match, table_id, v13::cookie_mask{0, 1}, out_port, out_group, xid
           }
        != v13::messages::flow_delete{
             match, table_id, v13::cookie_mask{0, 2}, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_out_port_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete{
             match, table_id, cookie_mask, 1, out_group, xid
           }
        != v13::messages::flow_delete{
             match, table_id, cookie_mask, 2, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_out_group_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete{
             match, table_id, cookie_mask, out_port, 1, xid
           }
        != v13::messages::flow_delete{
             match, table_id, cookie_mask, out_port, 2, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete{
             match, table_id, cookie_mask, out_port, out_group, 1
           }
        != v13::messages::flow_delete{
             match, table_id, cookie_mask, out_port, out_group, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_delete_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, flow_delete_fixture)
    {
      auto it = bin.begin();

      auto const flow_delete
        = v13::messages::flow_delete::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_delete == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // flow_delete


BOOST_AUTO_TEST_SUITE(flow_delete_strict)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_entry, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{1};

      v13::messages::flow_delete_strict const sut{entry, table_id};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 32);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.cookie_mask() == 0xffffffffffffffff);
      BOOST_TEST(sut.out_port() == protocol::OFPP_ANY);
      BOOST_TEST(sut.out_group() == protocol::OFPG_ANY);
    }
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_entry_and_port, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{1};
      auto const out_port = std::uint32_t{protocol::OFPP_CONTROLLER};
      auto const out_group = 1;

      v13::messages::flow_delete_strict const sut{
        entry, table_id, out_port, out_group
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 32);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.cookie_mask() == 0xffffffffffffffff);
      BOOST_TEST(sut.out_port() == out_port);
      BOOST_TEST(sut.out_group() == out_group);
    }
    BOOST_FIXTURE_TEST_CASE(constructible_from_table_id, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{protocol::OFPTT_ALL};

      v13::messages::flow_delete_strict const sut{
        entry.match(), entry.priority(), table_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 32);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.out_port() == protocol::OFPP_ANY);
      BOOST_TEST(sut.out_group() == protocol::OFPG_ANY);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_cookie_mask)
    {
      auto const empty_match = v13::oxm_match{};
      auto const priority = std::uint16_t{65535};
      auto const table_id = std::uint8_t{0};
      auto const cookie_mask
        = v13::cookie_mask{0x0f0f0f0f0f0f0f0f, 0xffffffff0000000f};

      v13::messages::flow_delete_strict const sut{
        empty_match, priority, table_id, cookie_mask
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST((sut.match() == empty_match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == cookie_mask.value());
      BOOST_TEST(sut.cookie_mask() == cookie_mask.mask());
      BOOST_TEST(sut.out_port() == protocol::OFPP_ANY);
      BOOST_TEST(sut.out_group() == protocol::OFPG_ANY);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_out_port)
    {
      auto const empty_match = v13::oxm_match{};
      auto const priority = std::uint16_t{protocol::OFP_DEFAULT_PRIORITY};
      auto const table_id = std::uint8_t{1};
      auto const out_port = std::uint32_t{1};
      auto const out_group = 254;

      v13::messages::flow_delete_strict const sut{
        empty_match, priority, table_id, out_port, out_group
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FLOW_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST((sut.match() == empty_match));
      BOOST_TEST(sut.priority() == priority);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == 0);
      BOOST_TEST(sut.cookie_mask() == 0);
      BOOST_TEST(sut.out_port() == out_port);
      BOOST_TEST(sut.out_group() == out_group);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, flow_delete_strict_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, flow_delete_strict_fixture)
    {
      auto moved = sut;
      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST(moved.match().length() == 4);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, flow_delete_strict_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = v13::messages::flow_delete_strict{v13::oxm_match{}, 0, 0};

      copy = sut;

      BOOST_TEST((copy == sut));
    }

    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto moved = sut;
      auto copy = v13::messages::flow_delete_strict{v13::oxm_match{}, 0, 0};

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_mod) + 8);
      BOOST_TEST(moved.match().length() == 4);
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, flow_delete_strict_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::flow_delete_strict{
        entry, table_id, out_port, out_group, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
             entry, table_id, out_port, out_group, xid
           }
        == v13::messages::flow_delete_strict{
             entry, table_id, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
               v13::oxm_match{eth_dst}
             , priority, table_id, cookie_mask, out_port, out_group, xid
           }
        != v13::messages::flow_delete_strict{
               v13::oxm_match{eth_src}
             , priority, table_id, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_priority_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
             match, 1, table_id, cookie_mask, out_port, out_group, xid
           }
        != v13::messages::flow_delete_strict{
             match, 2, table_id, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
               match, priority, table_id
             , v13::cookie_mask{1, 0}, out_port, out_group, xid
           }
        != v13::messages::flow_delete_strict{
               match, priority, table_id
             , v13::cookie_mask{2, 0}, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_mask_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
               match, priority, table_id
             , v13::cookie_mask{0, 1}, out_port, out_group, xid
           }
        != v13::messages::flow_delete_strict{
               match, priority, table_id
             , v13::cookie_mask{0, 2}, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
             entry, 1, out_port, out_group, xid
           }
        != v13::messages::flow_delete_strict{
             entry, 2, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_out_port_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
             entry, table_id, 1, out_group, xid
           }
        != v13::messages::flow_delete_strict{
             entry, table_id, 2, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_out_group_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
             entry, table_id, out_port, 1, xid
           }
        != v13::messages::flow_delete_strict{
             entry, table_id, out_port, 2, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::flow_delete_strict{
             entry, table_id, out_port, out_group, 1
           }
        != v13::messages::flow_delete_strict{
             entry, table_id, out_port, out_group, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_delete_strict_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        constructible_from_binary, flow_delete_strict_fixture)
    {
      auto it = bin.begin();

      auto const flow_delete_strict
        = v13::messages::flow_delete_strict::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_delete_strict == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // flow_delete_strict
BOOST_AUTO_TEST_SUITE_END() // message_test
