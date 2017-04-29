#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart_messages.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace v13 = canard::net::ofp::v13;
namespace protocol = v13::protocol;
namespace multipart = v13::messages::multipart;

using fixed_length_multipart_request_types = std::tuple<
    multipart::description_request
  , multipart::table_stats_request
  , multipart::port_stats_request
  , multipart::queue_stats_request
  , multipart::port_description_request
>;
using variable_length_multipart_request_types = std::tuple<
    multipart::flow_stats_request
  , multipart::aggregate_stats_request
  , multipart::table_features_request
>;
using all_multipart_request_types = std::tuple<
    multipart::description_request
  , multipart::flow_stats_request
  , multipart::aggregate_stats_request
  , multipart::table_stats_request
  , multipart::port_stats_request
  , multipart::queue_stats_request
  , multipart::table_features_request
  , multipart::port_description_request
>;
using fixed_length_multipart_reply_types = std::tuple<
    multipart::description_reply
  , multipart::aggregate_stats_reply
>;
using variable_length_multipart_reply_types = std::tuple<
    multipart::flow_stats_reply
  , multipart::table_features_reply
>;
using gradual_length_multipart_reply_types = std::tuple<
    multipart::table_stats_reply
  , multipart::port_stats_reply
  , multipart::queue_stats_reply
  , multipart::port_description_reply
>;
using all_multipart_reply_types = std::tuple<
    multipart::description_reply
  , multipart::flow_stats_reply
  , multipart::aggregate_stats_reply
  , multipart::table_stats_reply
  , multipart::port_stats_reply
  , multipart::queue_stats_reply
  , multipart::table_features_reply
  , multipart::port_description_reply
>;
using all_multipart_types = std::tuple<
    multipart::description_request
  , multipart::description_reply
  , multipart::flow_stats_request
  , multipart::flow_stats_reply
  , multipart::aggregate_stats_request
  , multipart::aggregate_stats_reply
  , multipart::table_stats_request
  , multipart::table_stats_reply
  , multipart::port_stats_request
  , multipart::port_stats_reply
  , multipart::queue_stats_request
  , multipart::queue_stats_reply
  , multipart::table_features_request
  , multipart::table_features_reply
  , multipart::port_description_request
  , multipart::port_description_reply
>;

static auto& random_xid = ::random<std::uint32_t>;
static auto& random_flags = ::random<std::uint16_t>;
static auto& random_pad = ::random<std::uint8_t>;

BOOST_AUTO_TEST_SUITE(message_test)
  BOOST_AUTO_TEST_SUITE(validate_header_for_ofp_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, T, all_multipart_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION, T::type()
        , sizeof(typename T::ofp_type), random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_version_is_not_v1, T, all_multipart_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION + 1, T::type()
        , sizeof(typename T::ofp_type), random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid version"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_type_is_incorrect, T, all_multipart_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION, T::type() + 1
        , sizeof(typename T::ofp_type), random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid message type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , T, all_multipart_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION, T::type()
        , sizeof(typename T::ofp_type) - 1, random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid message length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_length_is_greater_than_ofp_struct_size
        , T, all_multipart_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION, T::type()
        , sizeof(typename T::ofp_type) + 1, random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header_for_ofp_header
  BOOST_AUTO_TEST_SUITE(validate_header_for_multipart_request)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, T, all_multipart_request_types)
    {
      auto const header = protocol::ofp_multipart_request{
          { protocol::OFP_VERSION, T::type(), T::min_length(), random_xid() }
        , T::multipart_type(), random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_type_is_incorrect, T, all_multipart_request_types)
    {
      auto const header = protocol::ofp_multipart_request{
          { protocol::OFP_VERSION, T::type(), T::min_length(), random_xid() }
        , T::multipart_type() + 1, random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid multipart type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , T, all_multipart_request_types)
    {
      auto const header = protocol::ofp_multipart_request{
          { protocol::OFP_VERSION, T::type(), T::min_length() - 1, random_xid() }
        , T::multipart_type(), random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid multipart length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size_for
        , FixedLengthMultipart, fixed_length_multipart_request_types)
    {
      auto const header = protocol::ofp_multipart_request{
          {
              protocol::OFP_VERSION, FixedLengthMultipart::type()
            , FixedLengthMultipart::min_length() + 1, random_xid()
          }
        , FixedLengthMultipart::multipart_type(), random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = FixedLengthMultipart::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid multipart length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_length_is_greater_than_ofp_struct_size_for
        , VariableLengthMultipart, variable_length_multipart_request_types)
    {
      auto const header = protocol::ofp_multipart_request{
          {
              protocol::OFP_VERSION, VariableLengthMultipart::type()
            , VariableLengthMultipart::min_length() + 1, random_xid()
          }
        , VariableLengthMultipart::multipart_type(), random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = VariableLengthMultipart::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header_for_multipart_request
  BOOST_AUTO_TEST_SUITE(validate_header_for_multipart_reply)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, T, all_multipart_reply_types)
    {
      auto const header = protocol::ofp_multipart_reply{
          { protocol::OFP_VERSION, T::type(), T::min_length(), random_xid() }
        , T::multipart_type(), random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_type_is_incorrect, T, all_multipart_reply_types)
    {
      auto const header = protocol::ofp_multipart_reply{
          { protocol::OFP_VERSION, T::type(), T::min_length(), random_xid() }
        , T::multipart_type() + 1, random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid multipart type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , T, all_multipart_reply_types)
    {
      auto const header = protocol::ofp_multipart_reply{
          { protocol::OFP_VERSION, T::type(), T::min_length() - 1, random_xid() }
        , T::multipart_type(), random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid multipart length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size_for
        , FixedLengthMultipart, fixed_length_multipart_reply_types)
    {
      auto const header = protocol::ofp_multipart_reply{
          {
              protocol::OFP_VERSION, FixedLengthMultipart::type()
            , FixedLengthMultipart::min_length() + 1, random_xid()
          }
        , FixedLengthMultipart::multipart_type(), random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = FixedLengthMultipart::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid multipart length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_length_is_greater_than_ofp_struct_size_for
        , VariableLengthMultipart, variable_length_multipart_reply_types)
    {
      auto const header = protocol::ofp_multipart_reply{
          {
              protocol::OFP_VERSION, VariableLengthMultipart::type()
            , VariableLengthMultipart::min_length() + 1, random_xid()
          }
        , VariableLengthMultipart::multipart_type(), random_flags()
        , { random_pad(), random_pad(), random_pad(), random_pad() }
      };

      auto const error_msg = VariableLengthMultipart::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_length_is_adjust_to_element_size_for
        , GradualLengthMultipart, gradual_length_multipart_reply_types)
    {
      constexpr auto elem_length
        = GradualLengthMultipart::body_type::value_type::min_length();
      for (auto const num_elem : { 0, 1, 2, 3, 4 }) {
        auto const header = protocol::ofp_multipart_reply{
            {
                protocol::OFP_VERSION, GradualLengthMultipart::type()
              , std::uint16_t(
                  GradualLengthMultipart::min_length() + elem_length * num_elem)
              , random_xid()
            }
          , GradualLengthMultipart::multipart_type(), random_flags()
          , { random_pad(), random_pad(), random_pad(), random_pad() }
        };

        auto const error_msg = GradualLengthMultipart::validate_header(header);

        BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
      }
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_not_adjust_to_element_size_for
        , GradualLengthMultipart, gradual_length_multipart_reply_types)
    {
      constexpr auto elem_length
        = GradualLengthMultipart::body_type::value_type::min_length();
      for (auto const num_elem : { 0, 1, 2, 3, 4 }) {
        auto const header = protocol::ofp_multipart_reply{
            {
                protocol::OFP_VERSION, GradualLengthMultipart::type()
              , std::uint16_t(
                    GradualLengthMultipart::min_length()
                  + elem_length * num_elem + 1)
              , random_xid()
            }
          , GradualLengthMultipart::multipart_type(), random_flags()
          , { random_pad(), random_pad(), random_pad(), random_pad() }
        };

        auto const error_msg = GradualLengthMultipart::validate_header(header);

        BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
        BOOST_TEST(error_msg == "invalid multipart length"_sr);
      }
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header_for_multipart_reply
BOOST_AUTO_TEST_SUITE_END() // message_test
