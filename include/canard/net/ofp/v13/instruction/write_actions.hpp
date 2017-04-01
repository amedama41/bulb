#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP

#include <stdexcept>
#include <utility>
#include <canard/network/openflow/v13/action_list.hpp>
#include <canard/network/openflow/v13/detail/basic_instruction_actions.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/utility/action_set.hpp>

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
            , class = enable_if_is_all_constructible_t<Actions...>
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

        template <class Validator>
        void validate_instruction(Validator validator) const
        {
            if (!action_set::is_valid_set(actions())) {
                throw std::runtime_error{"duplicated action type"};
            }
            validator(actions());
        }

        auto is_equivalent_instruction(write_actions const& rhs) const noexcept
            -> bool
        {
            return action_set::equivalent_as_set(actions(), rhs.actions());
        }
    };

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP
