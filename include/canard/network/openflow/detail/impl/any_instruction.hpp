#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_INSTRUCTION_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_INSTRUCTION_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/any_instruction.hpp>

#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class InstructionDecoder>
  auto any_instruction<InstructionDecoder>::type() const noexcept
    -> ofp_instruction_type
  {
    return base_t::visit(detail::type_visitor<ofp_instruction_type>{});
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_INSTRUCTION_HPP
