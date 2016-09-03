#ifndef CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP
#define CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP

#include <cstdint>
#include <stdexcept> // TODO
#include <utility>
#include <vector>
#include <boost/operators.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/openflow.hpp>
#include <canard/network/openflow/v10/port.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class features_request
        : public v10_detail::basic_openflow_message<features_request>
        , private boost::equality_comparable<features_request>
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

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            return detail::encode(container, header_);
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> features_request
        {
            auto const header = detail::decode<raw_ofp_type>(first, last);
            return features_request{header};
        }

        static void validate(raw_ofp_type const& header)
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

        friend auto operator==(
                features_request const&, features_request const&) noexcept
            -> bool;

    private:
        explicit features_request(raw_ofp_type const& header) noexcept
            : header_(header)
        {
        }

        auto equal_impl(features_request const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(header_, rhs.header_);
        }

    private:
        raw_ofp_type header_;
    };

    inline auto operator==(
            features_request const& lhs, features_request const& rhs) noexcept
        -> bool
    {
        return lhs.equal_impl(rhs);
    }


    class features_reply
        : public v10_detail::basic_openflow_message<features_reply>
        , private boost::equality_comparable<features_reply>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_switch_features;
        using port_list = std::vector<port>;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_FEATURES_REPLY;

        features_reply(std::uint64_t const dpid
                     , std::uint32_t const n_buffers
                     , std::uint8_t const n_tables
                     , std::uint32_t const capabilities
                     , std::uint32_t const actions
                     , port_list ports
                     , std::uint32_t const xid = get_xid())
            : switch_features_{
                  v10_detail::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
                    , std::uint16_t(
                            sizeof(raw_ofp_type)
                          + ports.size() * sizeof(port::raw_ofp_type))
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
                     , port_list ports)
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
            -> port_list const&
        {
            return ports_;
        }

        auto extract_ports()
            -> port_list
        {
            auto tmp = port_list{};
            tmp.swap(ports_);
            switch_features_.header.length = sizeof(raw_ofp_type);
            return tmp;
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            detail::encode(container, switch_features_);
            boost::for_each(ports_, [&](port const& p) {
                p.encode(container);
            });
            return container;
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> features_reply
        {
            auto const features
                = detail::decode<raw_ofp_type>(first, last);

            auto const remaining_length
                = features.header.length - sizeof(features);
            last = std::next(first, remaining_length);

            auto ports = port_list{};
            ports.reserve(remaining_length / sizeof(port::raw_ofp_type));
            while (first != last) {
                ports.emplace_back(port::decode(first, last));
            }
            return features_reply{features, std::move(ports)};
        }

        static void validate(v10_detail::ofp_header const& header)
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

        friend auto operator==(
                features_reply const&, features_reply const&) noexcept
            -> bool;

    private:
        features_reply(raw_ofp_type const& features
                     , port_list&& ports)
            : switch_features_(features)
            , ports_(std::move(ports))
        {
        }

        auto equal_impl(features_reply const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(switch_features_, rhs.switch_features_)
                && ports_ == rhs.ports_;
        }

    private:
        raw_ofp_type switch_features_;
        port_list ports_;
    };

    inline auto operator==(
            features_reply const& lhs, features_reply const& rhs) noexcept
        -> bool
    {
        return lhs.equal_impl(rhs);
    }

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP
