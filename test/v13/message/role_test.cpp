#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/role.hpp>
#include <boost/test/unit_test.hpp>

#include <utility>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace protocol = v13::protocol;
namespace msg = v13::messages;

namespace {
  struct role_request_parameter {
    std::uint32_t role = protocol::OFPCR_ROLE_MASTER; // 2
    std::uint64_t generation_id = 0x0102030405060708;
    std::uint32_t xid = 0x12345678;
  };
  struct role_request_fixture : role_request_parameter {
    msg::role_request sut{role, generation_id, xid};
    std::vector<unsigned char> bin
      = "\x04\x18\x00\x18\x12\x34\x56\x78""\x00\x00\x00\x02\x00\x00\x00\x00"
        "\x01\x02\x03\x04\x05\x06\x07\x08"
        ""_bin;
  };
  struct role_reply_parameter {
    std::uint32_t role = protocol::OFPCR_ROLE_EQUAL; // 1
    std::uint64_t generation_id = 0x1020304050607080;
    std::uint32_t xid = 0x12345678;
  };
  struct role_reply_fixture : role_reply_parameter {
    msg::role_reply sut{role, generation_id, xid};
    std::vector<unsigned char> bin
      = "\x04\x19\x00\x18\x12\x34\x56\x78""\x00\x00\x00\x01\x00\x00\x00\x00"
        "\x10\x20\x30\x40\x50\x60\x70\x80"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(role_request)

  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(type_is_role_request)
    {
      using sut = msg::role_request;

      constexpr auto type = sut::type();

      BOOST_TEST(type == protocol::OFPT_ROLE_REQUEST);
    }
    BOOST_AUTO_TEST_CASE(min_length_is_size_of_ofp_role_request)
    {
      using sut = msg::role_request;

      constexpr auto min_length = sut::min_length();

      BOOST_TEST(min_length == sizeof(protocol::ofp_role_request));
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_role_and_generator_id_and_xid)
    {
      auto const role = protocol::OFPCR_ROLE_MASTER;
      auto const generation_id = std::uint64_t{0x0102030405060708};
      auto const xid = 0x12345678;

      msg::role_request const sut{role, generation_id, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_role_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.role() == role);
      BOOST_TEST(sut.generation_id() == generation_id);
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      auto const role = protocol::OFPCR_ROLE_SLAVE;
      auto const generation_id = std::uint64_t{0xA1A2A3A4A5A6A7A8};

      msg::role_request const sut{role, generation_id};

      BOOST_TEST(sut.role() == role);
      BOOST_TEST(sut.generation_id() == generation_id);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, role_request_fixture)
    {
      auto const& const_role_request = sut;

      auto const copy = const_role_request;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, role_request_fixture)
    {
      auto const moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, role_request_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = msg::role_request{role, generation_id, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_role_and_generator_id_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::role_request{role, generation_id, xid}
        == msg::role_request{role, generation_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_role_is_not_equal)
    {
      BOOST_TEST(
          (msg::role_request{protocol::OFPCR_ROLE_MASTER, generation_id, xid}
        != msg::role_request{protocol::OFPCR_ROLE_SLAVE, generation_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_generator_id_is_not_equal)
    {
      BOOST_TEST(
          (msg::role_request{role, 1, xid}
        != msg::role_request{role, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::role_request{role, generation_id, 1}
        != msg::role_request{role, generation_id, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, role_request_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, role_request_fixture)
    {
      auto it = bin.begin();

      auto const role_request = msg::role_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((role_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // role_request

BOOST_AUTO_TEST_SUITE(role_reply)

  BOOST_AUTO_TEST_SUITE(type_definition)
    BOOST_AUTO_TEST_CASE(type_is_role_reply)
    {
      using sut = msg::role_reply;

      constexpr auto type = sut::type();

      BOOST_TEST(type == protocol::OFPT_ROLE_REPLY);
    }
    BOOST_AUTO_TEST_CASE(min_length_is_size_of_ofp_role_request)
    {
      using sut = msg::role_reply;

      constexpr auto min_length = sut::min_length();

      BOOST_TEST(min_length == sizeof(protocol::ofp_role_request));
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_role_and_generator_id_and_xid)
    {
      auto const role = protocol::OFPCR_ROLE_MASTER;
      auto const generation_id = std::uint64_t{0x0102030405060708};
      auto const xid = 0x12345678;

      msg::role_reply const sut{role, generation_id, xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_role_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.role() == role);
      BOOST_TEST(sut.generation_id() == generation_id);
    }
    BOOST_AUTO_TEST_CASE(constructible_without_xid)
    {
      auto const role = protocol::OFPCR_ROLE_SLAVE;
      auto const generation_id = std::uint64_t{0xA1A2A3A4A5A6A7A8};

      msg::role_reply const sut{role, generation_id};

      BOOST_TEST(sut.role() == role);
      BOOST_TEST(sut.generation_id() == generation_id);
    }
    BOOST_FIXTURE_TEST_CASE(copy_constructible, role_reply_fixture)
    {
      auto const& const_role_reply = sut;

      auto const copy = const_role_reply;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(move_constructible, role_reply_fixture)
    {
      auto const moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, role_reply_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = msg::role_reply{role, generation_id, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_role_and_generator_id_and_xid_are_equal)
    {
      BOOST_TEST(
          (msg::role_reply{role, generation_id, xid}
        == msg::role_reply{role, generation_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_role_is_not_equal)
    {
      BOOST_TEST(
          (msg::role_reply{protocol::OFPCR_ROLE_MASTER, generation_id, xid}
        != msg::role_reply{protocol::OFPCR_ROLE_SLAVE, generation_id, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_generator_id_is_not_equal)
    {
      BOOST_TEST(
          (msg::role_reply{role, 1, xid}
        != msg::role_reply{role, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::role_reply{role, generation_id, 1}
        != msg::role_reply{role, generation_id, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, role_reply_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, role_reply_fixture)
    {
      auto it = bin.begin();

      auto const role_reply = msg::role_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((role_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // role_reply

BOOST_AUTO_TEST_SUITE_END() // message_test
