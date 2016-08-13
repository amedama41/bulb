#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP

#include <stdexcept>
#include <type_traits>
#include <utility>
#include <canard/network/openflow/detail/is_related.hpp>
#include <canard/network/openflow/v13/action_list.hpp>
#include <canard/network/openflow/v13/action_set.hpp>
#include <canard/network/openflow/v13/detail/basic_instruction_actions.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace instructions {

    class write_actions
        : public detail::v13::basic_instruction_actions<write_actions>
    {
    public:
        static constexpr protocol::ofp_instruction_type instruction_type
            = protocol::OFPIT_WRITE_ACTIONS;

        explicit write_actions(action_list actions)
            : basic_instruction_actions{std::move(actions)}
        {
        }

        explicit write_actions(action_set actions)
            : write_actions{std::move(actions).to_list()}
        {
        }

        template <
              class... Actions
            , typename std::enable_if<
                !detail::is_related<write_actions, Actions...>::value
              >::type* = nullptr
        >
        explicit write_actions(Actions&&... actions)
            : write_actions{action_set{std::forward<Actions>(actions)...}}
        {
        }

    private:
        friend basic_instruction_actions;

        write_actions(
                raw_ofp_type const& instruction_actions, action_list&& actions)
            : basic_instruction_actions{instruction_actions, std::move(actions)}
        {
        }

        static void validate_impl(write_actions const& write_actions)
        {
            if (!action_set::is_action_set(write_actions.actions())) {
                throw std::runtime_error{"duplicated action type"};
            }
        }
    };

    inline auto equivalent(
            write_actions const& lhs, write_actions const& rhs) noexcept
        -> bool
    {
        if (lhs.actions().size() != rhs.actions().size()) {
            return false;
        }

        return action_set::equivalent_as_action_set(
                lhs.actions(), rhs.actions());
    }

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP
