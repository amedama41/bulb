#ifndef CANARD_NET_OFP_V10_MESSAGES_ECHO_HPP
#define CANARD_NET_OFP_V10_MESSAGES_ECHO_HPP

#include <cstdint>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include <boost/operators.hpp>
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

    namespace echo_detail {

        template <class T>
        class echo_base
            : public v10_detail::basic_openflow_message<T>
            , private boost::equality_comparable<T>
        {
        public:
            using raw_ofp_type = v10_detail::ofp_header;
            using data_type = binary_data::pointer_type;

        protected:
            echo_base(binary_data&& data, std::uint32_t const xid) noexcept
                : header_{
                      protocol::OFP_VERSION
                    , T::message_type
                    , std::uint16_t(sizeof(raw_ofp_type) + data.size())
                    , xid
                  }
                , data_(std::move(data).data())
            {
            }

            echo_base(raw_ofp_type const& header, data_type&& data) noexcept
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
                , data_(std::move(other).data_)
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

        public:
            auto header() const noexcept
                -> v10_detail::ofp_header const&
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

            template <class Container>
            auto encode(Container& container) const
                -> Container&
            {
                detail::encode(container, header_);
                return detail::encode_byte_array(
                        container, data_.get(), data_length());
            }

            template <class Iterator>
            static auto decode(Iterator& first, Iterator last)
                -> T
            {
                auto const header = detail::decode<raw_ofp_type>(first, last);
                last = std::next(first, header.length - sizeof(raw_ofp_type));
                auto data = binary_data::copy_data(first, last);
                first = last;

                return T{header, std::move(data)};
            }

            static void validate(v10_detail::ofp_header const& header)
            {
                if (header.version != protocol::OFP_VERSION) {
                    throw std::runtime_error{"invalid version"};
                }
                if (header.type != T::message_type) {
                    throw std::runtime_error{"invalid message type"};
                }
                if (header.length < sizeof(raw_ofp_type)) {
                    throw std::runtime_error{"invalid length"};
                }
            }

            friend auto operator==(T const& lhs, T const& rhs) noexcept
                -> bool
            {
                return lhs.equal_impl(rhs);
            }

        private:
            auto equal_impl(T const& rhs) const noexcept
                -> bool
            {
                return detail::memcmp(header_, rhs.header_)
                    && data() == rhs.data();
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
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_ECHO_HPP
