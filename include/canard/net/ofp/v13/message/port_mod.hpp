#ifndef CANARD_NET_OFP_V13_MESSAGES_PORT_MOD_HPP
#define CANARD_NET_OFP_V13_MESSAGES_PORT_MOD_HPP

#include <cstdint>
#include <canard/mac_address.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/common/port.hpp>
#include <canard/network/openflow/v13/detail/basic_fixed_length_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class port_mod
        : public detail::v13::basic_fixed_length_message<port_mod>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PORT_MOD;

        using raw_ofp_type = protocol::ofp_port_mod;

        port_mod(std::uint32_t const port_no
               , canard::mac_address const& macaddr
               , std::uint32_t const config
               , std::uint32_t const mask
               , std::uint32_t const advertise
               , std::uint32_t const xid = get_xid()) noexcept
            : port_mod_{
                  protocol::ofp_header{
                      version()
                    , type()
                    , sizeof(raw_ofp_type)
                    , xid
                  }
                , port_no
                , { 0, 0, 0, 0 }
                , {
                      macaddr.to_bytes()[0], macaddr.to_bytes()[1]
                    , macaddr.to_bytes()[2], macaddr.to_bytes()[3]
                    , macaddr.to_bytes()[4], macaddr.to_bytes()[5]
                  }
                , { 0, 0 }
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
                  port.port_no(), port.hardware_address()
                , config, mask, advertise
                , xid
              }
        {
        }

        auto header() const noexcept
            -> protocol::ofp_header const&
        {
            return port_mod_.header;
        }

        auto port_no() const noexcept
            -> std::uint32_t
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

    private:
        friend basic_fixed_length_message;

        explicit port_mod(raw_ofp_type const& port_mod) noexcept
            : port_mod_(port_mod)
        {
        }

        auto ofp_message() const noexcept
            -> raw_ofp_type const&
        {
            return port_mod_;
        }

    private:
        raw_ofp_type port_mod_;
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_PORT_MOD_HPP
