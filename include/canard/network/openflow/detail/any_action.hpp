#ifndef CANARD_NET_OFP_DETAIL_ANY_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_ANY_ACTION_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <canard/network/openflow/detail/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class ActionDecoder>
  class any_action
    : public any_type<any_action<ActionDecoder>>
  {
    using base_t = any_type<any_action<ActionDecoder>>;

  public:
    using ofp_action_type = typename ActionDecoder::ofp_action_type;

    using base_t::base_t;

    CANARD_NET_OFP_DECL auto type() const noexcept
      -> ofp_action_type;
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/openflow/detail/impl/any_action.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_ACTION_HPP
