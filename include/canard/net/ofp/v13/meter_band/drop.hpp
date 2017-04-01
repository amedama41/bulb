#ifndef CANARD_NET_OFP_V13_METER_BANDS_DROP_HPP
#define CANARD_NET_OFP_V13_METER_BANDS_DROP_HPP

#include <cstdint>
#include <canard/network/openflow/v13/detail/basic_meter_band.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace meter_bands {

  class drop
    : public detail::v13::basic_meter_band<drop>
  {
    static constexpr protocol::ofp_meter_band_type band_type
      = protocol::OFPMBT_DROP;

  public:
    using raw_ofp_type = protocol::ofp_meter_band_drop;

    drop(std::uint32_t const rate, std::uint32_t const burst_size) noexcept
      : drop_{
            type()
          , length()
          , rate
          , burst_size
          , { 0, 0, 0, 0 }
        }
    {
    }

    explicit drop(std::uint32_t const rate) noexcept
      : drop{rate, 0}
    {
    }

  private:
    friend basic_meter_band;

    explicit drop(raw_ofp_type const& drop) noexcept
      : drop_(drop)
    {
    }

    auto ofp_meter_band() const noexcept
      -> raw_ofp_type const&
    {
      return drop_;
    }

    auto is_equivalent_meter_band(drop const& rhs) const noexcept
      -> bool
    {
      return rate() == rhs.rate()
          && burst_size() == rhs.burst_size();
    }

  private:
    raw_ofp_type drop_;
  };

} // namespace meter_bands
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_METER_BANDS_DROP_HPP
