#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_MEMCMP_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_MEMCMP_HPP

#include <cstring>
#include <memory>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    template <class T>
    auto memcmp(T const& lhs, T const& rhs) noexcept
        -> bool
    {
        return std::memcmp(
                std::addressof(lhs), std::addressof(rhs), sizeof(T)) == 0;
    }

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_MEMCMP_HPP
