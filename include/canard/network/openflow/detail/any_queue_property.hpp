#ifndef CANARD_NET_OFP_DETAIL_ANY_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_DETAIL_ANY_QUEUE_PROPERTY_HPP

#include <cstddef>
#include <cstdint>
#include <boost/mpl/size.hpp>
#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class QueuePropertyDecoder>
  class any_queue_property
    : public any_type<any_queue_property<QueuePropertyDecoder>>
  {
    using base_t = any_type<any_queue_property<QueuePropertyDecoder>>;

  public:
    using base_t::base_t;

    auto property() const noexcept
      -> std::uint16_t
    {
      return base_t::visit(detail::property_visitor{});
    }
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_ANY_QUEUE_PROPERTY_HPP
