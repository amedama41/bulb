#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_METADATA_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_METADATA_HPP

#include <cstdint>
#include <limits>
#include <canard/network/openflow/v13/detail/basic_instruction.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace instructions {

    class write_metadata
        : public detail::v13::basic_instruction<
            write_metadata, v13_detail::ofp_instruction_write_metadata
          >
    {
    public:
        static constexpr protocol::ofp_instruction_type instruction_type
            = protocol::OFPIT_WRITE_METADATA;

        explicit write_metadata(
                  std::uint64_t const metadata
                , std::uint64_t const metadata_mask
                    = std::numeric_limits<std::uint64_t>::max()) noexcept
            : instruction_write_metadata_{
                  instruction_type
                , sizeof(raw_ofp_type)
                , { 0, 0, 0, 0 }
                , metadata
                , metadata_mask
            }
        {
        }

        auto metadata() const noexcept
            -> std::uint64_t
        {
            return instruction_write_metadata_.metadata;
        }

        auto metadata_mask() const noexcept
            -> std::uint64_t
        {
            return instruction_write_metadata_.metadata_mask;
        }

    private:
        friend basic_instruction;

        explicit write_metadata(
                raw_ofp_type const& instruction_write_metadata) noexcept
            : instruction_write_metadata_(instruction_write_metadata)
        {
        }

        auto ofp_instruction() const noexcept
            -> raw_ofp_type const&
        {
            return instruction_write_metadata_;
        }

        template <class Validator>
        void validate_impl(Validator) const
        {
        }

    private:
        raw_ofp_type instruction_write_metadata_;
    };

    inline auto equivalent(
            write_metadata const& lhs, write_metadata const& rhs) noexcept
        -> bool
    {
        return lhs.metadata_mask() == rhs.metadata_mask()
            && ((lhs.metadata() & lhs.metadata_mask())
                    == (rhs.metadata() & lhs.metadata_mask()));
    }

} // namespace instructions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_WRITE_METADATA_HPP
