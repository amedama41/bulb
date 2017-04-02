#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_GOTO_TABLE_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_GOTO_TABLE_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/net/ofp/v13/detail/basic_fixed_length_instruction.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace instructions {

  class goto_table
    : public detail::v13::basic_fixed_length_instruction<goto_table>
  {
  public:
    using raw_ofp_type = protocol::ofp_instruction_goto_table;

    static constexpr protocol::ofp_instruction_type instruction_type
      = protocol::OFPIT_GOTO_TABLE;

    explicit goto_table(std::uint8_t const table_id) noexcept
      : instruction_goto_table_{
          instruction_type, sizeof(raw_ofp_type), table_id, { 0, 0, 0 }
        }
    {
    }

    auto table_id() const noexcept
      -> std::uint8_t
    {
      return instruction_goto_table_.table_id;
    }

  private:
    friend basic_fixed_length_instruction;

    explicit goto_table(raw_ofp_type const& instruction_goto_table) noexcept
      : instruction_goto_table_(instruction_goto_table)
    {
    }

    auto ofp_instruction() const noexcept
      -> raw_ofp_type const&
    {
      return instruction_goto_table_;
    }

    void validate_instruction() const
    {
      if (table_id() > protocol::OFPTT_MAX) {
        throw std::runtime_error{"invalid table id"};
      }
    }

    auto is_equivalent_instruction(goto_table const& rhs) const noexcept
      -> bool
    {
      return table_id() == rhs.table_id();
    }

  private:
    raw_ofp_type instruction_goto_table_;
  };

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_GOTO_TABLE_HPP
