#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_OPENFLOW_MESSAGE_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_OPENFLOW_MESSAGE_HPP

#include <cstdint>
#include <type_traits>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  struct basic_openflow_message_tag {};

  namespace basic_openflow_message_detail {

    constexpr auto is_valid_length(
          std::uint16_t const length, std::uint16_t const min_length
        , std::true_type) noexcept
      -> bool
    {
      return length == min_length;
    }

    constexpr auto is_valid_length(
          std::uint16_t const length, std::uint16_t const min_length
        , std::false_type) noexcept
      -> bool
    {
      return length >= min_length;
    }

    template <class T>
    constexpr auto get_min_message_length(
        basic_openflow_message_tag<T>, ...) noexcept
      -> std::uint16_t
    {
      return T::min_length();
    }

    template <class T>
    constexpr auto min_length() noexcept
      -> std::uint16_t
    {
      return get_min_message_length(basic_openflow_message_tag<T>{});
    }

  } // namespace basic_openflow_message_detail

  template <class T>
  class basic_openflow_message
    : public detail::basic_protocol_type<T>
  {
  private:
    auto base_header() const noexcept
      -> ofp::v13::protocol::ofp_header const&
    {
      return static_cast<T const*>(this)->header();
    }

  public:
    static constexpr auto version() noexcept
      -> std::uint8_t
    {
      return ofp::v13::protocol::OFP_VERSION;
    }

    static constexpr auto type() noexcept
      -> ofp::v13::protocol::ofp_type
    {
      return T::message_type;
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return base_header().length;
    }

    auto xid() const noexcept
      -> std::uint32_t
    {
      return base_header().xid;
    }

    static auto validate_header(
        ofp::v13::protocol::ofp_header const& header) noexcept
      -> char const*
    {
      if (header.version != version()) {
        return "invalid version";
      }
      if (header.type != type()) {
        return "invalid message type";
      }
      if (!basic_openflow_message_detail::is_valid_length(
              header.length, basic_openflow_message_detail::min_length<T>()
            , std::integral_constant<bool, T::is_fixed_length_message>{})) {
        return "invalid message length";
      }
      return nullptr;
    }
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_OPENFLOW_MESSAGE_HPP
