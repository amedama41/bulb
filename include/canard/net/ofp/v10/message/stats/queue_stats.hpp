#ifndef CANARD_NET_OFP_V10_MESSAGES_STATISTICS_QUEUE_STATS_HPP
#define CANARD_NET_OFP_V10_MESSAGES_STATISTICS_QUEUE_STATS_HPP

#include <cstddef>
#include <cstdint>
#include <utility>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v10/detail/basic_stats.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {
namespace statistics {

  class queue_stats
    : public detail::basic_protocol_type<queue_stats>
  {
  public:
    using ofp_type = protocol::ofp_queue_stats;

    queue_stats(
          std::uint32_t const queue_id
        , std::uint16_t const port_no
        , std::uint64_t const tx_packets
        , std::uint64_t const tx_bytes
        , std::uint64_t const tx_errors) noexcept
      : queue_stats_{
            port_no
          , { 0, 0 }
          , queue_id
          , tx_bytes
          , tx_packets
          , tx_errors
        }
    {
    }

    static constexpr auto length() noexcept
      -> std::uint16_t
    {
      return sizeof(ofp_type);
    }

    auto queue_id() const noexcept
      -> std::uint32_t
    {
      return queue_stats_.queue_id;
    }

    auto port_no() const noexcept
      -> std::uint16_t
    {
      return queue_stats_.port_no;
    }

    auto tx_packets() const noexcept
      -> std::uint64_t
    {
      return queue_stats_.tx_packets;
    }

    auto tx_bytes() const noexcept
      -> std::uint64_t
    {
      return queue_stats_.tx_bytes;
    }

    auto tx_errors() const noexcept
      -> std::uint64_t
    {
      return queue_stats_.tx_errors;
    }

  private:
    explicit queue_stats(ofp_type const& queue_stats) noexcept
      : queue_stats_(queue_stats)
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, queue_stats_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> queue_stats
    {
      return queue_stats{detail::decode<ofp_type>(first, last)};
    }

    auto equal_impl(queue_stats const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(queue_stats_, rhs.queue_stats_);
    }

  private:
    ofp_type queue_stats_;
  };


  class queue_stats_request
    : public stats_detail::basic_stats_request<
        queue_stats_request, protocol::ofp_queue_stats_request
      >
  {
  public:
    static constexpr protocol::ofp_stats_types stats_type_value
      = protocol::OFPST_QUEUE;

    explicit queue_stats_request(
          std::uint32_t const queue_id
        , std::uint16_t const port_no
        , std::uint32_t const xid = get_xid()) noexcept
      : basic_stats_request{0, body_type{port_no, { 0, 0 }, queue_id}, xid}
    {
    }

    auto queue_id() const noexcept
      -> std::uint32_t
    {
      return body().queue_id;
    }

    auto port_no() const noexcept
      -> std::uint16_t
    {
      return body().port_no;
    }

  private:
    friend basic_stats_request::base_type;

    queue_stats_request(
          ofp_type const& stats_request
        , body_type const& queue_stats_request) noexcept
      : basic_stats_request{stats_request, queue_stats_request}
    {
    }
  };


  class queue_stats_reply
    : public stats_detail::basic_stats_reply<queue_stats_reply, queue_stats[]>
  {
  public:
    static constexpr protocol::ofp_stats_types stats_type_value
      = protocol::OFPST_QUEUE;

    queue_stats_reply(
          body_type queue_stats
        , std::uint16_t const flags = 0
        , std::uint32_t const xid = get_xid())
      : basic_stats_reply{flags, std::move(queue_stats), xid}
    {
    }

  private:
    friend basic_stats_reply::base_type;

    queue_stats_reply(ofp_type const& stats_reply, body_type&& queue_stats)
      : basic_stats_reply{stats_reply, std::move(queue_stats)}
    {
    }

    static constexpr auto is_valid_stats_body_length(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length % body_type::value_type::min_length() == 0;
    }
  };

} // namespace statistics
} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_STATISTICS_QUEUE_STATS_HPP
