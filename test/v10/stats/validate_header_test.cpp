#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/messages.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <boost/fusion/adapted/std_tuple.hpp>
#include "../../test_utility.hpp"

namespace v10 = canard::net::ofp::v10;
namespace protocol = v10::protocol;
namespace stats = v10::messages::statistics;

using stats_request_types = std::tuple<
    stats::aggregate_stats_request
  , stats::description_request
  , stats::flow_stats_request
  , stats::port_stats_request
  , stats::queue_stats_request
  , stats::table_stats_request
>;
using fixed_length_stats_reply_types = std::tuple<
    stats::aggregate_stats_reply
  , stats::description_reply
>;
using gradual_length_stats_reply_types = std::tuple<
    stats::port_stats_reply
  , stats::queue_stats_reply
  , stats::table_stats_reply
>;
using stats_reply_types = std::tuple<
    stats::aggregate_stats_reply
  , stats::description_reply
  , stats::flow_stats_reply
  , stats::port_stats_reply
  , stats::queue_stats_reply
  , stats::table_stats_reply
>;
using all_stats_types = std::tuple<
    stats::aggregate_stats_request
  , stats::aggregate_stats_reply
  , stats::description_request
  , stats::description_reply
  , stats::flow_stats_request
  , stats::flow_stats_reply
  , stats::port_stats_request
  , stats::port_stats_reply
  , stats::queue_stats_request
  , stats::queue_stats_reply
  , stats::table_stats_request
  , stats::table_stats_reply
>;

static auto& random_xid = ::random<std::uint32_t>;
static auto& random_flags = ::random<std::uint16_t>;

BOOST_AUTO_TEST_SUITE(stats_test)
  BOOST_AUTO_TEST_SUITE(validate_header_for_ofp_header)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, T, all_stats_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION, T::type()
        , sizeof(typename T::ofp_type), random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_version_is_not_v1, T, all_stats_types)
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
        return_error_message_if_type_is_incorrect, T, all_stats_types)
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
        , T, all_stats_types)
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
        , T, all_stats_types)
    {
      auto const header = protocol::ofp_header{
          protocol::OFP_VERSION, T::type()
        , sizeof(typename T::ofp_type) + 1, random_xid()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header_for_ofp_header

  BOOST_AUTO_TEST_SUITE(validate_header_for_stats_request)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, T, stats_request_types)
    {
      auto const header = protocol::ofp_stats_request{
          {protocol::OFP_VERSION, T::type(), T::min_length(), random_xid()}
        , T::stats_type(), random_flags()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_stats_type_is_incorrect, T, stats_request_types)
    {
      auto const header = protocol::ofp_stats_request{
          {protocol::OFP_VERSION, T::type(), T::min_length(), random_xid()}
        , T::stats_type() + 1, random_flags()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid stats type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , T, stats_request_types)
    {
      auto const header = protocol::ofp_stats_request{
          {protocol::OFP_VERSION, T::type(), T::min_length() - 1, random_xid()}
        , T::stats_type(), random_flags()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid stats length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size
        , T, stats_request_types)
    {
      auto const header = protocol::ofp_stats_request{
          {protocol::OFP_VERSION, T::type(), T::min_length() + 1, random_xid()}
        , T::stats_type(), random_flags()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid stats length"_sr);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header_for_stats_request

  BOOST_AUTO_TEST_SUITE(validate_header_for_stats_reply)
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_nullptr_if_header_is_valid, T, stats_reply_types)
    {
      auto const header = protocol::ofp_stats_reply{
          {protocol::OFP_VERSION, T::type(), T::min_length(), random_xid()}
        , T::stats_type(), random_flags()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
        return_error_message_if_stats_type_is_incorrect, T, stats_reply_types)
    {
      auto const header = protocol::ofp_stats_reply{
          {protocol::OFP_VERSION, T::type(), T::min_length(), random_xid()}
        , T::stats_type() + 1, random_flags()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid stats type"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_less_than_ofp_struct_size
        , T, stats_reply_types)
    {
      auto const header = protocol::ofp_stats_reply{
          {protocol::OFP_VERSION, T::type(), T::min_length() - 1, random_xid()}
        , T::stats_type(), random_flags()
      };

      auto const error_msg = T::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid stats length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_greater_than_ofp_struct_size_for
        , FixedLengthStats, fixed_length_stats_reply_types)
    {
      auto const header = protocol::ofp_stats_reply{
          {
              protocol::OFP_VERSION, FixedLengthStats::type()
            , FixedLengthStats::min_length() + 1, random_xid()
          }
        , FixedLengthStats::stats_type(), random_flags()
      };

      auto const error_msg = FixedLengthStats::validate_header(header);

      BOOST_TEST_REQUIRE(bool(error_msg), "should not be nullptr");
      BOOST_TEST(error_msg == "invalid stats length"_sr);
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_nullptr_if_length_is_adjust_to_element_size_for
        , GradualLengthStats, gradual_length_stats_reply_types)
    {
      constexpr auto elem_length
        = GradualLengthStats::body_type::value_type::min_length();
      for (auto const num_elem : { 0, 1, 2, 3, 4 }) {
        auto const header = protocol::ofp_stats_reply{
            {
                protocol::OFP_VERSION
              , GradualLengthStats::type()
              , std::uint16_t(
                  GradualLengthStats::min_length() + elem_length * num_elem)
              , random_xid()
            }
          , GradualLengthStats::stats_type(), random_flags()
        };

        auto const error_msg = GradualLengthStats::validate_header(header);

        BOOST_TEST(
              !error_msg
            , "should be nullptr: " << error_msg << " on num_elem = " << num_elem);
      }
    }
    BOOST_AUTO_TEST_CASE_TEMPLATE(
          return_error_message_if_length_is_not_adjust_to_element_size_for
        , GradualLengthStats, gradual_length_stats_reply_types)
    {
      constexpr auto elem_length
        = GradualLengthStats::body_type::value_type::min_length();
      for (auto const num_elem : { 0, 1, 2, 3, 4 }) {
        auto const header = protocol::ofp_stats_reply{
            {
                protocol::OFP_VERSION
              , GradualLengthStats::type()
              , std::uint16_t(
                  GradualLengthStats::min_length() + elem_length * num_elem + 1)
              , random_xid()
            }
          , GradualLengthStats::stats_type(), random_flags()
        };

        auto const error_msg = GradualLengthStats::validate_header(header);

        BOOST_TEST_REQUIRE(
              bool(error_msg)
            , "should not be nullptr on num_elem = " << num_elem);
        BOOST_TEST(
              error_msg == "invalid stats length"_sr
            , error_msg << " on num_elem = " << num_elem);
      }
    }
    BOOST_AUTO_TEST_CASE(
        return_nullptr_if_length_is_greater_than_ofp_struct_size)
    {
      auto const header = protocol::ofp_stats_reply{
          {
              protocol::OFP_VERSION, stats::flow_stats_reply::type()
            , stats::flow_stats_reply::min_length() + 1, random_xid()
          }
        , stats::flow_stats_reply::stats_type(), random_flags()
      };

      auto const error_msg = stats::flow_stats_reply::validate_header(header);

      BOOST_TEST(!error_msg, "should be nullptr: " << error_msg);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header_for_stats_reply

BOOST_AUTO_TEST_SUITE_END() // stats_test
