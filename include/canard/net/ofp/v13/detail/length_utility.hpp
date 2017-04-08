#ifndef CANARD_NET_OFP_V13_DETAIL_LENGTH_UTILITY_HPP
#define CANARD_NET_OFP_V13_DETAIL_LENGTH_UTILITY_HPP

#include <cstdint>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  constexpr inline auto exact_length(std::uint16_t const length) noexcept
    -> std::uint16_t
  {
    return (length + 7) / 8 * 8;
  }

  constexpr inline auto padding_length(std::uint16_t const length) noexcept
    -> std::uint16_t
  {
    return exact_length(length) - length;
  }

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_DETAIL_LENGTH_UTILITY_HPP
