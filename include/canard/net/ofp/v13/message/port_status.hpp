#ifndef CANARD_NET_OFP_V13_MESSAGES_PORT_STATUS_HPP
#define CANARD_NET_OFP_V13_MESSAGES_PORT_STATUS_HPP

#include <cstdint>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/common/port.hpp>
#include <canard/net/ofp/v13/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/detail/port_adaptor.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  class port_status
    : public detail::v13::basic_fixed_length_message<port_status>
    , public v13_detail::port_adaptor<port_status>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_PORT_STATUS;

    using ofp_type = protocol::ofp_port_status;

    port_status(
          v13::protocol::ofp_port_reason const reason
        , v13::port const& port
        , std::uint32_t const xid = get_xid()) noexcept
      : port_status_{
            protocol::ofp_header{version(), type(), sizeof(ofp_type), xid}
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
      -> v13::port
    {
      return v13::port::from_ofp_port(port_status_.desc);
    }

    auto is_added() const noexcept
      -> bool
    {
      return reason() == protocol::OFPPR_ADD;
    }

    auto is_deleted() const noexcept
      -> bool
    {
      return reason() == protocol::OFPPR_DELETE;
    }

    auto is_modified() const noexcept
      -> bool
    {
      return reason() == protocol::OFPPR_MODIFY;
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
      -> protocol::ofp_port const&
    {
      return port_status_.desc;
    }

  private:
    ofp_type port_status_;
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_PORT_STATUS_HPP
