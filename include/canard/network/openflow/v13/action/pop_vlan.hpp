#ifndef CANARD_NET_OFP_V13_ACTIONS_POP_VLAN_HPP
#define CANARD_NET_OFP_V13_ACTIONS_POP_VLAN_HPP

#include <canard/network/openflow/v13/detail/basic_generic_action.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class pop_vlan
        : public detail::v13::basic_generic_action<pop_vlan>
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_POP_VLAN;

        pop_vlan() = default;

    private:
        friend basic_generic_action::basic_fixed_length_action;

        explicit pop_vlan(raw_ofp_type const& ofp_action) noexcept
            : basic_generic_action{ofp_action}
        {
        }
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_POP_VLAN_HPP
