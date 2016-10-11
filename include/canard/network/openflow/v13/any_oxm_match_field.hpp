#ifndef CANARD_NET_OFP_V13_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NET_OFP_V13_ANY_OXM_MATCH_FIELD_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <canard/network/openflow/detail/any_oxm_match_field_base.hpp>
#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/v13/decoder/oxm_match_field_decoder.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  using any_oxm_match_field = detail::any_type<
    oxm_match_field_decoder, detail::any_oxm_match_field_base
  >;

  template <class T>
  auto any_cast(any_oxm_match_field& field)
    -> T&
  {
    return detail::any_cast<T>(field);
  }

  template <class T>
  auto any_cast(any_oxm_match_field const& field)
    -> T const&
  {
    return detail::any_cast<T>(field);
  }

  template <class T>
  auto any_cast(any_oxm_match_field* const field)
    -> T*
  {
    return detail::any_cast<T>(field);
  }

  template <class T>
  auto any_cast(any_oxm_match_field const* const field)
    -> T const*
  {
    return detail::any_cast<T>(field);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  extern template class any_type<ofp::v13::oxm_match_field_decoder>;
  extern template class any_oxm_match_field_base<
    any_type<ofp::v13::oxm_match_field_decoder, any_oxm_match_field_base>
  >;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_ANY_OXM_MATCH_FIELD_HPP
