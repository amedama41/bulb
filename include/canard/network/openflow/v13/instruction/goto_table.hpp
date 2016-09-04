#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_GOTO_TABLE_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_GOTO_TABLE_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/v13/detail/basic_instruction.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace instructions {

    class goto_table
        : public detail::v13::basic_instruction<
            goto_table, v13_detail::ofp_instruction_goto_table
          >
    {
    public:
        static constexpr protocol::ofp_instruction_type instruction_type
            = protocol::OFPIT_GOTO_TABLE;

        explicit goto_table(std::uint8_t const table_id) noexcept
            : instruction_goto_table_{
                  instruction_type
                , sizeof(raw_ofp_type)
                , table_id
                , { 0, 0, 0 }
              }
        {
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return instruction_goto_table_.table_id;
        }

    private:
        friend basic_instruction;

        explicit goto_table(raw_ofp_type const& instruction_goto_table) noexcept
            : instruction_goto_table_(instruction_goto_table)
        {
        }

        auto ofp_instruction() const noexcept
            -> raw_ofp_type const&
        {
            return instruction_goto_table_;
        }

        template <class Validator>
        void validate_impl(Validator) const
        {
            if (table_id() > protocol::OFPTT_MAX) {
                throw std::runtime_error{"invalid table id"};
            }
        }

    private:
        raw_ofp_type instruction_goto_table_;
    };

    inline auto equivalent(
            goto_table const& lhs, goto_table const& rhs) noexcept
        -> bool
    {
        return lhs.table_id() == rhs.table_id();
    }

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_GOTO_TABLE_HPP
