#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/table_mod.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>
#include <canard/net/ofp/v13/openflow.hpp>
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;

namespace {
  struct table_mod_parameter {
    std::uint8_t table_id = protocol::OFPTT_ALL;
    std::uint32_t config = protocol::OFPTC_DEPRECATED_MASK;
    std::uint32_t xid = 0x234;
  };
  struct table_mod_fixture : table_mod_parameter {
    v13::messages::table_mod sut{table_id, config, xid};
    std::vector<std::uint8_t> bin
      = "\x04\x11\x00\x10\x00\x00\x02\x34""\xff\x00\x00\x00\x00\x00\x00\x03"_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(table_mod)
  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible)
    {
      auto const table_id = protocol::OFPTT_MAX;
      auto const config = 0;
      auto const xid = std::uint32_t{0x123};

      v13::messages::table_mod const sut{table_id, config, xid};

      BOOST_TEST(sut.version() == protocol::OFP_VERSION);
      BOOST_TEST(sut.type() == protocol::OFPT_TABLE_MOD);
      BOOST_TEST(sut.length() == sizeof(protocol::ofp_table_mod));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.table_id() == table_id);
      BOOST_TEST(sut.config() == config);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(assignment, table_mod_parameter)
    BOOST_AUTO_TEST_CASE(true_if_same_object)
    {
      auto const sut = v13::messages::table_mod{table_id, config, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(true_if_values_are_equal)
    {
      BOOST_TEST(
          (v13::messages::table_mod{table_id, config, xid}
        == v13::messages::table_mod{table_id, config, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_table_id_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::table_mod{1, config, xid}
        != v13::messages::table_mod{2, config, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_config_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::table_mod{table_id, 1, xid}
        != v13::messages::table_mod{table_id, 2, xid}));
    }
    BOOST_AUTO_TEST_CASE(false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (v13::messages::table_mod{table_id, config, 1}
        != v13::messages::table_mod{table_id, config, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // assignment

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, table_mod_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(constructible_from_binary, table_mod_fixture)
    {
      auto it = bin.begin();

      auto const table_mod = v13::messages::table_mod::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((table_mod == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // table_mod
BOOST_AUTO_TEST_SUITE_END() // message_test
