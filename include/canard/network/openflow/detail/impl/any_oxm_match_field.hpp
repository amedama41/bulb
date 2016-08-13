#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/detail/any_oxm_match_field.hpp>
#include <cstdint>
#include <boost/variant/apply_visitor.hpp>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::length() const noexcept
    -> std::uint16_t
  {
    auto visitor = detail::length_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_class() const noexcept
    -> std::uint16_t
  {
    auto visitor = detail::oxm_class_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_field() const noexcept
    -> std::uint8_t
  {
    auto visitor = detail::oxm_field_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_type() const noexcept
    -> std::uint32_t
  {
    auto visitor = detail::oxm_type_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_header() const noexcept
    -> std::uint32_t
  {
    auto visitor = detail::oxm_header_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_has_mask() const noexcept
    -> bool
  {
    auto visitor = detail::oxm_has_mask_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::oxm_length() const noexcept
    -> std::uint8_t
  {
    auto visitor = detail::oxm_length_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::is_wildcard() const noexcept
    -> bool
  {
    auto visitor = detail::is_wildcard_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::is_exact() const noexcept
    -> bool
  {
    auto visitor = detail::is_exact_visitor{};
    return boost::apply_visitor(visitor, variant_);
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::equal_impl(
      any_oxm_match_field const& rhs) const
    -> bool
  {
    return variant_ == rhs.variant_;
  }

  template <class OXMMatchFieldDecoder>
  auto any_oxm_match_field<OXMMatchFieldDecoder>::equivalent_impl(
      any_oxm_match_field const& rhs) const noexcept
    -> bool
  {
    auto visitor = detail::equivalent_visitor{};
    return boost::apply_visitor(visitor, variant_, rhs.variant_);
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_OXM_MATCH_FIELD_HPP
