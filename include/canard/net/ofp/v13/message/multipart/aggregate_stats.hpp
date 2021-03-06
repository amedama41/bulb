#ifndef CANARD_NET_OFP_V13_MESSAGES_MULTIPART_AGGREGATE_STATS_HPP
#define CANARD_NET_OFP_V13_MESSAGES_MULTIPART_AGGREGATE_STATS_HPP

#include <cstdint>
#include <utility>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/common/oxm_match.hpp>
#include <canard/net/ofp/v13/flow_entry.hpp>
#include <canard/net/ofp/v13/detail/basic_multipart.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

  class aggregate_stats_request
    : public multipart_detail::basic_multipart_request<
          aggregate_stats_request
        , protocol::ofp_aggregate_stats_request, true
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_AGGREGATE;

    aggregate_stats_request(
          oxm_match match
        , std::uint8_t const table_id
        , v13::cookie_mask const& cookie_mask
        , std::uint32_t const out_port = protocol::OFPP_ANY
        , std::uint32_t const out_group = protocol::OFPG_ANY
        , std::uint32_t const xid = get_xid())
      : basic_multipart_request{
            0
          , body_type{
                table_id
              , { 0, 0, 0 }
              , out_port
              , out_group
              , { 0, 0, 0, 0 }
              , cookie_mask.value()
              , cookie_mask.mask()
            }
          , std::move(match)
          , xid
        }
    {
    }

    aggregate_stats_request(
          oxm_match match
        , std::uint8_t const table_id
        , std::uint32_t const out_port = protocol::OFPP_ANY
        , std::uint32_t const out_group = protocol::OFPG_ANY
        , std::uint32_t const xid = get_xid())
      : aggregate_stats_request{
            std::move(match)
          , table_id
          , v13::cookie_mask{0, 0}
          , out_port
          , out_group
          , xid
        }
    {
    }

    auto table_id() const noexcept
      -> std::uint8_t
    {
      return body().table_id;
    }

    auto cookie() const noexcept
      -> std::uint64_t
    {
      return body().cookie;
    }

    auto cookie_mask() const noexcept
      -> std::uint64_t
    {
      return body().cookie_mask;
    }

    auto out_port() const noexcept
      -> std::uint32_t
    {
      return body().out_port;
    }

    auto out_group() const noexcept
      -> std::uint32_t
    {
      return body().out_group;
    }

  private:
    friend basic_multipart_request::base_type;

    aggregate_stats_request(
          ofp_type const& multipart_request
        , body_type const& aggregate_stats_request
        , oxm_match&& match)
      : basic_multipart_request{
        multipart_request, aggregate_stats_request, std::move(match)
      }
    {
    }
  };


  class aggregate_stats_reply
    : public multipart_detail::basic_multipart_reply<
        aggregate_stats_reply, protocol::ofp_aggregate_stats_reply
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_AGGREGATE;

    aggregate_stats_reply(
          v13::counters const& counters
        , std::uint32_t const flow_count
        , std::uint32_t const xid = get_xid()) noexcept
      : basic_multipart_reply{
            0
          , body_type{
                counters.packet_count()
              , counters.byte_count()
              , flow_count
              , { 0, 0, 0, 0 }
            }
          , xid
        }
    {
    }

    auto counters() const noexcept
      -> v13::counters
    {
      return v13::counters{packet_count(), byte_count()};
    }

    auto packet_count() const noexcept
      -> std::uint64_t
    {
      return body().packet_count;
    }

    auto byte_count() const noexcept
      -> std::uint64_t
    {
      return body().byte_count;
    }

    auto flow_count() const noexcept
      -> std::uint32_t
    {
      return body().flow_count;
    }

  private:
    friend basic_multipart_reply::base_type;

    aggregate_stats_reply(
          ofp_type const& multipart_reply
        , body_type const& aggregate_stats_reply) noexcept
      : basic_multipart_reply{multipart_reply, aggregate_stats_reply}
    {
    }
  };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_MULTIPART_AGGREGATE_STATS_HPP
