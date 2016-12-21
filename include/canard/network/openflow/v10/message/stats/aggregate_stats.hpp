#ifndef CANARD_NET_OFP_V10_MESSAGES_STATISTICS_AGGREGATE_STATS_HPP
#define CANARD_NET_OFP_V10_MESSAGES_STATISTICS_AGGREGATE_STATS_HPP

#include <cstdint>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/common/match.hpp>
#include <canard/network/openflow/v10/detail/basic_stats.hpp>
#include <canard/network/openflow/v10/flow_entry.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {
namespace statistics {

    class aggregate_stats_request
        : public stats_detail::basic_stats_request<
              aggregate_stats_request, v10_detail::ofp_aggregate_stats_request
          >
    {
    public:
        static constexpr protocol::ofp_stats_types stats_type_value
            = protocol::OFPST_AGGREGATE;

        aggregate_stats_request(
                  v10::match const& match
                , std::uint8_t const table_id
                , std::uint16_t const out_port = protocol::OFPP_NONE
                , std::uint32_t const xid = get_xid()) noexcept
            : basic_stats_request{
                  0
                , raw_ofp_stats_type{
                      match.ofp_match(), table_id, 0, out_port
                  }
                , xid
              }
        {
        }

        auto match() const noexcept
            -> v10::match
        {
            return v10::match{body().match};
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return body().table_id;
        }

        auto out_port() const noexcept
            -> std::uint16_t
        {
            return body().out_port;
        }

    private:
        friend basic_stats_request::base_type;

        aggregate_stats_request(
                  raw_ofp_type const& stats_request
                , raw_ofp_stats_type const& aggregate_stats_request) noexcept
            : basic_stats_request{stats_request, aggregate_stats_request}
        {
        }
    };


    class aggregate_stats_reply
        : public stats_detail::basic_stats_reply<
              aggregate_stats_reply, v10_detail::ofp_aggregate_stats_reply
          >
    {
    public:
        static constexpr protocol::ofp_stats_types stats_type_value
            = protocol::OFPST_AGGREGATE;

        aggregate_stats_reply(
                  v10::counters const& counters
                , std::uint32_t const flow_count
                , std::uint32_t const xid = get_xid()) noexcept
            : basic_stats_reply{
                  0
                , raw_ofp_stats_type{
                      counters.packet_count()
                    , counters.byte_count()
                    , flow_count
                    , { 0, 0, 0, 0 }
                  }
                , xid
              }
        {
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
        friend basic_stats_reply::base_type;

        aggregate_stats_reply(
                  raw_ofp_type const& stats_reply
                , raw_ofp_stats_type const& aggregate_stats_reply) noexcept
            : basic_stats_reply{stats_reply, aggregate_stats_reply}
        {
        }
    };

} // namespace statistics
} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_STATISTICS_AGGREGATE_STATS_HPP
