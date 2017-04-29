#ifndef CANARD_NET_OFP_DETAIL_BASIC_FIXED_LENGTH_MESSAGE_HPP
#define CANARD_NET_OFP_DETAIL_BASIC_FIXED_LENGTH_MESSAGE_HPP

#include <cstdint>
#include <canard/net/ofp/detail/basic_message.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class T, class HeaderType, class MessageType, std::uint8_t Version>
  class basic_fixed_length_message
    : public detail::basic_message<T, HeaderType, MessageType, Version>
  {
    using base_t = detail::basic_message<T, HeaderType, MessageType, Version>;

  protected:
    basic_fixed_length_message() = default;

  private:
    auto derived() const noexcept
      -> T const&
    {
      return *static_cast<T const*>(this);
    }

    friend base_t;

    static constexpr bool is_fixed_length_message = true;

    friend typename base_t::basic_protocol_type;

    template <class Validator>
    void validate_impl(Validator const&) const
    {
      return derived().validate_mssage();
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, derived().ofp_message());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      return T{detail::decode<typename T::ofp_type>(first, last)};
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(derived().ofp_message(), rhs.ofp_message());
    }

    auto equivalent_impl(T const& rhs) const noexcept(
        derived().is_equivalent_message(rhs))
      -> bool
    {
      return derived().is_equivalent_message(rhs);
    }
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_BASIC_FIXED_LENGTH_MESSAGE_HPP
