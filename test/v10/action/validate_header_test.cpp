#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/actions.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace v10 = canard::net::ofp::v10;
namespace protocol = v10::protocol;
namespace actions = v10::actions;

using all_action_types = std::tuple<
    actions::enqueue
  , actions::output
  , actions::set_eth_src
  , actions::set_eth_dst
  , actions::set_vlan_vid
  , actions::set_vlan_pcp
  , actions::set_ip_dscp
  , actions::set_ipv4_src
  , actions::set_ipv4_dst
  , actions::set_tcp_src
  , actions::set_tcp_dst
  , actions::strip_vlan
>;

static auto& random_pad = ::random<std::uint8_t>;

BOOST_AUTO_TEST_SUITE(actions_test)
  BOOST_AUTO_TEST_SUITE(validate_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, T, all_action_types)
    {
      auto const header = protocol::ofp_action_header{
          T::type(), T::min_length()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_type_is_incorrect, T, all_action_types)
    {
      auto const header = protocol::ofp_action_header{
          T::type() + 1, T::min_length()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid action type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , T, all_action_types)
    {
      auto const header = protocol::ofp_action_header{
          T::type(), T::min_length() - 1
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid action length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size
        , T, all_action_types)
    {
      auto const header = protocol::ofp_action_header{
          T::type(), T::min_length() + 1
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid action length"_sr);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header
BOOST_AUTO_TEST_SUITE_END() // actions_test
