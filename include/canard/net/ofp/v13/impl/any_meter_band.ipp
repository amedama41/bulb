#ifndef CANARD_NET_OFP_V13_IMPL_ANY_METER_BAND_IPP
#define CANARD_NET_OFP_V13_IMPL_ANY_METER_BAND_IPP

#include <canard/net/ofp/detail/config.hpp>
#include <canard/net/ofp/v13/any_meter_band.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

#include <canard/net/ofp/detail/impl/any_type.hpp>

namespace canard {
namespace net {
namespace ofp {

  template class detail::any_type<
    v13::meter_band_decoder, v13::any_meter_band_detail::base
  >;
  template class v13::any_meter_band_detail::base<
    detail::any_type<v13::meter_band_decoder, v13::any_meter_band_detail::base>
  >;

} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_IMPL_ANY_METER_BAND_IPP
