#ifndef CANARD_NET_OFP_V10_PORT_ADAPTOR_HPP
#define CANARD_NET_OFP_V10_PORT_ADAPTOR_HPP

#include <cstdint>
#include <boost/utility/string_ref.hpp>
#include <canard/mac_address.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace v10_detail {

  template <class T>
  class port_adaptor
  {
  public:
    auto port_no() const noexcept
      -> std::uint16_t
    {
      return base_port().port_no;
    }

    auto hardware_address() const noexcept
      -> canard::mac_address
    {
      return canard::mac_address{base_port().hw_addr};
    }

    auto name() const
      -> boost::string_ref
    {
      return base_port().name;
    }

    auto config() const noexcept
      -> std::uint32_t
    {
      return base_port().config;
    }

    auto state() const noexcept
      -> std::uint32_t
    {
      return base_port().state;
    }

    auto current_features() const noexcept
      -> std::uint32_t
    {
      return base_port().curr;
    }

    auto advertised_features() const noexcept
      -> std::uint32_t
    {
      return base_port().advertised;
    }

    auto supported_features() const noexcept
      -> std::uint32_t
    {
      return base_port().supported;
    }

    auto peer_advertised_features() const noexcept
      -> std::uint32_t
    {
      return base_port().peer;
    }

    auto is_reserved() const
      -> bool
    {
      return protocol::OFPP_MAX < port_no();
    }

    auto administratively_down() const noexcept
      -> bool
    {
      return config() & protocol::OFPPC_PORT_DOWN;
    }

    auto link_down() const noexcept
      -> bool
    {
      return state() & protocol::OFPPS_LINK_DOWN;
    }

  private:
    auto base_port() const noexcept
      -> protocol::ofp_phy_port const&
    {
      return static_cast<T const*>(this)->ofp_port();
    }
  };

} // namespace v10_detail
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_PORT_ADAPTOR_HPP
