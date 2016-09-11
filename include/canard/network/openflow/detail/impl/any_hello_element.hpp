#ifndef CANARD_NET_OFP_DETAIL_IMPL_HELLO_ELEMENT_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_HELLO_ELEMENT_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/any_hello_element.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class HelloElementDecoder>
  auto any_hello_element<HelloElementDecoder>::type() const noexcept
    -> std::uint16_t
  {
    return base_t::visit(detail::type_visitor<std::uint16_t>{});
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_HELLO_ELEMENT_HPP
