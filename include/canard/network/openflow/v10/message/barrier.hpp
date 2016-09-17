#ifndef CANARD_NET_OFP_V10_MESSAGES_BARRIER_HPP
#define CANARD_NET_OFP_V10_MESSAGES_BARRIER_HPP

#include <cstdint>
#include <stdexcept>
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

    namespace barrier_detail {

        template <class T>
        class barrier_base
            : public v10_detail::basic_openflow_message<T>
        {
        public:
            using raw_ofp_type = v10_detail::ofp_header;

            auto header() const noexcept
                -> v10_detail::ofp_header const&
            {
                return header_;
            }

            static void validate_header(v10_detail::ofp_header const& header)
            {
                if (header.version != protocol::OFP_VERSION) {
                    throw std::runtime_error{"invalid version"};
                }
                if (header.type != T::message_type) {
                    throw std::runtime_error{"invalid message type"};
                }
                if (header.length != sizeof(raw_ofp_type)) {
                    throw std::runtime_error{"invalid length"};
                }
            }

        protected:
            explicit barrier_base(std::uint32_t const xid) noexcept
                : header_{
                      protocol::OFP_VERSION
                    , T::message_type
                    , sizeof(raw_ofp_type)
                    , xid
                  }
            {
            }

            explicit barrier_base(raw_ofp_type const& header) noexcept
                : header_(header)
            {
            }

        private:
            friend typename
                v10_detail::basic_openflow_message<T>::basic_protocol_type;

            template <class Container>
            void encode_impl(Container& container) const
            {
                detail::encode(container, header_);
            }

            template <class Iterator>
            static auto decode_impl(Iterator& first, Iterator last)
                -> T
            {
                return T{detail::decode<raw_ofp_type>(first, last)};
            }

            auto equal_impl(T const& rhs) const noexcept
                -> bool
            {
                return detail::memcmp(header_, rhs.header_);
            }

        private:
            raw_ofp_type header_;
        };

    } // namespace barrier_detail


    class barrier_request
        : public barrier_detail::barrier_base<barrier_request>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_BARRIER_REQUEST;

        explicit barrier_request(std::uint32_t const xid = get_xid()) noexcept
            : barrier_base{xid}
        {
        }

    private:
        friend barrier_base;

        explicit barrier_request(raw_ofp_type const& header) noexcept
            : barrier_base{header}
        {
        }
    };


    class barrier_reply
        : public barrier_detail::barrier_base<barrier_reply>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_BARRIER_REPLY;

        explicit barrier_reply(std::uint32_t const xid = get_xid()) noexcept
            : barrier_base{xid}
        {
        }

        explicit barrier_reply(barrier_request const& request) noexcept
            : barrier_reply{request.xid()}
        {
        }

    private:
        friend barrier_base;

        explicit barrier_reply(raw_ofp_type const& header) noexcept
            : barrier_base{header}
        {
        }
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_BARRIER_HPP
