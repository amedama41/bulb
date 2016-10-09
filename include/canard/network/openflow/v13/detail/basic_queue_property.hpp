#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace queue_properties {
namespace queue_property_detail {

    template <class T, class OFPQueueProperty>
    class basic_queue_property
        : public detail::basic_protocol_type<T>
    {
    protected:
        basic_queue_property() = default;

    public:
        using raw_ofp_type = OFPQueueProperty;

        static constexpr auto property() noexcept
            -> protocol::ofp_queue_properties
        {
            return T::queue_property;
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

        static void validate_header(
                v13_detail::ofp_queue_prop_header const& prop_header)
        {
            if (prop_header.property != property()) {
                throw std::runtime_error{"invalid queue property"};
            }
            if (prop_header.len != length()) {
                throw std::runtime_error{"invalid queue property length"};
            }
        }

    private:
        auto base_property() const noexcept
            -> raw_ofp_type const&
        {
            return static_cast<T const*>(this)->ofp_queue_property();
        }

        friend detail::basic_protocol_type<T>;

        template <class Validator>
        void validate_impl(Validator) const
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, base_property());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> T
        {
            return T{detail::decode<raw_ofp_type>(first, last)};
        }

        auto equal_impl(T const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(base_property(), rhs.base_property());
        }

        auto equivalent_impl(T const& rhs) const noexcept
            -> bool
        {
            return static_cast<T const*>(this)->is_equivalent_property(rhs);
        }
    };

} // namespace queue_property_detail
} // namespace queue_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP
