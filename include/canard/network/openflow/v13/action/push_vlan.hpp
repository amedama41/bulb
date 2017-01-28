#ifndef CANARD_NET_OFP_V13_ACTIONS_PUSH_VLAN_HPP
#define CANARD_NET_OFP_V13_ACTIONS_PUSH_VLAN_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/v13/detail/basic_action_push.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class push_vlan
        : public detail::v13::basic_action_push<push_vlan>
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_PUSH_VLAN;

        explicit push_vlan(std::uint16_t const ethertype) noexcept
            : basic_action_push{ethertype}
        {
        }

        static auto ieee802_1Q() noexcept
            -> push_vlan
        {
            return push_vlan{0x8100};
        }

        static auto ieee802_1ad() noexcept
            -> push_vlan
        {
            return push_vlan{0x88a8};
        }

    private:
        friend basic_action_push::basic_fixed_length_action;

        explicit push_vlan(raw_ofp_type const& ofp_action) noexcept
            : basic_action_push{ofp_action}
        {
        }

        void validate_action() const
        {
            if (ethertype() != 0x8100 && ethertype() != 0x88a8) {
                throw std::runtime_error{"invalid ethertype"};
            }
        }
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_PUSH_VLAN_HPP
