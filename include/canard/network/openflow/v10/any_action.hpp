#ifndef CANARD_NET_OFP_V10_ANY_ACTION_HPP
#define CANARD_NET_OFP_V10_ANY_ACTION_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <canard/network/openflow/detail/any_action.hpp>
#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/v10/actions.hpp>
#include <canard/network/openflow/v10/decoder/action_decoder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  using any_action = detail::any_action<action_decoder>;

  template <class T>
  auto any_cast(any_action& action)
    -> T&
  {
    return detail::any_cast<T>(action);
  }

  template <class T>
  auto any_cast(any_action const& action)
    -> T const&
  {
    return detail::any_cast<T>(action);
  }

  template <class T>
  auto any_cast(any_action* const action)
    -> T*
  {
    return detail::any_cast<T>(action);
  }

  template <class T>
  auto any_cast(any_action const* const action)
    -> T const*
  {
    return detail::any_cast<T>(action);
  }

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  extern template class any_action<ofp::v10::action_decoder>;
  extern template class any_type<any_action<ofp::v10::action_decoder>>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V10_ANY_ACTION_HPP
