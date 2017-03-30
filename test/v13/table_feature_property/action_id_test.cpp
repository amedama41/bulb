#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/table_feature_property/id/action_id.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/range/algorithm_ext/push_back.hpp>
#include <canard/network/openflow/v13/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;

namespace {
  struct action_id_fixture
  {
    v13::action_id sut{0x0012};
    std::vector<unsigned char> bin = "\x00\x12\x00\x04"_bin;
  };
  struct experimenter_action_id_fixture
  {
    v13::action_id sut{0x12345678, "\x11\x22\x33\x44"_bbin};
    std::vector<unsigned char> bin
      = "\xff\xff\x00\x0c\x12\x34\x56\x78""\x11\x22\x33\x44"_bin;
  };
}

BOOST_AUTO_TEST_SUITE(table_feature_property_test)
BOOST_AUTO_TEST_SUITE(action_id)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_action_type)
    {
      auto const type = protocol::OFPAT_GROUP;

      v13::action_id const sut{type};

      BOOST_TEST(sut.type() == type);
      BOOST_TEST(sut.length() == 4);
      BOOST_TEST(!sut.is_experimenter());
      BOOST_TEST(sut.data().empty());
    }
    BOOST_AUTO_TEST_CASE(constructible_from_experimenter_id_and_data)
    {
      auto const experimenter_id = std::uint32_t{0x12345678};
      auto const data = "\x01\x02\x03\x04"_bbin;

      v13::action_id const sut{experimenter_id, data};

      BOOST_TEST(sut.type() == protocol::OFPAT_EXPERIMENTER);
      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_action_experimenter_header) + data.size());
      BOOST_TEST(sut.is_experimenter());
      BOOST_TEST(sut.experimenter_id() == experimenter_id);
      BOOST_TEST(sut.data() == data, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
        copy_constructible_from_non_const_action_id, action_id_fixture)
    {
      auto copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        copy_constructible_from_const_action_id, action_id_fixture)
    {
      auto const& const_id = sut;

      auto copy = const_id;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        move_constructible_from_action_id, action_id_fixture)
    {
      auto moved = sut;

      auto copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == 4);
      BOOST_TEST(moved.data().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
          copy_constructible_from_non_const_experimenter_action_id
        , experimenter_action_id_fixture)
    {
      auto copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          copy_constructible_from_const_experimenter_action_id
        , experimenter_action_id_fixture)
    {
      auto const& const_id = sut;

      auto copy = const_id;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          move_constructible_from_experimenter_action_id
        , experimenter_action_id_fixture)
    {
      auto moved = sut;

      auto copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(
          moved.length() == sizeof(protocol::ofp_action_experimenter_header));
      BOOST_TEST(moved.data().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_action_id_object)
    {
      auto const sut = v13::action_id{protocol::OFPAT_COPY_TTL_IN};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_same_experimenter_action_id_object)
    {
      auto const sut = v13::action_id{0x12345678, "\x01\x02"_bbin};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_action_type_is_equal)
    {
      auto const type = protocol::OFPAT_PUSH_VLAN;

      BOOST_TEST((v13::action_id{type} == v13::action_id{type}));
    }
    BOOST_AUTO_TEST_CASE(false_if_action_type_is_not_equal)
    {
      BOOST_TEST(
          (v13::action_id{protocol::OFPAT_PUSH_VLAN}
        != v13::action_id{protocol::OFPAT_POP_VLAN}));
    }
    BOOST_AUTO_TEST_CASE(true_if_experimenter_id_and_data_are_equal)
    {
      auto const experimenter_id = std::uint32_t{0x12345678};
      auto const data = "\x01\x02\x03\x04\x05\x06\x07\x08"_bbin;

      BOOST_TEST(
          (v13::action_id{experimenter_id, data}
        == v13::action_id{experimenter_id, data}));
    }
    BOOST_AUTO_TEST_CASE(false_if_experimenter_id_is_not_equal)
    {
      auto const data = "\x01\x02\x03\x04\x05\x06\x07\x08"_bbin;

      BOOST_TEST(
          (v13::action_id{0x01, data}
        != v13::action_id{0x02, data}));
    }
    BOOST_AUTO_TEST_CASE(false_if_exp_data_is_not_equal)
    {
      auto const experimenter_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          (v13::action_id{experimenter_id, "\x01"_bbin}
        != v13::action_id{experimenter_id, "\x02"_bbin}));
    }
    BOOST_AUTO_TEST_CASE(false_if_one_is_experimenter_and_another_is_not)
    {
      auto const bin = "\x00\x00\x00\x08\x01\x02\x03\x04"_bin;
      auto it = bin.begin();
      auto const non_exp_action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST((non_exp_action_id != v13::action_id{0x12345678, {}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_one_is_not_experimenter_and_another_is_that)
    {
      auto const bin = "\x00\x00\x00\x08\x01\x02\x03\x04"_bin;
      auto it = bin.begin();
      auto const non_exp_action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST((v13::action_id{0x12345678, {}} != non_exp_action_id));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(equivalent_func)
    BOOST_AUTO_TEST_CASE(true_if_same_action_id_object)
    {
      auto const sut = v13::action_id{protocol::OFPAT_COPY_TTL_IN};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_same_experimenter_action_id_object)
    {
      auto const sut = v13::action_id{0x12345678, "\x01\x02"_bbin};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_action_type_is_equal)
    {
      auto const type = protocol::OFPAT_PUSH_VLAN;

      BOOST_TEST(equivalent(v13::action_id{type}, v13::action_id{type}));
    }
    BOOST_AUTO_TEST_CASE(false_if_action_type_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::action_id{protocol::OFPAT_PUSH_VLAN}
            , v13::action_id{protocol::OFPAT_POP_VLAN}));
    }
    BOOST_AUTO_TEST_CASE(true_if_experimenter_id_and_data_are_equal)
    {
      auto const experimenter_id = std::uint32_t{0x12345678};
      auto const data = "\x01\x02\x03\x04\x05\x06\x07\x08"_bbin;

      BOOST_TEST(
          equivalent(
              v13::action_id{experimenter_id, data}
            , v13::action_id{experimenter_id, data}));
    }
    BOOST_AUTO_TEST_CASE(false_if_experimenter_id_is_not_equal)
    {
      auto const data = "\x01\x02\x03\x04\x05\x06\x07\x08"_bbin;

      BOOST_TEST(
          !equivalent(
              v13::action_id{0x01, data}
            , v13::action_id{0x02, data}));
    }
    BOOST_AUTO_TEST_CASE(false_if_exp_data_is_not_equal)
    {
      auto const experimenter_id = std::uint32_t{0x12345678};

      BOOST_TEST(
          !equivalent(
              v13::action_id{experimenter_id, "\x01"_bbin}
            , v13::action_id{experimenter_id, "\x02"_bbin}));
    }
    BOOST_AUTO_TEST_CASE(false_if_one_is_experimenter_and_another_is_not)
    {
      auto const bin = "\x00\x00\x00\x08\x01\x02\x03\x04"_bin;
      auto it = bin.begin();
      auto const non_exp_action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST(!equivalent(non_exp_action_id, v13::action_id{0x12345678, {}}));
    }
    BOOST_AUTO_TEST_CASE(false_if_one_is_not_experimenter_and_another_is_that)
    {
      auto const bin = "\x00\x00\x00\x08\x01\x02\x03\x04"_bin;
      auto it = bin.begin();
      auto const non_exp_action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST(!equivalent(v13::action_id{0x12345678, {}}, non_exp_action_id));
    }
  BOOST_AUTO_TEST_SUITE_END() // equivalent_func

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(
          generate_binary_from_non_experimenter_action_id
        , action_id_fixture)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.byte_length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
          generate_binary_from_experimenter_action_id
        , experimenter_action_id_fixture)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.byte_length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
          construct_non_experimenter_action_id_from_binary
        , action_id_fixture)
    {
      auto it = bin.begin();

      auto const action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((action_id == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_non_experimenter_action_id_from_too_long_binary
        , action_id_fixture)
    {
      auto const data = "\x11\x22\x33\x44"_bin;
      boost::push_back(bin, data);
      auto it = bin.begin();

      auto const action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), 4)));
      BOOST_TEST((action_id == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_non_experimenter_action_id_with_data_from_binary
        , action_id_fixture)
    {
      auto const data = "\x11\x22\x33\x44"_bin;
      boost::push_back(bin, data);
      bin[3] = bin.size();
      auto it = bin.begin();

      auto const action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST(action_id.type() == sut.type());
      BOOST_TEST(action_id.length() == bin.size());
      BOOST_TEST(action_id.data() == data, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_experimenter_action_id_from_binary
        , experimenter_action_id_fixture)
    {
      auto it = bin.begin();

      auto const action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((action_id == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_experimenter_action_id_from_too_long_binary
        , experimenter_action_id_fixture)
    {
      constexpr auto exp_size
        = sizeof(protocol::ofp_action_experimenter_header);
      bin[3] = exp_size;
      auto it = bin.begin();

      auto const action_id = v13::action_id::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), exp_size)));
      BOOST_TEST(action_id.type() == sut.type());
      BOOST_TEST(action_id.length() == exp_size);
      BOOST_TEST(action_id.experimenter_id() == sut.experimenter_id());
      BOOST_TEST(action_id.data().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
          throw_exception_if_action_id_len_is_too_small
        , action_id_fixture)
    {
      --bin[3];
      auto it = bin.begin();

      BOOST_CHECK_THROW(
            v13::action_id::decode(it, bin.end())
          , std::runtime_error);
      BOOST_TEST(std::distance(bin.begin(), it) == 4);
    }
    BOOST_FIXTURE_TEST_CASE(
          throw_exception_if_binary_length_is_smaller_than_action_id_len
        , action_id_fixture)
    {
      auto const data = "\x11\x22\x33\x44"_bin;
      boost::push_back(bin, data);
      bin[3] = bin.size() + 1;
      auto it = bin.begin();

      BOOST_CHECK_THROW(
            v13::action_id::decode(it, bin.end())
          , std::runtime_error);
      BOOST_TEST(std::distance(bin.begin(), it) == 4);
    }
    BOOST_FIXTURE_TEST_CASE(
          throw_exception_if_action_id_is_experimenter_and_the_len_is_too_small
        , experimenter_action_id_fixture)
    {
      bin[3] = sizeof(protocol::ofp_action_experimenter_header) - 1;
      auto it = bin.begin();

      BOOST_CHECK_THROW(
            v13::action_id::decode(it, bin.end())
          , std::runtime_error);
      BOOST_TEST(std::distance(bin.begin(), it) == 4);
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

BOOST_AUTO_TEST_SUITE_END() // action_id
BOOST_AUTO_TEST_SUITE_END() // table_feature_property_test
