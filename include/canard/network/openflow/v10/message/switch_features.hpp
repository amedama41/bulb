#ifndef CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP
#define CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP

#include <cstdint>
#include <stdexcept> // TODO
#include <utility>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/v10/common/port.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class features_request
        : public v10_detail::basic_openflow_message<features_request>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_header;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_FEATURES_REQUEST;

        explicit features_request(std::uint32_t const xid = get_xid()) noexcept
            : header_{
                  protocol::OFP_VERSION
                , message_type
                , sizeof(raw_ofp_type)
                , xid
              }
        {
        }

        auto header() const noexcept
            -> v10_detail::ofp_header
        {
            return header_;
        }

        static void validate_header(raw_ofp_type const& header)
        {
            if (header.version != protocol::OFP_VERSION) {
                throw std::runtime_error{"invalid version"};
            }
            if (header.type != message_type) {
                throw std::runtime_error{"invalid message type"};
            }
            if (header.length != sizeof(raw_ofp_type)) {
                throw std::runtime_error{"invalid length"};
            }
        }

    private:
        friend basic_openflow_message::basic_protocol_type;

        explicit features_request(raw_ofp_type const& header) noexcept
            : header_(header)
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, header_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> features_request
        {
            auto const header = detail::decode<raw_ofp_type>(first, last);
            return features_request{header};
        }

        auto equal_impl(features_request const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(header_, rhs.header_);
        }

    private:
        raw_ofp_type header_;
    };


    class features_reply
        : public v10_detail::basic_openflow_message<features_reply>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_switch_features;
        using ports_type = ofp::list<port>;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_FEATURES_REPLY;

        features_reply(std::uint64_t const dpid
                     , std::uint32_t const n_buffers
                     , std::uint8_t const n_tables
                     , std::uint32_t const capabilities
                     , std::uint32_t const actions
                     , ports_type ports
                     , std::uint32_t const xid = get_xid())
            : switch_features_{
                  v10_detail::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
                    , ports.calc_ofp_length(sizeof(raw_ofp_type))
                    , xid
                  }
                , dpid
                , n_buffers
                , n_tables
                , { 0, 0, 0 }
                , capabilities
                , actions
              }
            , ports_(std::move(ports))
        {
        }

        features_reply(features_request const& request
                     , std::uint64_t const dpid
                     , std::uint32_t const n_buffers
                     , std::uint8_t const n_tables
                     , std::uint32_t const capabilities
                     , std::uint32_t const actions
                     , ports_type ports)
            : features_reply{
                  dpid, n_buffers, n_tables, capabilities, actions
                , std::move(ports), request.xid()
              }
        {
        }

        features_reply(features_reply const&) = default;

        features_reply(features_reply&& other)
            : switch_features_(other.switch_features_)
            , ports_(other.extract_ports())
        {
        }

        auto operator=(features_reply const& other)
            -> features_reply&
        {
            return operator=(features_reply{other});
        }

        auto operator=(features_reply&& other)
            -> features_reply&
        {
            auto tmp = std::move(other);
            std::swap(switch_features_, tmp.switch_features_);
            ports_.swap(tmp.ports_);
            return *this;
        }

        auto header() const noexcept
            -> v10_detail::ofp_header
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

        auto capabilities() const noexcept
            -> std::uint32_t
        {
            return switch_features_.capabilities;
        }

        auto actions() const noexcept
            -> std::uint32_t
        {
            return switch_features_.actions;
        }

        auto ports() const noexcept
            -> ports_type const&
        {
            return ports_;
        }

        auto extract_ports()
            -> ports_type
        {
            auto tmp = ports_type{};
            tmp.swap(ports_);
            switch_features_.header.length = sizeof(raw_ofp_type);
            return tmp;
        }

        static void validate_header(v10_detail::ofp_header const& header)
        {
            if (header.version != protocol::OFP_VERSION) {
                throw std::runtime_error{"invalid version"};
            }
            if (header.type != message_type) {
                throw std::runtime_error{"invalid message type"};
            }
            if (header.length < sizeof(raw_ofp_type)) {
                throw std::runtime_error{"too small length"};
            }
            if ((header.length - sizeof(raw_ofp_type))
                    % sizeof(port::raw_ofp_type) != 0) {
                throw std::runtime_error{"invalid length"};
            }
        }

    private:
        friend basic_openflow_message::basic_protocol_type;

        features_reply(raw_ofp_type const& features, ports_type&& ports)
            : switch_features_(features)
            , ports_(std::move(ports))
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, switch_features_);
            ports_.encode(container);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> features_reply
        {
            auto const features = detail::decode<raw_ofp_type>(first, last);

            auto const ports_length
                = features.header.length - sizeof(raw_ofp_type);
            last = std::next(first, ports_length);

            auto ports = ports_type::decode(first, last);

            return features_reply{features, std::move(ports)};
        }

        auto equal_impl(features_reply const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(switch_features_, rhs.switch_features_)
                && ports_ == rhs.ports_;
        }

    private:
        raw_ofp_type switch_features_;
        ports_type ports_;
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP
