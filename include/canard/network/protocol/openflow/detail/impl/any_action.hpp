#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_ACTION_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>
#include <canard/network/protocol/openflow/detail/any_action.hpp>
#include <cstdint>
#include <boost/variant/apply_visitor.hpp>
#include <canard/network/protocol/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class ActionDecoder>
  auto any_action<ActionDecoder>::type() const noexcept
    -> ofp_action_type
  {
    auto visitor = detail::type_visitor<ofp_action_type>{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class ActionDecoder>
  auto any_action<ActionDecoder>::length() const noexcept
    -> std::uint16_t
  {
    auto visitor = detail::length_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class ActionDecoder>
  auto any_action<ActionDecoder>::index() const noexcept
    -> std::size_t
  {
    return variant_.which();
  }

  template <class ActionDecoder>
  auto any_action<ActionDecoder>::equal_impl(any_action const& rhs) const
    -> bool
  {
    return variant_ == rhs.variant_;
  }

  template <class ActionDecoder>
  auto any_action<ActionDecoder>::equivalent_impl(
      any_action const& rhs) const noexcept
    -> bool
  {
    auto visitor = detail::equivalent_visitor{};
    return boost::apply_visitor(visitor, variant_, rhs.variant_);
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_ACTION_HPP
