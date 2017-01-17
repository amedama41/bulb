#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_OPENFLOW_MESSAGE_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_OPENFLOW_MESSAGE_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

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

    static void validate_header(ofp::v13::protocol::ofp_header const& header)
    {
      if (header.version != version()) {
        throw std::runtime_error{"invalid version"};
      }
      if (header.type != type()) {
        throw std::runtime_error{"invalid message type"};
      }
      if (header.length < detail::basic_protocol_type<T>::min_length()) {
        throw std::runtime_error{"too small length"};
      }
    }
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_OPENFLOW_MESSAGE_HPP
