#ifndef CANARD_NET_OFP_V13_MESSAGES_ECHO_HPP
#define CANARD_NET_OFP_V13_MESSAGES_ECHO_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/network/openflow/data_type.hpp>
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
            using data_type = ofp::data_type;

            auto header() const noexcept
                -> v13_detail::ofp_header const&
            {
                return header_;
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
                header_.length = sizeof(raw_ofp_type);
                return data;
            }

        protected:
            echo_base(data_type&& data, std::uint32_t const xid) noexcept
                : header_{
                      base_t::version()
                    , base_t::type()
                    , ofp::calc_ofp_length(data, sizeof(raw_ofp_type))
                    , xid
                  }
                , data_(std::move(data))
            {
            }

            echo_base(raw_ofp_type const& header, data_type&& data) noexcept
                : header_(header)
                , data_(std::move(data))
            {
            }

            echo_base(echo_base const&) = default;

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
                std::swap(header_, tmp.header_);
                data_.swap(tmp.data_);
                return *this;
            }

        private:
            friend typename base_t::basic_protocol_type;

            template <class Container>
            void encode_impl(Container& container) const
            {
                detail::encode(container, header_);
                detail::encode_byte_array(container, data_.data(), data_.size());
            }

            template <class Iterator>
            static auto decode_impl(Iterator& first, Iterator last)
                -> T
            {
                auto const header = detail::decode<raw_ofp_type>(first, last);

                auto const data_length = header.length - sizeof(raw_ofp_type);
                auto data = ofp::decode_data(first, data_length);
                return T{header, std::move(data)};
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
                data_type data, std::uint32_t const xid = get_xid()) noexcept
            : echo_base{std::move(data), xid}
        {
        }

        explicit echo_request(std::uint32_t const xid = get_xid()) noexcept
            : echo_request{data_type{}, xid}
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
                data_type data, std::uint32_t const xid = get_xid()) noexcept
            : echo_base{std::move(data), xid}
        {
        }

        explicit echo_reply(std::uint32_t const xid = get_xid()) noexcept
            : echo_reply{data_type{}, xid}
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
