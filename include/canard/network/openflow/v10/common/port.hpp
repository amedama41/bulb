#ifndef CANARD_NET_OFP_V10_PORT_HPP
#define CANARD_NET_OFP_V10_PORT_HPP

#include <cstdint>
#include <cstring>
#include <algorithm>
#include <boost/utility/string_ref.hpp>
#include <canard/mac_address.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/detail/port_adaptor.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    class port
        : public v10_detail::port_adaptor<port>
        , public detail::basic_protocol_type<port>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_phy_port;

        port(std::uint16_t const port_no
           , canard::mac_address const addr
           , boost::string_ref const name
           , std::uint32_t const config
           , std::uint32_t const state
           , std::uint32_t const current_features
           , std::uint32_t const advertised_features
           , std::uint32_t const supported_features
           , std::uint32_t const peer_advertised_features) noexcept
            : port_{
                  port_no
                , {}
                , ""
                , config
                , state
                , current_features
                , advertised_features
                , supported_features
                , peer_advertised_features
              }
        {
            std::memcpy(port_.hw_addr, addr.to_bytes().data()
                      , sizeof(port_.hw_addr));
            std::memcpy(port_.name, &name[0]
                      , std::min(name.size(), sizeof(port_.name) - 1));
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
        }

        auto ofp_port() const noexcept
            -> raw_ofp_type const&
        {
            return port_;
        }

        static auto from_ofp_port(raw_ofp_type const& ofp_port) noexcept
            -> port
        {
            return port{ofp_port};
        }

    private:
        friend basic_protocol_type;

        explicit port(raw_ofp_type const& phy_port) noexcept
            : port_(phy_port)
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, port_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> port
        {
            return port{detail::decode<raw_ofp_type>(first, last)};
        }

        auto equal_impl(port const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(port_, rhs.port_);
        }

        auto equivalent_impl(port const& rhs) const noexcept
            -> bool
        {
          return port_no() == rhs.port_no()
              && hardware_address() == rhs.hardware_address()
              && name() == rhs.name()
              && config() == rhs.config()
              && state() == rhs.state()
              && current_features() == rhs.current_features()
              && advertised_features() == rhs.advertised_features()
              && supported_features() == rhs.supported_features()
              && peer_advertised_features() == rhs.peer_advertised_features();
        }

    private:
        raw_ofp_type port_;
    };

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_PORT_HPP
