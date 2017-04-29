#ifndef CANARD_NET_OFP_V10_MESSAGES_PORT_STATUS_HPP
#define CANARD_NET_OFP_V10_MESSAGES_PORT_STATUS_HPP

#include <cstdint>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v10/common/port.hpp>
#include <canard/net/ofp/v10/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/detail/port_adaptor.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

  class port_status
    : public v10_detail::basic_fixed_length_message<port_status>
    , public v10_detail::port_adaptor<port_status>
  {
  public:
    using ofp_type = protocol::ofp_port_status;

    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_PORT_STATUS;

    port_status(
          v10::protocol::ofp_port_reason const reason
        , v10::port const& port
        , std::uint32_t const xid = get_xid()) noexcept
      : port_status_{
            protocol::ofp_header{
              protocol::OFP_VERSION, message_type, sizeof(ofp_type), xid
            }
          , std::uint8_t(reason)
          , { 0, 0, 0, 0, 0, 0, 0 }
          , port.ofp_port()
        }
    {
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return port_status_.header;
    }

    auto reason() const noexcept
      -> protocol::ofp_port_reason
    {
      return protocol::ofp_port_reason(port_status_.reason);
    }

    auto port() const noexcept
      -> v10::port
    {
      return v10::port::from_ofp_port(port_status_.desc);
    }

  private:
    friend basic_fixed_length_message;

    explicit port_status(ofp_type const& status) noexcept
      : port_status_(status)
    {
    }

    auto ofp_message() const noexcept
      -> ofp_type const&
    {
      return port_status_;
    }

    friend port_adaptor;

    auto ofp_port() const noexcept
      -> protocol::ofp_phy_port const&
    {
      return port_status_.desc;
    }

  private:
    ofp_type port_status_;
  };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_PORT_STATUS_HPP
