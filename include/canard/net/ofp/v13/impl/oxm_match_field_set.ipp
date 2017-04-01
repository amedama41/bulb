#ifndef CANARD_NET_OFP_V13_IMPL_OXM_MATCH_FIELD_SET_HPP
#define CANARD_NET_OFP_V13_IMPL_OXM_MATCH_FIELD_SET_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/utility/oxm_match_field_set.hpp>
#include <canard/network/openflow/detail/impl/set_base.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  template class detail::set_base<
    v13::oxm_match_field_set, std::uint32_t, v13::any_oxm_match_field
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#endif

#endif // CANARD_NET_OFP_V13_IMPL_OXM_MATCH_FIELD_SET_HPP
