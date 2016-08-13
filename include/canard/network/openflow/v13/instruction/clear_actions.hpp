#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_CLEAR_ACTIONS_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_CLEAR_ACTIONS_HPP

#include <canard/network/openflow/v13/detail/basic_instruction.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace instructions {

    class clear_actions
        : public detail::v13::basic_instruction<
            clear_actions, v13_detail::ofp_instruction_actions
          >
    {
    public:
        static constexpr protocol::ofp_instruction_type instruction_type
            = protocol::OFPIT_CLEAR_ACTIONS;

        clear_actions() noexcept
            : instruction_actions_{
                  instruction_type
                , sizeof(raw_ofp_type)
                , { 0, 0, 0, 0 }
              }
        {
        }

    private:
        friend basic_instruction;

        explicit clear_actions(raw_ofp_type const& instruction_actions) noexcept
            : instruction_actions_(instruction_actions)
        {
        }

        auto ofp_instruction() const noexcept
            -> raw_ofp_type const&
        {
            return instruction_actions_;
        }

        static void validate_impl(clear_actions const&)
        {
        }

    private:
        raw_ofp_type instruction_actions_;
    };

    constexpr inline auto equivalent(
            clear_actions const&, clear_actions const&) noexcept
        -> bool
    {
        return true;
    }

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_CLEAR_ACTIONS_HPP
