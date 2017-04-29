#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_METER_BAND_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_METER_BAND_HPP

#include <cstdint>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class MeterBand>
  class basic_meter_band
    : public detail::basic_protocol_type<MeterBand>
  {
    using base_t = detail::basic_protocol_type<MeterBand>;

  public:
    using ofp_header_type = ofp::v13::protocol::ofp_meter_band_header;

    static constexpr auto type() noexcept
      -> std::uint16_t
    {
      return MeterBand::band_type;
    }

    static constexpr auto length() noexcept
      -> std::uint16_t
    {
      return sizeof(typename MeterBand::ofp_type);
    }

    auto rate() const noexcept
      -> std::uint32_t
    {
      return derived().ofp_meter_band().rate;
    }

    auto burst_size() const noexcept
      -> std::uint32_t
    {
      return derived().ofp_meter_band().burst_size;
    }

    static auto validate_header(ofp_header_type const& header) noexcept
      -> char const*
    {
      if (header.type != type()) {
        return "invalid meter band type";
      }
      if (!is_valid_meter_band_length(header)) {
        return "invalid meter band length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_meter_band_length(
            ofp_header_type const& header) noexcept
        -> bool
    {
        return header.len == length();
    }

  protected:
    basic_meter_band() = default;

  private:
    auto derived() const noexcept
      -> MeterBand const&
    {
      return *static_cast<MeterBand const*>(this);
    }

    friend base_t;

    auto equal_impl(MeterBand const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(derived().ofp_meter_band(), rhs.ofp_meter_band());
    }

    auto equivalent_impl(MeterBand const& rhs) const noexcept
      -> bool
    {
      return derived().is_equivalent_meter_band(rhs);
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, derived().ofp_meter_band());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> MeterBand
    {
      return MeterBand{
        detail::decode<typename MeterBand::ofp_type>(first, last)
      };
    }
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_METER_BAND_HPP
