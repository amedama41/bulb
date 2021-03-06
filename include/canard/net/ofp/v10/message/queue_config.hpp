#ifndef CANARD_NET_OFP_V10_MESSAGES_QUEUE_CONFIG_HPP
#define CANARD_NET_OFP_V10_MESSAGES_QUEUE_CONFIG_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v10/common/packet_queue.hpp>
#include <canard/net/ofp/v10/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v10/detail/basic_message.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

  class queue_get_config_request
    : public v10_detail::basic_fixed_length_message<queue_get_config_request>
  {
  public:
    using ofp_type = protocol::ofp_queue_get_config_request;

    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_QUEUE_GET_CONFIG_REQUEST;

    explicit queue_get_config_request(
          std::uint16_t const port_no
        , std::uint32_t const xid = get_xid()) noexcept
      : queue_get_config_request_{
            protocol::ofp_header{
              protocol::OFP_VERSION, message_type, sizeof(ofp_type), xid
            }
          , port_no
          , { 0, 0 }
        }
    {
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return queue_get_config_request_.header;
    }

    auto port_no() const noexcept
      -> std::uint16_t
    {
      return queue_get_config_request_.port;
    }

  private:
    friend basic_fixed_length_message;

    explicit queue_get_config_request(
        ofp_type const& queue_get_config_request) noexcept
      : queue_get_config_request_(queue_get_config_request)
    {
    }

    auto ofp_message() const noexcept
      -> ofp_type const&
    {
      return queue_get_config_request_;
    }

  private:
    ofp_type queue_get_config_request_;
  };


  class queue_get_config_reply
    : public v10_detail::basic_message<queue_get_config_reply>
  {
  public:
    using ofp_type = protocol::ofp_queue_get_config_reply;
    using queues_type = ofp::list<packet_queue>;

    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_QUEUE_GET_CONFIG_REPLY;

    queue_get_config_reply(
          std::uint16_t const port_no
        , queues_type queues
        , std::uint32_t const xid = get_xid())
      : queue_get_config_reply_{
            protocol::ofp_header{
                protocol::OFP_VERSION
              , message_type
              , queues.calc_ofp_length(sizeof(ofp_type))
              , xid
            }
          , port_no
          , { 0, 0, 0, 0, 0, 0 }
        }
      , queues_(std::move(queues))
    {
    }

    queue_get_config_reply(queue_get_config_reply const&) = default;

    queue_get_config_reply(queue_get_config_reply&& other)
      : queue_get_config_reply_(other.queue_get_config_reply_)
      , queues_(other.extract_queues())
    {
    }

    auto operator=(queue_get_config_reply const& other)
      -> queue_get_config_reply&
    {
      return operator=(queue_get_config_reply{other});
    }

    auto operator=(queue_get_config_reply&& other)
      -> queue_get_config_reply&
    {
      auto tmp = std::move(other);
      std::swap(queue_get_config_reply_, tmp.queue_get_config_reply_);
      queues_.swap(tmp.queues_);
      return *this;
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return queue_get_config_reply_.header;
    }

    auto port_no() const noexcept
      -> std::uint16_t
    {
      return queue_get_config_reply_.port;
    }

    auto queues() const noexcept
      -> queues_type const&
    {
      return queues_;
    }

    auto extract_queues()
      -> queues_type
    {
      auto queues = queues_type{};
      queues.swap(queues_);
      queue_get_config_reply_.header.length = sizeof(ofp_type);
      return queues;
    }

  private:
    friend basic_message;

    static constexpr bool is_fixed_length_message = false;

    friend basic_message::basic_protocol_type;

    queue_get_config_reply(
        ofp_type const& queue_get_config_reply, queues_type&& queues)
      : queue_get_config_reply_(queue_get_config_reply)
      , queues_(std::move(queues))
    {
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, queue_get_config_reply_);
      queues_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> queue_get_config_reply
    {
      auto const queue_get_config = detail::decode<ofp_type>(first, last);

      auto const queues_length
        = queue_get_config.header.length - sizeof(ofp_type);
      last = std::next(first, queues_length);

      auto queues = queues_type::decode(first, last);

      return queue_get_config_reply{queue_get_config, std::move(queues)};
    }

    auto equal_impl(queue_get_config_reply const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(queue_get_config_reply_, rhs.queue_get_config_reply_)
          && queues_ == rhs.queues_;
    }

  private:
    ofp_type queue_get_config_reply_;
    queues_type queues_;
  };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_QUEUE_CONFIG_HPP
