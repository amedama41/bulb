#ifndef CANARD_NET_OFP_DETAIL_MEMCMP_HPP
#define CANARD_NET_OFP_DETAIL_MEMCMP_HPP

#include <cstring>
#include <memory>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

    template <class T>
    auto memcmp(T const& lhs, T const& rhs) noexcept
        -> bool
    {
        return std::memcmp(
                std::addressof(lhs), std::addressof(rhs), sizeof(T)) == 0;
    }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_MEMCMP_HPP
