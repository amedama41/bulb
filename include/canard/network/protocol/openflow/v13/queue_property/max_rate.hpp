#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MAX_RATE_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MAX_RATE_HPP

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <canard/network/protocol/openflow/detail/decode.hpp>
#include <canard/network/protocol/openflow/detail/encode.hpp>
#include <canard/network/protocol/openflow/detail/memcmp.hpp>
#include <canard/network/protocol/openflow/v13/detail/byteorder.hpp>
#include <canard/network/protocol/openflow/v13/detail/basic_queue_property.hpp>
#include <canard/network/protocol/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace queue_properties {

    class max_rate
        : public queue_property_detail::basic_queue_property<max_rate>
    {
    public:
        static constexpr protocol::ofp_queue_properties queue_property
            = protocol::OFPQT_MAX_RATE;

        static constexpr std::size_t base_size
            = sizeof(v13_detail::ofp_queue_prop_max_rate);

        explicit max_rate(std::uint16_t const rate) noexcept
            : max_rate_{
                  v13_detail::ofp_queue_prop_header{
                      queue_property
                    , sizeof(v13_detail::ofp_queue_prop_max_rate)
                    , { 0, 0, 0, 0 }
                  }
                , rate
                , { 0, 0, 0, 0, 0, 0 }
              }
        {
        }

        auto queue_property_header() const noexcept
            -> v13_detail::ofp_queue_prop_header const&
        {
            return max_rate_.prop_header;
        }

        auto rate() const noexcept
            -> std::uint16_t
        {
            return max_rate_.rate;
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            return detail::encode(container, max_rate_);
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> max_rate
        {
            return max_rate{
                detail::decode<v13_detail::ofp_queue_prop_max_rate>(first, last)
            };
        }

        static void validate(
                v13_detail::ofp_queue_prop_header const& prop_header)
        {
            if (prop_header.property != queue_property) {
                throw std::runtime_error{"invalid queue property"};
            }
            if (prop_header.len != base_size) {
                throw std::runtime_error{"invalid queue property length"};
            }
        }

        friend auto operator==(
                max_rate const& lhs, max_rate const& rhs) noexcept
            -> bool
        {
            return detail::memcmp(lhs.max_rate_, rhs.max_rate_);
        }

    private:
        explicit max_rate(
                v13_detail::ofp_queue_prop_max_rate const& max_rate) noexcept
            : max_rate_(max_rate)
        {
        }

    private:
        v13_detail::ofp_queue_prop_max_rate max_rate_;
    };

    inline auto equivalent(max_rate const& lhs, max_rate const& rhs) noexcept
        -> bool
    {
        return lhs.rate() == rhs.rate();
    }

} // namespace queue_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_QUEUE_PROPERTIES_MAX_RATE_HPP
