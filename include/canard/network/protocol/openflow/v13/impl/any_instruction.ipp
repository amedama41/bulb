#ifndef CANARD_NETWORK_OPENFLOW_V13_IMPL_ANY_INSTRUCTION_IPP
#define CANARD_NETWORK_OPENFLOW_V13_IMPL_ANY_INSTRUCTION_IPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/protocol/openflow/detail/impl/any_instruction.hpp>
#  include <canard/network/protocol/openflow/v13/any_instruction.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

  template class any_instruction<openflow::v13::instruction_decoder>;

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

# endif
#endif

#endif // CANARD_NETWORK_OPENFLOW_V13_IMPL_ANY_INSTRUCTION_IPP
