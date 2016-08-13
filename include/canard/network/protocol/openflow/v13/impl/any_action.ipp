#ifndef CANARD_NET_OFP_V13_IMPL_ANY_ACTION_IPP
#define CANARD_NET_OFP_V13_IMPL_ANY_ACTION_IPP

#include <canard/network/protocol/openflow/detail/config.hpp>
#include <canard/network/protocol/openflow/v13/any_action.hpp>

#include <cstdint>
#include <boost/variant/static_visitor.hpp>
#include <canard/network/protocol/openflow/v13/action_order.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/protocol/openflow/detail/impl/any_action.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class any_action<ofp::v13::action_decoder>;

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

  namespace any_action_detail {

    struct action_order_visitor
      : boost::static_visitor<std::uint64_t>
    {
      template <class Action>
      auto operator()(Action const& action) const
        -> result_type
      {
        return action_order<Action>::get_value(action);
      }
    };

  } // namespace any_action_detail

  auto action_order<any_action>::get_value(any_action const& action)
      -> std::uint64_t
  {
    auto visitor = any_action_detail::action_order_visitor{};
    return action.visit(visitor);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_IMPL_ANY_ACTION_IPP
