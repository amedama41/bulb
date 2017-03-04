#ifndef CANARD_NET_OFP_V13_MESSAGES_SWITCH_FEATURES_HPP
#define CANARD_NET_OFP_V13_MESSAGES_SWITCH_FEATURES_HPP

#include <cstdint>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/detail/basic_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class features_request
        : public detail::v13::basic_message<features_request>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_FEATURES_REQUEST;

        using raw_ofp_type = protocol::ofp_header;

        features_request(std::uint32_t const xid = get_xid()) noexcept
            : header_{
                  version()
                , type()
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
        explicit features_request(raw_ofp_type const& header) noexcept
            : header_(header)
        {
        }

        friend basic_message;

        static constexpr bool is_fixed_length_message = true;

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, header_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> features_request
        {
            return features_request{detail::decode<raw_ofp_type>(first, last)};
        }

    private:
        raw_ofp_type header_;
    };


    class features_reply
        : public detail::v13::basic_message<features_reply>
    {
    public:
        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_FEATURES_REPLY;

        using raw_ofp_type = protocol::ofp_switch_features;

        features_reply(features_request const& request
                     , std::uint64_t const dpid
                     , std::uint32_t const n_buffers
                     , std::uint8_t const n_tables
                     , std::uint8_t const auxiliary_id
                     , std::uint32_t const capabilities) noexcept
            : switch_features_{
                  protocol::ofp_header{
                      version()
                    , type()
                    , sizeof(raw_ofp_type)
                    , request.xid()
                  }
                , dpid
                , n_buffers
                , n_tables
                , auxiliary_id
                , { 0, 0 }
                , capabilities
                , 0
              }
        {
        }

        auto header() const noexcept
            -> protocol::ofp_header const&
        {
            return switch_features_.header;
        }

        auto datapath_id() const noexcept
            -> std::uint64_t
        {
            return switch_features_.datapath_id;
        }

        auto num_buffers() const noexcept
            -> std::uint32_t
        {
            return switch_features_.n_buffers;
        }

        auto num_tables() const noexcept
            -> std::uint8_t
        {
            return switch_features_.n_tables;
        }

        auto auxiliary_id() const noexcept
            -> std::uint8_t
        {
            return switch_features_.auxiliary_id;
        }

        auto capabilities() const noexcept
            -> std::uint32_t
        {
            return switch_features_.capabilities;
        }

    private:
        explicit features_reply(raw_ofp_type const& switch_features) noexcept
            : switch_features_(switch_features)
        {
        }

        friend basic_message;

        static constexpr bool is_fixed_length_message = true;

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, switch_features_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> features_reply
        {
            return features_reply{detail::decode<raw_ofp_type>(first, last)};
        }

    private:
        raw_ofp_type switch_features_;
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_SWITCH_FEATURES_HPP
