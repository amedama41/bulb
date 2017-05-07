#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/multipart/group_description.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace actions = v13::actions;
namespace multipart = v13::messages::multipart;

namespace {
  using buckets_type = multipart::group_description::buckets_type;

  struct group_description_parameter {
    actions::output output1{0x12345678};
    actions::output output2{0xfffffffd, 0xffe5};
    actions::set_vlan_vid set_vlan_vid{protocol::OFPVID_PRESENT | 0x0234};
    actions::pop_vlan pop_vlan{};
    v13::bucket bucket1{
      0x0102, 0x03040506, 0x0708090a, v13::action_set{set_vlan_vid, output1}
    };
    v13::bucket bucket2{
      0x1122, 0x33445566, 0x778899aa, v13::action_set{pop_vlan, output2}
    };
    std::uint32_t group_id = 0x12345678;
    std::uint8_t group_type = protocol::group_type::ff;
    buckets_type buckets{bucket1, bucket2};
  };
  struct group_description_fixture : group_description_parameter {
    multipart::group_description sut{group_id, group_type, buckets};
    std::vector<std::uint8_t> bin
      = "\x00\x60\x03\x00\x12\x34\x56\x78"

        "\x00\x30\x01\x02\x03\x04\x05\x06""\x07\x08\x09\x0a\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x0c\x02""\x12\x34\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x10\x12\x34\x56\x78""\xff\xff\x00\x00\x00\x00\x00\x00"

        "\x00\x28\x11\x22\x33\x44\x55\x66""\x77\x88\x99\xaa\x00\x00\x00\x00"
        "\x00\x12\x00\x08\x00\x00\x00\x00"
        "\x00\x00\x00\x10\xff\xff\xff\xfd""\xff\xe5\x00\x00\x00\x00\x00\x00"
        ""_bin;
  };
  struct empty_buckets_group_description_fixture {
    std::uint32_t group_id = 0x11223344;
    std::uint8_t group_type = protocol::group_type::all;
    multipart::group_description sut{group_id, group_type, {}};
    std::vector<std::uint8_t> bin = "\x00\x08\x00\x00\x11\x22\x33\x44"_bin;
  };

  struct group_description_request_fixture {
    multipart::group_description_request sut{0x12345678};
    std::vector<std::uint8_t> bin
      = "\x04\x12\x00\x10\x12\x34\x56\x78""\x00\x07\x00\x00\x00\x00\x00\x00"
        ""_bin;
  };

  struct group_description_reply_parameter : group_description_parameter {
    multipart::group_description group_desc1{
      0x11111111, protocol::group_type::select, { bucket1, bucket2 }
    };
    multipart::group_description group_desc2{
      0x12345678, protocol::group_type::indirect, { bucket2, bucket1 }
    };
    multipart::group_description group_desc3{
      0xffffff00, protocol::group_type::all, { bucket1 }
    };
    multipart::group_description_reply::body_type body{
      group_desc1, group_desc2, group_desc3
    };
    std::uint16_t flags = protocol::multipart_reply_flags::reply_more;
    std::uint32_t xid = 0x12345678;
  };
  struct group_description_reply_fixture : group_description_reply_parameter {
    multipart::group_description_reply sut{body, flags, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x01\x08\x12\x34\x56\x78""\x00\x07\x00\x01\x00\x00\x00\x00"

        "\x00\x60\x01\x00\x11\x11\x11\x11"
        "\x00\x30\x01\x02\x03\x04\x05\x06""\x07\x08\x09\x0a\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x0c\x02""\x12\x34\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x10\x12\x34\x56\x78""\xff\xff\x00\x00\x00\x00\x00\x00"
        "\x00\x28\x11\x22\x33\x44\x55\x66""\x77\x88\x99\xaa\x00\x00\x00\x00"
        "\x00\x12\x00\x08\x00\x00\x00\x00"
        "\x00\x00\x00\x10\xff\xff\xff\xfd""\xff\xe5\x00\x00\x00\x00\x00\x00"

        "\x00\x60\x02\x00\x12\x34\x56\x78"
        "\x00\x28\x11\x22\x33\x44\x55\x66""\x77\x88\x99\xaa\x00\x00\x00\x00"
        "\x00\x12\x00\x08\x00\x00\x00\x00"
        "\x00\x00\x00\x10\xff\xff\xff\xfd""\xff\xe5\x00\x00\x00\x00\x00\x00"
        "\x00\x30\x01\x02\x03\x04\x05\x06""\x07\x08\x09\x0a\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x0c\x02""\x12\x34\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x10\x12\x34\x56\x78""\xff\xff\x00\x00\x00\x00\x00\x00"

        "\x00\x38\x00\x00\xff\xff\xff\x00"
        "\x00\x30\x01\x02\x03\x04\x05\x06""\x07\x08\x09\x0a\x00\x00\x00\x00"
        "\x00\x19\x00\x10\x80\x00\x0c\x02""\x12\x34\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x10\x12\x34\x56\x78""\xff\xff\x00\x00\x00\x00\x00\x00"

        ""_bin;
  };
  struct empty_body_group_description_reply_fixture {
    multipart::group_description_reply sut{{}, 0, 0x01020304};
    std::vector<std::uint8_t> bin
      = "\x04\x13\x00\x10\x01\x02\x03\x04""\x00\x07\x00\x00\x00\x00\x00\x00"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(multipart_test)

BOOST_AUTO_TEST_SUITE(group_description)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const group_id = std::uint32_t{1};
      auto const group_type = protocol::group_type::indirect;
      auto const buckets = buckets_type{
          // 16 + 8 + 16 = 40
          v13::bucket{2, {actions::pop_vlan{}, actions::output{1}}}
          // 16 + 16 + 16 = 48
        , v13::bucket{1, {actions::set_vlan_vid{4}, actions::output{2}}}
      };

      multipart::group_description const sut{group_id, group_type, buckets};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_group_desc) + 40 + 48);
      BOOST_TEST(sut.group_id() == group_id);
      BOOST_TEST(sut.group_type() == group_type);
      BOOST_TEST((sut.buckets() == buckets));
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, group_description_fixture)
    {
      auto const& const_lvalue = sut;

      auto const copy = const_lvalue;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, group_description_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_group_desc));
      BOOST_TEST(moved.buckets().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, group_description_fixture)
    BOOST_AUTO_TEST_CASE(copy_assignable)
    {
      auto copy = multipart::group_description{0, 0, {}};
      auto const& const_lvalue = sut;

      copy = const_lvalue;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(move_assignable)
    {
      auto copy = multipart::group_description{0, 0, {}};
      auto moved = sut;

      copy = std::move(moved);

      BOOST_TEST((copy == sut));
      BOOST_TEST(moved.length() == sizeof(protocol::ofp_group_desc));
      BOOST_TEST(moved.buckets().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_FIXTURE_TEST_SUITE(equality, group_description_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::group_description{
        group_id, group_type, buckets
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::group_description{group_id, group_type, buckets}
        == multipart::group_description{group_id, group_type, buckets}));
    }
    BOOST_AUTO_TEST_CASE(false_if_group_id_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_description{1, group_type, buckets}
        != multipart::group_description{2, group_type, buckets}));
    }
    BOOST_AUTO_TEST_CASE(false_if_group_type_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_description{group_id, 1, buckets}
        != multipart::group_description{group_id, 2, buckets}));
    }
    BOOST_AUTO_TEST_CASE(false_if_buckets_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_description{
             group_id, group_type, {v13::bucket{{output1}}}}
        != multipart::group_description{
             group_id, group_type, {v13::bucket{{output2}}}}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(extract_buckets)
    BOOST_FIXTURE_TEST_CASE(move_buckets_ownership, group_description_fixture)
    {
      auto const original_buckets = sut.buckets();

      auto const buckets = sut.extract_buckets();

      BOOST_TEST((buckets == original_buckets));
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_group_desc));
      BOOST_TEST(sut.buckets().empty());
    }
    BOOST_FIXTURE_TEST_CASE(
        move_empty_buckets_ownership, empty_buckets_group_description_fixture)
    {
      auto const original_buckets = sut.buckets();

      auto const buckets = sut.extract_buckets();

      BOOST_TEST((buckets == original_buckets));
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_group_desc));
      BOOST_TEST(sut.buckets().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_buckets

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, group_description_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
        generate_empty_buckets_binary, empty_buckets_group_description_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, group_description_fixture)
    {
      auto it = bin.begin();

      auto const group_description
        = multipart::group_description::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_description == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_from_empty_buckets_binary
        , empty_buckets_group_description_fixture)
    {
      auto it = bin.begin();

      auto const group_description
        = multipart::group_description::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_description == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // group_description

BOOST_AUTO_TEST_SUITE(group_description_request)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::group_description_request;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_request);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::group_description_request;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::group_desc);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(default_constructible)
    {
      multipart::group_description_request const sut{};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.flags() == 0);
    }
    BOOST_AUTO_TEST_CASE(constructible_from_xid)
    {
      auto const xid = std::uint32_t{123};

      multipart::group_description_request const sut{xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_multipart_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == 0);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::group_description_request{};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_xid_is_equal)
    {
      auto const xid = std::uint32_t{1234};

      BOOST_TEST(
          (multipart::group_description_request{xid}
        == multipart::group_description_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_description_request{1}
        != multipart::group_description_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(
        generate_binary, group_description_request_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        construct_from_binary, group_description_request_fixture)
    {
      auto it = bin.begin();

      auto const group_description_request
        = multipart::group_description_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_description_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // group_description_request

BOOST_AUTO_TEST_SUITE(group_description_reply)
  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(message_type)
    {
      using sut = multipart::group_description_reply;

      constexpr auto message_type = sut::type();

      BOOST_TEST(message_type == protocol::msg_type::multipart_reply);
    }
    BOOST_AUTO_TEST_CASE(multipart_type)
    {
      using sut = multipart::group_description_reply;

      constexpr auto multipart_type = sut::multipart_type();

      BOOST_TEST(multipart_type == protocol::multipart_type::group_desc);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_FIXTURE_TEST_SUITE(constructor, group_description_reply_parameter)
    BOOST_AUTO_TEST_CASE(constructible_from_parameters)
    {
      auto const body = multipart::group_description_reply::body_type{
        group_desc1, group_desc2
      };
      auto const flags = std::uint16_t{1};
      auto const xid = std::uint32_t{345};

      multipart::group_description_reply const sut{body, flags, xid};

      BOOST_TEST(
          sut.length() == sizeof(protocol::ofp_multipart_reply) + body.length());
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.body() == body));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, group_description_reply_fixture)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = multipart::group_description_reply{body, flags, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (multipart::group_description_reply{body, flags, xid}
        == multipart::group_description_reply{body, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_body_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_description_reply{{group_desc1}, flags, xid}
        != multipart::group_description_reply{{group_desc2}, flags, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_description_reply{body, 0, xid}
        != multipart::group_description_reply{body, 1, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (multipart::group_description_reply{body, flags, 1}
        != multipart::group_description_reply{body, flags, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, group_description_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
    BOOST_FIXTURE_TEST_CASE(
        generate_empty_body_binary, empty_body_group_description_reply_fixture)
    {
      auto buffer = std::vector<std::uint8_t>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(
        construct_from_binary, group_description_reply_fixture)
    {
      auto it = bin.begin();

      auto const group_description_reply
        = multipart::group_description_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_description_reply == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
          construct_from_empty_body_binary
        , empty_body_group_description_reply_fixture)
    {
      auto it = bin.begin();

      auto const group_description_reply
        = multipart::group_description_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((group_description_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // group_description_reply

BOOST_AUTO_TEST_SUITE_END() // multipart_test
BOOST_AUTO_TEST_SUITE_END() // message_test
