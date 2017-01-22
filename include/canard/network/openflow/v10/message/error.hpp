#ifndef CANARD_NET_OFP_V10_MESSAGES_ERROR_HPP
#define CANARD_NET_OFP_V10_MESSAGES_ERROR_HPP

#include <cstdint>
#include <iterator>
#include <limits>
#include <utility>
#include <canard/network/openflow/data_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class error
        : public v10_detail::basic_openflow_message<error>
    {
    public:
        using raw_ofp_type = protocol::ofp_error_msg;
        using data_type = ofp::data_type;

        static constexpr protocol::ofp_type message_type = protocol::OFPT_ERROR;

        error(protocol::ofp_error_type const type
            , std::uint16_t const code
            , data_type data
            , std::uint32_t const xid = get_xid()) noexcept
            : error_msg_{
                  protocol::ofp_header{
                        protocol::OFP_VERSION
                      , message_type
                      , ofp::calc_ofp_length(data, sizeof(raw_ofp_type))
                      , xid
                  }
                , std::uint16_t(type)
                , code
              }
            , data_(std::move(data))
        {
        }

        template <class Message>
        error(protocol::ofp_error_type const type
            , std::uint16_t const code
            , Message const& msg
            , std::uint16_t const data_size
                = std::numeric_limits<std::uint16_t>::max())
            : error{type, code, create_data(msg, data_size), msg.xid()}
        {
        }

        error(error const&) = default;

        error(error&& other) noexcept
            : error_msg_(other.error_msg_)
            , data_(std::move(other.data_))
        {
            other.error_msg_.header.length = sizeof(raw_ofp_type);
        }

        auto operator=(error const& other)
            -> error&
        {
            return operator=(error{other});
        }

        auto operator=(error&& other) noexcept
            -> error&
        {
            auto tmp = std::move(other);
            std::swap(error_msg_, tmp.error_msg_);
            data_.swap(tmp.data_);
            return *this;
        }

        auto header() const noexcept
            -> protocol::ofp_header const&
        {
            return error_msg_.header;
        }

        auto error_type() const noexcept
            -> protocol::ofp_error_type
        {
            return protocol::ofp_error_type(error_msg_.type);
        }

        auto error_code() const noexcept
            -> std::uint16_t
        {
            return error_msg_.code;
        }

        auto data() const noexcept
            -> data_type const&
        {
            return data_;
        }

        auto data_length() const noexcept
            -> std::uint16_t
        {
            return data_.size();
        }

        auto extract_data() noexcept
            -> data_type
        {
            auto data = data_type{};
            data.swap(data_);
            error_msg_.header.length = sizeof(raw_ofp_type);
            return data;
        }

        auto failed_request_header() const
            -> protocol::ofp_header
        {
            auto it = data_.data();
            auto const it_end = data_.data() + data_.size();
            return detail::decode<protocol::ofp_header>(it, it_end);
        }

    private:
        friend basic_openflow_message;

        static constexpr auto is_valid_message_length(
                std::uint16_t const length) noexcept
            -> bool
        {
            return length >= sizeof(raw_ofp_type);
        }

        friend basic_openflow_message::basic_protocol_type;

        error(raw_ofp_type const& error_msg
            , data_type&& data) noexcept
            : error_msg_(error_msg)
            , data_(std::move(data))
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, error_msg_);
            detail::encode_byte_array(container, data_.data(), data_.size());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> error
        {
            auto const error_msg = detail::decode<raw_ofp_type>(first, last);

            auto const data_length
                = error_msg.header.length - sizeof(raw_ofp_type);
            auto data = ofp::decode_data(first, data_length);

            return error{error_msg, std::move(data)};
        }

        auto equal_impl(error const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(error_msg_, rhs.error_msg_)
                && data() == rhs.data();
        }

        template <class Message>
        static auto create_data(
                Message const& msg, std::uint16_t data_size)
            -> data_type
        {
            auto data = data_type{};
            data.reserve(msg.length());
            msg.encode(data);

            constexpr auto max_data_size
                = std::numeric_limits<std::uint16_t>::max()
                - sizeof(raw_ofp_type);
            if (data_size > max_data_size) {
                data_size = max_data_size;
            }
            if (data.size() > data_size) {
                data.resize(data_size);
            }

            return data;
        }

    private:
        raw_ofp_type error_msg_;
        data_type data_;
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_ERROR_HPP
