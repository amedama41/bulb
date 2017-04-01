#ifndef CANARD_NET_OFP_V13_METER_BANDS_HPP
#define CANARD_NET_OFP_V13_METER_BANDS_HPP

#include <tuple>
#include <canard/network/openflow/v13/meter_band/dscp_remark.hpp>
#include <canard/network/openflow/v13/meter_band/drop.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  using default_meter_band_list = std::tuple<
      meter_bands::drop
    , meter_bands::dscp_remark
  >;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_METER_BANDS_HPP
