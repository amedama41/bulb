#ifndef CANARD_NET_OFP_V10_ANY_ACTION_HPP
#define CANARD_NET_OFP_V10_ANY_ACTION_HPP

#include <canard/net/ofp/detail/config.hpp>

#include <canard/net/ofp/detail/any_type.hpp>
#include <canard/net/ofp/v10/decoder/action_decoder.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  using any_action = detail::any_type<action_decoder>;

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

  extern template class any_type<ofp::v10::action_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V10_ANY_ACTION_HPP
