#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP

#include <cstdint>
#include <boost/operators.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace queue_properties {
namespace queue_property_detail {

    template <class T>
    class basic_queue_property
        : private boost::equality_comparable<T>
    {
    public:
        static constexpr auto property() noexcept
            -> protocol::ofp_queue_properties
        {
            return T::queue_property;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return static_cast<T const*>(this)->queue_property_header().len;
        }
    };

} // namespace queue_property_detail
} // namespace queue_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_QUEUE_PROPERTIES_BAISC_QUEUE_PROPERTY_HPP