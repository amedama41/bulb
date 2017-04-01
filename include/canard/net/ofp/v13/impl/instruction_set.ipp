#ifndef CANARD_NET_OFP_V13_IMPL_INSTRUCTION_SET_IPP
#define CANARD_NET_OFP_V13_IMPL_INSTRUCTION_SET_IPP

#include <canard/net/ofp/detail/config.hpp>
#include <canard/net/ofp/v13/utility/instruction_set.hpp>

#include <canard/net/ofp/detail/impl/set_base.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  template class detail::set_base<
    v13::instruction_set, std::uint64_t, v13::any_instruction
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#endif

#endif // CANARD_NET_OFP_V13_IMPL_INSTRUCTION_SET_IPP
