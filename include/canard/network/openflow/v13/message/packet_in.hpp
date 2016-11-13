#ifndef CANARD_NET_OFP_V13_MESSAGES_PACKET_IN_HPP
#define CANARD_NET_OFP_V13_MESSAGES_PACKET_IN_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <boost/range/iterator_range.hpp>
#include <canard/network/openflow/binary_data.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/padding.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/common/oxm_match_set.hpp>
#include <canard/network/openflow/v13/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class packet_in
        : public detail::v13::basic_openflow_message<packet_in>
    {
        static constexpr std::uint16_t data_alignment_padding_size = 2;
        static constexpr std::uint16_t base_pkt_in_size
            = sizeof(v13_detail::ofp_packet_in)
            + v13_detail::exact_length(oxm_match_set::min_length())
            + data_alignment_padding_size;

    public:
        using raw_ofp_type = v13_detail::ofp_packet_in;
        using data_type = binary_data::pointer_type;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PACKET_IN;

        packet_in(std::uint32_t const buffer_id
                , std::uint16_t const total_len
                , protocol::ofp_packet_in_reason const reason
                , std::uint8_t const table_id
                , std::uint64_t const cookie
                , oxm_match_set match
                , binary_data data
                , std::uint32_t const xid = get_xid())
            : packet_in_{
                  v13_detail::ofp_header{
                      version()
                    , type()
                    , std::uint16_t(
                              sizeof(raw_ofp_type)
                            + v13_detail::exact_length(match.length())
                            + data_alignment_padding_size
                            + data.size())
                    , xid
                  }
                , buffer_id
                , total_len
                , std::uint8_t(reason)
                , table_id
                , cookie
              }
            , match_(std::move(match))
            , data_(std::move(data).data())
        {
        }

        template <class Range>
        packet_in(std::uint32_t const buffer_id
                , std::uint16_t const total_len
                , protocol::ofp_packet_in_reason const reason
                , std::uint8_t const table_id
                , std::uint64_t const cookie
                , oxm_match_set match
                , Range const& data
                , std::uint32_t const xid = get_xid())
            : packet_in{
                  buffer_id, total_len, reason, table_id, cookie
                , std::move(match)
                , binary_data{data}
                , xid
              }
        {
        }

        packet_in(packet_in const& other)
            : packet_in_(other.packet_in_)
            , match_(other.match_)
            , data_(binary_data::copy_data(other.frame()))
        {
        }

        packet_in(packet_in&& other) noexcept
            : packet_in_(other.packet_in_)
            , match_(other.extract_match())
            , data_(std::move(other.data_))
        {
            other.packet_in_.header.length = base_pkt_in_size;
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
            packet_in_ = pkt_in.packet_in_;
            match_.swap(pkt_in.match_);
            data_.swap(pkt_in.data_);
            return *this;
        }

        auto header() const noexcept
            -> v13_detail::ofp_header const&
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

        auto in_port() const
            -> std::uint32_t
        {
            return match().get<oxm_match_fields::in_port>().oxm_value();
        }

        auto reason() const noexcept
            -> protocol::ofp_packet_in_reason
        {
            return protocol::ofp_packet_in_reason(packet_in_.reason);
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return packet_in_.table_id;
        }

        auto cookie() const noexcept
            -> std::uint64_t
        {
            return packet_in_.cookie;
        }

        auto match() const noexcept
            -> oxm_match_set const&
        {
            return match_;
        }

        auto extract_match()
            -> oxm_match_set
        {
            auto const frame_len = frame_length();
            auto match = oxm_match_set{};
            match.swap(match_);
            packet_in_.header.length = base_pkt_in_size + frame_len;
            return match;
        }

        auto frame() const noexcept
            -> boost::iterator_range<unsigned char const*>
        {
            return boost::make_iterator_range_n(data_.get(), frame_length());
        }

        auto frame_length() const noexcept
            -> std::uint16_t
        {
            return length()
                 - sizeof(raw_ofp_type)
                 - v13_detail::exact_length(match().length())
                 - data_alignment_padding_size;
        }

        auto extract_frame() noexcept
            -> binary_data
        {
            auto const frame_len = frame_length();
            packet_in_.header.length -= frame_len;
            return binary_data{std::move(data_), frame_len};
        }

    private:
        packet_in(raw_ofp_type const& pkt_in
                , oxm_match_set&& match
                , data_type&& data)
            : packet_in_(pkt_in)
            , match_(std::move(match))
            , data_(std::move(data))
        {
        }

        friend basic_protocol_type;

        friend constexpr auto get_min_length(packet_in*) noexcept
            -> std::uint16_t
        {
            return packet_in::base_pkt_in_size;
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, packet_in_);
            match_.encode(container);
            detail::encode_byte_array(
                    container, detail::padding, data_alignment_padding_size);
            detail::encode_byte_array(container, data_.get(), frame_length());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> packet_in
        {
            auto const pkt_in = detail::decode<raw_ofp_type>(first, last);
            last = std::next(
                    first, pkt_in.header.length - sizeof(raw_ofp_type));

            auto copy_first = first;
            auto const ofp_match
                = detail::decode<v13_detail::ofp_match>(copy_first, last);
            oxm_match_set::validate_ofp_match(ofp_match);
            auto const match_length
                = v13_detail::exact_length(ofp_match.length);
            if (std::distance(first, last) - data_alignment_padding_size
                    < match_length) {
                throw std::runtime_error{"oxm_match length is too big"};
            }

            auto match = oxm_match_set::decode(first, last);

            std::advance(first, data_alignment_padding_size);

            auto data = binary_data::copy_data(first, last);
            first = last;

            return packet_in{pkt_in, std::move(match), std::move(data)};
        }

    private:
        raw_ofp_type packet_in_;
        oxm_match_set match_;
        data_type data_;
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_PACKET_IN_HPP
