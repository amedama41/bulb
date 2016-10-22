#ifndef CANARD_NET_OFP_V13_MESSAGES_PACKET_OUT_HPP
#define CANARD_NET_OFP_V13_MESSAGES_PACKET_OUT_HPP

#include <cstdint>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <boost/range/iterator_range.hpp>
#include <canard/network/openflow/binary_data.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/action_list.hpp>
#include <canard/network/openflow/v13/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class packet_out
        : public detail::v13::basic_openflow_message<packet_out>
    {
    public:
        using raw_ofp_type = v13_detail::ofp_packet_out;
        using data_type = binary_data::pointer_type;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PACKET_OUT;

    private:
        packet_out(std::uint16_t const length
                 , std::uint32_t const buffer_id
                 , std::uint32_t const in_port
                 , action_list&& actions
                 , binary_data&& data
                 , std::uint32_t const xid)
            : packet_out_{
                  v13_detail::ofp_header{
                        version()
                      , type()
                      , length
                      , xid
                  }
                , buffer_id
                , in_port
                , std::uint16_t(length - (sizeof(raw_ofp_type) + data.size()))
                , { 0, 0, 0, 0, 0, 0 }
              }
            , actions_(std::move(actions))
            , data_(std::move(data).data())
        {
        }

    public:
        packet_out(std::uint32_t const buffer_id
                 , std::uint32_t const in_port
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                  actions.calc_ofp_length(sizeof(raw_ofp_type))
                , buffer_id, in_port, std::move(actions), binary_data{}, xid
              }
        {
        }

        packet_out(binary_data data
                 , std::uint32_t const in_port
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                  calc_packet_out_length(data, actions)
                , protocol::OFP_NO_BUFFER
                , in_port
                , std::move(actions)
                , std::move(data)
                , xid
              }
        {
        }

        packet_out(packet_out const& other)
            : packet_out_(other.packet_out_)
            , actions_(other.actions_)
            , data_(binary_data::copy_data(other.frame()))
        {
        }

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
            packet_out_ = tmp.packet_out_;
            actions_.swap(tmp.actions_);
            data_.swap(tmp.data_);
            return *this;
        }

        auto header() const noexcept
            -> v13_detail::ofp_header const&
        {
            return packet_out_.header;
        }

        auto buffer_id() const noexcept
            -> std::uint32_t
        {
            return packet_out_.buffer_id;
        }

        auto in_port() const noexcept
            -> std::uint32_t
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
            -> boost::iterator_range<unsigned char const*>
        {
            return boost::make_iterator_range_n(data_.get(), frame_length());
        }

        auto extract_frame() noexcept
            -> binary_data
        {
            auto data = binary_data{std::move(data_), frame_length()};
            packet_out_.header.length = sizeof(raw_ofp_type) + actions_length();
            return data;
        }

        auto frame_length() const noexcept
            -> std::uint16_t
        {
            return length() - sizeof(raw_ofp_type) - actions_length();
        }

    private:
        packet_out(raw_ofp_type const& pkt_out
                 , action_list&& actions
                 , data_type&& data)
            : packet_out_(pkt_out)
            , actions_(std::move(actions))
            , data_(std::move(data))
        {
        }

        static auto calc_packet_out_length(
                binary_data const& data, action_list const& actions)
            -> std::uint16_t
        {
            constexpr auto max_length
                = std::numeric_limits<std::uint16_t>::max();
            if (data.size() > max_length - sizeof(raw_ofp_type)) {
                throw std::runtime_error{"too large data length"};
            }
            return actions.calc_ofp_length(sizeof(raw_ofp_type) + data.size());
        }

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, packet_out_);
            actions_.encode(container);
            detail::encode_byte_array(container, data_.get(), frame_length());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> packet_out
        {
            auto const pkt_out = detail::decode<raw_ofp_type>(first, last);
            last = std::next(
                    first, pkt_out.header.length - sizeof(raw_ofp_type));
            if (std::distance(first, last) < pkt_out.actions_len) {
                throw std::runtime_error{"invalid actions length"};
            }

            auto const actions_last = std::next(first, pkt_out.actions_len);
            auto actions = action_list::decode(first, actions_last);

            auto data = binary_data::copy_data(first, last);
            first = last;
            return packet_out{pkt_out, std::move(actions), std::move(data)};
        }

    private:
        raw_ofp_type packet_out_;
        action_list actions_;
        data_type data_;
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_PACKET_OUT_HPP
