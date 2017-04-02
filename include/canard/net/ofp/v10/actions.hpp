#ifndef CANARD_NET_OFP_V10_ACTIONS_HPP
#define CANARD_NET_OFP_V10_ACTIONS_HPP

#include <tuple>
#include <canard/net/ofp/v10/action/enqueue.hpp>
#include <canard/net/ofp/v10/action/output.hpp>
#include <canard/net/ofp/v10/action/set_field.hpp>
#include <canard/net/ofp/v10/action/strip_vlan.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  using default_action_list = std::tuple<
      actions::output
    , actions::set_vlan_vid
    , actions::set_vlan_pcp
    , actions::strip_vlan
    , actions::set_eth_src
    , actions::set_eth_dst
    , actions::set_ip_dscp
    , actions::set_ipv4_src
    , actions::set_ipv4_dst
    , actions::set_tcp_dst
    , actions::set_tcp_src
    , actions::enqueue
  >;

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTIONS_HPP
