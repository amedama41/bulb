#ifndef CANARD_NET_OFP_V13_ACTIONS_GROUP_HPP
#define CANARD_NET_OFP_V13_ACTIONS_GROUP_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/net/ofp/v13/detail/basic_fixed_length_action.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

  class group
    : public detail::v13::basic_fixed_length_action<group>
  {
  public:
    using ofp_type = protocol::ofp_action_group;

    static constexpr protocol::ofp_action_type action_type
      = protocol::OFPAT_GROUP;

    explicit group(std::uint32_t const group_id) noexcept
      : action_group_{action_type, length(), group_id}
    {
    };

    auto group_id() const noexcept
      -> std::uint32_t
    {
      return action_group_.group_id;
    }

  private:
    friend basic_fixed_length_action;

    explicit group(ofp_type const& action_group) noexcept
      : action_group_(action_group)
    {
    }

    auto ofp_action() const noexcept
      -> ofp_type const&
    {
      return action_group_;
    }

    void validate_action() const
    {
      if (group_id() > protocol::OFPG_MAX) {
        throw std::runtime_error{"invalid group_id"};
      }
    }

    auto is_equivalent_action(group const& rhs) const noexcept
      -> bool
    {
      return group_id() == rhs.group_id();
    }

  private:
    ofp_type action_group_;
  };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_GROUP_HPP
