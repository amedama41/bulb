#ifndef CANARD_NET_OFP_V13_IMPL_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NET_OFP_V13_IMPL_ANY_OXM_MATCH_FIELD_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/any_oxm_match_field.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#include <canard/network/openflow/detail/impl/any_oxm_match_field_base.hpp>
#include <canard/network/openflow/detail/impl/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class any_type<
    ofp::v13::oxm_match_field_decoder, any_oxm_match_field_base
  >;
  template class any_oxm_match_field_base<
    any_type<ofp::v13::oxm_match_field_decoder, any_oxm_match_field_base>
  >;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_IMPL_ANY_OXM_MATCH_FIELD_HPP
