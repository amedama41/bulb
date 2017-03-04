#ifndef CANARD_NET_OFP_DETAIL_BASIC_MESSAGE_HPP
#define CANARD_NET_OFP_DETAIL_BASIC_MESSAGE_HPP

#include <cstdint>
#include <type_traits>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class T>
  struct basic_message_tag {};

  namespace basic_message_detail {

    constexpr auto is_valid_length_impl(
          std::uint16_t const length
        , std::uint16_t const min_length
        , std::true_type) noexcept
      -> bool
    {
      return length == min_length;
    }

    constexpr auto is_valid_length_impl(
          std::uint16_t const length
        , std::uint16_t const min_length
        , std::false_type) noexcept
      -> bool
    {
      return length >= min_length;
    }

    template <class T>
    constexpr auto min_message_length(basic_message_tag<T>) noexcept
      -> std::uint16_t
    {
      return T::min_length();
    }

    constexpr auto additonally_check_message_length(...) noexcept
      -> bool
    {
      return true;
    }

    template <class T, bool IsFixedLength>
    constexpr auto is_valid_length(std::uint16_t const length) noexcept
      -> bool
    {
      return basic_message_detail::is_valid_length_impl(
              length
            , min_message_length(basic_message_tag<T>{})
            , std::integral_constant<bool, IsFixedLength>{})
          && additonally_check_message_length(basic_message_tag<T>{}, length);
    }

  } // namespace basic_message_detail


  template <class T, class HeaderType, class MessageType, std::uint8_t Version>
  class basic_message
    : public detail::basic_protocol_type<T>
  {
  protected:
    basic_message() = default;

  public:
    using ofp_header_type = HeaderType;

    static constexpr auto version() noexcept
      -> std::uint8_t
    {
      return Version;
    }

    static constexpr auto type() noexcept
      -> MessageType
    {
      return T::message_type;
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return derived().header().length;
    }

    auto xid() const noexcept
      -> std::uint32_t
    {
      return derived().header().xid;
    }

    static auto validate_header(ofp_header_type const& header) noexcept
      -> char const*
    {
      if (header.version != version()) {
        return "invalid version";
      }
      if (header.type != type()) {
        return "invalid message type";
      }
      if (!is_valid_message_length(header)) {
        return "invalid message length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_message_length(
        ofp_header_type const& header) noexcept
      -> bool
    {
      return basic_message_detail
        ::is_valid_length<T, T::is_fixed_length_message>(header.length);
    }

  private:
    auto derived() const noexcept
      -> T const&
    {
      return *static_cast<T const*>(this);
    }
  };


} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_BASIC_MESSAGE_HPP
