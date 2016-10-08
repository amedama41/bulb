#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_TYPE_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_TYPE_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/any_type.hpp>

#include <cstddef>
#include <cstdint>
#include <boost/variant/apply_visitor.hpp>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Derived, class Decoder>
  auto any_type<Derived, Decoder>::length() const noexcept
    -> std::uint16_t
  {
    return visit(detail::length_visitor{});
  }

  template <class Derived, class Decoder>
  auto any_type<Derived, Decoder>::byte_length() const noexcept
    -> std::uint16_t
  {
    return visit(detail::byte_length_visitor{});
  }

  template <class Derived, class Decoder>
  auto any_type<Derived, Decoder>::index() const noexcept
    -> std::size_t
  {
    return variant_.which();
  }

  template <class Derived, class Decoder>
  auto any_type<Derived, Decoder>::equal_impl(
      any_type const& rhs) const noexcept
    -> bool
  {
    return variant_ == rhs.variant_;
  }

  template <class Derived, class Decoder>
  auto any_type<Derived, Decoder>::equivalent_impl(
      any_type const& rhs) const noexcept
    -> bool
  {
    auto visitor = detail::equivalent_visitor{};
    return boost::apply_visitor(visitor, variant_, rhs.variant_);
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_TYPE_HPP
