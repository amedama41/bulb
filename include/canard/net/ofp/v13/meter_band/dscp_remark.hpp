#ifndef CANARD_NET_OFP_V13_METER_BANDS_DSCP_REMARK_HPP
#define CANARD_NET_OFP_V13_METER_BANDS_DSCP_REMARK_HPP

#include <cstdint>
#include <canard/net/ofp/v13/detail/basic_meter_band.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace meter_bands {

  class dscp_remark
    : public detail::v13::basic_meter_band<dscp_remark>
  {
    static constexpr protocol::ofp_meter_band_type band_type
      = protocol::OFPMBT_DSCP_REMARK;

  public:
    using ofp_type = protocol::ofp_meter_band_dscp_remark;

    dscp_remark(
          std::uint8_t const prec_level
        , std::uint32_t const rate, std::uint32_t burst_size) noexcept
      : dscp_remark_{
            type()
          , length()
          , rate
          , burst_size
          , prec_level
          , { 0, 0, 0 }
        }
    {
    }

    dscp_remark(
        std::uint8_t const prec_level, std::uint32_t const rate) noexcept
      : dscp_remark{prec_level, rate, 0}
    {
    }

    auto precedence_level() const noexcept
      -> std::uint8_t
    {
      return dscp_remark_.prec_level;
    }

  private:
    friend basic_meter_band;

    explicit dscp_remark(ofp_type const& dscp_remark) noexcept
      : dscp_remark_(dscp_remark)
    {
    }

    auto ofp_meter_band() const noexcept
      -> ofp_type const&
    {
      return dscp_remark_;
    }

    auto is_equivalent_meter_band(dscp_remark const& rhs) const noexcept
      -> bool
    {
      return rate() == rhs.rate()
          && burst_size() == rhs.burst_size()
          && precedence_level() == rhs.precedence_level();
    }

  private:
    ofp_type dscp_remark_;
  };

} // namespace meter_bands
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_METER_BANDS_DSCP_REMARK_HPP
