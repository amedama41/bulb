#ifndef CANARD_NET_OFP_V13_MESSAGES_MULTIPART_PORT_DESCRIPTION_HPP
#define CANARD_NET_OFP_V13_MESSAGES_MULTIPART_PORT_DESCRIPTION_HPP

#include <cstdint>
#include <utility>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/common/port.hpp>
#include <canard/net/ofp/v13/detail/basic_multipart.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

  class port_description_request
    : public multipart_detail::basic_multipart_request<
        port_description_request, void
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_PORT_DESC;

    explicit port_description_request(
        std::uint32_t const xid = get_xid()) noexcept
      : basic_multipart_request{0, xid}
    {
    }

  private:
    friend basic_multipart_request::base_type;

    explicit port_description_request(
        ofp_type const& multipart_request) noexcept
      : basic_multipart_request{multipart_request}
    {
    }
  };


  class port_description_reply
    : public multipart_detail::basic_multipart_reply<
        port_description_reply, v13::port[]
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_PORT_DESC;

    port_description_reply(
          body_type ports
        , std::uint16_t const flags = 0
        , std::uint32_t const xid = get_xid())
      : basic_multipart_reply{flags, std::move(ports), xid}
    {
    }

  private:
    friend basic_multipart_reply::base_type;

    static constexpr bool is_fixed_length_element = true;

    port_description_reply(ofp_type const& multipart_reply, body_type&& ports)
      : basic_multipart_reply{multipart_reply, std::move(ports)}
    {
    }
  };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_MULTIPART_PORT_DESCRIPTION_HPP
