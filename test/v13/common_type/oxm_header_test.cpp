#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/common/oxm_header.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>

#include <cstdint>
#include <vector>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/io/openflow.hpp>
#include "../../test_utility.hpp"
#include "../oxm_headers.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;

namespace bdata = boost::unit_test::data;

namespace {
  struct oxm_header_fixture
  {
    v13::oxm_header sut{OXM_OF_IPV4_SRC_W};
    std::vector<unsigned char> bin = "\x80\x00\x17\x08"_bin;
  };
}

using header_vec = std::vector<std::uint32_t>;
using field_vec = std::vector<std::uint8_t>;

BOOST_AUTO_TEST_SUITE(common_type_test)
BOOST_AUTO_TEST_SUITE(oxm_header)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_unsigned_integer)
    {
      constexpr v13::oxm_header header{OXM_OF_IN_PORT};

      BOOST_TEST(header.to_ofp_type() == OXM_OF_IN_PORT);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor
  BOOST_AUTO_TEST_SUITE(oxm_class)
    BOOST_DATA_TEST_CASE(
        return_oxm_class, bdata::make(header_vec{
          OXM_OF_IN_PORT, OXM_OF_IN_PORT_W, OXM_OF_ARP_OP, OXM_OF_PBB_ISID_W
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(
          header.oxm_class() == v13::protocol::oxm_class::openflow_basic);
    }
  BOOST_AUTO_TEST_SUITE_END() // oxm_class
  BOOST_AUTO_TEST_SUITE(oxm_field)
    BOOST_DATA_TEST_CASE(
          return_oxm_field
        , bdata::make(header_vec{
            OXM_OF_IN_PORT, OXM_OF_IN_PORT_W, OXM_OF_VLAN_VID, OXM_OF_METADATA
          })
        ^ bdata::make(field_vec{
              v13::protocol::ofb_match_fields::in_port
            , v13::protocol::ofb_match_fields::in_port
            , v13::protocol::ofb_match_fields::vlan_vid
            , v13::protocol::ofb_match_fields::metadata
          })
        , oxm_header_value, oxm_field_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_field() == oxm_field_value);
    }
  BOOST_AUTO_TEST_SUITE_END() // oxm_field
  BOOST_AUTO_TEST_SUITE(oxm_hasmask)
    BOOST_DATA_TEST_CASE(
        true_if_hasmask_bit_0, bdata::make(header_vec{
          OXM_OF_IN_PORT, OXM_OF_ARP_OP, OXM_OF_ETH_SRC, OXM_OF_IPV6_ND_SLL
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(!header.oxm_hasmask());
    }
    BOOST_DATA_TEST_CASE(
        true_if_hasmask_bit_1, bdata::make(header_vec{
          OXM_OF_IN_PORT_W, OXM_OF_TUNNEL_ID_W, OXM_OF_IP_ECN_W
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_hasmask());
    }
  BOOST_AUTO_TEST_SUITE_END() // oxm_hasmask
  BOOST_AUTO_TEST_SUITE(oxm_length)
    BOOST_DATA_TEST_CASE(
        return_1_if_oxm_length_is_1, bdata::make(header_vec{
          OXM_OF_VLAN_PCP, OXM_OF_IP_DSCP, OXM_OF_MPLS_TC
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 1);
    }
    BOOST_DATA_TEST_CASE(
        return_2_if_oxm_length_is_2, bdata::make(header_vec{
          OXM_OF_ETH_TYPE, OXM_OF_VLAN_PCP_W, OXM_OF_TCP_DST, OXM_OF_IPV6_EXTHDR
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 2);
    }
    BOOST_DATA_TEST_CASE(
        return_3_if_oxm_length_is_3, bdata::make(header_vec{
          OXM_OF_PBB_ISID
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 3);
    }
    BOOST_DATA_TEST_CASE(
        return_4_if_oxm_length_is_4, bdata::make(header_vec{
          OXM_OF_IN_PORT, OXM_OF_ETH_TYPE_W, OXM_OF_IPV4_SRC, OXM_OF_MPLS_LABEL
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 4);
    }
    BOOST_DATA_TEST_CASE(
        return_6_if_oxm_length_is_6, bdata::make(header_vec{
          OXM_OF_ETH_DST, OXM_OF_PBB_ISID_W, OXM_OF_ARP_SHA, OXM_OF_IPV6_ND_SLL
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 6);
    }
    BOOST_DATA_TEST_CASE(
        return_8_if_oxm_length_is_8, bdata::make(header_vec{
          OXM_OF_IN_PORT_W, OXM_OF_IPV4_SRC_W, OXM_OF_ARP_TPA_W
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 8);
    }
    BOOST_DATA_TEST_CASE(
        return_12_if_oxm_length_is_12, bdata::make(header_vec{
          OXM_OF_ETH_DST_W, OXM_OF_ARP_SHA_W, OXM_OF_IPV6_ND_SLL_W
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 12);
    }
    BOOST_DATA_TEST_CASE(
        return_16_if_oxm_length_is_16, bdata::make(header_vec{
          OXM_OF_IPV6_SRC, OXM_OF_IPV6_DST, OXM_OF_IPV6_ND_TARGET
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 16);
    }
    BOOST_DATA_TEST_CASE(
        return_32_if_oxm_length_is_32, bdata::make(header_vec{
          OXM_OF_IPV6_SRC_W, OXM_OF_IPV6_DST_W, OXM_OF_IPV6_ND_TARGET_W
        }), oxm_header_value)
    {
      auto const header = v13::oxm_header{oxm_header_value};

      BOOST_TEST(header.oxm_length() == 32);
    }
  BOOST_AUTO_TEST_SUITE_END() // oxm_length
  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_oxm_header_value_is_equal)
    {
      BOOST_TEST(
          (v13::oxm_header{OXM_OF_IN_PORT}
        == v13::oxm_header{OXM_OF_IN_PORT}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_class_is_not_equal)
    {
      BOOST_TEST(
          (v13::oxm_header{0x80000004}
        != v13::oxm_header{0x80010004}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_field_is_not_equal)
    {
      BOOST_TEST(
          (v13::oxm_header{OXM_OF_IPV4_DST}
        != v13::oxm_header{OXM_OF_IPV4_SRC}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_has_mask_is_not_equal)
    {
      BOOST_TEST(
          (v13::oxm_header{OXM_OF_IN_PORT}
        != v13::oxm_header{OXM_OF_IN_PORT_W}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_length_is_not_equal)
    {
      BOOST_TEST(
          (v13::oxm_header{0x80000002}
        != v13::oxm_header{0x80000004}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality
  BOOST_AUTO_TEST_SUITE(function_equivalent)
    BOOST_AUTO_TEST_CASE(true_if_oxm_header_value_is_equal)
    {
      BOOST_TEST(
          equivalent(
              v13::oxm_header{OXM_OF_IN_PORT}
            , v13::oxm_header{OXM_OF_IN_PORT}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_class_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::oxm_header{0x80000004}
            , v13::oxm_header{0x80010004}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_field_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::oxm_header{OXM_OF_IPV4_DST}
            , v13::oxm_header{OXM_OF_IPV4_SRC}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_has_mask_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::oxm_header{OXM_OF_IN_PORT}
            , v13::oxm_header{OXM_OF_IN_PORT_W}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_length_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::oxm_header{0x80000002}
            , v13::oxm_header{0x80000004}));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent
  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, oxm_header_fixture)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.byte_length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode
  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, oxm_header_fixture)
    {
      auto it = bin.begin();

      auto const oxm_header = v13::oxm_header::decode(it, bin.end());

      BOOST_TEST(
            (it == bin.end())
          , "read only " << std::distance(bin.begin(), it));
      BOOST_TEST(
            (oxm_header == sut)
          , oxm_header.to_ofp_type() << " == " << sut.to_ofp_type());
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // oxm_header
BOOST_AUTO_TEST_SUITE_END() // common_type_test
