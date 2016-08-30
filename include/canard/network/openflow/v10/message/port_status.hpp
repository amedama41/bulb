#ifndef CANARD_NET_OFP_V10_MESSAGES_PORT_STATUS_HPP
#define CANARD_NET_OFP_V10_MESSAGES_PORT_STATUS_HPP

#include <stdexcept>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/detail/port_adaptor.hpp>
#include <canard/network/openflow/v10/openflow.hpp>
#include <canard/network/openflow/v10/port.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class port_status
        : public v10_detail::basic_openflow_message<port_status>
        , public v10_detail::port_adaptor<port_status>
        , private boost::equality_comparable<port_status>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_port_status;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PORT_STATUS;

        port_status(v10::protocol::ofp_port_reason const reason
                  , v10::port const& port
                  , std::uint32_t const xid = get_xid()) noexcept
            : port_status_{
                  v10_detail::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
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
            -> v10_detail::ofp_header const&
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

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            return detail::encode(container, port_status_);
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> port_status
        {
            return port_status{detail::decode<raw_ofp_type>(first, last)
            };
        }

        static void validate(v10_detail::ofp_header const& header)
        {
            if (header.version != protocol::OFP_VERSION) {
                throw std::runtime_error{"invalid version"};
            }
            if (header.type != message_type) {
                throw std::runtime_error{"invalid message type"};
            }
            if (header.length != sizeof(raw_ofp_type)) {
                throw std::runtime_error{"invalid length"};
            }
        }

        friend auto operator==(port_status const&, port_status const&) noexcept
            -> bool;

    private:
        explicit port_status(raw_ofp_type const& status) noexcept
            : port_status_(status)
        {
        }

        auto equal_impl(port_status const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(port_status_, rhs.port_status_);
        }

        friend port_adaptor;

        auto ofp_port() const noexcept
            -> v10_detail::ofp_phy_port const&
        {
            return port_status_.desc;
        }

    private:
        raw_ofp_type port_status_;
    };

    inline auto operator==(
            port_status const& lhs, port_status const& rhs) noexcept
        -> bool
    {
        return lhs.equal_impl(rhs);
    }

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_PORT_STATUS_HPP
