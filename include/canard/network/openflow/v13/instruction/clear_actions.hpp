#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_CLEAR_ACTIONS_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_CLEAR_ACTIONS_HPP

#include <utility>
#include <canard/network/openflow/v13/action_list.hpp>
#include <canard/network/openflow/v13/detail/basic_instruction_actions.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace instructions {

    class clear_actions
        : public detail::v13::basic_instruction_actions<clear_actions>
    {
    public:
        static constexpr protocol::ofp_instruction_type instruction_type
            = protocol::OFPIT_CLEAR_ACTIONS;

        clear_actions()
            : basic_instruction_actions{action_list{}}
        {
        }

    private:
        friend basic_instruction_actions;

        explicit clear_actions(
                raw_ofp_type const& instruction_actions, action_list&& actions)
            : basic_instruction_actions{instruction_actions, std::move(actions)}
        {
        }

        template <class Validator>
        void validate_instruction(Validator) const
        {
        }

        auto is_equivalent_instruction(clear_actions const&) const noexcept
            -> bool
        {
            return true;
        }
    };

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_CLEAR_ACTIONS_HPP
