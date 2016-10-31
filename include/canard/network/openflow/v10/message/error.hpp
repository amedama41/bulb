#ifndef CANARD_NET_OFP_V10_MESSAGES_ERROR_HPP
#define CANARD_NET_OFP_V10_MESSAGES_ERROR_HPP

#include <cstdint>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>
#include <boost/range/iterator_range.hpp>
#include <canard/network/openflow/binary_data.hpp>
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
        using raw_ofp_type = v10_detail::ofp_error_msg;
        using data_type = binary_data::pointer_type;

        static constexpr protocol::ofp_type message_type = protocol::OFPT_ERROR;

        error(protocol::ofp_error_type const type
            , std::uint16_t const code
            , binary_data data
            , std::uint32_t const xid = get_xid()) noexcept
            : error_msg_{
                  v10_detail::ofp_header{
                        protocol::OFP_VERSION
                      , message_type
                      , calc_ofp_length(data)
                      , xid
                  }
                , std::uint16_t(type)
                , code
              }
            , data_(std::move(data).data())
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

        error(error const& other)
            : error_msg_(other.error_msg_)
            , data_(binary_data::copy_data(other.data()))
        {
        }

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
            error_msg_ = tmp.error_msg_;
            data_.swap(tmp.data_);
            return *this;
        }

        auto header() const noexcept
            -> v10_detail::ofp_header const&
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
            -> boost::iterator_range<unsigned char const*>
        {
            return boost::make_iterator_range_n(data_.get(), data_length());
        }

        auto data_length() const noexcept
            -> std::uint16_t
        {
            return length() - sizeof(raw_ofp_type);
        }

        auto extract_data() noexcept
            -> binary_data
        {
            auto const data_len = data_length();
            error_msg_.header.length = sizeof(raw_ofp_type);
            return binary_data{std::move(data_), data_len};
        }

        auto failed_request_header() const
            -> v10_detail::ofp_header
        {
            auto it = data_.get();
            auto const it_end = data_.get() + data_length();
            return detail::decode<v10_detail::ofp_header>(it, it_end);
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
            detail::encode_byte_array(container, data_.get(), data_length());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> error
        {
            auto const error_msg = detail::decode<raw_ofp_type>(first, last);

            auto const data_length
                = error_msg.header.length - sizeof(raw_ofp_type);
            last = std::next(first, data_length);
            auto data = binary_data::copy_data(first, last);
            first = last;

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
            -> binary_data
        {
            auto buffer = std::vector<unsigned char>{};
            buffer.reserve(msg.length());
            msg.encode(buffer);

            constexpr auto max_data_size
                = std::numeric_limits<std::uint16_t>::max()
                - sizeof(raw_ofp_type);
            if (data_size > max_data_size) {
                data_size = max_data_size;
            }
            if (buffer.size() > data_size) {
                buffer.resize(data_size);
            }

            return binary_data{buffer};
        }

        static auto calc_ofp_length(binary_data const& data)
            -> std::uint16_t
        {
            constexpr auto max_length
                = std::numeric_limits<std::uint16_t>::max();
            if (data.size() > max_length - sizeof(raw_ofp_type)) {
                throw std::runtime_error{"too large error data length"};
            }
            return sizeof(raw_ofp_type) + data.size();
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
