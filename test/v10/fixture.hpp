#ifndef CANARD_NET_OFP_V10_TEST_FIXTURE_HPP
#define CANARD_NET_OFP_V10_TEST_FIXTURE_HPP

#include <canard/network/openflow/v10/actions.hpp>
#include <canard/network/openflow/v10/common/match_fields.hpp>
#include "../test_utility.hpp"

namespace {

struct match_fixture {
  canard::net::ofp::v10::match::in_port in_port{1};
  canard::net::ofp::v10::match::eth_src eth_src{"\x01\x02\x03\x04\x05\x06"_mac};
  canard::net::ofp::v10::match::eth_dst eth_dst{"\xf1\xf2\xf3\xf4\xf5\xf6"_mac};
  canard::net::ofp::v10::match::vlan_vid vlan_vid{0x0123};
  canard::net::ofp::v10::match::vlan_pcp vlan_pcp{2};
  canard::net::ofp::v10::match::eth_type eth_type{0x0800};
  canard::net::ofp::v10::match::ip_dscp ip_dscp{0x23};
  canard::net::ofp::v10::match::ip_proto ip_proto{7};
  canard::net::ofp::v10::match::ipv4_src ipv4_src{"192.168.1.2"_ipv4, 32};
  canard::net::ofp::v10::match::ipv4_dst ipv4_dst{"192.168.1.0"_ipv4, 28};
  canard::net::ofp::v10::match::tcp_src tcp_src{0x1823};
  canard::net::ofp::v10::match::tcp_src tcp_dst{8080};
};

struct action_fixture {
  canard::net::ofp::v10::actions::output output{
    canard::net::ofp::v10::protocol::OFPP_CONTROLLER, 64
  };
  std::vector<unsigned char> output_bin
    = "\x00\x00\x00\x08\xff\xfd\x00\x40"_bin;

  canard::net::ofp::v10::actions::set_vlan_vid set_vlan_vid{0x0123};
  std::vector<unsigned char> set_vlan_vid_bin
    = "\x00\x01\x00\x08\x01\x23\x00\x00"_bin;

  canard::net::ofp::v10::actions::set_vlan_pcp set_vlan_pcp{0x02};
  std::vector<unsigned char> set_vlan_pcp_bin
    = "\x00\x02\x00\x08\x02\x00\x00\x00"_bin;

  canard::net::ofp::v10::actions::strip_vlan strip_vlan{};
  std::vector<unsigned char> strip_vlan_bin
    = "\x00\x03\x00\x08\x00\x00\x00\x00"_bin;

  canard::net::ofp::v10::actions::set_eth_src set_eth_src{
    "\x01\x02\x03\x04\x05\x06"_mac
  };
  std::vector<unsigned char> set_eth_src_bin
    = "\x00\x04\x00\x10\x01\x02\x03\x04""\x05\x06\x00\x00\x00\x00\x00\x00"_bin;

  canard::net::ofp::v10::actions::set_eth_dst set_eth_dst{
    "\xf1\xf2\xf3\xf4\xf5\xf6"_mac
  };
  std::vector<unsigned char> set_eth_dst_bin
    = "\x00\x05\x00\x10\xf1\xf2\xf3\xf4""\xf5\xf6\x00\x00\x00\x00\x00\x00"_bin;

  canard::net::ofp::v10::actions::set_ipv4_src set_ipv4_src{"192.168.1.3"_ipv4};
  std::vector<unsigned char> set_ipv4_src_bin
    = "\x00\x06\x00\x08\xc0\xa8\x01\x03"_bin;

  canard::net::ofp::v10::actions::set_ipv4_dst set_ipv4_dst{"192.168.1.4"_ipv4};
  std::vector<unsigned char> set_ipv4_dst_bin
    = "\x00\x07\x00\x08\xc0\xa8\x01\x04"_bin;

  canard::net::ofp::v10::actions::set_ip_dscp set_ip_dscp{0x23};
  std::vector<unsigned char> set_ip_dscp_bin
    = "\x00\x08\x00\x08\x23\x00\x00\x00"_bin;

  canard::net::ofp::v10::actions::set_tcp_src set_tcp_src{0x6653};
  std::vector<unsigned char> set_tcp_src_bin
    = "\x00\x09\x00\x08\x66\x53\x00\x00"_bin;

  canard::net::ofp::v10::actions::set_tcp_dst set_tcp_dst{0x8080};
  std::vector<unsigned char> set_tcp_dst_bin
    = "\x00\x0a\x00\x08\x80\x80\x00\x00"_bin;

  canard::net::ofp::v10::actions::enqueue enqueue{0x12345678, 1};
  std::vector<unsigned char> enqueue_bin
    = "\x00\x0b\x00\x10\x00\x01\x00\x00""\x00\x00\x00\x00\x12\x34\x56\x78"_bin;
};

}

#endif // CANARD_NET_OFP_V10_TEST_FIXTURE_HPP
