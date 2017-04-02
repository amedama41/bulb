#ifndef CANARD_NET_OFP_V10_ACTIONS_STRIP_VLAN_HPP
#define CANARD_NET_OFP_V10_ACTIONS_STRIP_VLAN_HPP

#include <canard/net/ofp/v10/detail/basic_fixed_length_action.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace actions {

  class strip_vlan
    : public detail::v10::basic_fixed_length_action<strip_vlan>
  {
  public:
    using raw_ofp_type = protocol::ofp_action_header;

    static constexpr protocol::ofp_action_type action_type
      = protocol::OFPAT_STRIP_VLAN;

    strip_vlan() noexcept
      : strip_vlan_{action_type, sizeof(raw_ofp_type), { 0, 0, 0, 0 }}
    {
    }

  private:
    friend basic_fixed_length_action;

    explicit strip_vlan(raw_ofp_type const& action_header) noexcept
      : strip_vlan_(action_header)
    {
    }

    auto ofp_action() const noexcept
      -> raw_ofp_type const&
    {
      return strip_vlan_;
    }

    void validate_action() const
    {
    }

    auto is_equivalent_action(strip_vlan const&) const noexcept
      -> bool
    {
      return true;
    }

  private:
    raw_ofp_type strip_vlan_;
  };

  using pop_vlan = strip_vlan;

} // namespace actions
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTIONS_STRIP_VLAN_HPP
