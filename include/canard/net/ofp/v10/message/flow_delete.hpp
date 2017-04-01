#ifndef CANARD_NET_OFP_V10_MESSAGES_FLOW_DELETE_HPP
#define CANARD_NET_OFP_V10_MESSAGES_FLOW_DELETE_HPP

#include <cstdint>
#include <utility>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v10/action_list.hpp>
#include <canard/net/ofp/v10/common/match.hpp>
#include <canard/net/ofp/v10/detail/flow_mod_base.hpp>
#include <canard/net/ofp/v10/flow_entry.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    class flow_delete
        : public flow_mod_detail::flow_mod_base<flow_delete>
    {
    public:
        static protocol::ofp_flow_mod_command const command_type
            = protocol::OFPFC_DELETE;

        explicit flow_delete(
                  v10::match const& match
                , std::uint16_t const out_port = protocol::OFPP_NONE
                , std::uint32_t const xid = get_xid())
            : flow_mod_base{match, 0, out_port, xid}
        {
        }

        auto match() const noexcept
            -> v10::match
        {
            return v10::match{ofp_flow_mod().match};
        }

        auto out_port() const noexcept
            -> std::uint16_t
        {
            return ofp_flow_mod().out_port;
        }

    private:
        friend flow_mod_base;

        flow_delete(raw_ofp_type const& flow_mod, action_list&& actions)
            : flow_mod_base{flow_mod, std::move(actions)}
        {
        }
    };


    class flow_delete_strict
        : public flow_mod_detail::flow_mod_base<flow_delete_strict>
    {
    public:
        static protocol::ofp_flow_mod_command const command_type
            = protocol::OFPFC_DELETE_STRICT;

        flow_delete_strict(
                  v10::match const& match
                , std::uint16_t const priority
                , std::uint16_t const out_port = protocol::OFPP_NONE
                , std::uint32_t const xid = get_xid())
            : flow_mod_base{match, priority, out_port, xid}
        {
        }

        explicit flow_delete_strict(
                  flow_entry_id const& id
                , std::uint16_t const out_port = protocol::OFPP_NONE
                , std::uint32_t const xid = get_xid())
            : flow_delete_strict{id.match(), id.priority(), out_port, xid}
        {
        }

        auto match() const noexcept
            -> v10::match
        {
            return v10::match{ofp_flow_mod().match};
        }

        auto priority() const noexcept
            -> std::uint16_t
        {
            return ofp_flow_mod().priority;
        }

        auto out_port() const noexcept
            -> std::uint16_t
        {
            return ofp_flow_mod().out_port;
        }

        auto id() const noexcept
            -> flow_entry_id
        {
            return flow_entry_id{match(), priority()};
        }

    private:
        friend flow_mod_base;

        flow_delete_strict(raw_ofp_type const& flow_mod, action_list&& actions)
            : flow_mod_base{flow_mod, std::move(actions)}
        {
        }
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_FLOW_DELETE_HPP
