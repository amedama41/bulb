#ifndef CANARD_NET_OFP_V13_IMPL_ANY_TABLE_FEATURE_PROPERTY_IPP
#define CANARD_NET_OFP_V13_IMPL_ANY_TABLE_FEATURE_PROPERTY_IPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/any_table_feature_property.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#  include <canard/network/openflow/detail/impl/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template class any_type<ofp::v13::table_feature_property_decoder>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_IMPL_ANY_TABLE_FEATURE_PROPERTY_IPP
