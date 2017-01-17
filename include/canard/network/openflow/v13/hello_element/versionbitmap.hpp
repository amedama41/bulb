#ifndef CANARD_NET_OFP_V13_HELLO_ELEMENTS_VERSIONBITMAP_HPP
#define CANARD_NET_OFP_V13_HELLO_ELEMENTS_VERSIONBITMAP_HPP

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/mismatch.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace hello_elements {

  class versionbitmap
    : public detail::basic_protocol_type<versionbitmap>
  {
  public:
    using raw_ofp_type = protocol::ofp_hello_elem_versionbitmap;
    using bitmaps_type = std::vector<std::uint32_t>;

  private:
    using bitmap_type = bitmaps_type::value_type;
    static constexpr std::size_t bitmap_bits
      = std::numeric_limits<bitmap_type>::digits;

  public:
    static constexpr protocol::ofp_hello_elem_type hello_element_type
      = protocol::OFPHET_VERSIONBITMAP;

    explicit versionbitmap(bitmaps_type bitmaps)
      : versionbitmap_{
            hello_element_type
          , calc_length(bitmaps)
        }
      , bitmaps_{std::move(bitmaps)}
    {
    }

    versionbitmap(versionbitmap const&) = default;

    versionbitmap(versionbitmap&& other)
      : versionbitmap_(other.versionbitmap_)
      , bitmaps_(other.extract_bitmaps())
    {
    }

    auto operator=(versionbitmap const& other)
      -> versionbitmap&
    {
      return operator=(versionbitmap{other});
    }

    auto operator=(versionbitmap&& other)
      -> versionbitmap&
    {
      auto tmp = std::move(other);
      std::swap(versionbitmap_, tmp.versionbitmap_);
      bitmaps_.swap(tmp.bitmaps_);
      return *this;
    }

    static constexpr auto type() noexcept
      -> protocol::ofp_hello_elem_type
    {
      return hello_element_type;
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return versionbitmap_.length;
    }

    auto bitmaps() const noexcept
      -> bitmaps_type const&
    {
      return bitmaps_;
    }

    auto extract_bitmaps()
      -> bitmaps_type
    {
      auto bitmaps = bitmaps_type{};
      bitmaps.swap(bitmaps_);
      versionbitmap_.length = sizeof(raw_ofp_type);
      return bitmaps;
    }

    auto support(std::uint8_t const version) const noexcept
      -> bool
    {
      auto const index = std::size_t(version / bitmap_bits);
      auto const shift = std::size_t(version % bitmap_bits);
      if (bitmaps_.size() <= index) {
        return false;
      }
      return (bitmap_type{1} << shift) & bitmaps_[index];
    }

    auto max_support_version() const
      -> std::uint8_t
    {
      auto const rbitmaps = bitmaps_ | boost::adaptors::reversed;
      auto const rit
        = boost::find_if(rbitmaps, [](bitmap_type b) { return b != 0; });
      if (rit == rbitmaps.end()) {
        throw std::runtime_error{"no valid bitmaps"};
      }

      auto const it = std::prev(rit.base());
      auto bitmap = *it;
      auto shift = std::size_t{0};
      while (bitmap != 1) {
        bitmap >>= 1;
        ++shift;
      }
      return shift + std::distance(bitmaps_.begin(), it) * bitmap_bits;
    }

    static void validate_header(protocol::ofp_hello_elem_header const& header)
    {
      if (header.type != type()) {
        throw std::runtime_error{"type is not versionbitmap"};
      }
      if (header.length < min_length()) {
        throw std::runtime_error{"versionbitmap length is too small"};
      }
      if ((header.length - min_length()) % sizeof(bitmap_type) != 0) {
        throw std::runtime_error{"versionbitmap length is invalid"};
      }
    }

  private:
    versionbitmap(raw_ofp_type const& versionbitmap, bitmaps_type&& bitmaps)
      : versionbitmap_(versionbitmap)
      , bitmaps_(std::move(bitmaps))
    {
    }

    friend basic_protocol_type;

    friend constexpr auto exclude_padding(versionbitmap*) noexcept
      -> bool
    {
      return true;
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, versionbitmap_);
      boost::for_each(bitmaps_, [&](bitmap_type bitmap) {
          detail::encode(container, bitmap);
      });
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> versionbitmap
    {
      auto const vbitmap = detail::decode<raw_ofp_type>(first, last);

      auto const bitmaps_length = vbitmap.length - sizeof(raw_ofp_type);
      auto bitmaps = bitmaps_type(bitmaps_length / sizeof(bitmap_type));
      std::copy_n(
            first, bitmaps_length
          , reinterpret_cast<unsigned char*>(&bitmaps[0]));
      std::advance(first, bitmaps_length);
      boost::for_each(bitmaps, [](bitmap_type& bitmap) {
          boost::endian::big_to_native_inplace(bitmap);
      });

      return versionbitmap{vbitmap, std::move(bitmaps)};
    }

    template <class Validator>
    void validate_impl(Validator) const
    {
      if (bitmaps_.empty()) {
        throw std::runtime_error{"bitmaps is never empty"};
      }
      if (boost::algorithm::all_of(
            bitmaps_, [](bitmap_type b) { return b == 0; })) {
        throw std::runtime_error{
          "bitmaps must include at least one non zero bitmap"
        };
      }
    }

    auto equal_impl(versionbitmap const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(versionbitmap_, rhs.versionbitmap_)
          && bitmaps_ == rhs.bitmaps_;
    }

    auto equivalent_impl(versionbitmap const& rhs) const noexcept
      -> bool
    {
      using bt = versionbitmap::bitmaps_type;
      auto const compare = [](bt const& larger, bt const& smaller) -> bool {
        auto const result = boost::mismatch(smaller, larger);
        if (result.first != smaller.end()) {
          return false;
        }
        return std::all_of(
              result.second, larger.end()
            , [](bt::value_type const b) { return b == 0; });
      };

      return (bitmaps_.size() >= rhs.bitmaps_.size())
        ? compare(bitmaps_, rhs.bitmaps_)
        : compare(rhs.bitmaps_, bitmaps_);
    }

    static auto calc_length(bitmaps_type const& bitmaps)
      -> std::uint16_t
    {
      constexpr auto max_bitmaps_size
        = (std::numeric_limits<std::uint16_t>::max() - sizeof(raw_ofp_type))
        / sizeof(bitmap_type);
      if (bitmaps.size() > max_bitmaps_size) {
        throw std::runtime_error{"invalid bitmaps size"};
      }
      return sizeof(raw_ofp_type) + bitmaps.size() * sizeof(bitmap_type);
    }

  private:
    raw_ofp_type versionbitmap_;
    bitmaps_type bitmaps_;
  };

} // namespace hello_elements
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_HELLO_ELEMENTS_VERSIONBITMAP_HPP
