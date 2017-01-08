#ifndef CANARD_NET_OFP_DETAIL_DECODE_HPP
#define CANARD_NET_OFP_DETAIL_DECODE_HPP

#include <cstddef>
#include <cstring>
#include <algorithm>
#include <array>
#include <iterator>
#include <memory>
#include <type_traits>
#include <boost/endian/conversion.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <std::size_t N, class Iterator>
  auto decode_byte_array(Iterator& first, Iterator last)
    -> std::array<unsigned char, N>
  {
    std::array<unsigned char, N> bytes;
    last = std::next(first, N);
    std::copy(first, last, bytes.data());
    first = last;
    return bytes;
  }

  template <
      class T, class Iterator
    , std::size_t CopySize = sizeof(T)
    , class = typename std::enable_if<std::is_trivially_copyable<T>::value>::type
  >
  auto decode(Iterator& first, Iterator last
            , std::integral_constant<std::size_t, CopySize> = {})
    -> T
  {
    auto const bytes = decode_byte_array<CopySize>(first, last);
    auto value = T{};
    std::memcpy(std::addressof(value), bytes.data(), bytes.size());
    boost::endian::big_to_native_inplace(value);
    return value;
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_DECODE_HPP
