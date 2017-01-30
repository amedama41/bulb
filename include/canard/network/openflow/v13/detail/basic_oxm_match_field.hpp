#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_OXM_MATCH_FIELD_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_OXM_MATCH_FIELD_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <array>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/optional/optional.hpp>
#include <boost/range/algorithm_ext/for_each.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/v13/common/oxm_header.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  namespace basic_oxm_match_field_detail {

    template <class T>
    auto to_bytes(T const& t) noexcept
      -> std::array<unsigned char, sizeof(T)>
    {
      auto bytes = std::array<unsigned char, sizeof(T)>{};
      std::memcpy(bytes.data(), std::addressof(t), bytes.size());
      return bytes;
    }

    template <class T, std::size_t N>
    auto to_bytes(std::array<T, N> const& t) noexcept
      -> std::array<T, N> const&
    {
      return t;
    }

    template <class T, std::size_t N>
    auto is_array_class_impl(std::array<T, N> const&) noexcept
      -> std::true_type;

    auto is_array_class_impl(...) noexcept
      -> std::false_type;

    template <class T>
    struct is_array_class
      : decltype(
          basic_oxm_match_field_detail::is_array_class_impl(std::declval<T>()))
    {};

    template <
        class T
      , class = typename std::enable_if<!is_array_class<T>::value>::type
    >
    auto from_bytes(std::array<unsigned char, sizeof(T)> const& bytes) noexcept
      -> T
    {
      auto t = T{};
      std::memcpy(std::addressof(t), bytes.data(), bytes.size());
      return t;
    }

    template <
        class T
      , class = typename std::enable_if<is_array_class<T>::value>::type
    >
    auto from_bytes(std::array<unsigned char, sizeof(T)> const& bytes) noexcept
      -> T const&
    {
      return bytes;
    }

    template <class Mask, class T>
    auto all_of(Mask const& mask, T t) noexcept
      -> bool
    {
      return boost::algorithm::all_of(
            basic_oxm_match_field_detail::to_bytes(mask)
          , [=](std::uint8_t const e) { return e == t; });
    }

    template <class T>
    void validate_mask(T const& value, T const& mask)
    {
      boost::for_each(
            basic_oxm_match_field_detail::to_bytes(value)
          , basic_oxm_match_field_detail::to_bytes(mask)
          , [](std::uint8_t const v, std::uint8_t const m) {
          if (v & ~m) {
            throw std::runtime_error{"invalid wildcard"};
          }
      });
    }

  } // namespace basic_oxm_match_field_detail

  template <class T, class OXMMatchFieldTraits>
  class basic_oxm_match_field
      : public detail::basic_protocol_type<T>
  {
  public:
    using raw_ofp_type = std::uint32_t;
    using oxm_header_type = raw_ofp_type;
    using value_type = typename OXMMatchFieldTraits::value_type;

    static constexpr auto oxm_class() noexcept
      -> ofp::v13::protocol::ofp_oxm_class
    {
      return T::oxm_class_value;
    }

    static constexpr auto oxm_field() noexcept
      -> std::uint8_t
    {
      return T::oxm_field_value;
    }

    static constexpr auto oxm_type() noexcept
      -> std::uint32_t
    {
      return std::uint32_t{oxm_class()} << 7 | oxm_field();
    }

    auto oxm_has_mask() const noexcept
      -> bool
    {
      return static_cast<bool>(oxm_mask());
    }

    auto oxm_length() const noexcept
      -> std::uint8_t
    {
      return oxm_has_mask() ? value_length() * 2 : value_length();
    }

    auto oxm_header() const noexcept
      -> oxm_header_type
    {
      return (oxm_type() << 9)
           | (oxm_header_type{oxm_has_mask()} << 8)
           | oxm_length();
    }

    auto oxm_value() const noexcept
      -> value_type const&
    {
      return value_;
    }

    auto oxm_mask() const noexcept
      -> boost::optional<value_type> const&
    {
      return mask_;
    }

    static constexpr auto type() noexcept
      -> std::uint32_t
    {
      return oxm_type();
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return sizeof(oxm_header_type) + oxm_length();
    }

    auto is_wildcard() const noexcept
      -> bool
    {
      return oxm_has_mask()
           ? basic_oxm_match_field_detail::all_of(raw_mask(), 0)
           : false;
    }

    auto is_exact() const noexcept
      -> bool
    {
      return oxm_has_mask()
           ? basic_oxm_match_field_detail::all_of(raw_mask(), 0xff)
           : true;
    }

    static auto validate_header(oxm_header_type const oxm_header) noexcept
      -> char const*
    {
      return validate_header(ofp::v13::oxm_header{oxm_header});
    }

    static auto validate_header(ofp::v13::oxm_header const oxm_header) noexcept
      -> char const*
    {
      if (oxm_header.oxm_class() != oxm_class()) {
        return "invalid oxm class";
      }
      if (oxm_header.oxm_field() != oxm_field()) {
        return "invalid oxm field";
      }
      if (!is_valid_oxm_match_field_length(oxm_header)) {
        return "invalid oxm length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_oxm_match_field_length(
        ofp::v13::oxm_header const oxm_header) noexcept
      -> bool
    {
      return oxm_header.oxm_length()
          == (oxm_header.oxm_hasmask() ? value_length() * 2 : value_length());
    }

    static constexpr auto is_valid_oxm_match_field_length(
        oxm_header_type const oxm_header) noexcept
      -> bool
    {
      return is_valid_oxm_match_field_length(ofp::v13::oxm_header{oxm_header});
    }

  protected:
    using oxm_value_type = typename OXMMatchFieldTraits::oxm_value_type;

    explicit basic_oxm_match_field(value_type const& value)
      : value_(value)
      , mask_{boost::none}
    {
    }

    basic_oxm_match_field(value_type const& value, value_type const& mask)
      : value_(value)
      , mask_{mask}
    {
    }

  private:
    using byte_order = std::integral_constant<
      bool, OXMMatchFieldTraits::needs_byte_order_conversion
    >;

    auto raw_value() const noexcept
      -> oxm_value_type
    {
      return OXMMatchFieldTraits::to_oxm_value(oxm_value());
    }

    auto raw_mask() const noexcept
      -> oxm_value_type
    {
      return OXMMatchFieldTraits::to_oxm_value(*oxm_mask());
    }

    template <class Container>
    void encode_value_and_mask(Container& container, std::true_type) const
    {
      detail::encode(container, raw_value());
      if (oxm_has_mask()) {
        detail::encode(container, raw_mask());
      }
    }

    template <class Container>
    void encode_value_and_mask(Container& container, std::false_type) const
    {
      auto const value_bytes
        = basic_oxm_match_field_detail::to_bytes(raw_value());
      detail::encode_byte_array(
          container, value_bytes.data(), value_bytes.size());
      if (oxm_has_mask()) {
        auto const mask_bytes
          = basic_oxm_match_field_detail::to_bytes(raw_mask());
        detail::encode_byte_array(
            container, mask_bytes.data(), mask_bytes.size());
      }
    }

    template <class Iterator>
    static auto decode_value_and_mask(
          oxm_header_type const oxm_header, Iterator& first, Iterator last
        , std::true_type)
      -> T
    {
      auto const value = detail::decode<oxm_value_type>(first, last);
      if (oxm_header & 0x00000100) {
        auto const mask = detail::decode<oxm_value_type>(first, last);
        return T{value, mask};
      }
      else {
        return T{value};
      }
    }

    template <class Iterator>
    static auto decode_value_and_mask(
          oxm_header_type const oxm_header, Iterator& first, Iterator last
        , std::false_type)
      -> T
    {
      constexpr std::size_t N = sizeof(oxm_value_type);
      auto const value_bytes = detail::decode_byte_array<N>(first, last);
      if (oxm_header & 0x00000100) {
        auto const mask_bytes = detail::decode_byte_array<N>(first, last);
        return T{
            basic_oxm_match_field_detail::from_bytes<oxm_value_type>(value_bytes)
          , basic_oxm_match_field_detail::from_bytes<oxm_value_type>(mask_bytes)
        };
      }
      else {
        return T{
          basic_oxm_match_field_detail::from_bytes<oxm_value_type>(value_bytes)
        };
      }
    }

    friend detail::basic_protocol_type<T>;

    friend constexpr auto get_min_length(T*) noexcept
      -> std::uint16_t
    {
      return sizeof(oxm_header_type) + value_length();
    }

    static constexpr auto value_length() noexcept
      -> std::uint16_t
    {
      return sizeof(oxm_value_type);
    }

    template <class Validator>
    void validate_impl(Validator) const
    {
      T::validate_value(oxm_value());
      if (oxm_has_mask()) {
        basic_oxm_match_field_detail::validate_mask(raw_value(), raw_mask());
      }
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, oxm_header());
      encode_value_and_mask(container, byte_order{});
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      auto const oxm_header = detail::decode<oxm_header_type>(first, last);
      return decode_value_and_mask(oxm_header, first, last, byte_order{});
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return oxm_mask() == rhs.oxm_mask()
          && oxm_value() == rhs.oxm_value();
    }

    auto equivalent_impl(T const& rhs) const noexcept
      -> bool
    {
      if (is_wildcard()) {
        return rhs.is_wildcard();
      }
      if (is_exact()) {
        return rhs.is_exact()
            && oxm_value() == rhs.oxm_value();
      }
      return oxm_mask() == rhs.oxm_mask()
          && oxm_value() == rhs.oxm_value();
    }

  private:
    value_type value_;
    boost::optional<value_type> mask_;
  };

} // namespace detail
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_OXM_MATCH_FIELD_HPP
