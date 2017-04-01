#ifndef CANARD_NET_OFP_V13_IMPL_TABLE_FEATURE_PROPERTY_SET_IPP
#define CANARD_NET_OFP_V13_IMPL_TABLE_FEATURE_PROPERTY_SET_IPP

#include <canard/net/ofp/detail/config.hpp>
#include <canard/net/ofp/v13/utility/table_feature_property_set.hpp>

#include <canard/net/ofp/detail/impl/set_base.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  template class detail::set_base<
      v13::table_feature_property_set
    , std::uint16_t
    , v13::any_table_feature_property
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#endif

#endif // CANARD_NET_OFP_V13_IMPL_TABLE_FEATURE_PROPERTY_SET_IPP
