#ifndef CANARD_NET_OFP_V10_MESSAGES_QUEUE_CONFIG_HPP
#define CANARD_NET_OFP_V10_MESSAGES_QUEUE_CONFIG_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/numeric.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/common/packet_queue.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class queue_get_config_request
        : public v10_detail::basic_openflow_message<queue_get_config_request>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_queue_get_config_request;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_QUEUE_GET_CONFIG_REQUEST;

        explicit queue_get_config_request(
                  std::uint16_t const port_no
                , std::uint32_t const xid = get_xid()) noexcept
            : queue_get_config_request_{
                  v10_detail::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
                    , sizeof(raw_ofp_type)
                    , xid
                  }
                , port_no
                , { 0, 0 }
              }
        {
        }

        auto header() const noexcept
            -> v10_detail::ofp_header const&
        {
            return queue_get_config_request_.header;
        }

        auto port_no() const noexcept
            -> std::uint16_t
        {
            return queue_get_config_request_.port;
        }

        static void validate_header(v10_detail::ofp_header const& header)
        {
            if (header.version != protocol::OFP_VERSION) {
                throw std::runtime_error{"invalid version"};
            }
            if (header.type != message_type) {
                throw std::runtime_error{"invalid message type"};
            }
            if (header.length != sizeof(raw_ofp_type)) {
                throw std::runtime_error{"invalid length"};
            }
        }

    private:
        friend basic_openflow_message::basic_protocol_type;

        explicit queue_get_config_request(
                raw_ofp_type const& queue_get_config_request) noexcept
            : queue_get_config_request_(queue_get_config_request)
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, queue_get_config_request_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> queue_get_config_request
        {
            return queue_get_config_request{
                detail::decode<raw_ofp_type>(first, last)
            };
        }

        auto equal_impl(queue_get_config_request const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(
                    queue_get_config_request_, rhs.queue_get_config_request_);
        }

    private:
        raw_ofp_type queue_get_config_request_;
    };


    class queue_get_config_reply
        : public v10_detail::basic_openflow_message<queue_get_config_reply>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_queue_get_config_reply;
        using queues_type = std::vector<packet_queue>;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_QUEUE_GET_CONFIG_REPLY;

        queue_get_config_reply(
                  std::uint16_t const port_no
                , queues_type queues
                , std::uint32_t const xid = get_xid())
            : queue_get_config_reply_{
                  v10_detail::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
                    , std::uint16_t(
                            sizeof(raw_ofp_type) + calc_queues_length(queues))
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
            -> v10_detail::ofp_header const&
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
            queue_get_config_reply_.header.length = sizeof(raw_ofp_type);
            return queues;
        }

        static void validate_header(v10_detail::ofp_header const& header)
        {
            if (header.version != protocol::OFP_VERSION) {
                throw std::runtime_error{"invalid version"};
            }
            if (header.type != message_type) {
                throw std::runtime_error{"invalid message type"};
            }
            if (header.length < sizeof(raw_ofp_type)) {
                throw std::runtime_error{"invalid length"};
            }
        }

    private:
        friend basic_openflow_message::basic_protocol_type;

        queue_get_config_reply(
                  raw_ofp_type const& queue_get_config_reply
                , queues_type&& queues)
            : queue_get_config_reply_(queue_get_config_reply)
            , queues_(std::move(queues))
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, queue_get_config_reply_);
            boost::for_each(queues_, [&](queues_type::const_reference e) {
                e.encode(container);
            });
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> queue_get_config_reply
        {
            auto const queue_get_config
                = detail::decode<raw_ofp_type>(first, last);

            auto const queues_length
                = queue_get_config.header.length - sizeof(raw_ofp_type);
            last = std::next(first, queues_length);

            auto queues = queues_type{};
            constexpr auto min_queue_length
                = queues_type::value_type::min_length();
            queues.reserve(queues_length / min_queue_length);
            while (std::distance(first, last) >= min_queue_length) {
                queues.push_back(packet_queue::decode(first, last));
            }
            if (first != last) {
                throw std::runtime_error{
                    "invalid queue_get_config_reply length"
                };
            }
            return queue_get_config_reply{queue_get_config, std::move(queues)};
        }

        auto equal_impl(queue_get_config_reply const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(
                    queue_get_config_reply_, rhs.queue_get_config_reply_)
                && queues_ == rhs.queues_;
        }

        static auto calc_queues_length(queues_type const& queues)
            -> std::size_t
        {
            auto const queues_length = boost::accumulate(
                      queues, std::size_t{0}
                    , [](std::size_t const sum, queues_type::const_reference e)
                      { return sum + e.length(); });
            if (queues_length + sizeof(raw_ofp_type)
                    > std::numeric_limits<std::uint16_t>::max()) {
                throw std::runtime_error{"queues length is too big"};
            }
            return queues_length;
        }

    private:
        raw_ofp_type queue_get_config_reply_;
        queues_type queues_;
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_QUEUE_CONFIG_HPP
