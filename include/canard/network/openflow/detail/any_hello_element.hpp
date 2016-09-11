#ifndef CANARD_NET_OFP_DETAIL_ANY_HELLO_ELEMENT_HPP
#define CANARD_NET_OFP_DETAIL_ANY_HELLO_ELEMENT_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class HelloElementDecoder>
  class any_hello_element
    : public any_type<any_hello_element<HelloElementDecoder>>
  {
    using base_t = any_type<any_hello_element<HelloElementDecoder>>;

  public:
    using base_t::base_t;

    CANARD_NET_OFP_DECL auto type() const noexcept
      -> std::uint16_t;
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/openflow/detail/impl/any_hello_element.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_HELLO_ELEMENT_HPP
