#ifndef CANARD_NET_OFP_V13_OXM_HEADER_HPP
#define CANARD_NET_OFP_V13_OXM_HEADER_HPP

#include <cstdint>
#include <boost/endian/conversion.hpp>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  namespace oxm_header_ops {

    constexpr auto oxm_header(
          std::uint16_t const oxm_class
        , std::uint8_t const oxm_field
        , bool const oxm_hasmask
        , std::uint8_t const oxm_length) noexcept
      -> std::uint32_t
    {
      return std::uint32_t(oxm_class) << 16
           | std::uint32_t(oxm_field) << 9
           | std::uint32_t(oxm_hasmask) << 8
           | std::uint32_t(oxm_length);
    }

    constexpr auto oxm_class(std::uint32_t const oxm_header) noexcept
      -> std::uint16_t
    {
      return oxm_header >> 16;
    }

    constexpr auto oxm_field(std::uint32_t const oxm_header) noexcept
      -> std::uint8_t
    {
      return (oxm_header >> 9) & 0x7f;
    }

    constexpr auto oxm_type(std::uint32_t const oxm_header) noexcept
      -> std::uint32_t
    {
      return oxm_header >> 9;
    }

    constexpr auto oxm_hasmask(std::uint32_t const oxm_header) noexcept
      -> bool
    {
      return oxm_header & 0x00000100;
    }

    constexpr auto oxm_length(std::uint32_t const oxm_header) noexcept
      -> std::uint8_t
    {
      return oxm_header & 0x000000ff;
    }

  } // namespace oxm_header_ops


  class oxm_header
    : public detail::basic_protocol_type<oxm_header>
  {
  public:
    using ofp_type = std::uint32_t;

    constexpr explicit oxm_header(std::uint32_t const oxm_header) noexcept
      : oxm_header_(oxm_header)
    {
    }

    constexpr auto oxm_class() const noexcept
      -> std::uint16_t
    {
      return oxm_header_ops::oxm_class(oxm_header_);
    }

    constexpr auto oxm_field() const noexcept
      -> std::uint8_t
    {
      return oxm_header_ops::oxm_field(oxm_header_);
    }

    constexpr auto oxm_type() const noexcept
      -> std::uint32_t
    {
      return oxm_header_ops::oxm_type(oxm_header_);
    }

    constexpr auto oxm_hasmask() const noexcept
      -> bool
    {
      return oxm_header_ops::oxm_hasmask(oxm_header_);
    }

    constexpr auto oxm_length() const noexcept
      -> std::uint8_t
    {
      return oxm_header_ops::oxm_length(oxm_header_);
    }

    constexpr auto to_ofp_type() const noexcept
      -> ofp_type
    {
      return oxm_header_;
    }

    constexpr static auto length() noexcept
      -> std::uint16_t
    {
      return sizeof(ofp_type);
    }

    constexpr static auto make_header(
          std::uint16_t const oxm_class
        , std::uint8_t const oxm_field
        , bool const oxm_hasmask
        , std::uint8_t const oxm_length) noexcept
      -> ofp_type
    {
      return oxm_header_ops::oxm_header(
          oxm_class, oxm_field, oxm_hasmask, oxm_length);
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
      return oxm_header{detail::decode<ofp_type>(first, last)};
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
    ofp_type oxm_header_;
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OXM_HEADER_HPP
