#ifndef CANARD_NET_OFP_V13_IMPL_ACTION_SET_IPP
#define CANARD_NET_OFP_V13_IMPL_ACTION_SET_IPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/action_set.hpp>

#include <canard/network/openflow/detail/impl/set.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  template class detail::set_base<
    v13::action_set, std::uint64_t, v13::any_action
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#endif

#endif // CANARD_NET_OFP_V13_IMPL_ACTION_SET_IPP
