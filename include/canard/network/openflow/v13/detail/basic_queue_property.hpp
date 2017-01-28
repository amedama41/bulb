#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP

#include <cstdint>
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

    template <class T>
    class basic_queue_property
        : public detail::basic_protocol_type<T>
    {
    protected:
        basic_queue_property() = default;

    public:
        using ofp_header_type = protocol::ofp_queue_prop_header;

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
            return sizeof(typename T::raw_ofp_type);
        }

        static auto validate_header(ofp_header_type const& prop_header) noexcept
            -> char const*
        {
            if (prop_header.property != property()) {
                return "invalid queue property";
            }
            if (prop_header.len != length()) {
                return "invalid queue property length";
            }
            return nullptr;
        }

    private:
        auto derived() const noexcept
            -> T const&
        {
            return *static_cast<T const*>(this);
        }

        friend detail::basic_protocol_type<T>;

        template <class Validator>
        void validate_impl(Validator) const
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, derived().ofp_queue_property());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> T
        {
            return T{detail::decode<typename T::raw_ofp_type>(first, last)};
        }

        auto equal_impl(T const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(
                      derived().ofp_queue_property()
                    , rhs.derived().ofp_queue_property());
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
