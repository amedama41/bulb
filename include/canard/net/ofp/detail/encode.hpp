#ifndef CANARD_NET_OFP_DETAIL_ENCODE_HPP
#define CANARD_NET_OFP_DETAIL_ENCODE_HPP

#include <cstddef>
#include <cstring>
#include <memory>
#include <type_traits>
#include <boost/endian/conversion.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/iterator_range.hpp>
#include <canard/net/ofp/detail/copy_size.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Buffer>
  auto encode_byte_array(
      Buffer& buffer, unsigned char const* const first, std::size_t const size)
    -> Buffer&
  {
    return boost::push_back(buffer, boost::make_iterator_range_n(first, size));
  }

  template <
      class T, class Buffer
    , std::size_t CopySize = sizeof(T)
    , class = typename std::enable_if<std::is_trivially_copyable<T>::value>::type
  >
  auto encode(Buffer& buffer, T value, detail::copy_size<CopySize> = {})
    -> Buffer&
  {
    boost::endian::native_to_big_inplace(value);
    unsigned char bytes[CopySize];
    std::memcpy(bytes, std::addressof(value), CopySize);
    return detail::encode_byte_array(buffer, bytes, CopySize);
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_ENCODE_HPP
