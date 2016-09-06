#ifndef CANARD_NET_OFP_DETAIL_ANY_INSTRUCTION_HPP
#define CANARD_NET_OFP_DETAIL_ANY_INSTRUCTION_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstddef>
#include <boost/mpl/size.hpp>
#include <canard/network/openflow/detail/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class InstructionDecoder>
  class any_instruction
    : public any_type<any_instruction<InstructionDecoder>>
  {
    using base_t = any_type<any_instruction<InstructionDecoder>>;

  public:
    using ofp_instruction_type
      = typename InstructionDecoder::ofp_instruction_type;

    using base_t::base_t;

    CANARD_NET_OFP_DECL auto type() const noexcept
      -> ofp_instruction_type;
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/openflow/detail/impl/any_instruction.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_INSTRUCTION_HPP
