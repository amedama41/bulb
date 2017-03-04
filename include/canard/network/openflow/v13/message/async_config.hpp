#ifndef CANARD_NET_OFP_V13_MESSAGES_ASYNC_CONFIG_HPP
#define CANARD_NET_OFP_V13_MESSAGES_ASYNC_CONFIG_HPP

#include <cstdint>
#include <array>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/detail/basic_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  namespace async_config_detail {

    template <class T>
    class async_config_base
      : public detail::v13::basic_message<T>
    {
      using base_t = detail::v13::basic_message<T>;

    public:
      using raw_ofp_type = protocol::ofp_async_config;
      using mask_array_type = std::array<std::uint32_t, 2>;

    public:
      auto header() const noexcept
        -> protocol::ofp_header const&
      {
        return async_config_.header;
      }

      auto packet_in_master_mask() const noexcept
        -> std::uint32_t
      {
        return async_config_.packet_in_mask[0];
      }

      auto packet_in_slave_mask() const noexcept
        -> std::uint32_t
      {
        return async_config_.packet_in_mask[1];
      }

      auto packet_in_mask() const noexcept
        -> mask_array_type
      {
        return {{ packet_in_master_mask(), packet_in_slave_mask() }};
      }

      auto port_status_master_mask() const noexcept
        -> std::uint32_t
      {
        return async_config_.port_status_mask[0];
      }

      auto port_status_slave_mask() const noexcept
        -> std::uint32_t
      {
        return async_config_.port_status_mask[1];
      }

      auto port_status_mask() const noexcept
        -> mask_array_type
      {
        return {{ port_status_master_mask(), port_status_slave_mask() }};
      }

      auto flow_removed_master_mask() const noexcept
        -> std::uint32_t
      {
        return async_config_.flow_removed_mask[0];
      }

      auto flow_removed_slave_mask() const noexcept
        -> std::uint32_t
      {
        return async_config_.flow_removed_mask[1];
      }

      auto flow_removed_mask() const noexcept
        -> mask_array_type
      {
        return {{ flow_removed_master_mask(), flow_removed_slave_mask() }};
      }

    protected:
      async_config_base(
            std::uint32_t const packet_in_master_mask
          , std::uint32_t const packet_in_slave_mask
          , std::uint32_t const port_status_master_mask
          , std::uint32_t const port_status_slave_mask
          , std::uint32_t const flow_removed_master_mask
          , std::uint32_t const flow_removed_slave_mask
          , std::uint32_t const xid) noexcept
        : async_config_{
              {base_t::version(), base_t::type(), sizeof(raw_ofp_type), xid}
            , { packet_in_master_mask, packet_in_slave_mask }
            , { port_status_master_mask, port_status_slave_mask }
            , { flow_removed_master_mask, flow_removed_slave_mask }
          }
      {
      }

      explicit async_config_base(raw_ofp_type const& async_config) noexcept
        : async_config_(async_config)
      {
      }

    private:
      friend base_t;

      static constexpr bool is_fixed_length_message = true;

      friend typename base_t::basic_protocol_type;

      template <class Container>
      void encode_impl(Container& container) const
      {
        detail::encode(container, async_config_);
      }

      template <class Iterator>
      static auto decode_impl(Iterator& first, Iterator last)
        -> T
      {
        return T{detail::decode<raw_ofp_type>(first, last)};
      }

      auto equal_impl(T const& rhs) const noexcept
        -> bool
      {
        return detail::memcmp(async_config_, rhs.async_config_);
      }

    private:
      raw_ofp_type async_config_;
    };

  } // namespace async_config_detail


  class get_async_request
    : public detail::v13::basic_message<get_async_request>
  {
  public:
    using raw_ofp_type = protocol::ofp_header;

    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_GET_ASYNC_REQUEST;

    explicit get_async_request(std::uint32_t const xid = get_xid()) noexcept
      : header_{version(), type(), sizeof(raw_ofp_type), xid}
    {
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return header_;
    }

  private:
    explicit get_async_request(raw_ofp_type const& header) noexcept
      : header_(header)
    {
    }

    friend basic_message;

    static constexpr bool is_fixed_length_message = true;

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, header_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> get_async_request
    {
      return get_async_request{detail::decode<raw_ofp_type>(first, last)};
    }

    auto equal_impl(get_async_request const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(header_, rhs.header_);
    }

  private:
    raw_ofp_type header_;
  };

  class get_async_reply
    : public async_config_detail::async_config_base<get_async_reply>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_GET_ASYNC_REPLY;

    get_async_reply(
          std::uint32_t const packet_in_master_mask
        , std::uint32_t const packet_in_slave_mask
        , std::uint32_t const port_status_master_mask
        , std::uint32_t const port_status_slave_mask
        , std::uint32_t const flow_removed_master_mask
        , std::uint32_t const flow_removed_slave_mask
        , std::uint32_t const xid = get_xid()) noexcept
      : async_config_base{
            packet_in_master_mask, packet_in_slave_mask
          , port_status_master_mask, port_status_slave_mask
          , flow_removed_master_mask, flow_removed_slave_mask
          , xid
        }
    {
    }

    get_async_reply(
          mask_array_type const packet_in_mask
        , mask_array_type const port_status_mask
        , mask_array_type const flow_removed_mask
        , std::uint32_t const xid = get_xid()) noexcept
      : async_config_base{
            packet_in_mask[0], packet_in_mask[1]
          , port_status_mask[0], port_status_mask[1]
          , flow_removed_mask[0], flow_removed_mask[1]
          , xid
        }
    {
    }

  private:
    friend async_config_base;

    explicit get_async_reply(raw_ofp_type const& async_config) noexcept
      : async_config_base{async_config}
    {
    }
  };

  class set_async
    : public async_config_detail::async_config_base<set_async>
  {
  public:
    static constexpr protocol::ofp_type message_type = protocol::OFPT_SET_ASYNC;

    set_async(
          std::uint32_t const packet_in_master_mask
        , std::uint32_t const packet_in_slave_mask
        , std::uint32_t const port_status_master_mask
        , std::uint32_t const port_status_slave_mask
        , std::uint32_t const flow_removed_master_mask
        , std::uint32_t const flow_removed_slave_mask
        , std::uint32_t const xid = get_xid()) noexcept
      : async_config_base{
            packet_in_master_mask, packet_in_slave_mask
          , port_status_master_mask, port_status_slave_mask
          , flow_removed_master_mask, flow_removed_slave_mask
          , xid
        }
    {
    }

    set_async(
          mask_array_type const packet_in_mask
        , mask_array_type const port_status_mask
        , mask_array_type const flow_removed_mask
        , std::uint32_t const xid = get_xid()) noexcept
      : async_config_base{
            packet_in_mask[0], packet_in_mask[1]
          , port_status_mask[0], port_status_mask[1]
          , flow_removed_mask[0], flow_removed_mask[1]
          , xid
        }
    {
    }

  private:
    friend async_config_base;

    explicit set_async(raw_ofp_type const& async_config) noexcept
      : async_config_base{async_config}
    {
    }
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_ASYNC_CONFIG_HPP
