#ifndef CANARD_NET_OFP_DETAIL_BAISC_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_DETAIL_BAISC_QUEUE_PROPERTY_HPP

#include <cstdint>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class T, class HeaderType, class PropertyType>
  class basic_queue_property
    : public detail::basic_protocol_type<T>
  {
    using base_t = detail::basic_protocol_type<T>;

  protected:
    basic_queue_property() = default;

  public:
    using ofp_header_type = HeaderType;

    static constexpr auto property() noexcept
      -> PropertyType
    {
      return T::queue_property;
    }

    static constexpr auto type() noexcept
      -> PropertyType
    {
      return property();
    }

    static constexpr auto length() noexcept
      -> std::uint16_t
    {
      return sizeof(typename T::raw_ofp_type);
    }

    static auto validate_header(ofp_header_type const& prop_header) noexcept
      -> char const*
    {
      if (prop_header.property != property()) {
        return "invalid queue property type";
      }
      if (!is_valid_queue_property_length(prop_header)) {
        return "invalid queue property length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_queue_property_length(
        ofp_header_type const& header) noexcept
      -> bool
    {
      return header.len == length();
    }

  private:
    auto derived() const noexcept
      -> T const&
    {
      return *static_cast<T const*>(this);
    }

    friend base_t;

    template <class Validator>
    void validate_impl(Validator) const
    {
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, derived().ofp_queue_property());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      return T{detail::decode<typename T::raw_ofp_type>(first, last)};
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(
          derived().ofp_queue_property(), rhs.ofp_queue_property());
    }

    auto equivalent_impl(T const& rhs) const noexcept
      -> bool
    {
      return derived().is_equivalent_property(rhs);
    }
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_BAISC_QUEUE_PROPERTY_HPP
