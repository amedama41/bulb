#ifndef CANARD_NET_OFP_V10_ANY_QUEUE_PROPEPRTY_HPP
#define CANARD_NET_OFP_V10_ANY_QUEUE_PROPEPRTY_HPP

#include <canard/net/ofp/detail/any_type.hpp>
#include <canard/net/ofp/v10/decoder/queue_property_decoder.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  using any_queue_property = detail::any_type<queue_property_decoder>;

  template <class T>
  auto any_cast(any_queue_property& property)
    -> T&
  {
    return detail::any_cast<T>(property);
  }

  template <class T>
  auto any_cast(any_queue_property const& property)
    -> T const&
  {
    return detail::any_cast<T>(property);
  }

  template <class T>
  auto any_cast(any_queue_property* const property)
    -> T*
  {
    return detail::any_cast<T>(property);
  }

  template <class T>
  auto any_cast(any_queue_property const* const property)
    -> T const*
  {
    return detail::any_cast<T>(property);
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

  extern template class any_type<ofp::v10::queue_property_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V10_ANY_QUEUE_PROPEPRTY_HPP
