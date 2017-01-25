#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/messages.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <ctime>
#include <random>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace v10 = canard::net::ofp::v10;
namespace protocol = v10::protocol;
namespace msg = v10::messages;

namespace {
using fixed_length_message_types = std::tuple<
    msg::barrier_request
  , msg::barrier_reply
  , msg::flow_removed
  , msg::port_mod
  , msg::port_status
  , msg::queue_get_config_request
  , msg::get_config_request
  , msg::get_config_reply
  , msg::set_config
  , msg::features_request
>;
using variable_length_message_types = std::tuple<
    msg::echo_request
  , msg::echo_reply
  , msg::error
  , msg::flow_add
  , msg::flow_delete
  , msg::flow_modify
  , msg::packet_in
  , msg::packet_out
  , msg::queue_get_config_reply
>;
using all_message_types = std::tuple<
    msg::barrier_request
  , msg::barrier_reply
  , msg::echo_request
  , msg::echo_reply
  , msg::error
  , msg::flow_add
  , msg::flow_delete
  , msg::flow_modify
  , msg::flow_removed
  , msg::packet_in
  , msg::packet_out
  , msg::port_mod
  , msg::port_status
  , msg::queue_get_config_request
  , msg::queue_get_config_reply
  , msg::get_config_request
  , msg::get_config_reply
  , msg::set_config
  , msg::features_request
  , msg::features_reply
>;

auto random_xid()
  -> std::uint32_t
{
  static std::mt19937 rnd(std::time(nullptr));
  return rnd();
}
} // namespace

BOOST_AUTO_TEST_SUITE(message_test)
  BOOST_AUTO_TEST_SUITE(validate_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, T, all_message_types)
    {
      auto const header = protocol::ofp_header{
        protocol::OFP_VERSION, T::type(), T::min_length(), random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_version_is_not_v1, T, all_message_types)
    {
      auto const header = protocol::ofp_header{
        protocol::OFP_VERSION + 1, T::type(), T::min_length(), random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid version"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_type_is_incorrect, T, all_message_types)
    {
      auto const header = protocol::ofp_header{
        protocol::OFP_VERSION, T::type() + 1, T::min_length(), random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid message type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , T, all_message_types)
    {
      auto const header = protocol::ofp_header{
        protocol::OFP_VERSION, T::type(), T::min_length() - 1, random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid message length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size_for
        , FixedLengthMessage, fixed_length_message_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION
        , FixedLengthMessage::type()
        , FixedLengthMessage::min_length() + 1
        , random_xid()
      };

      auto const error_msg = FixedLengthMessage::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid message length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_length_is_greater_than_ofp_struct_size_for
        , VariableLengthMessage, variable_length_message_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION
        , VariableLengthMessage::type()
        , VariableLengthMessage::min_length() + 1
        , random_xid()
      };

      auto const error_msg = VariableLengthMessage::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_DATA_TEST_CASE(
          return_nullptr_if_length_is_adjust_to_port_length
        , boost::unit_test::data::xrange<std::size_t>(0, 5)
        , num_port)
    {
      constexpr auto port_size = sizeof(protocol::ofp_phy_port);
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION
        , msg::features_reply::type()
        , std::uint16_t(
            sizeof(protocol::ofp_switch_features) + port_size * num_port)
        , random_xid()
      };

      auto const error_msg = msg::features_reply::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_DATA_TEST_CASE(
          return_error_message_if_length_is_not_adjust_to_port_length
        , boost::unit_test::data::xrange<std::size_t>(0, 5)
        , num_port)
    {
      constexpr auto port_size = std::uint16_t(sizeof(protocol::ofp_port));
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION
        , msg::features_reply::type()
        , std::uint16_t(
            sizeof(protocol::ofp_switch_features) + port_size * num_port + 1)
        , random_xid()
      };

      auto const error_msg = msg::features_reply::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid message length"_sr);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header
BOOST_AUTO_TEST_SUITE_END() // message_test
