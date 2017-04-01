#ifndef CANARD_NET_OFP_V10_MESSAGES_PACKET_OUT_HPP
#define CANARD_NET_OFP_V10_MESSAGES_PACKET_OUT_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/network/openflow/data_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v10/exception.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/action_list.hpp>
#include <canard/network/openflow/v10/detail/basic_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class packet_out
        : public v10_detail::basic_message<packet_out>
    {
    public:
        using raw_ofp_type = protocol::ofp_packet_out;
        using data_type = ofp::data_type;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PACKET_OUT;

    private:
        packet_out(std::uint16_t const length
                 , std::uint32_t const buffer_id
                 , std::uint16_t const in_port
                 , action_list&& actions
                 , data_type&& data
                 , std::uint32_t const xid)
            : packet_out_{
                  protocol::ofp_header{
                        protocol::OFP_VERSION
                      , message_type
                      , length
                      , xid
                  }
                , buffer_id
                , in_port
                , std::uint16_t(length - (sizeof(raw_ofp_type) + data.size()))
              }
            , actions_(std::move(actions))
            , data_(std::move(data))
        {
        }

    public:
        packet_out(std::uint32_t const buffer_id
                 , std::uint16_t const in_port
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                  actions.calc_ofp_length(sizeof(raw_ofp_type))
                , buffer_id, in_port, std::move(actions), data_type{}, xid
              }
        {
        }

        packet_out(std::uint32_t const buffer_id
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                buffer_id, protocol::OFPP_NONE, std::move(actions), xid
              }
        {
        }

        packet_out(data_type data
                 , std::uint16_t const in_port
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                  actions.calc_ofp_length(
                          ofp::calc_ofp_length(data, sizeof(raw_ofp_type)))
                , protocol::OFP_NO_BUFFER
                , in_port
                , std::move(actions)
                , std::move(data)
                , xid
              }
        {
        }

        packet_out(data_type data
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                std::move(data), protocol::OFPP_NONE, std::move(actions), xid
              }
        {
        }

        packet_out(packet_out const&) = default;

        packet_out(packet_out&& other)
            : packet_out_(other.packet_out_)
            , actions_(other.extract_actions())
            , data_(std::move(other.data_))
        {
            other.packet_out_.header.length = sizeof(raw_ofp_type);
        }

        auto operator=(packet_out const& other)
            -> packet_out&
        {
            return operator=(packet_out{other});
        }

        auto operator=(packet_out&& other)
            -> packet_out&
        {
            auto tmp = std::move(other);
            std::swap(packet_out_, tmp.packet_out_);
            actions_.swap(tmp.actions_);
            data_.swap(tmp.data_);
            return *this;
        }

        auto header() const noexcept
            -> protocol::ofp_header const&
        {
            return packet_out_.header;
        }

        auto buffer_id() const noexcept
            -> std::uint32_t
        {
            return packet_out_.buffer_id;
        }

        auto in_port() const noexcept
            -> std::uint16_t
        {
            return packet_out_.in_port;
        }

        auto actions_length() const noexcept
            -> std::uint16_t
        {
            return packet_out_.actions_len;
        }

        auto actions() const noexcept
            -> action_list const&
        {
            return actions_;
        }

        auto extract_actions()
            -> action_list
        {
            auto actions = action_list{};
            actions.swap(actions_);
            packet_out_.header.length -= packet_out_.actions_len;
            packet_out_.actions_len = 0;
            return actions;
        }

        auto frame() const noexcept
            -> data_type const&
        {
            return data_;
        }

        auto extract_frame() noexcept
            -> data_type
        {
            auto data = data_type{};
            data.swap(data_);
            packet_out_.header.length -= data.size();
            return data;
        }

        auto frame_length() const noexcept
            -> std::uint16_t
        {
            return data_.size();
        }

    private:
        friend basic_message;

        static constexpr bool is_fixed_length_message = false;

        friend basic_message::basic_protocol_type;

        packet_out(raw_ofp_type const& pkt_out
                 , action_list&& actions
                 , data_type&& data)
            : packet_out_(pkt_out)
            , actions_(std::move(actions))
            , data_(std::move(data))
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, packet_out_);
            actions_.encode(container);
            detail::encode_byte_array(container, data_.data(), data_.size());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> packet_out
        {
            auto const pkt_out = detail::decode<raw_ofp_type>(first, last);
            auto const rest_size = pkt_out.header.length - sizeof(raw_ofp_type);
            if (rest_size < pkt_out.actions_len) {
                throw v10::exception{
                    protocol::OFPBRC_BAD_LEN, "too small data size for actions"
                } << CANARD_NET_OFP_ERROR_INFO();
            }

            auto const actions_last = std::next(first, pkt_out.actions_len);
            auto actions = action_list::decode(first, actions_last);

            auto const data_length = rest_size - pkt_out.actions_len;
            auto data = ofp::decode_data(first, data_length);

            return packet_out{pkt_out, std::move(actions), std::move(data)};
        }

        auto equal_impl(packet_out const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(packet_out_, rhs.packet_out_)
                && actions_ == rhs.actions_
                && frame() == rhs.frame();
        }

    private:
        raw_ofp_type packet_out_;
        action_list actions_;
        data_type data_;
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_PACKET_OUT_HPP
