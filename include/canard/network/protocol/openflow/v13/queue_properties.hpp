#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTIES_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTIES_HPP

#include <tuple>
#include <canard/network/protocol/openflow/v13/queue_property/max_rate.hpp>
#include <canard/network/protocol/openflow/v13/queue_property/min_rate.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    using default_queue_property_list = std::tuple<
          queue_properties::min_rate
        , queue_properties::max_rate
    >;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_QUEUE_PROPERTIES_HPP
