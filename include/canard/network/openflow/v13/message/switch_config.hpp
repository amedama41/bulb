#ifndef CANARD_NET_OFP_V13_MESSAGES_SWITCH_CONFIG_HPP
#define CANARD_NET_OFP_V13_MESSAGES_SWITCH_CONFIG_HPP

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

    namespace switch_config_detail {

        template <class T>
        class switch_config_base
             : public v13_detail::basic_openflow_message<T>
        {
        protected:
            switch_config_base(
                      std::uint16_t const flags
                    , std::uint16_t const miss_send_len
                    , std::uint32_t const xid) noexcept
                : switch_config_{
                      v13_detail::ofp_header{
                          protocol::OFP_VERSION
                        , T::message_type
                        , sizeof(v13_detail::ofp_switch_config)
                        , xid
                      }
                    , flags
                    , miss_send_len
                  }
            {
            }

            explicit switch_config_base(
                    v13_detail::ofp_switch_config const& config) noexcept
                : switch_config_(config)
            {
            }

        public:
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

            template <class Container>
            auto encode(Container& container) const
                -> Container&
            {
                return detail::encode(container, switch_config_);
            }

            template <class Iterator>
            static auto decode(Iterator& first, Iterator last)
                -> T
            {
                return T{
                    detail::decode<v13_detail::ofp_switch_config>(first, last)
                };
            }

            static void validate(v13_detail::ofp_header const& header)
            {
                if (header.version != protocol::OFP_VERSION) {
                    throw std::runtime_error{"invalid version"};
                }
                if (header.type != T::message_type) {
                    throw std::runtime_error{"invalid message type"};
                }
                if (header.length != sizeof(v13_detail::ofp_switch_config)) {
                    throw std::runtime_error{"invalid length"};
                }
            }

        private:
            v13_detail::ofp_switch_config switch_config_;
        };

    } // namespace switch_config_detail


    class get_config_request
        : public v13_detail::basic_openflow_message<get_config_request>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_GET_CONFIG_REQUEST;

        explicit get_config_request(
                std::uint32_t const xid = get_xid()) noexcept
            : header_{
                  protocol::OFP_VERSION
                , message_type
                , sizeof(v13_detail::ofp_header)
                , xid
              }
        {
        }

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
            -> get_config_request
        {
            return get_config_request{
                detail::decode<v13_detail::ofp_header>(first, last)
            };
        }

        static void validate(v13_detail::ofp_header const& header)
        {
            if (header.version != protocol::OFP_VERSION) {
                throw std::runtime_error{"invalid version"};
            }
            if (header.type != message_type) {
                throw std::runtime_error{"invalid type"};
            }
            if (header.length != sizeof(v13_detail::ofp_header)) {
                throw std::runtime_error{"invalid length"};
            }
        }

    private:
        explicit get_config_request(
                v13_detail::ofp_header const& header) noexcept
            : header_(header)
        {
        }

    private:
        v13_detail::ofp_header header_;
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