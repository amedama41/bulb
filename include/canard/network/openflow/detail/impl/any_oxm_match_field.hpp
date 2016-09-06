#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/any_oxm_match_field.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_class() const noexcept
    -> std::uint16_t
  {
    return base_t::visit(detail::oxm_class_visitor{});
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_field() const noexcept
    -> std::uint8_t
  {
    return base_t::visit(detail::oxm_field_visitor{});
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_type() const noexcept
    -> std::uint32_t
  {
    return base_t::visit(detail::oxm_type_visitor{});
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_header() const noexcept
    -> std::uint32_t
  {
    return base_t::visit(detail::oxm_header_visitor{});
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_has_mask() const noexcept
    -> bool
  {
    return base_t::visit(detail::oxm_has_mask_visitor{});
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_length() const noexcept
    -> std::uint8_t
  {
    return base_t::visit(detail::oxm_length_visitor{});
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::is_wildcard() const noexcept
    -> bool
  {
    return base_t::visit(detail::is_wildcard_visitor{});
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::is_exact() const noexcept
    -> bool
  {
    return base_t::visit(detail::is_exact_visitor{});
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_HPP
