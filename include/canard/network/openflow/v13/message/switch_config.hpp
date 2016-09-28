#ifndef CANARD_NET_OFP_V13_MESSAGES_SWITCH_CONFIG_HPP
#define CANARD_NET_OFP_V13_MESSAGES_SWITCH_CONFIG_HPP

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

    namespace switch_config_detail {

        template <class T>
        class switch_config_base
             : public detail::v13::basic_openflow_message<T>
        {
            using base_t = detail::v13::basic_openflow_message<T>;

        public:
            using raw_ofp_type = v13_detail::ofp_switch_config;

            auto header() const noexcept
                -> v13_detail::ofp_header const&
            {
                return switch_config_.header;
            }

            auto flags() const noexcept
                -> std::uint16_t
            {
                return switch_config_.flags;
            }

            auto miss_send_length() const noexcept
                -> std::uint16_t
            {
                return switch_config_.miss_send_len;
            }

        protected:
            switch_config_base(
                      std::uint16_t const flags
                    , std::uint16_t const miss_send_len
                    , std::uint32_t const xid) noexcept
                : switch_config_{
                      v13_detail::ofp_header{
                          base_t::version()
                        , base_t::type()
                        , sizeof(raw_ofp_type)
                        , xid
                      }
                    , flags
                    , miss_send_len
                  }
            {
            }

            explicit switch_config_base(raw_ofp_type const& config) noexcept
                : switch_config_(config)
            {
            }

        private:
            friend typename base_t::basic_protocol_type;

            template <class Container>
            void encode_impl(Container& container) const
            {
                detail::encode(container, switch_config_);
            }

            template <class Iterator>
            static auto decode_impl(Iterator& first, Iterator last)
                -> T
            {
                return T{detail::decode<raw_ofp_type>(first, last)};
            }

        private:
            raw_ofp_type switch_config_;
        };

    } // namespace switch_config_detail


    class get_config_request
        : public detail::v13::basic_openflow_message<get_config_request>
    {
    public:
        using raw_ofp_type = v13_detail::ofp_header;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_GET_CONFIG_REQUEST;

        explicit get_config_request(
                std::uint32_t const xid = get_xid()) noexcept
            : header_{
                  version()
                , type()
                , sizeof(raw_ofp_type)
                , xid
              }
        {
        }

        auto header() const noexcept
            -> v13_detail::ofp_header const&
        {
            return header_;
        }

    private:
        explicit get_config_request(raw_ofp_type const& header) noexcept
            : header_(header)
        {
        }

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, header_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> get_config_request
        {
            return get_config_request{
                detail::decode<raw_ofp_type>(first, last)
            };
        }

    private:
        raw_ofp_type header_;
    };


    class get_config_reply
        : public switch_config_detail::switch_config_base<get_config_reply>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_GET_CONFIG_REPLY;

        get_config_reply(
                  std::uint16_t const flags
                , std::uint16_t const miss_send_len
                , std::uint32_t const xid = get_xid()) noexcept
            : switch_config_base{flags, miss_send_len, xid}
        {
        }

        get_config_reply(
                  get_config_request const& request
                , std::uint16_t const flags
                , std::uint16_t const miss_send_len) noexcept
            : get_config_reply{flags, miss_send_len, request.xid()}
        {
        }

    private:
        friend switch_config_base;

        explicit get_config_reply(
                v13_detail::ofp_switch_config const& config) noexcept
            : switch_config_base{config}
        {
        }
    };


    class set_config
        : public switch_config_detail::switch_config_base<set_config>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_SET_CONFIG;

        set_config(std::uint16_t const flags
                 , std::uint16_t const miss_send_len
                 , std::uint32_t const xid = get_xid()) noexcept
            : switch_config_base{flags, miss_send_len, xid}
        {
        }

    private:
        friend switch_config_base;

        explicit set_config(
                v13_detail::ofp_switch_config const& config) noexcept
            : switch_config_base{config}
        {
        }
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_SWITCH_CONFIG_HPP
