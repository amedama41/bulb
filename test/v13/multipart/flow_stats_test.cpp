#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/flow_stats.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/instructions.hpp>
#include <canard/net/ofp/v13/io/openflow.hpp>
#include <canard/net/ofp/v13/common/oxm_match.hpp>

#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace match = v13::oxm_match_fields;
namespace multipart = v13::messages::multipart;
namespace protocol = v13::protocol;

namespace {

  using body_type = multipart::flow_stats_reply::body_type;

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

  struct flow_stats_parameter : flow_entry_fixture {
    std::uint8_t table_id = 0x01;
    std::uint16_t flags
      = protocol::OFPFF_SEND_FLOW_REM | protocol::OFPFF_NO_PKT_COUNTS;
    v13::timeouts timeouts{0x1234, 0};
    v13::elapsed_time elapsed_time{0x12345678, 0x87654321};
    v13::counters counters{0, 0x1234567887654321};
  };
  struct flow_stats_fixture : flow_stats_parameter {
    multipart::flow_stats sut{
      entry, table_id, flags, timeouts, elapsed_time, counters
    };
    std::vector<std::uint8_t> bin
      = "\x00\xb8\x01\x00\x12\x34\x56\x78""\x87\x65\x43\x21\x80\x00\x12\x34"
        "\x00\x00\x00\x09\x00\x00\x00\x00""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x12\x34\x56\x78\x87\x65\x43\x21"

        "\x00\x01\x00\x20\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00""\x08\x06\x11\x12\x13\x14\x15\x16"

        "\x00\x04\x00\x38\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"
        "\x00\x00\x00\x10\x00\x00\x00\x04""\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x05\x00\x08\x00\x00\x00\x00"
        "\x00\x03\x00\x28\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"_bin
        ;
    multipart::flow_stats& flow_stats = sut;
  };

  struct flow_stats_request_parameter : flow_entry_fixture {
    std::uint8_t table_id = 0xf3;
    std::uint32_t out_port = protocol::OFPP_FLOOD;
    std::uint32_t out_group = protocol::OFPG_MAX;
    std::uint32_t xid = 0x12345678;
    v13::cookie_mask cookie_mask{cookie, 0xffffffffffffffff};
  };
  struct flow_stats_request_fixture : flow_stats_request_parameter {
    multipart::flow_stats_request sut{
      entry, table_id, out_port, out_group, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x50\x12\x34\x56\x78""\x00\x01\x00\x00\x00\x00\x00\x00"
        "\xf3\x00\x00\x00\xff\xff\xff\xfb""\xff\xff\xff\x00\x00\x00\x00\x00"
        "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8""\xff\xff\xff\xff\xff\xff\xff\xff"
        "\x00\x01\x00\x20\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00""\x08\x06\x11\x12\x13\x14\x15\x16"_bin
        ;
  };

  struct flow_stats_reply_parameter : flow_stats_fixture {
    ::body_type body{6, flow_stats};
    std::uint16_t flags = protocol::OFPMPF_REPLY_MORE;
    std::uint32_t xid = 0x12345678;
  };
  struct flow_stats_reply_fixture : flow_stats_reply_parameter {
    multipart::flow_stats_reply sut = multipart::flow_stats_reply{
      body, flags, xid
    };
    std::vector<std::uint8_t> bin
      = "\x04\x13\x04\x60\x12\x34\x56\x78""\x00\x01\x00\x01\x00\x00\x00\x00"

        "\x00\xb8\x01\x00\x12\x34\x56\x78""\x87\x65\x43\x21\x80\x00\x12\x34"
        "\x00\x00\x00\x09\x00\x00\x00\x00""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x12\x34\x56\x78\x87\x65\x43\x21"
        "\x00\x01\x00\x20\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00""\x08\x06\x11\x12\x13\x14\x15\x16"
        "\x00\x04\x00\x38\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"
        "\x00\x00\x00\x10\x00\x00\x00\x04""\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x05\x00\x08\x00\x00\x00\x00"
        "\x00\x03\x00\x28\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"

        "\x00\xb8\x01\x00\x12\x34\x56\x78""\x87\x65\x43\x21\x80\x00\x12\x34"
        "\x00\x00\x00\x09\x00\x00\x00\x00""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x12\x34\x56\x78\x87\x65\x43\x21"
        "\x00\x01\x00\x20\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00""\x08\x06\x11\x12\x13\x14\x15\x16"
        "\x00\x04\x00\x38\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"
        "\x00\x00\x00\x10\x00\x00\x00\x04""\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x05\x00\x08\x00\x00\x00\x00"
        "\x00\x03\x00\x28\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"

        "\x00\xb8\x01\x00\x12\x34\x56\x78""\x87\x65\x43\x21\x80\x00\x12\x34"
        "\x00\x00\x00\x09\x00\x00\x00\x00""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x12\x34\x56\x78\x87\x65\x43\x21"
        "\x00\x01\x00\x20\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00""\x08\x06\x11\x12\x13\x14\x15\x16"
        "\x00\x04\x00\x38\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"
        "\x00\x00\x00\x10\x00\x00\x00\x04""\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x05\x00\x08\x00\x00\x00\x00"
        "\x00\x03\x00\x28\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"

        "\x00\xb8\x01\x00\x12\x34\x56\x78""\x87\x65\x43\x21\x80\x00\x12\x34"
        "\x00\x00\x00\x09\x00\x00\x00\x00""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x12\x34\x56\x78\x87\x65\x43\x21"
        "\x00\x01\x00\x20\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00""\x08\x06\x11\x12\x13\x14\x15\x16"
        "\x00\x04\x00\x38\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"
        "\x00\x00\x00\x10\x00\x00\x00\x04""\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x05\x00\x08\x00\x00\x00\x00"
        "\x00\x03\x00\x28\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"

        "\x00\xb8\x01\x00\x12\x34\x56\x78""\x87\x65\x43\x21\x80\x00\x12\x34"
        "\x00\x00\x00\x09\x00\x00\x00\x00""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x12\x34\x56\x78\x87\x65\x43\x21"
        "\x00\x01\x00\x20\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00""\x08\x06\x11\x12\x13\x14\x15\x16"
        "\x00\x04\x00\x38\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"
        "\x00\x00\x00\x10\x00\x00\x00\x04""\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x05\x00\x08\x00\x00\x00\x00"
        "\x00\x03\x00\x28\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"

        "\x00\xb8\x01\x00\x12\x34\x56\x78""\x87\x65\x43\x21\x80\x00\x12\x34"
        "\x00\x00\x00\x09\x00\x00\x00\x00""\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8"
        "\x00\x00\x00\x00\x00\x00\x00\x00""\x12\x34\x56\x78\x87\x65\x43\x21"
        "\x00\x01\x00\x20\x80\x00\x00\x04""\x00\x00\x00\x04\x80\x00\x06\x06"
        "\x01\x02\x03\x04\x05\x06\x80\x00""\x08\x06\x11\x12\x13\x14\x15\x16"
        "\x00\x04\x00\x38\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"
        "\x00\x00\x00\x10\x00\x00\x00\x04""\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x05\x00\x08\x00\x00\x00\x00"
        "\x00\x03\x00\x28\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x06\x06""\x11\x12\x13\x14\x15\x16\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x08\x06""\x01\x02\x03\x04\x05\x06\x00\x00"_bin
        ;
  };

}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(flow_stats)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_entry, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{protocol::OFPTT_MAX};
      auto const flags = std::uint16_t(
          protocol::OFPFF_SEND_FLOW_REM | protocol::OFPFF_CHECK_OVERLAP);
      auto const timeouts = v13::timeouts{0x1234, 0x5678};
      auto const elapsed_time = v13::elapsed_time{3624, 5432};
      auto const counters
        = v13::counters{0xf1f2f3f4f5f6f7f8, 0x1234567887654321};

      multipart::flow_stats const sut{
        entry, table_id, flags, timeouts, elapsed_time, counters
      };

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_flow_stats) + 32 + 104);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.priority() == entry.priority());
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST(sut.idle_timeout() == timeouts.idle_timeout());
      BOOST_TEST(sut.hard_timeout() == timeouts.hard_timeout());
      BOOST_TEST(sut.duration_sec() == elapsed_time.duration_sec());
      BOOST_TEST(sut.duration_nsec() == elapsed_time.duration_nsec());
      BOOST_TEST(sut.packet_count() == counters.packet_count());
      BOOST_TEST(sut.byte_count() == counters.byte_count());
      BOOST_TEST((sut.instructions() == entry.instructions()));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, flow_stats_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }

    BOOST_FIXTURE_TEST_CASE(move_constructible, flow_stats_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_stats)
          + sizeof(protocol::ofp_match));
      BOOST_TEST((moved.match() == v13::oxm_match{}));
      BOOST_TEST((moved.instructions().empty()));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, flow_stats_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = multipart::flow_stats{
          v13::flow_entry{v13::oxm_match{}, 0, 0, v13::instruction_set{}}
        , 0, 0
        , v13::timeouts{0, 0}, v13::elapsed_time{0, 0}, v13::counters{0, 0}
      };

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = multipart::flow_stats{
          v13::flow_entry{v13::oxm_match{}, 0, 0, v13::instruction_set{}}
        , 0, 0
        , v13::timeouts{0, 0}, v13::elapsed_time{0, 0}, v13::counters{0, 0}
      };
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_flow_stats)
          + sizeof(protocol::ofp_match));
      BOOST_TEST((moved.match() == v13::oxm_match{}));
      BOOST_TEST(moved.instructions().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, flow_stats_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::flow_stats{
        entry, table_id, flags, timeouts, elapsed_time, counters
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, table_id, flags, timeouts, elapsed_time, counters
           }
        == multipart::flow_stats{
             entry, table_id, flags, timeouts, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
               v13::flow_entry{
                 v13::oxm_match{eth_dst}, priority, cookie, instructions
               }
             , table_id, flags, timeouts, elapsed_time, counters
           }
        != multipart::flow_stats{
               v13::flow_entry{
                 v13::oxm_match{eth_src}, priority, cookie, instructions
               }
             , table_id, flags, timeouts, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_priority_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
               v13::flow_entry{match, 1, cookie, instructions}
             , table_id, flags, timeouts, elapsed_time, counters
           }
        != multipart::flow_stats{
               v13::flow_entry{match, 2, cookie, instructions}
             , table_id, flags, timeouts, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
               v13::flow_entry{match, priority, 1, instructions}
             , table_id, flags, timeouts, elapsed_time, counters
           }
        != multipart::flow_stats{
               v13::flow_entry{match, priority, 2, instructions}
             , table_id, flags, timeouts, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_instructions_is_not_equal)
    {
      using v13::instructions::apply_actions;

      BOOST_TEST(
          (multipart::flow_stats{
               v13::flow_entry{
                  match, priority, cookie
                , v13::instruction_set{apply_actions{set_eth_src}}
               }
             , table_id, flags, timeouts, elapsed_time, counters
           }
        != multipart::flow_stats{
               v13::flow_entry{
                  match, priority, cookie
                , v13::instruction_set{apply_actions{set_eth_dst}}
               }
             , table_id, flags, timeouts, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, 1, flags, timeouts, elapsed_time, counters
           }
        != multipart::flow_stats{
             entry, 2, flags, timeouts, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, table_id, 1, timeouts, elapsed_time, counters
           }
        != multipart::flow_stats{
             entry, table_id, 2, timeouts, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_idle_timeout_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, table_id, flags, v13::timeouts{1, 0}, elapsed_time, counters
           }
        != multipart::flow_stats{
             entry, table_id, flags, v13::timeouts{2, 0}, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_hard_timeout_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, table_id, flags, v13::timeouts{0, 1}, elapsed_time, counters
           }
        != multipart::flow_stats{
             entry, table_id, flags, v13::timeouts{0, 2}, elapsed_time, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_sec_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, table_id, flags, timeouts, v13::elapsed_time{1, 0}, counters
           }
        != multipart::flow_stats{
             entry, table_id, flags, timeouts, v13::elapsed_time{2, 0}, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_duration_nsec_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, table_id, flags, timeouts, v13::elapsed_time{0, 1}, counters
           }
        != multipart::flow_stats{
             entry, table_id, flags, timeouts, v13::elapsed_time{0, 2}, counters
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_packet_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, table_id, flags, timeouts, elapsed_time, v13::counters{1, 0}
           }
        != multipart::flow_stats{
             entry, table_id, flags, timeouts, elapsed_time, v13::counters{2, 0}
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_byte_count_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats{
             entry, table_id, flags, timeouts, elapsed_time, v13::counters{0, 1}
           }
        != multipart::flow_stats{
             entry, table_id, flags, timeouts, elapsed_time, v13::counters{0, 2}
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_stats_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, flow_stats_fixture)
    {
      auto it = bin.begin();

      auto const flow_stats = multipart::flow_stats::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_stats == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // flow_stats

BOOST_AUTO_TEST_SUITE(flow_stats_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible_from_entry, flow_entry_fixture)
    {
      auto const table_id = std::uint8_t{1};

      multipart::flow_stats_request const sut{
        entry, table_id
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_flow_stats_request)
          + entry.match().length());
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_FLOW);
      BOOST_TEST(sut.flags() == 0);
      BOOST_TEST((sut.match() == entry.match()));
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.cookie() == entry.cookie());
      BOOST_TEST(sut.cookie_mask() == 0xffffffffffffffff);
      BOOST_TEST(sut.out_port() == protocol::OFPP_ANY);
      BOOST_TEST(sut.out_group() == protocol::OFPG_ANY);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, flow_stats_request_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, flow_stats_request_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_flow_stats_request)
          + sizeof(protocol::ofp_match));
      BOOST_TEST((moved.match() == v13::oxm_match{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, flow_stats_request_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = multipart::flow_stats_request{v13::oxm_match{}, 0};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = multipart::flow_stats_request{v13::oxm_match{}, 0};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_request)
          + sizeof(protocol::ofp_flow_stats_request)
          + sizeof(protocol::ofp_match));
      BOOST_TEST((moved.match() == v13::oxm_match{}));
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, flow_stats_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::flow_stats_request{
        match, table_id, cookie_mask, out_port, out_group, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_request{
             match, table_id, cookie_mask, out_port, out_group, xid
           }
        == multipart::flow_stats_request{
             match, table_id, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_request{
             v13::oxm_match{eth_src}
           , table_id, cookie_mask, out_port, out_group, xid
           }
        != multipart::flow_stats_request{
             v13::oxm_match{eth_dst}
           , table_id, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_request{
             match, 1, cookie_mask, out_port, out_group, xid
           }
        != multipart::flow_stats_request{
             match, 2, cookie_mask, out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_request{
               match, table_id, v13::cookie_mask{1, 0}
             , out_port, out_group, xid
           }
        != multipart::flow_stats_request{
               match, table_id, v13::cookie_mask{2, 0}
             , out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_cookie_mask_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_request{
               match, table_id, v13::cookie_mask{0, 1}
             , out_port, out_group, xid
           }
        != multipart::flow_stats_request{
               match, table_id, v13::cookie_mask{0, 2}
             , out_port, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_out_port_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_request{
             match, table_id, cookie_mask, 1, out_group, xid
           }
        != multipart::flow_stats_request{
             match, table_id, cookie_mask, 2, out_group, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_out_group_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_request{
             match, table_id, cookie_mask, out_port, 1, xid
           }
        != multipart::flow_stats_request{
             match, table_id, cookie_mask, out_port, 2, xid
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_request{
             match, table_id, cookie_mask, out_port, out_group, 1
           }
        != multipart::flow_stats_request{
             match, table_id, cookie_mask, out_port, out_group, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_stats_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, flow_stats_request_fixture)
    {
      auto it = bin.begin();

      auto const flow_stats_request
        = multipart::flow_stats_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_stats_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // flow_stats_request


BOOST_AUTO_TEST_SUITE(flow_stats_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(constructible, flow_stats_fixture)
    {
      auto const size = std::size_t{32};
      auto body = ::body_type(size, flow_stats);
      auto const flags = std::uint16_t{protocol::OFPMPF_REPLY_MORE};

      multipart::flow_stats_reply const sut{body, flags};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_MULTIPART_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_reply)
          + flow_stats_fixture::sut.length() * size);
      BOOST_TEST(sut.multipart_type() == protocol::OFPMP_FLOW);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, flow_stats_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, flow_stats_reply_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, flow_stats_reply_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = multipart::flow_stats_reply{::body_type{}};

      copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = multipart::flow_stats_reply{::body_type{}};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_multipart_reply));
      BOOST_TEST(moved.body().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, flow_stats_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::flow_stats_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_reply{body, flags, xid}
        == multipart::flow_stats_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(true_if_body_is_empty)
    {
      BOOST_TEST(
          (multipart::flow_stats_reply{body_type{}, flags, xid}
        == multipart::flow_stats_reply{body_type{}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      auto const flow_stats1 = multipart::flow_stats{
        entry, 1, flags, timeouts, elapsed_time, counters
      };
      auto const flow_stats2 = multipart::flow_stats{
        entry, 2, flags, timeouts, elapsed_time, counters
      };

      BOOST_TEST(
          (multipart::flow_stats_reply{body_type{flow_stats1}, flags, xid}
        != multipart::flow_stats_reply{body_type{flow_stats2}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_reply{body, 0, xid}
        != multipart::flow_stats_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::flow_stats_reply{body, flags, 1}
        != multipart::flow_stats_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, flow_stats_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, flow_stats_reply_fixture)
    {
      auto it = bin.begin();

      auto const flow_stats_reply
        = multipart::flow_stats_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((flow_stats_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // flow_stats_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
