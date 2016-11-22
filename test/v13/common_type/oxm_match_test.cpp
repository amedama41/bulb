#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/common/oxm_match.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace fields = v13::oxm_match_fields;

namespace {

  using proto = v13::protocol;
  using oxm_fields_type = v13::oxm_match::oxm_fields_type;

  struct parameter
  {
    fields::in_port in_port{proto::OFPP_MAX}; // 8
    fields::eth_dst eth_dst{"\x01\x02\x03\x04\x05\x06"_mac}; // 10
    fields::eth_src eth_src{"\x01\x02\x03\x04\x05\x06"_mac}; // 10
    fields::eth_type eth_type{0x0800}; // 6
    fields::vlan_vid vlan_vid{
      proto::OFPVID_PRESENT | 0x0023, proto::OFPVID_PRESENT | 0x00ff
    }; // 8
    fields::ip_proto ip_proto{6}; // 5
    fields::ipv4_src ipv4_src{"192.168.10.0"_ipv4, 24}; // 12
    oxm_fields_type oxm_fields{
      in_port, eth_dst, eth_src, eth_type, vlan_vid, ip_proto, ipv4_src
    }; // 8 + 10 + 10 + 6 + 8 + 5 + 12 = 59
  };

  struct oxm_match_fixture : parameter
  {
    v13::oxm_match sut{ oxm_fields };
    std::vector<unsigned char> bin
      = "\x00\x01\x00\x3f"
        "\x80\x00\x00\x04\xff\xff\xff\x00"
        "\x80\x00\x06\x06\x01\x02\x03\x04\x05\x06"
        "\x80\x00\x08\x06\x01\x02\x03\x04\x05\x06"
        "\x80\x00\x0a\x02\x08\x00"
        "\x80\x00\x0d\x04\x10\x23\x10\xff"
        "\x80\x00\x14\x01\x06"
        "\x80\x00\x17\x08\xc0\xa8\x0a\x00\xff\xff\xff\x00"
        "\x00"
        ""_bin;
  };

  struct empty_oxm_match_fixture
  {
    v13::oxm_match sut{};
    std::vector<unsigned char> bin = "\x00\x01\x00\x04\x00\x00\x00\x00"_bin;
  };

  struct non_padding_fixture : parameter
  {
    v13::oxm_match sut{oxm_fields_type{ eth_dst, eth_src }};
    std::vector<unsigned char> bin
      = "\x00\x01\x00\x18"
        "\x80\x00\x06\x06\x01\x02\x03\x04\x05\x06"
        "\x80\x00\x08\x06\x01\x02\x03\x04\x05\x06"
        ""_bin;
  };

  struct duplicated_field_fixture : parameter
  {
    v13::oxm_match sut{oxm_fields_type{ in_port, in_port }};
    std::vector<unsigned char> bin
      = "\x00\x01\x00\x14"
        "\x80\x00\x00\x04\xff\xff\xff\x00"
        "\x80\x00\x00\x04\xff\xff\xff\x00"
        "\x00\x00\x00\x00"
        ""_bin;
  };

  struct prerequisite_violation_fixture : parameter
  {
    v13::oxm_match sut{oxm_fields_type{ ip_proto }};
    std::vector<unsigned char> bin
      = "\x00\x01\x00\x09"
        "\x80\x00\x14\x01\x06"
        "\x00\x00\x00\x00\x00\x00\x00"
        ""_bin;
  };

}

BOOST_AUTO_TEST_SUITE(common_type_test)
BOOST_AUTO_TEST_SUITE(oxm_match)

  BOOST_AUTO_TEST_SUITE(type_definition_test)
    BOOST_AUTO_TEST_CASE(type)
    {
      using sut = v13::oxm_match;

      using type = std::integral_constant<std::uint16_t, sut::type()>;

      BOOST_TEST(type::value == v13::protocol::OFPMT_OXM);
    }
    BOOST_AUTO_TEST_CASE(min_length)
    {
      using sut = v13::oxm_match;

      using min_length
        = std::integral_constant<std::uint16_t, sut::min_length()>;

      BOOST_TEST(min_length::value == 4);
    }
    BOOST_AUTO_TEST_CASE(min_byte_length)
    {
      using sut = v13::oxm_match;

      using min_byte_length
        = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

      BOOST_TEST(min_byte_length::value == 8);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition_test

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      v13::oxm_match const sut{};

      BOOST_TEST(sut.length() == 4);
      BOOST_TEST(sut.byte_length() == 8);
      BOOST_TEST(sut.oxm_fields().empty());
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_empty_list)
    {
      auto const list = oxm_fields_type{};

      v13::oxm_match const sut{list};

      BOOST_TEST(sut.length() == 4);
      BOOST_TEST(sut.byte_length() == 8);
      BOOST_TEST(sut.oxm_fields().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_single_element_list, parameter)
    {
      auto const list = oxm_fields_type{ in_port };

      v13::oxm_match const sut{list};

      BOOST_TEST(sut.length() == 12);
      BOOST_TEST(sut.byte_length() == 16);
      BOOST_TEST((sut.oxm_fields() == list));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_multiple_elements_list, parameter)
    {
      auto const list = oxm_fields_type{ eth_dst, eth_type };

      v13::oxm_match const sut{list};

      BOOST_TEST(sut.length() == 20);
      BOOST_TEST(sut.byte_length() == 24);
      BOOST_TEST((sut.oxm_fields() == list));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_constructible_from_duplicated_elements_list, parameter)
    {
      auto const list = oxm_fields_type{ eth_type, eth_type };

      v13::oxm_match const sut{list};

      BOOST_TEST(sut.length() == 16);
      BOOST_TEST(sut.byte_length() == 16);
      BOOST_TEST((sut.oxm_fields() == list));
    }
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_single_field, parameter)
    {
      v13::oxm_match const sut{ in_port };

      BOOST_TEST(sut.length() == 12);
      BOOST_TEST(sut.byte_length() == 16);
      BOOST_TEST_REQUIRE(sut.oxm_fields().size() == 1);
      BOOST_TEST((sut.oxm_fields()[0] == in_port));
    }
    BOOST_FIXTURE_TEST_CASE(is_constructible_from_multiple_fields, parameter)
    {
      v13::oxm_match const sut{ in_port, eth_type, ip_proto };

      BOOST_TEST(sut.length() == 23);
      BOOST_TEST(sut.byte_length() == 24);
      BOOST_TEST_REQUIRE(sut.oxm_fields().size() == 3);
      BOOST_TEST((sut.oxm_fields()[0] == in_port));
      BOOST_TEST((sut.oxm_fields()[1] == eth_type));
      BOOST_TEST((sut.oxm_fields()[2] == ip_proto));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, oxm_match_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, oxm_match_fixture)
    {
      auto const& const_oxm_match = sut;

      auto const copy = const_oxm_match;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, oxm_match_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == 4);
      BOOST_TEST(moved.oxm_fields().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(extract_oxm_fields, oxm_match_fixture)
    BOOST_AUTO_TEST_CASE(return_oxm_fields_and_empty_self_fields)
    {
      auto const prev_fields = sut.oxm_fields();

      auto const fields = sut.extract_oxm_fields();

      BOOST_TEST((fields == prev_fields));
      BOOST_TEST(sut.length() == 4);
      BOOST_TEST(sut.byte_length() == 8);
      BOOST_TEST(sut.oxm_fields().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_oxm_fields

  BOOST_FIXTURE_TEST_SUITE(calc_ofp_length, oxm_match_fixture)
    BOOST_AUTO_TEST_CASE(return_length)
    {
      auto const max_length = std::numeric_limits<std::uint16_t>::max();
      auto const base_length = max_length - 64;

      BOOST_TEST(sut.calc_ofp_length(base_length) == max_length);
    }
    BOOST_AUTO_TEST_CASE(throw_exception_if_base_length_is_too_large)
    {
      auto const max_length = std::numeric_limits<std::uint16_t>::max();
      auto const base_length = max_length - 63;

      BOOST_CHECK_THROW(sut.calc_ofp_length(base_length), std::runtime_error);
    }
  BOOST_AUTO_TEST_SUITE_END() // calc_ofp_length

  BOOST_AUTO_TEST_SUITE(validate_header)
    BOOST_AUTO_TEST_CASE(
        throw_no_exception_if_type_is_oxm_and_length_is_equal_to_4)
    {
      using sut = v13::oxm_match;
      auto const match = v13::v13_detail::ofp_match{ proto::OFPMT_OXM, 4 };

      BOOST_CHECK_NO_THROW(sut::validate_header(match));
    }
    BOOST_AUTO_TEST_CASE(
        throw_no_exception_if_type_is_oxm_and_length_is_larger_than_4)
    {
      using sut = v13::oxm_match;
      auto const match = v13::v13_detail::ofp_match{ proto::OFPMT_OXM, 5 };

      BOOST_CHECK_NO_THROW(sut::validate_header(match));
    }
    BOOST_AUTO_TEST_CASE(
        throw_exception_if_type_is_not_oxm)
    {
      using sut = v13::oxm_match;
      auto const match = v13::v13_detail::ofp_match{ proto::OFPMT_OXM + 1, 4 };

      BOOST_CHECK_THROW(sut::validate_header(match), std::runtime_error);
    }
    BOOST_AUTO_TEST_CASE(
        throw_exception_if_type_is_oxm_but_length_is_smaller_than_4)
    {
      using sut = v13::oxm_match;
      auto const match = v13::v13_detail::ofp_match{ proto::OFPMT_OXM, 3 };

      BOOST_CHECK_THROW(sut::validate_header(match), std::runtime_error);
    }
  BOOST_AUTO_TEST_SUITE_END() // validate_header

  BOOST_FIXTURE_TEST_SUITE(equality, parameter)
    BOOST_AUTO_TEST_CASE(is_true_if_same_object)
    {
      auto const sut = v13::oxm_match{
        oxm_fields_type{ in_port, eth_type, ip_proto }
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_both_fiels_are_empty)
    {
      BOOST_TEST((v13::oxm_match{} == v13::oxm_match{}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_fields_are_equal)
    {
      BOOST_TEST(
          (v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src } }
        == v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src } }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_fields_are_not_equal)
    {
      using fields::eth_type;

      BOOST_TEST(
          (v13::oxm_match{oxm_fields_type{ in_port, eth_type{0x0800} }}
        != v13::oxm_match{oxm_fields_type{ in_port, eth_type{0x0806} }}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_fields_are_not_equal_but_equivalent)
    {
      using fields::vlan_pcp;

      BOOST_TEST(
          (v13::oxm_match{oxm_fields_type{ vlan_vid, vlan_pcp{4} }}
        != v13::oxm_match{oxm_fields_type{ vlan_vid, vlan_pcp{4, 0xff} }}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_field_type_is_not_equal)
    {
      BOOST_TEST(
          (v13::oxm_match{oxm_fields_type{ in_port, eth_dst }}
        != v13::oxm_match{oxm_fields_type{ in_port, eth_src }}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_field_order_is_not_equal)
    {
      BOOST_TEST(
          (v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src } }
        != v13::oxm_match{oxm_fields_type{ ipv4_src, eth_type } }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_fields_size_is_not_equal)
    {
      BOOST_TEST(
          (v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src, ip_proto } }
        != v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src } }));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_fields_size_is_not_equal_but_difference_field_is_wildcard)
    {
      using fields::vlan_pcp;

      BOOST_TEST(
          (v13::oxm_match{oxm_fields_type{ vlan_vid }}
        != v13::oxm_match{oxm_fields_type{ vlan_vid, vlan_pcp{2, 0x00} }}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(function_equivalent, parameter)
    BOOST_AUTO_TEST_CASE(is_true_if_same_object)
    {
      auto const sut = v13::oxm_match{
        oxm_fields_type{ in_port, eth_type, ip_proto }
      };

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_both_fiels_are_empty)
    {
      BOOST_TEST(equivalent(v13::oxm_match{}, v13::oxm_match{}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_fields_are_equal)
    {
      BOOST_TEST(
          equivalent(
              v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src } }
            , v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src } }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_fields_are_not_equal)
    {
      using fields::eth_type;

      BOOST_TEST(
          !equivalent(
              v13::oxm_match{oxm_fields_type{ in_port, eth_type{0x0800} }}
            , v13::oxm_match{oxm_fields_type{ in_port, eth_type{0x0806} }}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_fields_are_not_equal_but_equivalent)
    {
      using fields::vlan_pcp;

      BOOST_TEST(
          equivalent(
              v13::oxm_match{oxm_fields_type{ vlan_vid, vlan_pcp{4} }}
            , v13::oxm_match{oxm_fields_type{ vlan_vid, vlan_pcp{4, 0xff} }}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_field_type_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::oxm_match{oxm_fields_type{ in_port, eth_dst }}
            , v13::oxm_match{oxm_fields_type{ in_port, eth_src }}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_field_order_is_not_equal)
    {
      BOOST_TEST(
          equivalent(
              v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src } }
            , v13::oxm_match{oxm_fields_type{ ipv4_src, eth_type } }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_fields_size_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src, ip_proto } }
            , v13::oxm_match{oxm_fields_type{ eth_type, ipv4_src } }));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_fields_size_is_not_equal_but_difference_field_is_wildcard)
    {
      using fields::vlan_pcp;

      BOOST_TEST(
          equivalent(
              v13::oxm_match{oxm_fields_type{ vlan_vid }}
            , v13::oxm_match{oxm_fields_type{ vlan_vid, vlan_pcp{2, 0x00} }}));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, oxm_match_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
        generate_binary_from_empty_oxm_match, empty_oxm_match_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(generate_non_padding_binary, non_padding_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
        generate_binary_from_duplicated_field_match, duplicated_field_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
          generate_binary_from_prerequisite_violation_match
        , prerequisite_violation_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_oxm_match_from_binary, oxm_match_fixture)
    {
      auto it = bin.begin();

      auto const oxm_match = v13::oxm_match::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((oxm_match == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        construct_empty_oxm_match_from_binary, empty_oxm_match_fixture)
    {
      auto it = bin.begin();

      auto const oxm_match = v13::oxm_match::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((oxm_match == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        construct_oxm_match_from_non_padding_binary, non_padding_fixture)
    {
      auto it = bin.begin();

      auto const oxm_match = v13::oxm_match::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((oxm_match == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        construct_duplicated_field_match_from_binary, duplicated_field_fixture)
    {
      auto it = bin.begin();

      auto const oxm_match = v13::oxm_match::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((oxm_match == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_prerequisite_violation_match_from_binary
        , prerequisite_violation_fixture)
    {
      auto it = bin.begin();

      auto const oxm_match = v13::oxm_match::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((oxm_match == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // oxm_match
BOOST_AUTO_TEST_SUITE_END() // common_type_test
