#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/switch_features.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/openflow.hpp>
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;

namespace {
  struct features_request_fixture {
    v13::messages::features_request sut{64};
    std::vector<std::uint8_t> bin = "\x04\x05\00\x08\x00\x00\x00\x40"_bin;
  };
  struct features_reply_parameter {
    v13::messages::features_request request{8};
    std::uint64_t dpid = 5;
    std::uint32_t n_buffers = 7;
    std::uint8_t n_tables = 10;
    std::uint8_t auxiliary_id = 11;
    std::uint32_t capabilities
      = protocol::OFPC_TABLE_STATS | protocol::OFPC_PORT_BLOCKED;
  };
  struct features_reply_fixture : features_reply_parameter {
    v13::messages::features_reply sut{
      request, dpid, n_buffers, n_tables, auxiliary_id, capabilities
    };
    std::vector<std::uint8_t> bin
      = "\x04\x06\x00\x20\x00\x00\x00\x08""\x00\x00\x00\x00\x00\x00\x00\x05"
        "\x00\x00\x00\x07\x0a\x0b\x00\x00""\x00\x00\x01\x02\x00\x00\x00\x00"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(features_request)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      v13::messages::features_request const sut{};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FEATURES_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
    }
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const xid = std::uint32_t{32};

      v13::messages::features_request const sut{xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FEATURES_REQUEST);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_header));
      BOOST_TEST(sut.xid() == xid);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::features_request{12};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = std::uint32_t{1};

      BOOST_TEST(
          (v13::messages::features_request{xid}
        == v13::messages::features_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::features_request{1}
        != v13::messages::features_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, features_request_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, features_request_fixture)
    {
      auto it = bin.begin();

      auto const features_request
        = v13::messages::features_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((features_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // features_request


BOOST_AUTO_TEST_SUITE(features_reply)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const request = v13::messages::features_request{16};
      auto const dpid = std::uint64_t{4};
      auto const n_buffers = std::uint32_t{9};
      auto const n_tables = std::uint32_t{8};
      auto const auxiliary_id = std::uint8_t{12};
      auto const capabilities
        = protocol::OFPC_FLOW_STATS | protocol::OFPC_PORT_STATS;

      v13::messages::features_reply const sut{
        request, dpid, n_buffers, n_tables, auxiliary_id, capabilities
      };

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_FEATURES_REPLY);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_switch_features));
      BOOST_TEST(sut.xid() == request.xid());
      BOOST_TEST(sut.datapath_id() == dpid);
      BOOST_TEST(sut.num_buffers() == n_buffers);
      BOOST_TEST(sut.num_tables() == n_tables);
      BOOST_TEST(sut.auxiliary_id() == auxiliary_id);
      BOOST_TEST(sut.capabilities() == capabilities);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, features_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::features_reply{
        request, dpid, n_buffers, n_tables, auxiliary_id, capabilities
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::features_reply{
             request, dpid, n_buffers, n_tables, auxiliary_id, capabilities
           }
        == v13::messages::features_reply{
             request, dpid, n_buffers, n_tables, auxiliary_id, capabilities
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      using request = v13::messages::features_request;

      BOOST_TEST(
          (v13::messages::features_reply{
             request{1}, dpid, n_buffers, n_tables, auxiliary_id, capabilities
           }
        != v13::messages::features_reply{
             request{2}, dpid, n_buffers, n_tables, auxiliary_id, capabilities
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_dpid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::features_reply{
             request, 1, n_buffers, n_tables, auxiliary_id, capabilities
           }
        != v13::messages::features_reply{
             request, 2, n_buffers, n_tables, auxiliary_id, capabilities
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_n_buffers_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::features_reply{
             request, dpid, 1, n_tables, auxiliary_id, capabilities
           }
        != v13::messages::features_reply{
             request, dpid, 2, n_tables, auxiliary_id, capabilities
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_n_tables_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::features_reply{
             request, dpid, n_buffers, 1, auxiliary_id, capabilities
           }
        != v13::messages::features_reply{
             request, dpid, n_buffers, 2, auxiliary_id, capabilities
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_auxiliary_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::features_reply{
             request, dpid, n_buffers, n_tables, 1, capabilities
           }
        != v13::messages::features_reply{
             request, dpid, n_buffers, n_tables, 2, capabilities
           }));
    }
    BOOST_AUTO_TEST_CASE(false_if_capabilities_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::features_reply{
             request, dpid, n_buffers, n_tables, auxiliary_id, 1
           }
        != v13::messages::features_reply{
             request, dpid, n_buffers, n_tables, auxiliary_id, 2
           }));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, features_reply_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, features_reply_fixture)
    {
      auto it = bin.begin();

      auto const features_reply
        = v13::messages::features_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((features_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // features_reply
BOOST_AUTO_TEST_SUITE_END() // message_test
