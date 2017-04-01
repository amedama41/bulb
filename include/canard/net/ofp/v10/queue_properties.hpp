#ifndef CANARD_NET_OFP_V10_QUEUE_PROPERTIES_HPP
#define CANARD_NET_OFP_V10_QUEUE_PROPERTIES_HPP

#include <tuple>
#include <canard/network/openflow/v10/queue_property/min_rate.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    using default_queue_property_list = std::tuple<
        queue_properties::min_rate
    >;

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_QUEUE_PROPERTIES_HPP
