#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

    template <class T, class OFPInstruction>
    class basic_instruction
        : public detail::basic_protocol_type<T>
    {
    protected:
        basic_instruction() = default;

    public:
        using raw_ofp_type = OFPInstruction;

        static constexpr auto type() noexcept
            -> ofp::v13::protocol::ofp_instruction_type
        {
            return T::instruction_type;
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
        }

        static void validate_header(
                ofp::v13::protocol::ofp_instruction const& instruction)
        {
            if (instruction.type != type()) {
                throw std::runtime_error{"invalid instruction type"};
            }
            if (instruction.len != length()) {
                throw std::runtime_error{"invalid instruction length"};
            }
        }

    private:
        auto base_instruction() const noexcept
            -> raw_ofp_type const&
        {
            return static_cast<T const*>(this)->ofp_instruction();
        }

        friend detail::basic_protocol_type<T>;

        template <class Validator>
        void validate_impl(Validator) const
        {
            static_cast<T const*>(this)->validate_instruction();
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, base_instruction());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> T
        {
            return T{detail::decode<raw_ofp_type>(first, last)};
        }

        auto equal_impl(T const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(base_instruction(), rhs.base_instruction());
        }

        auto equivalent_impl(T const& rhs) const noexcept
            -> bool
        {
            return static_cast<T const*>(this)->is_equivalent_instruction(rhs);
        }
    };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP
