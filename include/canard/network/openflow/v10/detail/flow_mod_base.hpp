#ifndef CANARD_NET_OFP_V10_FLOW_MOD_BASE_HPP
#define CANARD_NET_OFP_V10_FLOW_MOD_BASE_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v10/action_list.hpp>
#include <canard/network/openflow/v10/common/match_set.hpp>
#include <canard/network/openflow/v10/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/flow_entry.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {
namespace flow_mod_detail {

    template <class FlowMod>
    class flow_mod_base
        : public v10_detail::basic_openflow_message<FlowMod>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_flow_mod;

        static constexpr protocol::ofp_type message_type
            = protocol::OFPT_FLOW_MOD;

        static constexpr auto command() noexcept
            -> std::uint16_t
        {
            return FlowMod::command_type;
        }

        auto header() const noexcept
            -> v10_detail::ofp_header const&
        {
            return flow_mod_.header;
        }

        auto actions() const noexcept
            -> action_list const&
        {
            return actions_;
        }

        auto extract_actions()
            -> action_list
        {
            auto actions = action_list{};
            actions.swap(actions_);
            flow_mod_.header.length = sizeof(raw_ofp_type);
            return actions;
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
        }

    protected:
        flow_mod_base(
                  match_set const& match
                , std::uint16_t const priority
                , std::uint64_t const cookie
                , action_list&& actions
                , std::uint16_t const idle_timeout
                , std::uint16_t const hard_timeout
                , std::uint16_t const flags
                , std::uint32_t const buffer_id
                , std::uint32_t const xid)
            : flow_mod_{
                  v10_detail::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
                    , std::uint16_t(sizeof(raw_ofp_type) + actions.length())
                    , xid
                  }
                , match.ofp_match()
                , cookie
                , command()
                , idle_timeout
                , hard_timeout
                , priority
                , buffer_id
                , 0
                , flags
              }
            , actions_(std::move(actions))
        {
        }

        flow_mod_base(
                  match_set const& match
                , std::uint16_t const priority
                , std::uint16_t const out_port
                , std::uint32_t const xid)
            : flow_mod_{
                  v10_detail::ofp_header{
                      protocol::OFP_VERSION
                    , message_type
                    , std::uint16_t(sizeof(raw_ofp_type))
                    , xid
                  }
                , match.ofp_match()
                , 0
                , command()
                , 0
                , 0
                , priority
                , 0
                , out_port
                , 0
              }
            , actions_{}
        {
        }

        flow_mod_base(flow_mod_base const& other) = default;

        flow_mod_base(flow_mod_base&& other)
            : flow_mod_(other.flow_mod_)
            , actions_(other.extract_actions())
        {
        }

        auto operator=(flow_mod_base const& other)
            -> flow_mod_base& = default;

        auto operator=(flow_mod_base&& other)
            -> flow_mod_base&
        {
            auto tmp = std::move(other);
            std::swap(flow_mod_, tmp.flow_mod_);
            actions_.swap(tmp.actions_);
            return *this;
        }

        flow_mod_base(raw_ofp_type const& flow_mod, action_list&& actions)
            : flow_mod_(flow_mod)
            , actions_(std::move(actions))
        {
        }

        auto ofp_flow_mod() const noexcept
            -> raw_ofp_type const&
        {
            return flow_mod_;
        }

    private:
        friend typename
            v10_detail::basic_openflow_message<FlowMod>::basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, flow_mod_);
            actions_.encode(container);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> FlowMod
        {
            auto const flow_mod = detail::decode<raw_ofp_type>(first, last);
            auto const actions_length
                = flow_mod.header.length - sizeof(raw_ofp_type);
            last = std::next(first, actions_length);

            auto actions = action_list::decode(first, last);
            return FlowMod{flow_mod, std::move(actions)};
        }

        auto equal_impl(FlowMod const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(flow_mod_, rhs.flow_mod_)
                && actions_ == rhs.actions_;
        }

    private:
        raw_ofp_type flow_mod_;
        action_list actions_;
    };

} // namespace flow_mod_detail
} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_FLOW_MOD_BASE_HPP
