#ifndef CANARD_NET_OFP_DETAIL_V10_BASIC_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_DETAIL_V10_BASIC_QUEUE_PROPERTY_HPP

#include <canard/network/openflow/detail/basic_queue_property.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v10 {

  template <class T>
  using basic_queue_property = ofp::detail::basic_queue_property<
      T
    , ofp::v10::protocol::ofp_queue_prop_header
    , ofp::v10::protocol::ofp_queue_properties
  >;

} // namespace v10
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V10_BASIC_QUEUE_PROPERTY_HPP
