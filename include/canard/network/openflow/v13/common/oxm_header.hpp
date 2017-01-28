#ifndef CANARD_NET_OFP_V13_OXM_HEADER_HPP
#define CANARD_NET_OFP_V13_OXM_HEADER_HPP

#include <cstdint>
#include <boost/endian/conversion.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class oxm_header
    : public detail::basic_protocol_type<oxm_header>
  {
  public:
    using raw_ofp_type = std::uint32_t;

    constexpr explicit oxm_header(std::uint32_t const oxm_header) noexcept
      : oxm_header_(oxm_header)
    {
    }

    constexpr auto oxm_class() const noexcept
      -> std::uint16_t
    {
      return oxm_header_ >> 16;
    }

    constexpr auto oxm_field() const noexcept
      -> std::uint8_t
    {
      return (oxm_header_ >> 9) & 0x7f;
    }

    constexpr auto oxm_hasmask() const noexcept
      -> bool
    {
      return oxm_header_ & 0x00000100;
    }

    constexpr auto oxm_length() const noexcept
      -> std::uint8_t
    {
      return oxm_header_ & 0x000000ff;
    }

    constexpr auto to_ofp_type() const noexcept
      -> raw_ofp_type
    {
      return oxm_header_;
    }

    constexpr static auto length() noexcept
      -> std::uint16_t
    {
      return sizeof(raw_ofp_type);
    }

  private:
    friend basic_protocol_type;

    template <class Validator>
    void validate_impl(Validator&&) const
    {
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, oxm_header_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> oxm_header
    {
      return oxm_header{detail::decode<std::uint32_t>(first, last)};
    }

    auto equal_impl(oxm_header const& rhs) const noexcept
      -> bool
    {
      return oxm_header_ == rhs.oxm_header_;
    }

    auto equivalent_impl(oxm_header const& rhs) const noexcept
      -> bool
    {
      return oxm_header_ == rhs.oxm_header_;
    }

  private:
    std::uint32_t oxm_header_;
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OXM_HEADER_HPP
