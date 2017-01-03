#ifndef CANARD_NET_OFP_V10_MESSAGES_PACKET_IN_HPP
#define CANARD_NET_OFP_V10_MESSAGES_PACKET_IN_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <boost/container/vector.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class packet_in
        : public v10_detail::basic_openflow_message<packet_in>
    {
        static constexpr std::uint16_t min_pkt_in_len
            = offsetof(v10_detail::ofp_packet_in, pad)
            + sizeof(v10_detail::ofp_packet_in::pad);

        friend constexpr auto get_min_length(packet_in*) noexcept
            -> std::uint16_t
        {
            return packet_in::min_pkt_in_len;
        }

    public:
        using raw_ofp_type = v10_detail::ofp_packet_in;
        using data_type = boost::container::vector<std::uint8_t>;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PACKET_IN;

        packet_in(data_type data
                , std::uint16_t const total_len
                , std::uint16_t const in_port
                , protocol::ofp_packet_in_reason const reason
                , std::uint32_t const buffer_id
                , std::uint32_t const xid = get_xid()) noexcept
            : packet_in_{
                  v10_detail::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
                    , calc_ofp_length(data)
                    , xid
                  }
                , buffer_id
                , total_len
                , in_port
                , std::uint8_t(reason)
                , 0
              }
            , data_(std::move(data))
        {
        }

        packet_in(packet_in const&) = default;

        packet_in(packet_in&& other) noexcept
            : packet_in_(other.packet_in_)
            , data_(std::move(other).data_)
        {
            other.packet_in_.header.length = min_pkt_in_len;
        }

        auto operator=(packet_in const& other)
            -> packet_in&
        {
            return operator=(packet_in{other});
        }

        auto operator=(packet_in&& other) noexcept
            -> packet_in&
        {
            auto pkt_in = std::move(other);
            std::swap(packet_in_, pkt_in.packet_in_);
            data_.swap(pkt_in.data_);
            return *this;
        }

        auto header() const noexcept
            -> v10_detail::ofp_header const&
        {
            return packet_in_.header;
        }

        auto buffer_id() const noexcept
            -> std::uint32_t
        {
            return packet_in_.buffer_id;
        }

        auto total_length() const noexcept
            -> std::uint16_t
        {
            return packet_in_.total_len;
        }

        auto in_port() const noexcept
            -> std::uint16_t
        {
            return packet_in_.in_port;
        }

        auto reason() const noexcept
            -> protocol::ofp_packet_in_reason
        {
            return protocol::ofp_packet_in_reason(packet_in_.reason);
        }

        auto frame() const noexcept
            -> data_type const&
        {
            return data_;
        }

        auto frame_length() const noexcept
            -> std::uint16_t
        {
            return data_.size();
        }

        auto extract_frame() noexcept
            -> data_type
        {
            auto data = data_type{};
            data.swap(data_);
            packet_in_.header.length = min_pkt_in_len;
            return data;
        }

        static void validate_header(v10_detail::ofp_header const& header)
        {
            if (header.version != protocol::OFP_VERSION) {
                throw std::runtime_error{"invalid version"};
            }
            if (header.type != message_type) {
                throw std::runtime_error{"invalid message type"};
            }
            if (header.length < min_pkt_in_len) {
                throw std::runtime_error{"invalid length"};
            }
        }

    private:
        friend basic_openflow_message::basic_protocol_type;

        packet_in(raw_ofp_type const& pkt_in, data_type&& data) noexcept
            : packet_in_(pkt_in)
            , data_(std::move(data))
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, packet_in_, min_pkt_in_len);
            detail::encode_byte_array(container, data_.data(), data_.size());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> packet_in
        {
            auto pkt_in
                = detail::decode<raw_ofp_type>(first, last, min_pkt_in_len);

            auto const data_length
                = std::uint16_t(pkt_in.header.length - min_pkt_in_len);
            last = std::next(first, data_length);

            auto data = data_type{data_length, boost::container::default_init};
            std::copy(first, last, data.data());
            first = last;

            return packet_in{pkt_in, std::move(data)};
        }

        auto equal_impl(packet_in const& rhs) const noexcept
            -> bool
        {
            return std::memcmp(
                    &packet_in_, &rhs.packet_in_, min_pkt_in_len) == 0
                && frame() == rhs.frame();
        }

        static auto calc_ofp_length(data_type const& data)
            -> std::uint16_t
        {
            constexpr auto max_length
                = std::numeric_limits<std::uint16_t>::max();
            if (data.size() > max_length - min_pkt_in_len) {
                throw std::runtime_error{"too large packet_in data length"};
            }
            return min_pkt_in_len + data.size();
        }

    public:
        raw_ofp_type packet_in_;
        data_type data_;
    };

} // namespace message
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_PACKET_IN_HPP
