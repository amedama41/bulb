#ifndef CANARD_NET_OFP_V10_ACTIONS_STRIP_VLAN_HPP
#define CANARD_NET_OFP_V10_ACTIONS_STRIP_VLAN_HPP

#include <type_traits>
#include <utility>
#include <canard/network/openflow/detail/is_same_value_type.hpp>
#include <canard/network/openflow/v10/detail/basic_action.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace actions {

    class strip_vlan
        : public actions_detail::basic_action<
                strip_vlan, v10_detail::ofp_action_header
          >
    {
        using raw_ofp_type = v10_detail::ofp_action_header;

    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_STRIP_VLAN;

        strip_vlan() noexcept
            : strip_vlan_{action_type, sizeof(raw_ofp_type), { 0, 0, 0, 0 }}
        {
        }

        template <class Action>
        static auto validate(Action&& action)
            -> typename std::enable_if<
                  detail::is_same_value_type<Action, strip_vlan>::value, Action&&
               >::type
        {
            return std::forward<Action>(action);
        }

    private:
        friend basic_action;

        explicit strip_vlan(raw_ofp_type const& action_header) noexcept
            : strip_vlan_(action_header)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return strip_vlan_;
        }

    private:
        raw_ofp_type strip_vlan_;
    };

    constexpr inline auto operator==(
            strip_vlan const&, strip_vlan const&) noexcept
        -> bool
    {
        return true;
    }

    using pop_vlan = strip_vlan;

} // namespace actions
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTIONS_STRIP_VLAN_HPP
