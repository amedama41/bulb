#ifndef CANARD_NET_OFP_DETAIL_ANY_OXM_MATCH_FIELD_BASE_HPP
#define CANARD_NET_OFP_DETAIL_ANY_OXM_MATCH_FIELD_BASE_HPP

#include <canard/net/ofp/detail/config.hpp>

#include <cstdint>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Derived>
  class any_oxm_match_field_base
  {
  public:
    CANARD_NET_OFP_DECL auto oxm_class() const noexcept
      -> std::uint16_t;

    CANARD_NET_OFP_DECL auto oxm_field() const noexcept
      -> std::uint8_t;

    CANARD_NET_OFP_DECL auto oxm_type() const noexcept
      -> std::uint32_t;

    CANARD_NET_OFP_DECL auto oxm_header() const noexcept
      -> std::uint32_t;

    CANARD_NET_OFP_DECL auto oxm_hasmask() const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto oxm_length() const noexcept
      -> std::uint8_t;

    CANARD_NET_OFP_DECL auto is_wildcard() const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto is_exact() const noexcept
      -> bool;

  private:
    CANARD_NET_OFP_DECL auto derived() const noexcept
      -> Derived const&;
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/net/ofp/detail/impl/any_oxm_match_field_base.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_OXM_MATCH_FIELD_BASE_HPP
