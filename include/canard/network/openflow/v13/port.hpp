#ifndef CANARD_NET_OFP_V13_PORT_HPP
#define CANARD_NET_OFP_V13_PORT_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <boost/operators.hpp>
#include <boost/utility/string_ref.hpp>
#include <canard/mac_address.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/port_adaptor.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    class port
        : public v13_detail::port_adaptor<port>
        , private boost::equality_comparable<port>
    {
    public:
        using raw_ofp_type = v13_detail::ofp_port;

        static constexpr std::size_t base_size = sizeof(raw_ofp_type);

        port(std::uint32_t const port_no
           , canard::mac_address const addr
           , boost::string_ref const name
           , std::uint32_t const config
           , std::uint32_t const state
           , std::uint32_t const current_features
           , std::uint32_t const advertised_features
           , std::uint32_t const supported_features
           , std::uint32_t const peer_advertised_features
           , std::uint32_t const current_speed
           , std::uint32_t const max_speed) noexcept
            : port_{
                  port_no
                , { 0, 0, 0, 0 }
                , {}
                , { 0, 0 }
                , ""
                , config
                , state
                , current_features
                , advertised_features
                , supported_features
                , peer_advertised_features
                , current_speed
                , max_speed
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

        auto is_in_port() const
            -> bool
        {
            return port_no() == protocol::OFPP_IN_PORT;
        }

        auto is_table() const
            -> bool
        {
            return port_no() == protocol::OFPP_TABLE;
        }

        auto is_normal() const
            -> bool
        {
            return port_no() == protocol::OFPP_NORMAL;
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            return detail::encode(container, port_);
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> port
        {
            return port{detail::decode<raw_ofp_type>(first, last)};
        }

        static auto from_ofp_port(raw_ofp_type const& ofp_port) noexcept
            -> port
        {
            return port{ofp_port};
        }

    private:
        explicit port(raw_ofp_type const& port) noexcept
            : port_(port)
        {
        }

    private:
        raw_ofp_type port_;
    };

    inline auto operator==(port const& lhs, port const& rhs) noexcept
        -> bool
    {
        return detail::memcmp(lhs.ofp_port(), rhs.ofp_port());
    }

    inline auto equivalent(port const& lhs, port const& rhs) noexcept
        -> bool
    {
        return lhs.port_no() == rhs.port_no()
            && lhs.hardware_address() == rhs.hardware_address()
            && lhs.name() == rhs.name()
            && lhs.config() == rhs.config()
            && lhs.state() == rhs.state()
            && lhs.current_features() == rhs.current_features()
            && lhs.advertised_features() == rhs.advertised_features()
            && lhs.supported_features() == rhs.supported_features()
            && lhs.peer_advertised_features() == rhs.peer_advertised_features()
            && lhs.current_speed() == rhs.current_speed()
            && lhs.max_speed() == rhs.max_speed();
    }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_PORT_HPP
