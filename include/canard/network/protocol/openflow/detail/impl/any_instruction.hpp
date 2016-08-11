#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_IMPL_ANY_INSTRUCTION_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_IMPL_ANY_INSTRUCTION_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>
#include <canard/network/protocol/openflow/detail/any_instruction.hpp>
#include <cstdint>
#include <boost/variant/apply_visitor.hpp>
#include <canard/network/protocol/openflow/detail/visitors.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

  template <class InstructionDecoder>
  auto any_instruction<InstructionDecoder>::type() const noexcept
    -> ofp_instruction_type
  {
    auto visitor = detail::type_visitor<ofp_instruction_type>{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class InstructionDecoder>
  auto any_instruction<InstructionDecoder>::length() const noexcept
    -> std::uint16_t
  {
    auto visitor = detail::length_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class InstructionDecoder>
  auto any_instruction<InstructionDecoder>::index() const noexcept
    -> std::size_t
  {
    return variant_.which();
  }

  template <class InstructionDecoder>
  auto any_instruction<InstructionDecoder>::equal_impl(
      any_instruction const& rhs) const
    -> bool
  {
    return variant_ == rhs.variant_;
  }

  template <class InstructionDecoder>
  auto any_instruction<InstructionDecoder>::equivalent_impl(
      any_instruction const& rhs) const noexcept
    -> bool
  {
    auto visitor = detail::equivalent_visitor{};
    return boost::apply_visitor(visitor, variant_, rhs.variant_);
  }

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_IMPL_ANY_INSTRUCTION_HPP
