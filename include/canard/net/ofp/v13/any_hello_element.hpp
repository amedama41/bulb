#ifndef CANARD_NET_OFP_V13_ANY_HELLO_ELEMENT_HPP
#define CANARD_NET_OFP_V13_ANY_HELLO_ELEMENT_HPP

#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/v13/decoder/hello_element_decoder.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  using any_hello_element = detail::any_type<hello_element_decoder>;

  template <class T>
  auto any_cast(any_hello_element& hello_elem)
      -> T&
  {
      return detail::any_cast<T>(hello_elem);
  }

  template <class T>
  auto any_cast(any_hello_element const& hello_elem)
      -> T const&
  {
      return detail::any_cast<T>(hello_elem);
  }

  template <class T>
  auto any_cast(any_hello_element* const hello_elem)
      -> T*
  {
      return detail::any_cast<T>(hello_elem);
  }

  template <class T>
  auto any_cast(any_hello_element const* const hello_elem)
      -> T const*
  {
      return detail::any_cast<T>(hello_elem);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)

#include <canard/network/openflow/v13/impl/any_hello_element.ipp>

#elif defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  extern template class any_type<ofp::v13::hello_element_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif

#endif // CANARD_NET_OFP_V13_ANY_HELLO_ELEMENT_HPP
