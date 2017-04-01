#ifndef CANARD_NET_OFP_DETAIL_COPY_SIZE_HPP
#define CANARD_NET_OFP_DETAIL_COPY_SIZE_HPP

#include <cstddef>
#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <std::size_t N>
  using copy_size = std::integral_constant<std::size_t, N>;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_COPY_SIZE_HPP
