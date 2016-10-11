#ifndef CANARD_NET_OFP_V13_ANY_INSTRUCTION_HPP
#define CANARD_NET_OFP_V13_ANY_INSTRUCTION_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/v13/decoder/instruction_decoder.hpp>
#include <canard/network/openflow/v13/instruction_order.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  using any_instruction = detail::any_type<instruction_decoder>;

  template <class T>
  auto any_cast(any_instruction& instruction)
    -> T&
  {
    return detail::any_cast<T>(instruction);
  }

  template <class T>
  auto any_cast(any_instruction const& instruction)
    -> T const&
  {
    return detail::any_cast<T>(instruction);
  }

  template <class T>
  auto any_cast(any_instruction* const instruction)
    -> T*
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
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)

#include <canard/network/openflow/v13/impl/any_instruction.ipp>

#elif defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  extern template class any_type<ofp::v13::instruction_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif

#endif // CANARD_NET_OFP_V13_ANY_INSTRUCTION_HPP
