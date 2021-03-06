#ifndef CANARD_NET_OFP_V13_ACTIONS_SET_MPLS_TTL_HPP
#define CANARD_NET_OFP_V13_ACTIONS_SET_MPLS_TTL_HPP

#include <cstdint>
#include <canard/net/ofp/v13/detail/basic_fixed_length_action.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

  class set_mpls_ttl
    : public detail::v13::basic_fixed_length_action<set_mpls_ttl>
  {
  public:
    using ofp_type = protocol::ofp_action_mpls_ttl;

    static constexpr protocol::ofp_action_type action_type
      = protocol::OFPAT_SET_MPLS_TTL;

    explicit set_mpls_ttl(std::uint8_t const mpls_ttl) noexcept
      : action_mpls_ttl_{action_type, length(), mpls_ttl, { 0, 0, 0 }}
    {
    }

    auto ttl() const noexcept
      -> std::uint8_t
    {
      return action_mpls_ttl_.mpls_ttl;
    }

  private:
    friend basic_fixed_length_action;

    explicit set_mpls_ttl(ofp_type const& action_mpls_ttl) noexcept
      : action_mpls_ttl_(action_mpls_ttl)
    {
    }

    auto ofp_action() const noexcept
      -> ofp_type const&
    {
      return action_mpls_ttl_;
    }

    void validate_action() const
    {
    }

    auto is_equivalent_action(set_mpls_ttl const& rhs) const noexcept
      -> bool
    {
      return ttl() == rhs.ttl();
    }

  private:
    ofp_type action_mpls_ttl_;
  };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_SET_MPLS_TTL_HPP
