#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP

#include <canard/net/ofp/detail/basic_queue_property.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace queue_properties {
namespace queue_property_detail {

  template <class T>
  using basic_queue_property = ofp::detail::basic_queue_property<
      T
    , ofp::v13::protocol::ofp_queue_prop_header
    , ofp::v13::protocol::ofp_queue_properties
  >;

} // namespace queue_property_detail
} // namespace queue_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP
