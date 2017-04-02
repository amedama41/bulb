#ifndef CANARD_NET_OFP_V10_QUEUE_PROPERTIES_MIN_RATE_HPP
#define CANARD_NET_OFP_V10_QUEUE_PROPERTIES_MIN_RATE_HPP

#include <cstdint>
#include <canard/net/ofp/v10/detail/basic_queue_property.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace queue_properties {

  class min_rate
    : public detail::v10::basic_queue_property<min_rate>
  {
  public:
    using raw_ofp_type = protocol::ofp_queue_prop_min_rate;

    static constexpr protocol::ofp_queue_properties queue_property
      = protocol::OFPQT_MIN_RATE;

    explicit min_rate(std::uint16_t const rate) noexcept
      : min_rate_{
          {queue_property, sizeof(raw_ofp_type), { 0, 0, 0, 0 }}
        , rate
        , { 0, 0, 0, 0, 0, 0 }
      }
    {
    }

    auto rate() const noexcept
      -> std::uint16_t
    {
      return min_rate_.rate;
    }

    auto is_disabled() const noexcept
      -> bool
    {
      return rate() > 1000;
    }

  private:
    friend basic_queue_property;

    explicit min_rate(raw_ofp_type const& min_rate) noexcept
      : min_rate_(min_rate)
    {
    }

    auto ofp_queue_property() const noexcept
      -> raw_ofp_type const&
    {
      return min_rate_;
    }

    auto is_equivalent_property(min_rate const& rhs) const noexcept
      -> bool
    {
      return rate() == rhs.rate() || (is_disabled() && rhs.is_disabled());
    }

  private:
    raw_ofp_type min_rate_;
  };

} // namespace queue_properties
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_QUEUE_PROPERTIES_MIN_RATE_HPP
