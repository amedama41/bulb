#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MIN_RATE_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MIN_RATE_HPP

#include <cstdint>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/basic_queue_property.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace queue_properties {

    class min_rate
        : public queue_property_detail::basic_queue_property<
            min_rate, v13_detail::ofp_queue_prop_min_rate
          >
    {
    public:
        static constexpr protocol::ofp_queue_properties queue_property
            = protocol::OFPQT_MIN_RATE;

        explicit min_rate(std::uint16_t const rate) noexcept
            : min_rate_{
                  v13_detail::ofp_queue_prop_header{
                      queue_property
                    , sizeof(raw_ofp_type)
                    , { 0, 0, 0, 0 }
                  }
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

        auto is_configurable() const noexcept
            -> bool
        {
            return rate() != protocol::OFPQ_MIN_RATE_UNCFG;
        }

        auto is_disabled() const noexcept
            -> bool
        {
            return rate() > 1000 && is_configurable();
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
            return rate() == rhs.rate()
                || (is_disabled() && rhs.is_disabled());
        }

    private:
        raw_ofp_type min_rate_;
    };

} // namespace queue_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MIN_RATE_HPP
