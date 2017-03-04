#ifndef CANARD_NET_OFP_V13_MESSAGES_ROLE_HPP
#define CANARD_NET_OFP_V13_MESSAGES_ROLE_HPP

#include <cstdint>
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

  namespace role_detail {

    template <class T>
    class role_base
      : public detail::v13::basic_message<T>
    {
      using base_t = detail::v13::basic_message<T>;

    public:
      using raw_ofp_type = protocol::ofp_role_request;

      auto header() const noexcept
        -> protocol::ofp_header const&
      {
        return role_request_.header;
      }

      auto role() const noexcept
        -> std::uint32_t
      {
        return role_request_.role;
      }

      auto generation_id() const noexcept
        -> std::uint64_t
      {
        return role_request_.generation_id;
      }

    protected:
      role_base(
            std::uint32_t const role
          , std::uint64_t const generation_id
          , std::uint32_t const xid) noexcept
        : role_request_{
              {base_t::version(), base_t::type(), sizeof(raw_ofp_type), xid}
            , role
            , { 0, 0, 0, 0 }
            , generation_id
          }
      {
      }

      explicit role_base(raw_ofp_type const& role_request) noexcept
        : role_request_(role_request)
      {
      }

    private:
      friend base_t;

      static constexpr bool is_fixed_length_message = true;

      friend typename base_t::basic_protocol_type;

      template <class Container>
      void encode_impl(Container& container) const
      {
        detail::encode(container, role_request_);
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
        return detail::memcmp(role_request_, rhs.role_request_);
      }

    private:
      raw_ofp_type role_request_;
    };

  } // namespace role_detail


  class role_request
    : public role_detail::role_base<role_request>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_ROLE_REQUEST;

    role_request(
          std::uint32_t const role
        , std::uint64_t const generation_id
        , std::uint32_t const xid = get_xid()) noexcept
      : role_base{role, generation_id, xid}
    {
    }

  private:
    friend role_base;

    explicit role_request(raw_ofp_type const& role_request) noexcept
      : role_base{role_request}
    {
    }
  };

  class role_reply
    : public role_detail::role_base<role_reply>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_ROLE_REPLY;

    role_reply(
          std::uint32_t const role
        , std::uint64_t const generation_id
        , std::uint32_t const xid = get_xid()) noexcept
      : role_base{role, generation_id, xid}
    {
    }

  private:
    friend role_base;

    explicit role_reply(raw_ofp_type const& role_request) noexcept
      : role_base{role_request}
    {
    }
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_ROLE_HPP
