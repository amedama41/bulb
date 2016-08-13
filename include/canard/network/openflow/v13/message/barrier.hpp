#ifndef CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP
#define CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP

#include <cstdint>
#include <stdexcept>
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

    namespace barrier_detail {

        template <class T>
        class barrier_base
            : public v13_detail::basic_openflow_message<T>
        {
        protected:
            explicit barrier_base(std::uint32_t const xid) noexcept
                : header_{
                      protocol::OFP_VERSION
                    , T::message_type
                    , sizeof(v13_detail::ofp_header)
                    , xid
                  }
            {
            }

        public:
            auto header() const noexcept
                -> v13_detail::ofp_header const&
            {
                return header_;
            }

            template <class Container>
            auto encode(Container& container) const
                -> Container&
            {
                return detail::encode(container, header_);
            }

            template <class Iterator>
            static auto decode(Iterator& first, Iterator last)
                -> T
            {
                return T{detail::decode<v13_detail::ofp_header>(first, last)};
            }

            static void validate(v13_detail::ofp_header const& header)
            {
                if (header.version != protocol::OFP_VERSION) {
                    throw std::runtime_error{"invalid version"};
                }
                if (header.type != T::message_type) {
                    throw std::runtime_error{"invalid message type"};
                }
                if (header.length != sizeof(v13_detail::ofp_header)) {
                    throw std::runtime_error{"invalid length"};
                }
            }

        protected:
            explicit barrier_base(v13_detail::ofp_header const& header) noexcept
                : header_(header)
            {
            }

        private:
            v13_detail::ofp_header header_;
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

        explicit barrier_request(v13_detail::ofp_header const& header) noexcept
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

        explicit barrier_reply(barrier_request const& request)
            : barrier_reply{request.xid()}
        {
        }

    private:
        friend barrier_base;

        explicit barrier_reply(v13_detail::ofp_header const& header) noexcept
            : barrier_base{header}
        {
        }
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP