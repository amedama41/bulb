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
#include <boost/operators.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/mismatch.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
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
    : private boost::equality_comparable<versionbitmap>
  {
  public:
    using raw_ofp_type = v13_detail::ofp_hello_elem_versionbitmap;
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

    template <class Container>
    auto encode(Container& container) const
      -> Container&
    {
      detail::encode(container, versionbitmap_);
      boost::for_each(bitmaps_, [&](bitmap_type bitmap) {
          detail::encode(container, bitmap);
      });
      return detail::encode_byte_array(
          container, detail::padding, v13_detail::padding_length(length()));
    }

    template <class Iterator>
    static auto decode(Iterator& first, Iterator last)
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

      std::advance(first, v13_detail::padding_length(vbitmap.length));

      return versionbitmap{vbitmap, std::move(bitmaps)};
    }

    template <class Validator>
    void validate(Validator) const
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

    static void validate_header(v13_detail::ofp_hello_elem_header const& header)
    {
      if (header.type != hello_element_type) {
        throw std::runtime_error{"type is not versionbitmap"};
      }
      if (header.length < sizeof(raw_ofp_type)) {
        throw std::runtime_error{"versionbitmap length is too small"};
      }
      if ((header.length - sizeof(raw_ofp_type)) % sizeof(bitmap_type) != 0) {
        throw std::runtime_error{"versionbitmap length is invalid"};
      }
    }

    friend auto operator==(versionbitmap const&, versionbitmap const&) noexcept
      -> bool;

  private:
    versionbitmap(raw_ofp_type const& versionbitmap, bitmaps_type&& bitmaps)
      : versionbitmap_(versionbitmap)
      , bitmaps_(std::move(bitmaps))
    {
    }

    auto equal_impl(versionbitmap const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(versionbitmap_, rhs.versionbitmap_)
          && bitmaps_ == rhs.bitmaps_;
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

  inline auto operator==(
      versionbitmap const& lhs, versionbitmap const& rhs) noexcept
    -> bool
  {
    return lhs.equal_impl(rhs);
  }

  inline auto equivalent(
      versionbitmap const& lhs, versionbitmap const& rhs) noexcept
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

    auto const& lhs_bitmaps = lhs.bitmaps();
    auto const& rhs_bitmaps = rhs.bitmaps();
    return (lhs_bitmaps.size() >= rhs_bitmaps.size())
      ? compare(lhs_bitmaps, rhs_bitmaps)
      : compare(rhs_bitmaps, lhs_bitmaps);
  }

} // namespace hello_elements
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_HELLO_ELEMENTS_VERSIONBITMAP_HPP
