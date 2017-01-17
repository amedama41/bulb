#ifndef CANARD_NET_OFP_V10_MESSAGES_PORT_MOD_HPP
#define CANARD_NET_OFP_V10_MESSAGES_PORT_MOD_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/mac_address.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/common/port.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class port_mod
        : public v10_detail::basic_openflow_message<port_mod>
    {
    public:
        using raw_ofp_type = protocol::ofp_port_mod;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PORT_MOD;

        port_mod(std::uint16_t const port_no
               , canard::mac_address const& macaddr
               , std::uint32_t const config
               , std::uint32_t const mask
               , std::uint32_t const advertise
               , std::uint32_t const xid = get_xid()) noexcept
            : port_mod_{
                  protocol::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
                    , sizeof(raw_ofp_type)
                    , xid
                  }
                , port_no
                , {
                      macaddr.to_bytes()[0], macaddr.to_bytes()[1]
                    , macaddr.to_bytes()[2], macaddr.to_bytes()[3]
                    , macaddr.to_bytes()[4], macaddr.to_bytes()[5]
                  }
                , config
                , mask
                , advertise
                , { 0, 0, 0, 0 }
              }
        {
        }

        port_mod(port const& port
               , std::uint32_t const config
               , std::uint32_t const mask
               , std::uint32_t const advertise
               , std::uint32_t const xid = get_xid()) noexcept
            : port_mod{
                  port.port_no()
                , port.hardware_address()
                , config, mask, advertise, xid
              }
        {
        }

        auto header() const noexcept
            -> protocol::ofp_header const&
        {
            return port_mod_.header;
        }

        auto port_no() const noexcept
            -> std::uint16_t
        {
            return port_mod_.port_no;
        }

        auto hardware_address() const noexcept
            -> canard::mac_address
        {
            return canard::mac_address{port_mod_.hw_addr};
        }

        auto config() const noexcept
            -> std::uint32_t
        {
            return port_mod_.config;
        }

        auto mask() const noexcept
            -> std::uint32_t
        {
            return port_mod_.mask;
        }

        auto advertised_features() const noexcept
            -> std::uint32_t
        {
            return port_mod_.advertise;
        }

        static void validate_header(protocol::ofp_header const& header)
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

    private:
        friend basic_openflow_message::basic_protocol_type;

        explicit port_mod(raw_ofp_type const& port_mod) noexcept
            : port_mod_(port_mod)
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, port_mod_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> port_mod
        {
            return port_mod{detail::decode<raw_ofp_type>(first, last)};
        }

        auto equal_impl(port_mod const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(port_mod_, rhs.port_mod_);
        }

    private:
        raw_ofp_type port_mod_;
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_PORT_MOD_HPP
