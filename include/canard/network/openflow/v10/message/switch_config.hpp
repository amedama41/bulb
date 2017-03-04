#ifndef CANARD_NET_OFP_V10_MESSAGES_SWITCH_CONFIG_HPP
#define CANARD_NET_OFP_V10_MESSAGES_SWITCH_CONFIG_HPP

#include <cstdint>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/detail/basic_fixed_length_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    namespace switch_config_detail {

        template <class T>
        class switch_config_base
            : public v10_detail::basic_fixed_length_message<T>
        {
            using base_t = v10_detail::basic_fixed_length_message<T>;

        public:
            using raw_ofp_type = protocol::ofp_switch_config;

        protected:
            switch_config_base(
                      std::uint16_t const flags
                    , std::uint16_t const miss_send_len
                    , std::uint32_t const xid = get_xid()) noexcept
                : config_{
                      protocol::ofp_header{
                          protocol::OFP_VERSION
                        , T::message_type
                        , sizeof(raw_ofp_type)
                        , xid
                      }
                    , flags
                    , miss_send_len
                  }
            {
            }

            explicit switch_config_base(raw_ofp_type const& config) noexcept
                : config_(config)
            {
            }

        public:
            auto header() const noexcept
                -> protocol::ofp_header const&
            {
                return config_.header;
            }

            auto flags() const noexcept
                -> std::uint16_t
            {
                return config_.flags;
            }

            auto miss_send_length() const noexcept
                -> std::uint16_t
            {
                return config_.miss_send_len;
            }

        private:
            friend base_t;

            auto ofp_message() const noexcept
                -> raw_ofp_type const&
            {
                return config_;
            }

        private:
            raw_ofp_type config_;
        };

    } // namespace switch_config_detail


    class get_config_request
        : public v10_detail::basic_fixed_length_message<get_config_request>
    {
    public:
        using raw_ofp_type = protocol::ofp_header;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_GET_CONFIG_REQUEST;

        explicit get_config_request(
                std::uint32_t const xid = get_xid()) noexcept
            : header_{
                  protocol::OFP_VERSION
                , message_type
                , sizeof(raw_ofp_type)
                , xid
              }
        {
        }

        auto header() const noexcept
            -> protocol::ofp_header const&
        {
            return header_;
        }

    private:
        friend basic_fixed_length_message;

        explicit get_config_request(raw_ofp_type const header) noexcept
            : header_(header)
        {
        }

        auto ofp_message() const noexcept
            -> raw_ofp_type const&
        {
            return header_;
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
        friend switch_config_base::basic_fixed_length_message;

        explicit get_config_reply(raw_ofp_type const& config) noexcept
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
        friend switch_config_base::basic_fixed_length_message;

        explicit set_config(raw_ofp_type const& config) noexcept
            : switch_config_base{config}
        {
        }
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_SWITCH_CONFIG_HPP
