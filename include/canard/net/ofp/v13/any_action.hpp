#ifndef CANARD_NET_OFP_V13_ANY_ACTION_HPP
#define CANARD_NET_OFP_V13_ANY_ACTION_HPP

#include <canard/net/ofp/detail/config.hpp>

#include <cstdint>
#include <canard/net/ofp/detail/any_type.hpp>
#include <canard/net/ofp/v13/action_order.hpp>
#include <canard/net/ofp/v13/decoder/action_decoder.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

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

  template <>
  struct action_order<any_action>
  {
    CANARD_NET_OFP_DECL static auto get_value(any_action const&)
      -> std::uint64_t;
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)

#include <canard/net/ofp/v13/impl/any_action.ipp>

#elif defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  extern template class any_type<ofp::v13::action_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif

#endif // CANARD_NET_OFP_V13_ANY_ACTION_HPP
