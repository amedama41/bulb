#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/flow_entry.hpp>
#include <boost/test/unit_test.hpp>

#include <canard/network/openflow/v10/actions.hpp>
#include <canard/network/openflow/v10/match_fields.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace act = v10::actions;

namespace {

struct flow_entry_id_fixture {
  v10::match::in_port in_port{1};
  v10::match::eth_src eth_src{"\x01\x02\x03\x04\x05\x06"_mac};
  v10::match::eth_dst eth_dst{"\xf1\xf2\xf3\xf4\xf5\xf6"_mac};
  v10::match::vlan_vid vlan_vid{0x0123};
  v10::match::vlan_pcp vlan_pcp{2};
  v10::match::eth_type eth_type{0x0800};
  v10::match::ip_dscp ip_dscp{0x23};
  v10::match::ip_proto ip_proto{7};
  v10::match::ipv4_src ipv4_src{"192.168.1.2"_ipv4, 32};
  v10::match::ipv4_dst ipv4_dst{"192.168.1.0"_ipv4, 28};
  v10::match::tcp_src tcp_src{6653};
  v10::match::tcp_src tcp_dst{8080};
};

struct flow_entry_fixture : flow_entry_id_fixture {
  act::output output{v10::protocol::OFPP_CONTROLLER, 64};
  act::set_vlan_vid set_vlan_vid{0x0123};
  act::set_vlan_pcp set_vlan_pcp{0x02};
  act::strip_vlan strip_vlan{};
  act::set_eth_src set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac};
  act::set_eth_dst set_eth_dst{"\xf1\xf2\xf3\xf4\xf5\xf6"_mac};
  act::set_ip_dscp set_ip_dscp{0x23};
  act::set_ipv4_src set_ipv4_src{"192.168.1.3"_ipv4};
  act::set_ipv4_dst set_ipv4_dst{"192.168.1.4"_ipv4};
  act::set_tcp_src set_tcp_src{6653};
  act::set_tcp_dst set_tcp_dst{8080};
  act::enqueue enqueue{0x12345678, 1};
};

}

BOOST_AUTO_TEST_SUITE(utility_test)

BOOST_AUTO_TEST_SUITE(flow_entry_id)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_match_and_priority, flow_entry_id_fixture)
    {
      auto const match = v10::match_set{ in_port, eth_src, vlan_vid, ipv4_dst };
      auto const priority = std::uint16_t{30000};

      auto const sut = v10::flow_entry_id{match, priority};

      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_empty_match_and_priority)
    {
      auto const match = v10::match_set{};
      auto const priority = std::uint16_t{0};

      auto const sut = v10::flow_entry_id{match, priority};

      BOOST_TEST((sut.match() == match));
      BOOST_TEST(sut.priority() == priority);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, flow_entry_id_fixture)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = v10::flow_entry_id{
        v10::match_set{ in_port, eth_type, ip_proto}, 1234
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_match_and_priority_are_equal)
    {
      auto const match = v10::match_set{ ipv4_dst, tcp_src };
      auto const priority = std::uint16_t{v10::protocol::OFP_DEFAULT_PRIORITY};

      BOOST_TEST(
          (v10::flow_entry_id{match, priority}
        == v10::flow_entry_id{match, priority}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_match_is_not_equal)
    {
      auto const priority = std::uint16_t{v10::protocol::OFP_DEFAULT_PRIORITY};

      BOOST_TEST(
          (v10::flow_entry_id{v10::match_set{ in_port, ipv4_dst }, priority}
        != v10::flow_entry_id{v10::match_set{ in_port, ipv4_src }, priority}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_match_is_not_equal_but_equivalent)
    {
      auto const priority = std::uint16_t{v10::protocol::OFP_DEFAULT_PRIORITY};
      using v10::match::ipv4_src;

      BOOST_TEST(
          (v10::flow_entry_id{v10::match_set{
              ipv4_src{"0.0.0.0"_ipv4, 8} }, priority}
        == v10::flow_entry_id{v10::match_set{
              ipv4_src{"0.0.0.1"_ipv4, 8} }, priority}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_priority_is_not_equal)
    {
      auto const match = v10::match_set{};

      BOOST_TEST(
          (v10::flow_entry_id{match, 1} != v10::flow_entry_id{match, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(table_miss)
    BOOST_AUTO_TEST_CASE(constructs_empty_match_and_zero_priority_flow_entry_id)
    {
      auto const sut = v10::flow_entry_id::table_miss();

      BOOST_TEST((sut.match() == v10::match_set{}));
      BOOST_TEST(sut.priority() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // table_miss

BOOST_AUTO_TEST_SUITE_END() // flow_entry_id

BOOST_AUTO_TEST_SUITE(flow_entry)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_FIXTURE_TEST_CASE(
          is_constructible_from_flow_entry_id_and_cookie_and_action_list
        , flow_entry_fixture)
    {
      auto const id = v10::flow_entry_id{
        v10::match_set{ in_port, eth_type, ip_proto }, 0x1234
      };
      auto const cookie = 0x0102030405060708;
      auto const actions
        = v10::action_list{ set_vlan_vid, set_ip_dscp, output };

      auto const sut = v10::flow_entry{id, cookie, actions};

      BOOST_TEST((sut.id() == id));
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.actions() == actions));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_empty_action_list, flow_entry_fixture)
    {
      auto const id = v10::flow_entry_id{
        v10::match_set{ in_port, eth_type, ip_proto }, 0x1234
      };
      auto const cookie = 0x0102030405060708;
      auto const empty_actions = v10::action_list{};

      auto const sut = v10::flow_entry{id, cookie, empty_actions};

      BOOST_TEST((sut.id() == id));
      BOOST_TEST(sut.cookie() == cookie);
      BOOST_TEST((sut.actions() == empty_actions));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, flow_entry_fixture)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = v10::flow_entry{
          v10::flow_entry_id{v10::match_set{ in_port }, 0x1234}
        , 0x0102030405060708
        , v10::action_list{ strip_vlan, enqueue }
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_id_and_cookie_and_actions_are_equal)
    {
      auto const id = v10::flow_entry_id{v10::match_set{ eth_type }, 0x1000 };
      auto const cookie = std::uint64_t{1234};
      auto const actions
        = v10::action_list{ set_vlan_vid, set_ip_dscp, output };

      BOOST_TEST(
          (v10::flow_entry{id, cookie, actions}
        == v10::flow_entry{id, cookie, actions}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_id_is_not_equal)
    {
      auto const cookie = std::uint64_t{1234};
      auto const actions
        = v10::action_list{ set_vlan_vid, set_ip_dscp, output };

      BOOST_TEST(
          (v10::flow_entry{
              v10::flow_entry_id{v10::match_set{ eth_src }, 1}, cookie, actions}
        != v10::flow_entry{
              v10::flow_entry_id{v10::match_set{ eth_dst }, 2}, cookie, actions}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_cookie_is_not_equal)
    {
      auto const id = v10::flow_entry_id{v10::match_set{ eth_type }, 0x1000 };
      auto const actions
        = v10::action_list{ set_vlan_vid, set_ip_dscp, output };

      BOOST_TEST(
          (v10::flow_entry{id, 1, actions} != v10::flow_entry{id, 2, actions}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_actions_is_not_equal)
    {
      auto const id = v10::flow_entry_id{v10::match_set{ eth_type }, 0x1000 };
      auto const cookie = std::uint64_t{1234};

      BOOST_TEST(
          (v10::flow_entry{id, cookie, v10::action_list{ set_ipv4_dst }}
        != v10::flow_entry{id, cookie, v10::action_list{ set_ipv4_src }}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

BOOST_AUTO_TEST_SUITE_END() // flow_entry


BOOST_AUTO_TEST_SUITE(timeouts)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_idle_timeout_and_hard_timeout)
    {
      auto const idle_timeout = 0x1234;
      auto const hard_timeout = 0x5678;

      auto const sut = v10::timeouts{idle_timeout, hard_timeout};

      BOOST_TEST(sut.idle_timeout() == idle_timeout);
      BOOST_TEST(sut.hard_timeout() == hard_timeout);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = v10::timeouts{1, 2};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_idle_timeout_and_hard_timeout_are_equal)
    {
      auto const idle_timeout = 0x1234;
      auto const hard_timeout = 0x5678;

      BOOST_TEST(
          (v10::timeouts{idle_timeout, hard_timeout}
        == v10::timeouts{idle_timeout, hard_timeout}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_idle_timeout_is_not_equal)
    {
      auto const hard_timeout = 0x5678;

      BOOST_TEST(
          (v10::timeouts{1, hard_timeout} != v10::timeouts{2, hard_timeout}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_hard_timeout_is_not_equal)
    {
      auto const idle_timeout = 0x1234;

      BOOST_TEST(
          (v10::timeouts{idle_timeout, 1} != v10::timeouts{idle_timeout, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

BOOST_AUTO_TEST_SUITE_END() // timeouts


BOOST_AUTO_TEST_SUITE(counters)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_packet_count_and_byte_count)
    {
      auto const packet_count = 0x0102030405060708;
      auto const byte_count = 0x1020304050607080;

      auto const sut = v10::counters{packet_count, byte_count};

      BOOST_TEST(sut.packet_count() == packet_count);
      BOOST_TEST(sut.byte_count() == byte_count);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = v10::counters{1, 2};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_packet_count_and_byte_count_are_equal)
    {
      auto const packet_count = 0x0102030405060708;
      auto const byte_count = 0x1020304050607080;

      BOOST_TEST(
          (v10::counters{packet_count, byte_count}
        == v10::counters{packet_count, byte_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_packet_count_is_not_equal)
    {
      auto const byte_count = 0x1020304050607080;

      BOOST_TEST(
          (v10::counters{1, byte_count} != v10::counters{2, byte_count}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_byte_count_is_not_equal)
    {
      auto const packet_count = 0x0102030405060708;

      BOOST_TEST(
          (v10::counters{packet_count, 1} != v10::counters{packet_count, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

BOOST_AUTO_TEST_SUITE_END() // counters


BOOST_AUTO_TEST_SUITE(elapsed_time)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_duration_sec_and_duration_nsec)
    {
      auto const duration_sec = 0x01020304;
      auto const duration_nsec = 0x05060708;

      auto const sut = v10::elapsed_time{duration_sec, duration_nsec};

      BOOST_TEST(sut.duration_sec() == duration_sec);
      BOOST_TEST(sut.duration_nsec() == duration_nsec);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = v10::elapsed_time{1, 2};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_duration_sec_and_duration_nsec_are_equal)
    {
      auto const duration_sec = 0x01020304;
      auto const duration_nsec = 0x05060708;

      BOOST_TEST(
          (v10::elapsed_time{duration_sec, duration_nsec}
        == v10::elapsed_time{duration_sec, duration_nsec}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_duration_sec_is_not_equal)
    {
      auto const duration_nsec = 0x05060708;

      BOOST_TEST(
          (v10::elapsed_time{1, duration_nsec}
        != v10::elapsed_time{2, duration_nsec}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_duration_nsec_is_not_equal)
    {
      auto const duration_sec = 0x01020304;

      BOOST_TEST(
          (v10::elapsed_time{duration_sec, 1}
        != v10::elapsed_time{duration_sec, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

BOOST_AUTO_TEST_SUITE_END() // elapsed_time

BOOST_AUTO_TEST_SUITE_END() // utility_test
