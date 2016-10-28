#ifndef CANARD_NET_OFP_V13_MESSAGES_ECHO_HPP
#define CANARD_NET_OFP_V13_MESSAGES_ECHO_HPP

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
#include <canard/network/openflow/v13/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    namespace echo_detail {

        template <class T>
        class echo_base
            : public detail::v13::basic_openflow_message<T>
        {
            using base_t = detail::v13::basic_openflow_message<T>;

        public:
            using raw_ofp_type = v13_detail::ofp_header;
            using data_type = binary_data::pointer_type;

            auto header() const noexcept
                -> v13_detail::ofp_header const&
            {
                return header_;
            }

            auto data() const noexcept
                -> boost::iterator_range<unsigned char const*>
            {
                return boost::make_iterator_range_n(data_.get(), data_length());
            }

            auto data_length() const noexcept
                -> std::uint16_t
            {
                return this->length() - sizeof(raw_ofp_type);
            }

            auto extract_data() noexcept
                -> binary_data
            {
                auto const data_len = data_length();
                header_.length = sizeof(raw_ofp_type);
                return binary_data{std::move(data_), data_len};
            }

        protected:
            echo_base(binary_data&& data, std::uint32_t const xid) noexcept
                : header_{
                      base_t::version()
                    , base_t::type()
                    , calc_ofp_length(data)
                    , xid
                  }
                , data_(std::move(data).data())
            {
            }

            echo_base(raw_ofp_type const& header, data_type&& data)
                : header_(header)
                , data_(std::move(data))
            {
            }

            echo_base(echo_base const& other)
                : header_(other.header_)
                , data_(binary_data::copy_data(other.data()))
            {
            }

            echo_base(echo_base&& other) noexcept
                : header_(other.header_)
                , data_(std::move(other.data_))
            {
                other.header_.length = sizeof(raw_ofp_type);
            }

            auto operator=(echo_base const& other)
                -> echo_base&
            {
                return operator=(echo_base{other});
            }

            auto operator=(echo_base&& other) noexcept
                -> echo_base&
            {
                auto tmp = std::move(other);
                header_ = tmp.header_;
                data_ = std::move(tmp.data_);
                return *this;
            }

        private:
            friend typename base_t::basic_protocol_type;

            template <class Container>
            void encode_impl(Container& container) const
            {
                detail::encode(container, header_);
                detail::encode_byte_array(
                        container, data_.get(), data_length());
            }

            template <class Iterator>
            static auto decode_impl(Iterator& first, Iterator last)
                -> T
            {
                auto const header = detail::decode<raw_ofp_type>(first, last);
                last = std::next(first, header.length - sizeof(raw_ofp_type));
                auto data = binary_data::copy_data(first, last);
                first = last;

                return T{header, std::move(data)};
            }

            static auto calc_ofp_length(binary_data const& data)
                -> std::uint16_t
            {
                constexpr auto max_length
                    = std::numeric_limits<std::uint16_t>::max();
                if (data.size() > max_length - sizeof(raw_ofp_type)) {
                    throw std::runtime_error{"too large echo data length"};
                }
                return sizeof(raw_ofp_type) + data.size();
            }

        private:
            raw_ofp_type header_;
            data_type data_;
        };

    } // namespace echo_detail


    class echo_request
        : public echo_detail::echo_base<echo_request>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_ECHO_REQUEST;

        explicit echo_request(
                binary_data data, std::uint32_t const xid = get_xid()) noexcept
            : echo_base{std::move(data), xid}
        {
        }

        explicit echo_request(std::uint32_t const xid = get_xid()) noexcept
            : echo_request{binary_data{}, xid}
        {
        }

    private:
        friend echo_base;

        echo_request(raw_ofp_type const& header
                   , echo_base::data_type&& data) noexcept
            : echo_base{header, std::move(data)}
        {
        }
    };


    class echo_reply
        : public echo_detail::echo_base<echo_reply>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_ECHO_REPLY;

        explicit echo_reply(
                binary_data data, std::uint32_t const xid = get_xid()) noexcept
            : echo_base{std::move(data), xid}
        {
        }

        explicit echo_reply(std::uint32_t const xid = get_xid()) noexcept
            : echo_reply{binary_data{}, xid}
        {
        }

        explicit echo_reply(echo_request request) noexcept
            : echo_reply{request.extract_data(), request.xid()}
        {
        }

    private:
        friend echo_base;

        echo_reply(raw_ofp_type const& header
                 , echo_base::data_type&& data) noexcept
            : echo_base{header, std::move(data)}
        {
        }
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_ECHO_HPP
