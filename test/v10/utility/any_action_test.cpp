#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/any_action.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace actions = ofp::v10::actions;

namespace {

struct any_action_fixiture
{
  std::vector<unsigned char> buf;

  actions::output output = actions::output::to_controller(0x1234);
  std::vector<unsigned char> output_binary
    = "\x00\x00\x00\x08\xff\xfd\x12\x34"_bin;

  actions::enqueue enqueue{0x12345678, ofp::v10::protocol::OFPP_MAX};
  std::vector<unsigned char> enqueue_binary
    = "\x00\x0b\x00\x10\xff\x00\x00\x00""\x00\x00\x00\x00\x12\x34\x56\x78"_bin;

  actions::strip_vlan strip_vlan{};
  std::vector<unsigned char> strip_vlan_binary
    = "\x00\x03\x00\x08\x00\x00\x00\x00"_bin;

  actions::set_eth_src set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac};
  std::vector<unsigned char> set_eth_src_binary
    = "\x00\x04\x00\x10\x01\x02\x03\x04""\x05\x06\x00\x00\x00\x00\x00\x00"_bin;

  actions::set_ipv4_dst set_ipv4_dst{"192.168.1.254"_ipv4};
  std::vector<unsigned char> set_ipv4_dst_binary
    = "\x00\x07\x00\x08\xc0\xa8\x01\xfe"_bin;

  actions::set_tcp_src set_tcp_src{0x1234};
  std::vector<unsigned char> set_tcp_src_binary
    = "\x00\x09\x00\x08\x12\x34\x00\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(utility_test)
BOOST_AUTO_TEST_SUITE(any_action_test)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_output_action)
    {
      auto const output = actions::output{1};

      auto const sut = ofp::v10::any_action{output};

      BOOST_TEST(sut.type() == output.type());
      BOOST_TEST(sut.length() == output.length());
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::output>(&sut) != nullptr));
      BOOST_TEST((ofp::v10::any_cast<actions::output>(sut) == output));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_enqueue_action)
    {
      auto const enqueue = actions::enqueue{1, 1};

      auto const sut = ofp::v10::any_action{enqueue};

      BOOST_TEST(sut.type() == enqueue.type());
      BOOST_TEST(sut.length() == enqueue.length());
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::enqueue>(&sut) != nullptr));
      BOOST_TEST((ofp::v10::any_cast<actions::enqueue>(sut) == enqueue));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_strip_vlan_action)
    {
      auto const strip_vlan = actions::strip_vlan{};

      auto const sut = ofp::v10::any_action{strip_vlan};

      BOOST_TEST(sut.type() == strip_vlan.type());
      BOOST_TEST(sut.length() == strip_vlan.length());
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::strip_vlan>(&sut) != nullptr));
      BOOST_TEST((ofp::v10::any_cast<actions::strip_vlan>(sut) == strip_vlan));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_set_vlan_vid_action)
    {
      auto const set_vlan_vid = actions::set_vlan_vid{0x0123};

      auto const sut = ofp::v10::any_action{set_vlan_vid};

      BOOST_TEST(sut.type() == set_vlan_vid.type());
      BOOST_TEST(sut.length() == set_vlan_vid.length());
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::set_vlan_vid>(&sut) != nullptr));
      BOOST_TEST(
          (ofp::v10::any_cast<actions::set_vlan_vid>(sut) == set_vlan_vid));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_set_eth_src_action)
    {
      auto const set_eth_src
        = actions::set_eth_src{"\x01\x02\x03\x04\x05\x06"_mac};

      auto const sut = ofp::v10::any_action{set_eth_src};

      BOOST_TEST(sut.type() == set_eth_src.type());
      BOOST_TEST(sut.length() == set_eth_src.length());
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::set_eth_src>(&sut) != nullptr));
      BOOST_TEST(
          (ofp::v10::any_cast<actions::set_eth_src>(sut) == set_eth_src));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_set_ipv4_dst_action)
    {
      auto const set_ipv4_dst = actions::set_ipv4_dst{"127.0.0.1"_ipv4};

      auto const sut = ofp::v10::any_action{set_ipv4_dst};

      BOOST_TEST(sut.type() == set_ipv4_dst.type());
      BOOST_TEST(sut.length() == set_ipv4_dst.length());
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::set_ipv4_dst>(&sut) != nullptr));
      BOOST_TEST(
          (ofp::v10::any_cast<actions::set_ipv4_dst>(sut) == set_ipv4_dst));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_set_tcp_src_action)
    {
      auto const set_tcp_src = actions::set_tcp_src{6653};

      auto const sut = ofp::v10::any_action{set_tcp_src};

      BOOST_TEST(sut.type() == set_tcp_src.type());
      BOOST_TEST(sut.length() == set_tcp_src.length());
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::set_tcp_src>(&sut) != nullptr));
      BOOST_TEST(
          (ofp::v10::any_cast<actions::set_tcp_src>(sut) == set_tcp_src));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = ofp::v10::any_action{actions::output{1}};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_contained_action_value_is_equal)
    {
      auto const enqueue = actions::enqueue{1, 2};
      BOOST_TEST(
          (ofp::v10::any_action{enqueue} == ofp::v10::any_action{enqueue}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_contained_action_value_is_not_equal)
    {
      BOOST_TEST(
          (ofp::v10::any_action{actions::enqueue{1, 2}}
        != ofp::v10::any_action{actions::enqueue{1, 3}}));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_contained_action_is_not_equal_but_equivalent)
    {
      BOOST_TEST(
          (ofp::v10::any_action{actions::output{1, 2}}
        != ofp::v10::any_action{actions::output{1, 3}}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_contained_action_type_is_not_equal)
    {
      BOOST_TEST(
          (ofp::v10::any_action{actions::set_tcp_src{1}}
        != ofp::v10::any_action{actions::set_tcp_dst{1}}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_right_operand_is_equal_value_action)
    {
      auto const output = actions::output{1};

      BOOST_TEST((ofp::v10::any_action{output} == output));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_right_operand_is_not_equal_value_action)
    {
      BOOST_TEST(
          (ofp::v10::any_action{actions::output{1}} != actions::output{2}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_right_operand_is_not_equal_type_action)
    {
      BOOST_TEST(
          (ofp::v10::any_action{actions::output{1}} != actions::strip_vlan{}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_left_operand_is_equal_value_action)
    {
      auto const set_ipv4_dst = actions::set_ipv4_dst{"127.0.0.1"_ipv4};

      BOOST_TEST((set_ipv4_dst == ofp::v10::any_action{set_ipv4_dst}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_left_operand_is_not_equal_value_action)
    {
      BOOST_TEST(
          (actions::set_vlan_vid{2}
        != ofp::v10::any_action{actions::set_vlan_vid{1}}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_left_operand_is_not_equal_type_action)
    {
      BOOST_TEST(
          (actions::set_vlan_pcp{1}
        != ofp::v10::any_action{actions::set_vlan_vid{1}}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(function_equivalent)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = ofp::v10::any_action{actions::output{1}};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_contained_action_value_is_equal)
    {
      auto const enqueue = actions::enqueue{1, 2};
      BOOST_TEST(
          equivalent(
            ofp::v10::any_action{enqueue}, ofp::v10::any_action{enqueue}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_contained_action_value_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              ofp::v10::any_action{actions::enqueue{1, 2}}
            , ofp::v10::any_action{actions::enqueue{1, 3}}));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_contained_action_is_not_equal_but_equivalent)
    {
      BOOST_TEST(
          equivalent(
              ofp::v10::any_action{actions::output{1, 2}}
            , ofp::v10::any_action{actions::output{1, 3}}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_contained_action_type_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              ofp::v10::any_action{actions::set_tcp_src{1}}
            , ofp::v10::any_action{actions::set_tcp_dst{1}}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_right_operand_is_equal_value_action)
    {
      auto const output = actions::output{1};

      BOOST_TEST(equivalent(ofp::v10::any_action{output}, output));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_right_operand_is_not_equal_value_action)
    {
      BOOST_TEST(
          !equivalent(
            ofp::v10::any_action{actions::output{1}}, actions::output{2}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_right_operand_is_not_equal_type_action)
    {
      BOOST_TEST(
          !equivalent(
            ofp::v10::any_action{actions::output{1}}, actions::strip_vlan{}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_left_operand_is_equal_value_action)
    {
      auto const set_ipv4_dst = actions::set_ipv4_dst{"127.0.0.1"_ipv4};

      BOOST_TEST(equivalent(set_ipv4_dst, ofp::v10::any_action{set_ipv4_dst}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_left_operand_is_not_equal_value_action)
    {
      BOOST_TEST(
          !equivalent(
              actions::set_vlan_vid{2}
            , ofp::v10::any_action{actions::set_vlan_vid{1}}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_left_operand_is_not_equal_type_action)
    {
      BOOST_TEST(
          !equivalent(
              actions::set_vlan_pcp{1}
            , ofp::v10::any_action{actions::set_vlan_vid{1}}));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_FIXTURE_TEST_SUITE(encode, any_action_fixiture)
    BOOST_AUTO_TEST_CASE(generate_output_binary_if_containing_output)
    {
      auto const sut = ofp::v10::any_action{output};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == output_binary, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generate_enqueue_binary_if_containing_enqueue)
    {
      auto const sut = ofp::v10::any_action{enqueue};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == enqueue_binary, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generate_strip_vlan_binary_if_containing_strip_vlan)
    {
      auto const sut = ofp::v10::any_action{strip_vlan};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == strip_vlan_binary, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generate_set_eth_src_binary_if_containing_set_eth_src)
    {
      auto const sut = ofp::v10::any_action{set_eth_src};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == set_eth_src_binary, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(
        generate_set_ipv4_dst_binary_if_containing_set_ipv4_dst)
    {
      auto const sut = ofp::v10::any_action{set_ipv4_dst};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == set_ipv4_dst_binary, boost::test_tools::per_element{});
    }
    BOOST_AUTO_TEST_CASE(generate_set_tcp_src_binary_if_containing_set_tcp_src)
    {
      auto const sut = ofp::v10::any_action{set_tcp_src};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == set_tcp_src_binary, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, any_action_fixiture)
    BOOST_AUTO_TEST_CASE(constructible_from_output_binary)
    {
      auto it = output_binary.begin();

      auto const sut = ofp::v10::any_action::decode(it, output_binary.end());

      BOOST_TEST((it == output_binary.end()));
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::output>(&sut) != nullptr));
      BOOST_TEST((sut == output));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_enqueue_binary)
    {
      auto it = enqueue_binary.begin();

      auto const sut = ofp::v10::any_action::decode(it, enqueue_binary.end());

      BOOST_TEST((it == enqueue_binary.end()));
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::enqueue>(&sut) != nullptr));
      BOOST_TEST((sut == enqueue));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_strip_vlan_binary)
    {
      auto it = strip_vlan_binary.begin();

      auto const sut
        = ofp::v10::any_action::decode(it, strip_vlan_binary.end());

      BOOST_TEST((it == strip_vlan_binary.end()));
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::strip_vlan>(&sut) != nullptr));
      BOOST_TEST((sut == strip_vlan));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_set_eth_src_binary)
    {
      auto it = set_eth_src_binary.begin();

      auto const sut
        = ofp::v10::any_action::decode(it, set_eth_src_binary.end());

      BOOST_TEST((it == set_eth_src_binary.end()));
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::set_eth_src>(&sut) != nullptr));
      BOOST_TEST((sut == set_eth_src));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_set_ipv4_dst_binary)
    {
      auto it = set_ipv4_dst_binary.begin();

      auto const sut
        = ofp::v10::any_action::decode(it, set_ipv4_dst_binary.end());

      BOOST_TEST((it == set_ipv4_dst_binary.end()));
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::set_ipv4_dst>(&sut) != nullptr));
      BOOST_TEST((sut == set_ipv4_dst));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_set_tcp_src_binary)
    {
      auto it = set_tcp_src_binary.begin();

      auto const sut
        = ofp::v10::any_action::decode(it, set_tcp_src_binary.end());

      BOOST_TEST((it == set_tcp_src_binary.end()));
      BOOST_TEST_REQUIRE(
          (ofp::v10::any_cast<actions::set_tcp_src>(&sut) != nullptr));
      BOOST_TEST((sut == set_tcp_src));
    }
    BOOST_AUTO_TEST_CASE(throw_exception_if_action_type_is_unknown)
    {
      auto const binary = "\xff\x00\x00\x08\x00\x00\x00\x00"_bin;
      auto it = binary.begin();
      namespace protocol = ofp::v10::protocol;

      BOOST_CHECK_EXCEPTION(
            ofp::v10::any_action::decode(it, binary.end())
          , ofp::v10::exception
          , [](ofp::v10::exception const& e) {
              return e.error_type() == protocol::error_type::bad_action
                  && e.error_code() == protocol::bad_action_code::bad_type;
            });
      BOOST_TEST((it == binary.begin()));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

  BOOST_AUTO_TEST_SUITE(reference_style_any_cast)
    BOOST_AUTO_TEST_CASE(return_contained_action_if_specified_type_is_same)
    {
      using action_type = actions::enqueue;
      auto const action = action_type{1, 2};
      auto const sut = ofp::v10::any_action{action};

      auto const& value = ofp::v10::any_cast<action_type>(sut);

      BOOST_TEST((value == action));
    }
    BOOST_AUTO_TEST_CASE(throw_exception_if_specified_type_is_not_same)
    {
      auto const sut = ofp::v10::any_action{actions::strip_vlan{}};

      BOOST_CHECK_THROW(
          ofp::v10::any_cast<actions::output>(sut), boost::bad_get);
    }
  BOOST_AUTO_TEST_SUITE_END() // reference_style_any_cast

  BOOST_AUTO_TEST_SUITE(pointer_style_any_cast)
    BOOST_AUTO_TEST_CASE(
        return_pointer_to_contained_action_if_specified_type_is_same)
    {
      using action_type = actions::enqueue;
      auto const action = action_type{1, 2};
      auto const sut = ofp::v10::any_action{action};

      auto const value = ofp::v10::any_cast<action_type>(&sut);

      BOOST_TEST_REQUIRE((value != nullptr));
      BOOST_TEST((*value == action));
    }
    BOOST_AUTO_TEST_CASE(return_null_pointer_if_specified_type_is_not_same)
    {
      auto const sut = ofp::v10::any_action{actions::strip_vlan{}};

      auto const value = ofp::v10::any_cast<actions::set_vlan_vid>(&sut);

      BOOST_TEST((value == nullptr));
    }
  BOOST_AUTO_TEST_SUITE_END() // pointer_style_any_cast

BOOST_AUTO_TEST_SUITE_END() // any_action_test
BOOST_AUTO_TEST_SUITE_END() // utility_test
