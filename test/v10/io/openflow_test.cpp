#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/io/openflow.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <sstream>

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace protocol = v10::protocol;

namespace data = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(io_test)
BOOST_AUTO_TEST_SUITE(openflow)
  BOOST_AUTO_TEST_SUITE(scoped_enum)
    BOOST_AUTO_TEST_CASE(
        constructible_from_unscoped_type_by_copy_initialization)
    {
      protocol::port_no sut = protocol::OFPP_MAX;

      BOOST_TEST(sut == protocol::port_no::max);
    }
    BOOST_AUTO_TEST_CASE(equality)
    {
      auto const sut = protocol::port_no{protocol::port_no::max};
      auto const int_port_no = std::uint32_t{protocol::OFPP_MAX};

      BOOST_TEST(sut == int_port_no);
    }
    BOOST_AUTO_TEST_CASE(additive)
    {
      auto const sut = protocol::port_no{protocol::port_no::max};
      auto const int_port_no = std::uint32_t{protocol::OFPP_MAX};

      BOOST_TEST(sut + 1 == int_port_no + 1);
    }
    BOOST_AUTO_TEST_CASE(bitwise_operation)
    {
      auto const drop = protocol::config_flags{protocol::config_flags::drop};
      auto const reasm = protocol::config_flags{protocol::config_flags::reasm};

      BOOST_TEST(
          (drop | reasm)
       == (protocol::OFPC_FRAG_DROP | protocol::OFPC_FRAG_REASM));
    }
    BOOST_AUTO_TEST_CASE(ostream)
    {
      auto const sut = protocol::port_config::port_down;
      std::ostringstream oss;

      oss << sut;

      BOOST_TEST(oss.str() == "OFPPC_PORT_DOWN");
    }
  BOOST_AUTO_TEST_SUITE_END() // scoped_enum

  BOOST_AUTO_TEST_SUITE(port_no)
    BOOST_DATA_TEST_CASE(
          reserved_port_no
        , data::make(std::vector<protocol::port_no>{
            protocol::port_no::max , protocol::port_no::in_port
          , protocol::port_no::table, protocol::port_no::normal
          , protocol::port_no::flood , protocol::port_no::all
          , protocol::port_no::controller , protocol::port_no::local
          , protocol::port_no::none, protocol::port_no::any
          })
        ^ data::make(std::vector<char const*>{
            "OFPP_MAX", "OFPP_IN_PORT", "OFPP_TABLE", "OFPP_NORMAL", "OFPP_FLOOD"
          , "OFPP_ALL", "OFPP_CONTROLLER", "OFPP_LOCAL", "OFPP_NONE", "OFPP_NONE"
          })
        , sut, output_string)
    {
      std::ostringstream oss;

      oss << sut;

      BOOST_TEST(oss.str() == output_string);
    }
    BOOST_AUTO_TEST_CASE(physical_port_no)
    {
      auto const sut = protocol::port_no(1);
      std::ostringstream oss;

      oss << sut;

      BOOST_TEST(oss.str() == "switch_port(1)");
    }
  BOOST_AUTO_TEST_SUITE_END() // port_no

BOOST_AUTO_TEST_SUITE_END() // openflow
BOOST_AUTO_TEST_SUITE_END() // io_test
