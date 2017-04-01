#ifndef CANARD_NET_OFP_V13_IMPL_ANY_INSTRUCTION_IPP
#define CANARD_NET_OFP_V13_IMPL_ANY_INSTRUCTION_IPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/any_instruction.hpp>

#include <cstdint>
#include <boost/variant/static_visitor.hpp>
#include <canard/network/openflow/v13/instruction_order.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/openflow/detail/impl/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class any_type<ofp::v13::instruction_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  namespace any_instruction_detail {

    struct instruction_order_visitor
      : boost::static_visitor<std::uint64_t>
    {
      template <class Instruction>
      auto operator()(Instruction const& instruction) const
        -> std::uint64_t
      {
        return instruction_order<Instruction>::get_value(instruction);
      }
    };

  } // namespace any_instruction_detail

  auto instruction_order<any_instruction>::get_value(
      any_instruction const& instruction)
    -> std::uint64_t
  {
    auto visitor = any_instruction_detail::instruction_order_visitor{};
    return instruction.visit(visitor);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_IMPL_ANY_INSTRUCTION_IPP
