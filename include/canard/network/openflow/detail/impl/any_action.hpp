#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_ACTION_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/any_action.hpp>

#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class ActionDecoder>
  auto any_action<ActionDecoder>::type() const noexcept
    -> ofp_action_type
  {
    return base_t::visit(detail::type_visitor<ofp_action_type>{});
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_ACTION_HPP
