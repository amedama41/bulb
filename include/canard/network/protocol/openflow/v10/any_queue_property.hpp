#ifndef CANARD_NET_OFP_V10_ANY_QUEUE_PROPEPRTY_HPP
#define CANARD_NET_OFP_V10_ANY_QUEUE_PROPEPRTY_HPP

#include <canard/network/protocol/openflow/detail/any_queue_property.hpp>
#include <canard/network/protocol/openflow/v10/decoder/queue_property_decoder.hpp>
#include <canard/network/protocol/openflow/v10/queue_properties.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    using any_queue_property = detail::any_queue_property<
        default_queue_property_list, queue_property_decoder
    >;

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ANY_QUEUE_PROPEPRTY_HPP
