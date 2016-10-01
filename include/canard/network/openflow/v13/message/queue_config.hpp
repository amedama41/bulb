#ifndef CANARD_NET_OFP_V13_MESSAGES_QUEUE_CONFIG_HPP
#define CANARD_NET_OFP_V13_MESSAGES_QUEUE_CONFIG_HPP

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
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/common/packet_queue.hpp>
#include <canard/network/openflow/v13/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class queue_get_config_request
        : public detail::v13::basic_openflow_message<queue_get_config_request>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_QUEUE_GET_CONFIG_REQUEST;

        using raw_ofp_type = v13_detail::ofp_queue_get_config_request;

        explicit queue_get_config_request(
                  std::uint32_t const port_no
                , std::uint32_t const xid = get_xid()) noexcept
            : queue_get_config_request_{
                  v13_detail::ofp_header{
                      version()
                    , type()
                    , sizeof(raw_ofp_type)
                    , xid
                  }
                , port_no
                , { 0, 0, 0, 0 }
              }
        {
        }

        auto header() const noexcept
            -> v13_detail::ofp_header const&
        {
            return queue_get_config_request_.header;
        }

        auto port_no() const noexcept
            -> std::uint32_t
        {
            return queue_get_config_request_.port;
        }

    private:
        explicit queue_get_config_request(
                raw_ofp_type const& queue_get_config_request) noexcept
            : queue_get_config_request_(queue_get_config_request)
        {
        }

        friend basic_protocol_type;

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

    private:
        raw_ofp_type queue_get_config_request_;
    };


    class queue_get_config_reply
        : public detail::v13::basic_openflow_message<queue_get_config_reply>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_QUEUE_GET_CONFIG_REPLY;

        using raw_ofp_type = v13_detail::ofp_queue_get_config_reply;
        using queues_type = std::vector<packet_queue>;
        using iterator = queues_type::const_iterator;
        using const_iterator = queues_type::const_iterator;

        queue_get_config_reply(
                  std::uint32_t const port_no
                , queues_type queues
                , std::uint32_t const xid = get_xid())
            : queue_get_config_reply_{
                  v13_detail::ofp_header{
                      version()
                    , type()
                    , std::uint16_t(
                            sizeof(raw_ofp_type) + calc_queues_length(queues))
                    , xid
                  }
                , port_no
                , { 0, 0, 0, 0 }
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
            -> v13_detail::ofp_header const&
        {
            return queue_get_config_reply_.header;
        }

        auto port_no() const noexcept
            -> std::uint32_t
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
            queue_get_config_reply_.header.length = min_length();
            return queues;
        }

        auto begin() const noexcept
            -> const_iterator
        {
            return queues_.begin();
        }

        auto end() const noexcept
            -> const_iterator
        {
            return queues_.end();
        }

    private:
        queue_get_config_reply(
                  raw_ofp_type const& queue_get_config_reply
                , queues_type&& queues)
            : queue_get_config_reply_(queue_get_config_reply)
            , queues_(std::move(queues))
        {
        }

        static auto calc_queues_length(queues_type const& queues)
            -> std::size_t
        {
            auto const queues_length = boost::accumulate(
                      queues, std::size_t{0}
                    , [](std::size_t const sum, queues_type::const_reference q)
                      { return sum + q.length(); });
            if (queues_length + sizeof(raw_ofp_type)
                    > std::numeric_limits<std::uint16_t>::max()) {
                throw std::runtime_error{"queues length is too big"};
            }
            return queues_length;
        }

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, queue_get_config_reply_);
            boost::for_each(
                      queues_
                    , [&](queues_type::const_reference q)
                      { q.encode(container); });
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
            queues.reserve(
                    queues_length / sizeof(v13_detail::ofp_packet_queue));
            while (std::distance(first, last)
                    >= sizeof(v13_detail::ofp_packet_queue)) {
                queues.push_back(queues_type::value_type::decode(first, last));
            }
            if (first != last) {
                throw std::runtime_error{
                    "invalid queue_get_config_reply length"
                };
            }
            return queue_get_config_reply{queue_get_config, std::move(queues)};
        }

    private:
        raw_ofp_type queue_get_config_reply_;
        queues_type queues_;
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_QUEUE_CONFIG_HPP
