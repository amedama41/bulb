#ifndef CANARD_NETWORK_OPENFLOW_V13_INSTRUCTIONS_APPLY_ACTIONS_HPP
#define CANARD_NETWORK_OPENFLOW_V13_INSTRUCTIONS_APPLY_ACTIONS_HPP

#include <type_traits>
#include <utility>
#include <canard/network/protocol/openflow/detail/is_related.hpp>
#include <canard/network/protocol/openflow/v13/action_list.hpp>
#include <canard/network/protocol/openflow/v13/detail/basic_instruction_actions.hpp>
#include <canard/network/protocol/openflow/v13/openflow.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {
namespace instructions {

    class apply_actions
        : public detail::v13::basic_instruction_actions<apply_actions>
    {
    public:
        static constexpr protocol::ofp_instruction_type instruction_type
            = protocol::OFPIT_APPLY_ACTIONS;

        explicit apply_actions(action_list actions)
            : basic_instruction_actions{std::move(actions)}
        {
        }

        template <
              class... Actions
            , typename std::enable_if<
                !detail::is_related<apply_actions, Actions...>::value
              >::type* = nullptr
        >
        explicit apply_actions(Actions&&... actions)
            : apply_actions{action_list{std::forward<Actions>(actions)...}}
        {
        }

    private:
        friend basic_instruction_actions;

        apply_actions(
                raw_ofp_type const& instruction_actions, action_list&& actions)
            : basic_instruction_actions{instruction_actions, std::move(actions)}
        {
        }

        static void validate_impl(apply_actions const&)
        {
        }
    };

    inline auto equivalent(
            apply_actions const& lhs, apply_actions const& rhs) noexcept
        -> bool
    {
        return equivalent(lhs.actions(), rhs.actions());
    }

} // namespace instructions
} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_V13_INSTRUCTIONS_APPLY_ACTIONS_HPP
