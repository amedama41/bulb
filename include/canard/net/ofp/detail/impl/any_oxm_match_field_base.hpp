#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_BASE_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_BASE_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/any_oxm_match_field_base.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::oxm_class() const noexcept
    -> std::uint16_t
  {
    return derived().visit(detail::oxm_class_visitor{});
  }

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::oxm_field() const noexcept
    -> std::uint8_t
  {
    return derived().visit(detail::oxm_field_visitor{});
  }

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::oxm_type() const noexcept
    -> std::uint32_t
  {
    return derived().visit(detail::oxm_type_visitor{});
  }

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::oxm_header() const noexcept
    -> std::uint32_t
  {
    return derived().visit(detail::oxm_header_visitor{});
  }

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::oxm_hasmask() const noexcept
    -> bool
  {
    return derived().visit(detail::oxm_hasmask_visitor{});
  }

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::oxm_length() const noexcept
    -> std::uint8_t
  {
    return derived().visit(detail::oxm_length_visitor{});
  }

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::is_wildcard() const noexcept
    -> bool
  {
    return derived().visit(detail::is_wildcard_visitor{});
  }

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::is_exact() const noexcept
    -> bool
  {
    return derived().visit(detail::is_exact_visitor{});
  }

  template <class Derived>
  auto any_oxm_match_field_base<Derived>::derived() const noexcept
    -> Derived const&
  {
    return *static_cast<Derived const*>(this);
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_BASE_HPP
