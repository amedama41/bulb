#ifndef CANARD_NET_OFP_V10_MESSAGES_PACKET_OUT_HPP
#define CANARD_NET_OFP_V10_MESSAGES_PACKET_OUT_HPP

#include <cstdint>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include <boost/operators.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/iterator_range.hpp>
#include <canard/network/openflow/binary_data.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/action_list.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class packet_out
        : public v10_detail::basic_openflow_message<packet_out>
        , private boost::equality_comparable<packet_out>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_packet_out;
        using data_type = binary_data::pointer_type;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_PACKET_OUT;

    private:
        packet_out(std::uint32_t const buffer_id
                 , std::uint16_t const in_port
                 , action_list&& actions
                 , binary_data&& data
                 , std::uint32_t const xid)
            : packet_out_{
                  v10_detail::ofp_header{
                        protocol::OFP_VERSION
                      , message_type
                      , std::uint16_t(
                                sizeof(raw_ofp_type)
                              + actions.length() + data.size())
                      , xid
                  }
                , buffer_id
                , in_port
                , std::uint16_t(actions.length())
              }
            , actions_(std::move(actions))
            , data_(std::move(data).data())
        {
        }

    public:
        packet_out(std::uint32_t const buffer_id
                 , std::uint16_t const in_port
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                buffer_id, in_port, std::move(actions), binary_data{}, xid
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

        packet_out(binary_data data
                 , std::uint16_t const in_port
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                  protocol::OFP_NO_BUFFER
                , in_port
                , std::move(actions)
                , std::move(data)
                , xid
              }
        {
        }

        packet_out(binary_data data
                 , action_list actions
                 , std::uint32_t const xid = get_xid())
            : packet_out{
                std::move(data), protocol::OFPP_NONE, std::move(actions), xid
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
            -> v10_detail::ofp_header const&
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
            -> boost::iterator_range<unsigned char const*>
        {
            return boost::make_iterator_range_n(data_.get(), frame_length());
        }

        auto extract_frame()
            -> binary_data
        {
            auto data = binary_data{std::move(data_), frame_length()};
            packet_out_.header.length -= data.size();
            return data;
        }

        auto frame_length() const noexcept
            -> std::uint16_t
        {
            return length() - sizeof(raw_ofp_type) - actions_length();
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            detail::encode(container, packet_out_);
            actions_.encode(container);
            return detail::encode_byte_array(
                    container, data_.get(), frame_length());
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
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

        static void validate(v10_detail::ofp_header const& header)
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

        friend auto operator==(packet_out const&, packet_out const&) noexcept
            -> bool;

    private:
        packet_out(raw_ofp_type const& pkt_out
                 , action_list&& actions
                 , data_type&& data)
            : packet_out_(pkt_out)
            , actions_(std::move(actions))
            , data_(std::move(data))
        {
        }

        auto equal_impl(packet_out const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(packet_out_, rhs.packet_out_)
                && actions_ == rhs.actions_
                && boost::equal(frame(), rhs.frame());
        }

    private:
        raw_ofp_type packet_out_;
        action_list actions_;
        data_type data_;
    };

    inline auto operator==(
            packet_out const& lhs, packet_out const& rhs) noexcept
        -> bool
    {
        return lhs.equal_impl(rhs);
    }

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_PACKET_OUT_HPP
