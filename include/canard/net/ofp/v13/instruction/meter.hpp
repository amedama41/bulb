#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_METER_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_METER_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/net/ofp/v13/detail/basic_fixed_length_instruction.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace instructions {

    class meter
        : public detail::v13::basic_fixed_length_instruction<meter>
    {
    public:
        using raw_ofp_type = protocol::ofp_instruction_meter;

        static constexpr protocol::ofp_instruction_type instruction_type
            = protocol::OFPIT_METER;

        explicit meter(std::uint32_t const meter_id) noexcept
            : instruction_meter_{
                  instruction_type
                , sizeof(raw_ofp_type)
                , meter_id
              }
        {
        }

        auto meter_id() const noexcept
            -> std::uint32_t
        {
            return instruction_meter_.meter_id;
        }

    private:
        friend basic_fixed_length_instruction;

        explicit meter(raw_ofp_type const& instruction_meter) noexcept
            : instruction_meter_(instruction_meter)
        {
        }

        auto ofp_instruction() const noexcept
            -> raw_ofp_type const&
        {
            return instruction_meter_;
        }

        void validate_instruction() const
        {
            if (meter_id() == 0 || meter_id() > protocol::OFPM_MAX) {
                throw std::runtime_error{"invalid meter id"};
            }
        }

        auto is_equivalent_instruction(meter const& rhs) const noexcept
            -> bool
        {
            return meter_id() == rhs.meter_id();
        }

    private:
        raw_ofp_type instruction_meter_;
    };

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_METER_HPP
