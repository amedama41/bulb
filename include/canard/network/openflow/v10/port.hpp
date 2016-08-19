#ifndef CANARD_NET_OFP_V10_PORT_HPP
#define CANARD_NET_OFP_V10_PORT_HPP

#include <cstdint>
#include <boost/operators.hpp>
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
        , private boost::equality_comparable<port>
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

        friend auto operator==(port const&, port const&) noexcept
            -> bool;

    private:
        explicit port(v10_detail::ofp_phy_port const& phy_port) noexcept
            : port_(phy_port)
        {
        }

        auto equal_impl(port const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(port_, rhs.port_);
        }

    private:
        v10_detail::ofp_phy_port port_;
    };

    inline auto operator==(port const& lhs, port const& rhs) noexcept
        -> bool
    {
        return lhs.equal_impl(rhs);
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
          && lhs.peer_advertised_features() == rhs.peer_advertised_features();
    }

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_PORT_HPP
