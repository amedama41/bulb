#ifndef CANARD_NET_OFP_V10_MESSAGES_STATISTICS_PORT_STATS_HPP
#define CANARD_NET_OFP_V10_MESSAGES_STATISTICS_PORT_STATS_HPP

#include <cstddef>
#include <cstdint>
#include <utility>
#include <canard/network/protocol/openflow/detail/decode.hpp>
#include <canard/network/protocol/openflow/detail/encode.hpp>
#include <canard/network/protocol/openflow/get_xid.hpp>
#include <canard/network/protocol/openflow/v10/detail/byteorder.hpp>
#include <canard/network/protocol/openflow/v10/detail/basic_stats.hpp>
#include <canard/network/protocol/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {
namespace statistics {

    class port_stats
    {
    public:
        static std::uint16_t const base_size
            = sizeof(v10_detail::ofp_port_stats);

        port_stats(std::uint16_t const port_no
                 , std::uint64_t const rx_packets
                 , std::uint64_t const tx_packets
                 , std::uint64_t const rx_bytes
                 , std::uint64_t const tx_bytes
                 , std::uint64_t const rx_dropped
                 , std::uint64_t const tx_dropped
                 , std::uint64_t const rx_errors
                 , std::uint64_t const tx_errors
                 , std::uint64_t const rx_frame_err
                 , std::uint64_t const rx_over_err
                 , std::uint64_t const rx_crc_err
                 , std::uint64_t const collisions) noexcept
            : port_stats_{
                  port_no
                , { 0, 0, 0, 0, 0, 0 }
                , rx_packets
                , tx_packets
                , rx_bytes
                , tx_bytes
                , rx_dropped
                , tx_dropped
                , rx_errors
                , tx_errors
                , rx_frame_err
                , rx_over_err
                , rx_crc_err
                , collisions
              }
        {
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(v10_detail::ofp_port_stats);
        }

        auto port_no() const noexcept
            -> std::uint16_t
        {
            return port_stats_.port_no;
        }

        auto rx_packets() const noexcept
            -> std::uint64_t
        {
            return port_stats_.rx_packets;
        }

        auto tx_packets() const noexcept
            -> std::uint64_t
        {
            return port_stats_.tx_packets;
        }

        auto rx_bytes() const noexcept
            -> std::uint64_t
        {
            return port_stats_.rx_bytes;
        }

        auto tx_bytes() const noexcept
            -> std::uint64_t
        {
            return port_stats_.tx_bytes;
        }

        auto rx_dropped() const noexcept
            -> std::uint64_t
        {
            return port_stats_.rx_dropped;
        }

        auto tx_dropped() const noexcept
            -> std::uint64_t
        {
            return port_stats_.tx_dropped;
        }

        auto rx_errors() const noexcept
            -> std::uint64_t
        {
            return port_stats_.rx_errors;
        }

        auto tx_errors() const noexcept
            -> std::uint64_t
        {
            return port_stats_.tx_errors;
        }

        auto rx_frame_errors() const noexcept
            -> std::uint64_t
        {
            return port_stats_.rx_frame_err;
        }

        auto rx_over_errors() const noexcept
            -> std::uint64_t
        {
            return port_stats_.rx_over_err;
        }

        auto rx_crc_errors() const noexcept
            -> std::uint64_t
        {
            return port_stats_.rx_crc_err;
        }

        auto collisions() const noexcept
            -> std::uint64_t
        {
            return port_stats_.collisions;
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            return detail::encode(container, port_stats_);
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> port_stats
        {
            return port_stats{
                detail::decode<v10_detail::ofp_port_stats>(first, last)
            };
        }

    private:
        explicit port_stats(
                v10_detail::ofp_port_stats const& port_stats) noexcept
            : port_stats_(port_stats)
        {
        }

    private:
        v10_detail::ofp_port_stats port_stats_;
    };


    class port_stats_request
        : public stats_detail::basic_stats_request<
                port_stats_request, v10_detail::ofp_port_stats_request
          >
    {
    public:
        static constexpr protocol::ofp_stats_types stats_type_value
            = protocol::OFPST_PORT;

        explicit port_stats_request(
                  std::uint16_t const port_no
                , std::uint32_t const xid = get_xid()) noexcept
            : basic_stats_request{
                  0
                , v10_detail::ofp_port_stats_request{
                    port_no, { 0, 0, 0, 0, 0, 0 }
                  }
                , xid
              }
        {
        }

        auto port_no() const noexcept
            -> std::uint16_t
        {
            return body().port_no;
        }

    private:
        friend basic_stats_request::base_type;

        port_stats_request(
                  v10_detail::ofp_stats_request const& stats_request
                , v10_detail::ofp_port_stats_request const& port_stats_request) noexcept
            : basic_stats_request{stats_request, port_stats_request}
        {
        }
    };


    class port_stats_reply
        : public stats_detail::basic_stats_reply<
                port_stats_reply, port_stats[]
          >
    {
    public:
        static constexpr protocol::ofp_stats_types stats_type_value
            = protocol::OFPST_PORT;

        explicit port_stats_reply(
                  body_type port_stats
                , std::uint16_t const flags = 0
                , std::uint32_t const xid = get_xid())
            : basic_stats_reply{flags, std::move(port_stats), xid}
        {
        }

    private:
        friend basic_stats_reply::base_type;

        port_stats_reply(
                  v10_detail::ofp_stats_reply const& stats_reply
                , body_type&& port_stats)
            : basic_stats_reply{stats_reply, std::move(port_stats)}
        {
        }
    };

} // namespace statistics
} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_STATISTICS_PORT_STATS_HPP
