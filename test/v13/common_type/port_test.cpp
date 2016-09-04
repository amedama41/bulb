#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/common/port.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;

using protocol = v13::protocol;

namespace {

struct feature_fixture {
  std::uint32_t curr
    = protocol::OFPPF_100GB_FD
    | protocol::OFPPF_FIBER
    | protocol::OFPPF_AUTONEG
    | protocol::OFPPF_PAUSE
    ;
  std::uint32_t advertised
    = protocol::OFPPF_100MB_FD
    | protocol::OFPPF_1GB_FD
    | protocol::OFPPF_10GB_FD
    | protocol::OFPPF_40GB_FD
    | protocol::OFPPF_100GB_FD
    | protocol::OFPPF_FIBER
    | protocol::OFPPF_AUTONEG
    | protocol::OFPPF_PAUSE
    ;
  std::uint32_t supported
    = protocol::OFPPF_10MB_FD
    | protocol::OFPPF_100MB_FD
    | protocol::OFPPF_1GB_FD
    | protocol::OFPPF_10GB_FD
    | protocol::OFPPF_40GB_FD
    | protocol::OFPPF_100GB_FD
    | protocol::OFPPF_FIBER
    | protocol::OFPPF_AUTONEG
    | protocol::OFPPF_PAUSE | protocol::OFPPF_PAUSE_ASYM
    ;
  std::uint32_t peer
    = protocol::OFPPF_40GB_FD
    | protocol::OFPPF_100GB_FD
    | protocol::OFPPF_1TB_FD
    | protocol::OFPPF_FIBER
    | protocol::OFPPF_AUTONEG
    | protocol::OFPPF_PAUSE | protocol::OFPPF_PAUSE_ASYM
    ;
};

struct port_fixture : feature_fixture {
  std::uint32_t port_no = 0x12345678;
  canard::mac_address hw_addr = "\x01\x02\x03\x04\x05\x06"_mac;
  boost::string_ref name = "eth1";
  std::uint32_t config = protocol::OFPPC_PORT_DOWN | protocol::OFPPC_NO_RECV;
  std::uint32_t state = protocol::OFPPS_LINK_DOWN | protocol::OFPPS_BLOCKED;
  std::uint32_t curr_speed = 0x10002000;
  std::uint32_t max_speed = 0x30004000;

  v13::port sut{
      port_no, hw_addr, name, config, state
    , curr, advertised, supported, peer, curr_speed, max_speed
  };
  std::vector<unsigned char> bin
    = "\x12\x34\x56\x78\x00\x00\x00\x00""\x01\x02\x03\x04\x05\x06\x00\x00"
      "e" "t" "h" "1" "\x00\x00\x00\x00""\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x05\x00\x00\x00\x03""\x00\x00\x71\x00\x00\x00\x71\xe8"
      "\x00\x00\xf1\xea\x00\x00\xf3\x80""\x10\x00\x20\x00\x30\x00\x40\x00"_bin;
};

}

BOOST_AUTO_TEST_SUITE(common_type_test)
BOOST_AUTO_TEST_SUITE(port_test)

  BOOST_FIXTURE_TEST_SUITE(constructor, feature_fixture)
    BOOST_AUTO_TEST_CASE(is_constructible)
    {
      auto const port_no = std::uint32_t{32};
      auto const hw_addr = "\x01\x02\x03\x04\x05\x06"_mac;
      auto const name = "eth0";
      auto const config
        = std::uint32_t{protocol::OFPPC_PORT_DOWN | protocol::OFPPC_NO_FWD};
      auto const state = std::uint32_t{protocol::OFPPS_LIVE};
      auto const curr_speed = 1000000;
      auto const max_speed = 100000000;

      auto const sut = v13::port{
          port_no, hw_addr, name, config, state
        , curr, advertised, supported, peer, curr_speed, max_speed
      };

      BOOST_TEST(sut.port_no() == port_no);
      BOOST_TEST(sut.hardware_address() == hw_addr);
      BOOST_TEST(sut.name() == name);
      BOOST_TEST(sut.config() == config);
      BOOST_TEST(sut.state() == state);
      BOOST_TEST(sut.current_features() == curr);
      BOOST_TEST(sut.advertised_features() == advertised);
      BOOST_TEST(sut.supported_features() == supported);
      BOOST_TEST(sut.peer_advertised_features() == peer);
      BOOST_TEST(sut.current_speed() == curr_speed);
      BOOST_TEST(sut.max_speed() == max_speed);
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, port_fixture)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      auto const port_no = std::uint32_t{protocol::OFPP_MAX};

      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }
        == v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               1, hw_addr, name, config, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }
        != v13::port{
               2, hw_addr, name, config, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_hardware_address_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, "\x01\x02\x03\x04\x05\x06"_mac, name, config, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }
        != v13::port{
               port_no, "\xa1\xa2\xa3\xa4\xa5\xa6"_mac, name, config, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_name_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, "eth1", config, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }
        != v13::port{
               port_no, hw_addr, "ETH1", config, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_config_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, protocol::OFPPC_NO_RECV, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }
        != v13::port{
               port_no, hw_addr, name, protocol::OFPPC_NO_PACKET_IN, state
             , curr, advertised, supported, peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_state_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, config, protocol::OFPPS_LINK_DOWN
             , curr, advertised, supported, peer, curr_speed, max_speed
           }
        != v13::port{
               port_no, hw_addr, name, config, protocol::OFPPS_LIVE
             , curr, advertised, supported, peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_current_features_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, config, state
             , protocol::OFPPF_10MB_HD
             , advertised, supported, peer, curr_speed, max_speed
           }
        != v13::port{
               port_no, hw_addr, name, config, state
             , protocol::OFPPF_10MB_FD
             , advertised, supported, peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_advertised_features_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, config, state
             , curr, protocol::OFPPF_10MB_HD
             , supported, peer, curr_speed, max_speed
           }
        != v13::port{
               port_no, hw_addr, name, config, state
             , curr, protocol::OFPPF_10MB_FD
             , supported, peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_supported_features_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, protocol::OFPPF_10MB_HD
             , peer, curr_speed, max_speed
           }
        != v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, protocol::OFPPF_10MB_FD
             , peer, curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_peer_advertised_features_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, supported, protocol::OFPPF_10MB_HD
             , curr_speed, max_speed
           }
        != v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, supported, protocol::OFPPF_10MB_FD
             , curr_speed, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_current_speed_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, supported, peer, 1, max_speed
           }
        != v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, supported, peer, 2, max_speed
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_max_speed_is_not_equal)
    {
      BOOST_TEST(
          (v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, supported, peer, curr_speed, 1
           }
        != v13::port{
               port_no, hw_addr, name, config, state
             , curr, advertised, supported, peer, curr_speed, 2
           }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_padding_is_not_equal)
    {
      bin[offsetof(v13::port::raw_ofp_type, name) - 1] = 0xff;
      auto it = bin.begin();

      auto port = v13::port::decode(it, bin.end());

      BOOST_TEST((port != sut));
    }
    BOOST_AUTO_TEST_CASE(
        is_false_if_byte_after_name_termination_null_is_not_equal)
    {
      bin[offsetof(v13::port::raw_ofp_type, config) - 1] = 0xff;
      auto it = bin.begin();

      auto port = v13::port::decode(it, bin.end());

      BOOST_TEST((port != sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(function_equivalent, port_fixture)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_all_parameters_are_equal)
    {
      auto const port_no = std::uint32_t{protocol::OFPP_MAX};

      BOOST_TEST(
          equivalent(
              v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }
            , v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_port_no_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  1, hw_addr, name, config, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }
            , v13::port{
                  2, hw_addr, name, config, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_hardware_address_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, "\x01\x02\x03\x04\x05\x06"_mac, name, config, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }
            , v13::port{
                  port_no, "\xa1\xa2\xa3\xa4\xa5\xa6"_mac, name, config, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_name_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, "eth1", config, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }
            , v13::port{
                  port_no, hw_addr, "ETH1", config, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_config_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, name, protocol::OFPPC_NO_RECV, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }
            , v13::port{
                  port_no, hw_addr, name, protocol::OFPPC_NO_PACKET_IN, state
                , curr, advertised, supported, peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_state_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, name, config, protocol::OFPPS_LINK_DOWN
                , curr, advertised, supported, peer, curr_speed, max_speed
              }
            , v13::port{
                  port_no, hw_addr, name, config, protocol::OFPPS_LIVE
                , curr, advertised, supported, peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_current_features_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, name, config, state
                , protocol::OFPPF_10MB_HD
                , advertised, supported, peer, curr_speed, max_speed
              }
            , v13::port{
                  port_no, hw_addr, name, config, state
                , protocol::OFPPF_10MB_FD
                , advertised, supported, peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_advertised_features_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, name, config, state
                , curr, protocol::OFPPF_10MB_HD
                , supported, peer, curr_speed, max_speed
              }
            , v13::port{
                  port_no, hw_addr, name, config, state
                , curr, protocol::OFPPF_10MB_FD
                , supported, peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_supported_features_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, protocol::OFPPF_10MB_HD
                , peer, curr_speed, max_speed
              }
            , v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, protocol::OFPPF_10MB_FD
                , peer, curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_peer_advertised_features_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, supported, protocol::OFPPF_10MB_HD
                , curr_speed, max_speed
              }
            , v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, supported, protocol::OFPPF_10MB_FD
                , curr_speed, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_current_speed_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, supported, peer, 1, max_speed
              }
            , v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, supported, peer, 2, max_speed
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_max_speed_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, supported, peer, curr_speed, 1
              }
            , v13::port{
                  port_no, hw_addr, name, config, state
                , curr, advertised, supported, peer, curr_speed, 2
              }));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_padding_is_not_equal)
    {
      bin[offsetof(v13::port::raw_ofp_type, name) - 1] = 0xff;
      auto it = bin.begin();

      auto port = v13::port::decode(it, bin.end());

      BOOST_TEST(equivalent(port, sut));
    }
    BOOST_AUTO_TEST_CASE(
        is_true_if_byte_after_name_termination_null_is_not_equal)
    {
      bin[offsetof(v13::port::raw_ofp_type, config) - 1] = 0xff;
      auto it = bin.begin();

      auto port = v13::port::decode(it, bin.end());

      BOOST_TEST(equivalent(port, sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_FIXTURE_TEST_SUITE(encode, port_fixture)
    BOOST_AUTO_TEST_CASE(generate_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, port_fixture)
    BOOST_AUTO_TEST_CASE(creates_port_from_binary)
    {
      auto it = bin.begin();

      auto const port = v13::port::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((port == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // port_test
BOOST_AUTO_TEST_SUITE_END() // common_type_test

