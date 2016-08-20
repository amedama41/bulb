#ifndef CANARD_NET_OFP_V13_ANY_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_V13_ANY_QUEUE_PROPERTY_HPP

#include <canard/network/openflow/detail/any_queue_property.hpp>
#include <canard/network/openflow/v13/decoder/queue_property_decoder.hpp>
#include <canard/network/openflow/v13/queue_properties.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    using any_queue_property = detail::any_queue_property<
        default_queue_property_list, queue_property_decoder
    >;

    template <class T>
    auto any_cast(any_queue_property const& property)
        -> T const&
    {
        return detail::any_cast<T>(property);
    }

    template <class T>
    auto any_cast(any_queue_property const* const property)
        -> T const*
    {
        return detail::any_cast<T>(property);
    }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ANY_QUEUE_PROPERTY_HPP
