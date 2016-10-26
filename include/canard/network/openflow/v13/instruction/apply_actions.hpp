#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_APPLY_ACTIONS_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_APPLY_ACTIONS_HPP

#include <utility>
#include <canard/network/openflow/v13/action_list.hpp>
#include <canard/network/openflow/v13/detail/basic_instruction_actions.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
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
            , class = enable_if_is_all_constructible_t<Actions...>
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

        template <class Validator>
        void validate_instruction(Validator validator) const
        {
            validator(actions());
        }

        auto is_equivalent_instruction(apply_actions const& rhs) const noexcept
            -> bool
        {
            return equivalent(actions(), rhs.actions());
        }
    };

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_APPLY_ACTIONS_HPP
