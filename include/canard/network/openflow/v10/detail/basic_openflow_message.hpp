#ifndef CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP
#define CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP

#include <cstdint>
#include <type_traits>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace v10_detail {

  template <class T>
  struct basic_openflow_message_tag {};

  namespace basic_openflow_message_detail {

    constexpr auto is_valid_length_impl(
          std::uint16_t const length, std::uint16_t const min_length
        , std::true_type) noexcept
      -> bool
    {
      return length == min_length;
    }

    constexpr auto is_valid_length_impl(
          std::uint16_t const length, std::uint16_t const min_length
        , std::false_type) noexcept
      -> bool
    {
      return length >= min_length;
    }

    template <class T>
    constexpr auto min_message_length(basic_openflow_message_tag<T>) noexcept
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
      return basic_openflow_message_detail::is_valid_length_impl(
              length
            , min_message_length(basic_openflow_message_tag<T>{})
            , std::integral_constant<bool, IsFixedLength>{})
          && additonally_check_message_length(
              basic_openflow_message_tag<T>{}, length);
    }

  } // namespace basic_openflow_message_detail


  template <class T>
  class basic_openflow_message
    : public detail::basic_protocol_type<T>
  {
  private:
    auto header() const
      -> protocol::ofp_header
    {
      return static_cast<T const*>(this)->header();
    }

  public:
    using ofp_header_type = protocol::ofp_header;

    static constexpr auto version() noexcept
      -> std::uint8_t
    {
      return protocol::OFP_VERSION;
    }

    static constexpr auto type() noexcept
      -> protocol::ofp_type
    {
      return T::message_type;
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return header().length;
    }

    auto xid() const noexcept
      -> std::uint32_t
    {
      return header().xid;
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
      return basic_openflow_message_detail
        ::is_valid_length<T, T::is_fixed_length_message>(header.length);
    }
  };

} // namespace v10_detail
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP
