#ifndef CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP
#define CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP

#include <cstdint>
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
            : public detail::v13::basic_openflow_message<T>
        {
            using base_t = detail::v13::basic_openflow_message<T>;

        public:
            using raw_ofp_type = protocol::ofp_header;

            auto header() const noexcept
                -> protocol::ofp_header const&
            {
                return header_;
            }

        protected:
            explicit barrier_base(std::uint32_t const xid) noexcept
                : header_{
                      base_t::version()
                    , base_t::type()
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
            friend base_t;

            static constexpr bool is_fixed_length_message = true;

            friend typename base_t::basic_protocol_type;

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

        explicit barrier_reply(barrier_request const& request)
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
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_BARRIER_HPP
