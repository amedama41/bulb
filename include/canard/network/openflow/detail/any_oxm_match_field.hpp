#ifndef CANARD_NET_OFP_DETAIL_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NET_OFP_DETAIL_ANY_OXM_MATCH_FIELD_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class OXMMatchFieldDecoder>
  class any_oxm_match_field
    : public any_type<any_oxm_match_field<OXMMatchFieldDecoder>>
  {
    using base_t = any_type<any_oxm_match_field<OXMMatchFieldDecoder>>;

  public:
    using base_t::base_t;

    CANARD_NET_OFP_DECL auto oxm_class() const noexcept
      -> std::uint16_t;

    CANARD_NET_OFP_DECL auto oxm_field() const noexcept
      -> std::uint8_t;

    CANARD_NET_OFP_DECL auto oxm_type() const noexcept
      -> std::uint32_t;

    CANARD_NET_OFP_DECL auto oxm_header() const noexcept
      -> std::uint32_t;

    CANARD_NET_OFP_DECL auto oxm_has_mask() const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto oxm_length() const noexcept
      -> std::uint8_t;

    CANARD_NET_OFP_DECL auto is_wildcard() const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto is_exact() const noexcept
      -> bool;
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/openflow/detail/impl/any_oxm_match_field.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_OXM_MATCH_FIELD_HPP
