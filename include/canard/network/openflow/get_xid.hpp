#ifndef CANARD_NET_OFP_GET_XID_HPP
#define CANARD_NET_OFP_GET_XID_HPP

#include <cstdint>
#include <atomic>

namespace canard {
namespace net {
namespace ofp {

    inline auto get_xid()
        -> std::uint32_t
    {
        static std::atomic<std::uint32_t> xid{0};
        return xid.fetch_add(1, std::memory_order_relaxed);
    }

} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_GET_XID_HPP
