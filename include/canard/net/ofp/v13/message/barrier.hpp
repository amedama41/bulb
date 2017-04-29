#ifndef CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP
#define CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP

#include <cstdint>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  namespace barrier_detail {

    template <class T>
    class barrier_base
      : public detail::v13::basic_fixed_length_message<T>
    {
      using base_t = detail::v13::basic_fixed_length_message<T>;

    public:
      using ofp_type = protocol::ofp_header;

      auto header() const noexcept
        -> protocol::ofp_header const&
      {
        return header_;
      }

    protected:
      explicit barrier_base(std::uint32_t const xid) noexcept
        : header_{base_t::version(), base_t::type(), sizeof(ofp_type), xid}
      {
      }

      explicit barrier_base(ofp_type const& header) noexcept
        : header_(header)
      {
      }

    private:
      friend base_t;

      auto ofp_message() const noexcept
        -> ofp_type const&
      {
        return header_;
      }

    private:
      ofp_type header_;
    };

  } // namespace barrier_detail


  class barrier_request
    : public barrier_detail::barrier_base<barrier_request>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_BARRIER_REQUEST;

    explicit barrier_request(std::uint32_t const xid = get_xid()) noexcept
      : barrier_base{xid}
    {
    }

  private:
    friend barrier_base::basic_fixed_length_message;

    explicit barrier_request(ofp_type const& header) noexcept
      : barrier_base{header}
    {
    }
  };


  class barrier_reply
    : public barrier_detail::barrier_base<barrier_reply>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_BARRIER_REPLY;

    explicit barrier_reply(std::uint32_t const xid = get_xid()) noexcept
      : barrier_base{xid}
    {
    }

    explicit barrier_reply(barrier_request const& request)
      : barrier_reply{request.xid()}
    {
    }

  private:
    friend barrier_base::basic_fixed_length_message;

    explicit barrier_reply(ofp_type const& header) noexcept
      : barrier_base{header}
    {
    }
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP
