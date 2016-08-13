#ifndef CANARD_NET_OFP_V10_PORT_HPP
#define CANARD_NET_OFP_V10_PORT_HPP

#include <cstdint>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/detail/port_adaptor.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    class port
        : public v10_detail::port_adaptor<port>
    {
    public:
        auto ofp_port() const noexcept
            -> v10_detail::ofp_phy_port const&
        {
            return port_;
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
            return port{detail::decode<v10_detail::ofp_phy_port>(first, last)};
        }

        static auto from_ofp_port(v10_detail::ofp_phy_port const& ofp_port) noexcept
            -> port
        {
            return port{ofp_port};
        }

    private:
        explicit port(v10_detail::ofp_phy_port const& phy_port) noexcept
            : port_(phy_port)
        {
        }

    private:
        v10_detail::ofp_phy_port port_;
    };

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_PORT_HPP
