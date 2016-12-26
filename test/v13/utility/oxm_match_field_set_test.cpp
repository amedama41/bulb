#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/utility/oxm_match_field_set.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace match = v13::oxm_match_fields;

namespace {
  namespace proto = v13::protocol;
  using list_type = v13::oxm_match_field_set::list_type;

  struct parameters
  {
    match::in_port in_port{proto::OFPP_MAX}; // 8
    match::eth_dst eth_dst{"\x01\x02\x03\x04\x05\x06"_mac}; // 10
    match::eth_src eth_src{"\x01\x02\x03\x04\x05\x06"_mac}; // 10
    match::eth_type eth_type{0x0800}; // 6
    match::vlan_vid vlan_vid{proto::OFPVID_PRESENT, proto::OFPVID_PRESENT}; // 8
    match::vlan_pcp vlan_pcp{1};
    match::ip_proto ip_proto{6}; // 5
    match::ipv4_src ipv4_src{"192.168.10.0"_ipv4, 24}; // 12
    match::ipv4_dst ipv4_dst{"192.168.10.0"_ipv4, 24}; // 12
  };
}

BOOST_AUTO_TEST_SUITE(utility_test)
BOOST_AUTO_TEST_SUITE(oxm_match_field_set)

  BOOST_AUTO_TEST_SUITE(is_valid_set)
    BOOST_AUTO_TEST_CASE(true_if_match_is_empty)
    {
      auto const list = list_type{};

      BOOST_TEST(v13::oxm_match_field_set::is_valid_set(list));
    }
    BOOST_AUTO_TEST_CASE(true_if_match_containes_single_field)
    {
      auto const list = list_type{ match::eth_type{0x0800} };

      BOOST_TEST(v13::oxm_match_field_set::is_valid_set(list));
    }
    BOOST_AUTO_TEST_CASE(true_if_match_containes_different_multiple_fields)
    {
      auto const list
        = list_type{ match::eth_type{0x0800}, match::ip_proto{6} };

      BOOST_TEST(v13::oxm_match_field_set::is_valid_set(list));
    }
    BOOST_AUTO_TEST_CASE(false_if_match_containes_same_fields)
    {
      auto const list = list_type{ match::in_port{3}, match::in_port{4} };

      BOOST_TEST(!v13::oxm_match_field_set::is_valid_set(list));
    }
  BOOST_AUTO_TEST_SUITE_END() // is_valid_set

  BOOST_FIXTURE_TEST_SUITE(equivalent_as_set, parameters)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const list = list_type{ eth_type, vlan_vid };

      BOOST_TEST(v13::oxm_match_field_set::equivalent_as_set(list, list));
    }
    BOOST_AUTO_TEST_CASE(true_if_both_matchs_are_empty)
    {

      BOOST_TEST(
          v13::oxm_match_field_set::equivalent_as_set(
              list_type{}
            , list_type{}));
    }
    BOOST_AUTO_TEST_CASE(true_if_both_field_values_are_equal)
    {
      BOOST_TEST(
          v13::oxm_match_field_set::equivalent_as_set(
              list_type{ in_port, vlan_vid }
            , list_type{ in_port, vlan_vid }));
    }
    BOOST_AUTO_TEST_CASE(false_if_field_values_is_not_equal)
    {
      BOOST_TEST(
          !v13::oxm_match_field_set::equivalent_as_set(
              list_type{ in_port, match::eth_type{0x0800} }
            , list_type{ in_port, match::eth_type{0x8100} }));
    }
    BOOST_AUTO_TEST_CASE(true_if_field_value_is_not_equal_but_equivalent)
    {
      BOOST_TEST(
          v13::oxm_match_field_set::equivalent_as_set(
              list_type{ vlan_vid, match::vlan_pcp{4} }
            , list_type{ vlan_vid, match::vlan_pcp{4, 0xff} }));
    }
    BOOST_AUTO_TEST_CASE(false_if_fiel_type_is_not_equal)
    {
      BOOST_TEST(
          !v13::oxm_match_field_set::equivalent_as_set(
              list_type{ in_port, eth_dst }
            , list_type{ in_port, eth_src }));
    }
    BOOST_AUTO_TEST_CASE(true_if_field_order_is_not_equal)
    {
      BOOST_TEST(
          v13::oxm_match_field_set::equivalent_as_set(
              list_type{ in_port, eth_type, eth_dst }
            , list_type{ eth_type, in_port, eth_dst }));
    }
    BOOST_AUTO_TEST_CASE(false_if_number_of_field_is_not_equal)
    {
      BOOST_TEST(
          !v13::oxm_match_field_set::equivalent_as_set(
              list_type{ in_port, vlan_vid }
            , list_type{ in_port }));
    }
    BOOST_AUTO_TEST_CASE(
      true_if_number_of_field_is_not_equal_but_one_is_wildcard)
    {
      BOOST_TEST(
          v13::oxm_match_field_set::equivalent_as_set(
              list_type{ in_port, match::vlan_vid{0, 0} }
            , list_type{ in_port }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equivalent_as_set

BOOST_AUTO_TEST_SUITE_END() // oxm_match_field_set
BOOST_AUTO_TEST_SUITE_END() // utility_test
