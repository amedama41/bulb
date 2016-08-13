#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>
#include <canard/network/protocol/openflow/detail/decode.hpp>
#include <canard/network/protocol/openflow/detail/encode.hpp>
#include <canard/network/protocol/openflow/detail/is_same_value_type.hpp>
#include <canard/network/protocol/openflow/detail/memcmp.hpp>
#include <canard/network/protocol/openflow/v13/detail/byteorder.hpp>
#include <canard/network/protocol/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

    template <class T, class OFPInstruction>
    class basic_instruction
        : private boost::equality_comparable<T>
    {
    protected:
        using raw_ofp_type = OFPInstruction;

        basic_instruction() = default;

    public:
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

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            return detail::encode(container, base_instruction());
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> T
        {
            return T{detail::decode<raw_ofp_type>(first, last)};
        }

        template <class... Args>
        static auto create(Args&&... args)
            -> T
        {
            return T::validate(T(std::forward<Args>(args)...));
        }

        static void validate_instruction(
                ofp::v13::v13_detail::ofp_instruction const& instruction)
        {
            if (instruction.type != T::instruction_type) {
                throw std::runtime_error{"invalid instruction type"};
            }
            if (instruction.len != sizeof(raw_ofp_type)) {
                throw std::runtime_error{"invalid instruction length"};
            }
        }

        template <class Instruction>
        static auto validate(Instruction&& instruction)
            -> typename std::enable_if<
                  detail::is_same_value_type<Instruction, T>::value
                , Instruction&&
               >::type
        {
            T::validate_impl(instruction);
            return std::forward<Instruction>(instruction);
        }

        friend auto operator==(T const& lhs, T const& rhs) noexcept
            -> bool
        {
            return detail::memcmp(
                    lhs.base_instruction(), rhs.base_instruction());
        }

    private:
        auto base_instruction() const noexcept
            -> raw_ofp_type const&
        {
            return static_cast<T const*>(this)->ofp_instruction();
        }
    };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP
