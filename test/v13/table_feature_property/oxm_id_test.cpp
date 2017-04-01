#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/table_feature_property/id/oxm_id.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/range/algorithm_ext/push_back.hpp>
#include <canard/net/ofp/v13/common/oxm_match_field.hpp>
#include <canard/net/ofp/v13/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace header_ops = v13::oxm_header_ops;

namespace {
  struct oxm_id_fixture
  {
    v13::oxm_id sut{protocol::OFPXMC_OPENFLOW_BASIC, 27, true, 32};
    std::vector<unsigned char> bin = "\x80\x00\x37\x20"_bin;
  };
  struct experimenter_oxm_id_fixture
  {
    v13::oxm_id sut{0x32, true, 16, 0x12345678};
    std::vector<unsigned char> bin = "\xff\xff\x65\x10\x12\x34\x56\x78"_bin;
  };
}

BOOST_AUTO_TEST_SUITE(table_feature_property_test)
BOOST_AUTO_TEST_SUITE(oxm_id)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_oxm_header)
    {
      auto const oxm_header
        = v13::oxm_match_fields::ipv4_src::make_wild_header();

      v13::oxm_id const sut{oxm_header};

      BOOST_TEST(sut.oxm_header() == oxm_header);
      BOOST_TEST(sut.oxm_class() == header_ops::oxm_class(oxm_header));
      BOOST_TEST(sut.oxm_field() == header_ops::oxm_field(oxm_header));
      BOOST_TEST(sut.oxm_type() == header_ops::oxm_type(oxm_header));
      BOOST_TEST(sut.oxm_hasmask() == header_ops::oxm_hasmask(oxm_header));
      BOOST_TEST(sut.oxm_length() == header_ops::oxm_length(oxm_header));
      BOOST_TEST(!sut.is_experimenter());
      BOOST_TEST(sut.length() == sizeof(std::uint32_t));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_experimenter_id_and_data)
    {
      auto const oxm_field = std::uint8_t{0x12};
      auto const oxm_hasmask = true;
      auto const oxm_length = std::uint8_t{8};
      auto const exp_id = std::uint32_t{0x12345678};

      v13::oxm_id const sut{oxm_field, oxm_hasmask, oxm_length, exp_id};

      BOOST_TEST(sut.oxm_header() == header_ops::oxm_header(
            protocol::OFPXMC_EXPERIMENTER, oxm_field, oxm_hasmask, oxm_length));
      BOOST_TEST(sut.oxm_class() == protocol::OFPXMC_EXPERIMENTER);
      BOOST_TEST(sut.oxm_field() == oxm_field);
      BOOST_TEST(sut.oxm_hasmask() == oxm_hasmask);
      BOOST_TEST(sut.oxm_length() == oxm_length);
      BOOST_TEST(sut.is_experimenter());
      BOOST_TEST(sut.experimenter_id() == exp_id);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_oxm_experimenter_header));
    }
    BOOST_FIXTURE_TEST_CASE(
        copy_constructible_from_non_const_oxm_id, oxm_id_fixture)
    {
      auto copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        copy_constructible_from_const_oxm_id, oxm_id_fixture)
    {
      auto const& const_id = sut;

      auto copy = const_id;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        move_constructible_from_oxm_id, oxm_id_fixture)
    {
      auto moved = sut;

      auto copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          copy_constructible_from_non_const_experimenter_oxm_id
        , experimenter_oxm_id_fixture)
    {
      auto copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          copy_constructible_from_const_experimenter_oxm_id
        , experimenter_oxm_id_fixture)
    {
      auto const& const_id = sut;

      auto copy = const_id;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          move_constructible_from_experimenter_oxm_id
        , experimenter_oxm_id_fixture)
    {
      auto moved = sut;

      auto copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_oxm_id_object)
    {
      auto const sut
        = v13::oxm_id{v13::oxm_match_fields::vlan_vid::make_header()};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_oxm_header_is_equal)
    {
      auto const oxm_header
        = v13::oxm_match_fields::in_port::make_header();

      BOOST_TEST((v13::oxm_id{oxm_header} == v13::oxm_id{oxm_header}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_class_is_not_equal)
    {
      auto const oxm_field = protocol::ofb_match_fields::in_port;
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};

      BOOST_TEST(
          (v13::oxm_id{
           protocol::OFPXMC_NXM_0, oxm_field, oxm_hasmask, oxm_length}
        != v13::oxm_id{
           protocol::OFPXMC_NXM_1, oxm_field, oxm_hasmask, oxm_length}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_field_is_not_equal)
    {
      auto const oxm_class = protocol::OFPXMC_OPENFLOW_BASIC;
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};
      using protocol::ofb_match_fields;

      BOOST_TEST(
          (v13::oxm_id{
           oxm_class, ofb_match_fields::eth_src, oxm_hasmask, oxm_length}
        != v13::oxm_id{
           oxm_class, ofb_match_fields::eth_dst, oxm_hasmask, oxm_length}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_hasmask_is_not_equal)
    {
      auto const oxm_class = protocol::OFPXMC_OPENFLOW_BASIC;
      auto const oxm_field = protocol::ofb_match_fields::in_port;
      auto const oxm_length = std::uint8_t{4};

      BOOST_TEST(
          (v13::oxm_id{oxm_class, oxm_field, true, oxm_length}
        != v13::oxm_id{oxm_class, oxm_field, false, oxm_length}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_length_is_not_equal)
    {
      auto const oxm_class = protocol::OFPXMC_OPENFLOW_BASIC;
      auto const oxm_field = protocol::ofb_match_fields::in_port;
      auto const oxm_hasmask = false;

      BOOST_TEST(
          (v13::oxm_id{oxm_class, oxm_field, oxm_hasmask, 2}
        != v13::oxm_id{oxm_class, oxm_field, oxm_hasmask, 3}));
    }
    BOOST_AUTO_TEST_CASE(true_if_same_experimenter_oxm_id_object)
    {
      auto const sut = v13::oxm_id{1, false, 4, 0x12345678};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_oxm_header_and_experimenter_id_are_equal)
    {
      auto const oxm_field = 12;
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};
      auto const exp_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          (v13::oxm_id{oxm_field, oxm_hasmask, oxm_length, exp_id}
        == v13::oxm_id{oxm_field, oxm_hasmask, oxm_length, exp_id}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_field_is_not_equal_for_experimenter)
    {
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};
      auto const exp_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          (v13::oxm_id{1, oxm_hasmask, oxm_length, exp_id}
        != v13::oxm_id{2, oxm_hasmask, oxm_length, exp_id}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_hasmask_is_not_equal_for_experimenter)
    {
      auto const oxm_field = 12;
      auto const oxm_length = std::uint8_t{4};
      auto const exp_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          (v13::oxm_id{oxm_field, true, oxm_length, exp_id}
        != v13::oxm_id{oxm_field, false, oxm_length, exp_id}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_length_is_not_equal_for_experimenter)
    {
      auto const oxm_field = 12;
      auto const oxm_hasmask = false;
      auto const exp_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          (v13::oxm_id{oxm_field, oxm_hasmask, 1, exp_id}
        != v13::oxm_id{oxm_field, oxm_hasmask, 2, exp_id}));
    }
    BOOST_AUTO_TEST_CASE(false_if_experimenter_id_is_not_equal)
    {
      auto const oxm_field = 12;
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};

      BOOST_TEST(
          (v13::oxm_id{oxm_field, oxm_hasmask, oxm_length, 1}
        != v13::oxm_id{oxm_field, oxm_hasmask, oxm_length, 2}));
    }
    BOOST_AUTO_TEST_CASE(false_if_one_is_experimenter_and_another_is_not)
    {
      BOOST_TEST(
          (v13::oxm_id{protocol::OFPXMC_OPENFLOW_BASIC, 1, false, 1}
        != v13::oxm_id{1, false, 1, std::uint32_t{0}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_one_is_not_experimenter_and_another_is_that)
    {
      BOOST_TEST(
          (v13::oxm_id{1, false, 1, std::uint32_t{0}}
        != v13::oxm_id{protocol::OFPXMC_OPENFLOW_BASIC, 1, false, 1}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(equivalent_func)
    BOOST_AUTO_TEST_CASE(true_if_same_oxm_id_object)
    {
      auto const sut
        = v13::oxm_id{v13::oxm_match_fields::vlan_vid::make_header()};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_oxm_header_is_equal)
    {
      auto const oxm_header
        = v13::oxm_match_fields::in_port::make_header();

      BOOST_TEST(equivalent(v13::oxm_id{oxm_header}, v13::oxm_id{oxm_header}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_class_is_not_equal)
    {
      auto const oxm_field = protocol::ofb_match_fields::in_port;
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};

      BOOST_TEST(
          !equivalent(
              v13::oxm_id{
                protocol::OFPXMC_NXM_0, oxm_field, oxm_hasmask, oxm_length}
            , v13::oxm_id{
                protocol::OFPXMC_NXM_1, oxm_field, oxm_hasmask, oxm_length}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_field_is_not_equal)
    {
      auto const oxm_class = protocol::OFPXMC_OPENFLOW_BASIC;
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};
      using protocol::ofb_match_fields;

      BOOST_TEST(
          !equivalent(
              v13::oxm_id{
                oxm_class, ofb_match_fields::eth_src, oxm_hasmask, oxm_length}
            , v13::oxm_id{
                oxm_class, ofb_match_fields::eth_dst, oxm_hasmask, oxm_length}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_hasmask_is_not_equal)
    {
      auto const oxm_class = protocol::OFPXMC_OPENFLOW_BASIC;
      auto const oxm_field = protocol::ofb_match_fields::in_port;
      auto const oxm_length = std::uint8_t{4};

      BOOST_TEST(
          !equivalent(
              v13::oxm_id{oxm_class, oxm_field, true, oxm_length}
            , v13::oxm_id{oxm_class, oxm_field, false, oxm_length}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_length_is_not_equal)
    {
      auto const oxm_class = protocol::OFPXMC_OPENFLOW_BASIC;
      auto const oxm_field = protocol::ofb_match_fields::in_port;
      auto const oxm_hasmask = false;

      BOOST_TEST(
          !equivalent(
              v13::oxm_id{oxm_class, oxm_field, oxm_hasmask, 2}
            , v13::oxm_id{oxm_class, oxm_field, oxm_hasmask, 3}));
    }
    BOOST_AUTO_TEST_CASE(true_if_same_experimenter_oxm_id_object)
    {
      auto const sut = v13::oxm_id{1, false, 4, 0x12345678};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_oxm_header_and_experimenter_id_are_equal)
    {
      auto const oxm_field = 12;
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};
      auto const exp_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          equivalent(
              v13::oxm_id{oxm_field, oxm_hasmask, oxm_length, exp_id}
            , v13::oxm_id{oxm_field, oxm_hasmask, oxm_length, exp_id}));
    }
    BOOST_AUTO_TEST_CASE(false_if_experimenter_id_is_not_equal)
    {
      auto const oxm_field = 12;
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};

      BOOST_TEST(
          !equivalent(
              v13::oxm_id{oxm_field, oxm_hasmask, oxm_length, 1}
            , v13::oxm_id{oxm_field, oxm_hasmask, oxm_length, 2}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_field_is_not_equal_for_experimenter)
    {
      auto const oxm_hasmask = false;
      auto const oxm_length = std::uint8_t{4};
      auto const exp_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          !equivalent(
              v13::oxm_id{1, oxm_hasmask, oxm_length, exp_id}
            , v13::oxm_id{2, oxm_hasmask, oxm_length, exp_id}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_hasmask_is_not_equal_for_experimenter)
    {
      auto const oxm_field = 12;
      auto const oxm_length = std::uint8_t{4};
      auto const exp_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          !equivalent(
              v13::oxm_id{oxm_field, true, oxm_length, exp_id}
            , v13::oxm_id{oxm_field, false, oxm_length, exp_id}));
    }
    BOOST_AUTO_TEST_CASE(false_if_oxm_length_is_not_equal_for_experimenter)
    {
      auto const oxm_field = 12;
      auto const oxm_hasmask = false;
      auto const exp_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          !equivalent(
              v13::oxm_id{oxm_field, oxm_hasmask, 1, exp_id}
            , v13::oxm_id{oxm_field, oxm_hasmask, 2, exp_id}));
    }
    BOOST_AUTO_TEST_CASE(false_if_one_is_experimenter_and_another_is_not)
    {
      BOOST_TEST(
          !equivalent(
              v13::oxm_id{protocol::OFPXMC_OPENFLOW_BASIC, 1, false, 1}
            , v13::oxm_id{1, false, 1, std::uint32_t{0}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_one_is_not_experimenter_and_another_is_that)
    {
      BOOST_TEST(
          !equivalent(
              v13::oxm_id{1, false, 1, std::uint32_t{0}}
            , v13::oxm_id{protocol::OFPXMC_OPENFLOW_BASIC, 1, false, 1}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equivalent_func

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(
          generate_binary_from_non_experimenter_oxm_id
        , oxm_id_fixture)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.byte_length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
          generate_binary_from_experimenter_oxm_id
        , experimenter_oxm_id_fixture)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.byte_length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
          construct_non_experimenter_oxm_id_from_binary
        , oxm_id_fixture)
    {
      auto it = bin.begin();

      auto const oxm_id = v13::oxm_id::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((oxm_id == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_non_experimenter_oxm_id_from_too_long_binary
        , oxm_id_fixture)
    {
      bin.resize(bin.size() + 1);
      auto it = bin.begin();

      auto const oxm_id = v13::oxm_id::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), sizeof(std::uint32_t))));
      BOOST_TEST((oxm_id == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_experimenter_oxm_id_from_binary
        , experimenter_oxm_id_fixture)
    {
      auto it = bin.begin();

      auto const oxm_id = v13::oxm_id::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((oxm_id == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_experimenter_oxm_id_from_too_long_binary
        , experimenter_oxm_id_fixture)
    {
      bin.resize(bin.size() + 1);
      auto it = bin.begin();

      auto const oxm_id = v13::oxm_id::decode(it, bin.end());

      BOOST_TEST((it == std::next(
              bin.begin(), sizeof(protocol::ofp_oxm_experimenter_header))));
      BOOST_TEST((oxm_id == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          throw_exception_if_oxm_id_is_experimenter_and_binary_len_is_too_small
        , experimenter_oxm_id_fixture)
    {
      bin.resize(bin.size() - 1);
      auto it = bin.begin();

      BOOST_CHECK_EXCEPTION(
            v13::oxm_id::decode(it, bin.end())
          , v13::exception
          , [](v13::exception const& e) {
              return e.error_type() == protocol::error_type::bad_request
                  && e.error_code() == protocol::bad_request_code::bad_len;
            });
      BOOST_TEST(std::distance(bin.begin(), it) == sizeof(std::uint32_t));
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

BOOST_AUTO_TEST_SUITE_END() // oxm_id
BOOST_AUTO_TEST_SUITE_END() // table_feature_property_test
