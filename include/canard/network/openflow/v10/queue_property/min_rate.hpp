#ifndef CANARD_NET_OFP_V10_QUEUE_PROPERTIES_MIN_RATE_HPP
#define CANARD_NET_OFP_V10_QUEUE_PROPERTIES_MIN_RATE_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace queue_properties {

    class min_rate
        : public detail::basic_protocol_type<min_rate>
    {
    public:
        using raw_ofp_type = protocol::ofp_queue_prop_min_rate;

        static constexpr protocol::ofp_queue_properties queue_property
            = protocol::OFPQT_MIN_RATE;

        explicit min_rate(std::uint16_t const rate) noexcept
            : min_rate_{
                  protocol::ofp_queue_prop_header{
                      queue_property
                    , sizeof(raw_ofp_type)
                    , { 0, 0, 0, 0 }
                  }
                , rate
                , { 0, 0, 0, 0, 0, 0 }
              }
        {
        }

        static constexpr auto property() noexcept
            -> protocol::ofp_queue_properties
        {
            return queue_property;
        }

        static constexpr auto type() noexcept
            -> protocol::ofp_queue_properties
        {
            return property();
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
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

        static void validate_header(
                protocol::ofp_queue_prop_header const& prop_header)
        {
            if (prop_header.property != queue_property) {
                throw std::runtime_error{"invalid queue property"};
            }
            if (prop_header.len != sizeof(raw_ofp_type)) {
                throw std::runtime_error{"invalid queue property length"};
            }
        }

    private:
        friend basic_protocol_type;

        explicit min_rate(raw_ofp_type const& min_rate) noexcept
            : min_rate_(min_rate)
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, min_rate_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> min_rate
        {
            return min_rate{detail::decode<raw_ofp_type>(first, last)};
        }

        template <class Validator>
        void validate_impl(Validator) const
        {
        }

        auto equal_impl(min_rate const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(min_rate_, rhs.min_rate_);
        }

        auto equivalent_impl(min_rate const& rhs) const noexcept
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
