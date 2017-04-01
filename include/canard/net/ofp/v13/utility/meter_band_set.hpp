#ifndef CANARD_NET_OFP_V13_METER_BAND_SET_HPP
#define CANARD_NET_OFP_V13_METER_BAND_SET_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/set_base.hpp>
#include <canard/network/openflow/v13/any_meter_band.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class meter_band_set
    : public detail::set_base<meter_band_set, std::uint64_t, any_meter_band>
  {
  public:
    using set_base::set_base;

    static auto calc_key(
        std::uint32_t const rate, std::uint32_t const burst_size) noexcept
      -> key_type
    {
      return (std::uint64_t{rate} << 32) | burst_size;
    }

  private:
    friend set_base;

    using set_info = set_base::default_set_info;

    template <class MeterBand>
    static auto get_key_impl(MeterBand const& meter_band) noexcept
      -> key_type
    {
      return calc_key(meter_band.rate(), meter_band.burst_size());
    }
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  extern template class detail::set_base<
    v13::meter_band_set, std::uint64_t, v13::any_meter_band
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#else
# include <canard/network/openflow/v13/impl/meter_band_set.ipp>
#endif

#endif // CANARD_NET_OFP_V13_METER_BAND_SET_HPP
