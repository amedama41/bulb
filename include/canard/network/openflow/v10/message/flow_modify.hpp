#ifndef CANARD_NET_OFP_V10_MESSAGES_FLOW_MODIFY_HPP
#define CANARD_NET_OFP_V10_MESSAGES_FLOW_MODIFY_HPP

#include <cstdint>
#include <utility>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/action_list.hpp>
#include <canard/network/openflow/v10/common/match_set.hpp>
#include <canard/network/openflow/v10/detail/flow_mod_base.hpp>
#include <canard/network/openflow/v10/flow_entry.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    namespace flow_mod_detail {

        template <class FlowModify>
        class flow_modify_base
            : public flow_mod_base<FlowModify>
        {
        public:
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

            auto id() const noexcept
                -> flow_entry_id
            {
                return flow_entry_id{match(), priority()};
            }

            auto cookie() const noexcept
                -> std::uint64_t
            {
                return ofp_flow_mod().cookie;
            }

            auto entry() const
                -> flow_entry
            {
                return flow_entry{
                    id(), cookie(), flow_mod_base<FlowModify>::actions()
                };
            }

            auto idle_timeout() const noexcept
                -> std::uint16_t
            {
                return ofp_flow_mod().idle_timeout;
            }

            auto hard_timeout() const noexcept
                -> std::uint16_t
            {
                return ofp_flow_mod().hard_timeout;
            }

            auto timeouts() const noexcept
                -> v10::timeouts
            {
                return v10::timeouts{idle_timeout(), hard_timeout()};
            }

            auto flags() const noexcept
                -> std::uint16_t
            {
                return ofp_flow_mod().flags;
            }

            auto buffer_id() const noexcept
                -> std::uint32_t
            {
                return ofp_flow_mod().buffer_id;
            }

        protected:
            using flow_mod_base<FlowModify>::flow_mod_base;

        private:
            using flow_mod_base<FlowModify>::ofp_flow_mod;
        };

    } // namespace flow_mod_detail


    class flow_modify
        : public flow_mod_detail::flow_modify_base<flow_modify>
    {
    public:
        static constexpr protocol::ofp_flow_mod_command command_type
            = protocol::OFPFC_MODIFY;

        flow_modify(
                  v10::match const& match
                , std::uint16_t const priority
                , std::uint64_t const cookie
                , action_list actions
                , v10::timeouts const& timeouts
                , std::uint16_t const flags = 0
                , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
                , std::uint32_t const xid = get_xid())
            : flow_modify_base{
                  match
                , priority
                , cookie
                , std::move(actions)
                , timeouts.idle_timeout()
                , timeouts.hard_timeout()
                , flags
                , buffer_id
                , xid
              }
        {
        }

        flow_modify(
                  v10::match const& match
                , std::uint64_t const cookie
                , action_list actions
                , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
                , std::uint32_t const xid = get_xid())
            : flow_modify_base{
                match, 0, cookie, std::move(actions), 0, 0, 0, buffer_id, xid
              }
        {
        }

        flow_modify(
                  flow_entry const& entry
                , action_list actions
                , v10::timeouts const& timeouts
                , std::uint16_t const flags = 0
                , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
                , std::uint32_t const xid = get_xid())
            : flow_modify{
                  entry.match(), entry.priority(), entry.cookie()
                , std::move(actions), timeouts, flags, buffer_id, xid
              }
        {
        }

    private:
        friend flow_mod_base;

        flow_modify(raw_ofp_type const& flow_mod, action_list&& actions)
            : flow_modify_base{flow_mod, std::move(actions)}
        {
        }
    };


    class flow_modify_strict
        : public flow_mod_detail::flow_modify_base<flow_modify_strict>
    {
    public:
        static constexpr protocol::ofp_flow_mod_command command_type
            = protocol::OFPFC_MODIFY_STRICT;

        flow_modify_strict(
                  v10::match const& match
                , std::uint16_t const priority
                , std::uint64_t const cookie
                , action_list actions
                , v10::timeouts const& timeouts
                , std::uint16_t const flags = 0
                , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
                , std::uint32_t const xid = get_xid())
            : flow_modify_base{
                  match
                , priority
                , cookie
                , std::move(actions)
                , timeouts.idle_timeout()
                , timeouts.hard_timeout()
                , flags
                , buffer_id
                , xid
              }
        {
        }

        flow_modify_strict(
                  v10::match const& match
                , std::uint16_t const priority
                , std::uint64_t const cookie
                , action_list actions
                , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
                , std::uint32_t const xid = get_xid())
            : flow_modify_base{
                  match
                , priority
                , cookie
                , std::move(actions)
                , 0
                , 0
                , 0
                , buffer_id
                , xid
              }
        {
        }

        flow_modify_strict(
                  flow_entry const& entry
                , action_list actions
                , v10::timeouts const& timeouts
                , std::uint16_t const flags = 0
                , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
                , std::uint32_t const xid = get_xid())
            : flow_modify_strict{
                  entry.match(), entry.priority(), entry.cookie()
                , std::move(actions), timeouts, flags, buffer_id, xid
              }
        {
        }

    private:
        friend flow_mod_base;

        flow_modify_strict(raw_ofp_type const& flow_mod, action_list&& actions)
            : flow_modify_base{flow_mod, std::move(actions)}
        {
        }
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_FLOW_MODIFY_HPP
