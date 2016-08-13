#ifndef CANARD_NETWORK_OPENFLOW_V13_ANY_INSTRUCTION_HPP
#define CANARD_NETWORK_OPENFLOW_V13_ANY_INSTRUCTION_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#include <cstdint>
#include <canard/network/protocol/openflow/detail/any_instruction.hpp>
#include <canard/network/protocol/openflow/v13/decoder/instruction_decoder.hpp>
#include <canard/network/protocol/openflow/v13/instruction_order.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {

    using any_instruction = detail::any_instruction<instruction_decoder>;

    template <class T>
    auto any_cast(any_instruction const& instruction)
        -> T const&
    {
        return detail::any_cast<T>(instruction);
    }

    template <class T>
    auto any_cast(any_instruction const* const instruction)
        -> T const*
    {
        return detail::any_cast<T>(instruction);
    }

    template <>
    struct instruction_order<any_instruction>
    {
        CANARD_NET_OFP_DECL static auto get_value(any_instruction const&)
            -> std::uint64_t;
    };

} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)

#include <canard/network/protocol/openflow/v13/impl/any_instruction.ipp>

#elif defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    extern template class any_instruction<openflow::v13::instruction_decoder>;

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif

#endif // CANARD_NETWORK_OPENFLOW_V13_ANY_INSTRUCTION_HPP
