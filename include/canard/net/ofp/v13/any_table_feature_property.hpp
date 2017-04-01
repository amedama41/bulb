#ifndef CANARD_NET_OFP_V13_ANY_TABLE_FEATURE_PROPERTY_HPP
#define CANARD_NET_OFP_V13_ANY_TABLE_FEATURE_PROPERTY_HPP

#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/v13/decoder/table_feature_property_decoder.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  using any_table_feature_property
    = detail::any_type<table_feature_property_decoder>;

  template <class T>
  auto any_cast(any_table_feature_property& property)
    -> T&
  {
    return detail::any_cast<T>(property);
  }

  template <class T>
  auto any_cast(any_table_feature_property const& property)
    -> T const&
  {
    return detail::any_cast<T>(property);
  }

  template <class T>
  auto any_cast(any_table_feature_property* const property)
    -> T*
  {
    return detail::any_cast<T>(property);
  }

  template <class T>
  auto any_cast(any_table_feature_property const* const property)
    -> T const*
  {
    return detail::any_cast<T>(property);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)

#include <canard/network/openflow/v13/impl/any_table_feature_property.ipp>

#elif defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  extern template class any_type<ofp::v13::table_feature_property_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif

#endif // CANARD_NET_OFP_V13_ANY_TABLE_FEATURE_PROPERTY_HPP
