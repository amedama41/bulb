#ifndef CANARD_NETWORK_OPENFLOW_V13_IMPL_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NETWORK_OPENFLOW_V13_IMPL_ANY_OXM_MATCH_FIELD_HPP

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#include <canard/network/protocol/openflow/detail/impl/any_oxm_match_field.hpp>
#include <canard/network/protocol/openflow/v13/any_oxm_match_field.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

  template class any_oxm_match_field<openflow::v13::oxm_match_field_decoder>;

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

# endif
#endif

#endif // CANARD_NETWORK_OPENFLOW_V13_IMPL_ANY_OXM_MATCH_FIELD_HPP
