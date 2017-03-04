#ifndef CANARD_NET_OFP_V13_MESSAGES_PORT_STATUS_HPP
#define CANARD_NET_OFP_V13_MESSAGES_PORT_STATUS_HPP

#include <cstdint>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/common/port.hpp>
#include <canard/network/openflow/v13/detail/basic_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/port_adaptor.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class port_status
        : public detail::v13::basic_message<port_status>
        , public v13_detail::port_adaptor<port_status>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PORT_STATUS;

        using raw_ofp_type = protocol::ofp_port_status;

        port_status(v13::protocol::ofp_port_reason const reason
                  , v13::port const& port
                  , std::uint32_t const xid = get_xid()) noexcept
            : port_status_{
                  protocol::ofp_header{
                      version()
                    , type()
                    , sizeof(raw_ofp_type)
                    , xid
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
        explicit port_status(raw_ofp_type const& status) noexcept
            : port_status_(status)
        {
        }

        friend basic_message;

        static constexpr bool is_fixed_length_message = true;

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
             detail::encode(container, port_status_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> port_status
        {
            return port_status{detail::decode<raw_ofp_type>(first, last)};
        }

        friend port_adaptor;

        auto ofp_port() const noexcept
            -> protocol::ofp_port const&
        {
            return port_status_.desc;
        }

    private:
        raw_ofp_type port_status_;
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_PORT_STATUS_HPP
