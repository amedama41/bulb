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
#include <canard/network/openflow/detail/copy_size.hpp>

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
  auto decode(Iterator& first, Iterator last, detail::copy_size<CopySize> = {})
    -> T
  {
    auto const bytes = decode_byte_array<CopySize>(first, last);
    auto value = T{};
    std::memcpy(std::addressof(value), bytes.data(), bytes.size());
    boost::endian::big_to_native_inplace(value);
    return value;
  }

  template <
      class T, class Iterator
    , std::size_t CopySize = sizeof(T)
    , class = typename std::enable_if<std::is_trivially_copyable<T>::value>::type
  >
  auto decode_without_consumption(
      Iterator first, Iterator last, detail::copy_size<CopySize> copy_size = {})
    -> T
  {
    return detail::decode<T>(first, last, copy_size);
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_DECODE_HPP
