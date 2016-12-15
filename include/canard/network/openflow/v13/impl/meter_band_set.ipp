#ifndef CANARD_NET_OFP_V13_IMPL_METER_BAND_SET_HPP
#define CANARD_NET_OFP_V13_IMPL_METER_BAND_SET_HPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/utility/meter_band_set.hpp>

#include <canard/network/openflow/detail/impl/set_base.hpp>

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  template class detail::set_base<
    v13::meter_band_set, std::uint64_t, v13::any_meter_band
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#endif

#endif // CANARD_NET_OFP_V13_IMPL_METER_BAND_SET_HPP