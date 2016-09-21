#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MAX_RATE_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MAX_RATE_HPP

#include <cstddef>
#include <cstdint>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/basic_queue_property.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace queue_properties {

    class max_rate
        : public queue_property_detail::basic_queue_property<
            max_rate, v13_detail::ofp_queue_prop_max_rate
          >
    {
    public:
        static constexpr protocol::ofp_queue_properties queue_property
            = protocol::OFPQT_MAX_RATE;

        static constexpr std::size_t base_size = sizeof(raw_ofp_type);

        explicit max_rate(std::uint16_t const rate) noexcept
            : max_rate_{
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
            return max_rate_.rate;
        }

        auto is_configurable() const noexcept
            -> bool
        {
            return rate() != protocol::OFPQ_MAX_RATE_UNCFG;
        }

        auto is_disabled() const noexcept
            -> bool
        {
            return rate() > 1000 && is_configurable();
        }

    private:
        friend basic_queue_property;

        explicit max_rate(raw_ofp_type const& max_rate) noexcept
            : max_rate_(max_rate)
        {
        }

        auto ofp_queue_property() const noexcept
            -> raw_ofp_type const&
        {
            return max_rate_;
        }

        auto is_equivalent_property(max_rate const& rhs) const noexcept
            -> bool
        {
            return rate() == rhs.rate()
                || (is_disabled() && rhs.is_disabled());
        }

    private:
        raw_ofp_type max_rate_;
    };

} // namespace queue_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MAX_RATE_HPP
