#ifndef CANARD_NET_OFP_V13_HELLO_ELEMENTS_VERSIONBITMAP_HPP
#define CANARD_NET_OFP_V13_HELLO_ELEMENTS_VERSIONBITMAP_HPP

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>
#include <boost/endian/conversion.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/padding.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace hello_elements {

  class versionbitmap
  {
  public:
    static protocol::ofp_hello_elem_type const hello_element_type
      = protocol::OFPHET_VERSIONBITMAP;

    explicit versionbitmap(std::vector<std::uint32_t> bitmaps)
      : versionbitmap_{
            hello_element_type
          , std::uint16_t(
              sizeof(versionbitmap_) + bitmaps.size() * sizeof(bitmaps[0]))
        }
      , bitmaps_{std::move(bitmaps)}
    {
      using boost::adaptors::reversed;
      auto const it = boost::find_if(
            bitmaps_ | reversed
          , [](std::uint32_t const bitmap) { return bitmap != 0; });
      bitmaps_.erase(it.base(), bitmaps_.end());
      if (bitmaps_.empty()) {
        throw std::runtime_error{"versionbitmap nust not be empty"};
      }
    }

    auto type() const
      -> protocol::ofp_hello_elem_type
    {
      return protocol::ofp_hello_elem_type(versionbitmap_.type);
    }

    auto length() const
      -> std::uint16_t
    {
      return versionbitmap_.length;
    }

    auto support(std::uint8_t const version) const
      -> bool
    {
      auto const bitmap_bits = std::size_t(32);
      auto const array_index = std::size_t(version / bitmap_bits);
      auto const bit_index = std::size_t(version % bitmap_bits);
      if (bitmaps_.size() < array_index) {
        return false;
      }
      return (std::uint32_t(1) << bit_index) & bitmaps_[array_index];
    }

    auto max_support_version() const
      -> std::uint8_t
    {
      auto bit_index = std::size_t{0};
      auto bitmap = bitmaps_.back();
      while (bitmap != 0) {
        bitmap >>= 1;
        ++bit_index;
      }
      return bit_index - 1 + (bitmaps_.size() - 1) * 32;
    }

    template <class Container>
    auto encode(Container& container) const
      -> Container&
    {
      detail::encode(container, versionbitmap_);
      boost::for_each(bitmaps_, [&](std::uint32_t const bitmap) {
          detail::encode(container, bitmap);
      });
      return detail::encode_byte_array(
          container, detail::padding, v13_detail::padding_length(length()));
    }

  private:
    versionbitmap(
          v13_detail::ofp_hello_elem_versionbitmap const& versionbitmap
        , std::vector<std::uint32_t> bitmaps)
      : versionbitmap_(versionbitmap)
      , bitmaps_(std::move(bitmaps))
    {
      if (versionbitmap_.type != hello_element_type) {
        throw 1;
      }
      if (versionbitmap_.length
          != sizeof(versionbitmap_) + bitmaps_.size() * sizeof(std::uint32_t)) {
        throw 1;
      }
    }

  public:
    template <class Iterator>
    static auto decode(Iterator& first, Iterator last)
      -> versionbitmap
    {
      auto const vbitmap
        = detail::decode<v13_detail::ofp_hello_elem_versionbitmap>(first, last);

      auto bitmaps = std::vector<std::uint32_t>(
          (vbitmap.length - sizeof(vbitmap)) / sizeof(std::uint32_t));
      std::copy_n(
            first, bitmaps.size() * sizeof(std::uint32_t)
          , reinterpret_cast<unsigned char*>(&bitmaps[0]));
      std::advance(first, bitmaps.size() * sizeof(std::uint32_t));
      boost::for_each(bitmaps, [](std::uint32_t& bitmap) {
          boost::endian::big_to_native_inplace(bitmap);
      });

      std::advance(first, v13_detail::padding_length(vbitmap.length));

      return versionbitmap{vbitmap, std::move(bitmaps)};
    }

  private:
    v13_detail::ofp_hello_elem_versionbitmap versionbitmap_;
    std::vector<std::uint32_t> bitmaps_;
  };

} // namespace hello_elements
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_HELLO_ELEMENTS_VERSIONBITMAP_HPP
