#ifndef CANARD_NET_OFP_V13_MESSAGES_FLOW_DELETE_HPP
#define CANARD_NET_OFP_V13_MESSAGES_FLOW_DELETE_HPP

#include <cstdint>
#include <limits>
#include <utility>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/common/oxm_match_set.hpp>
#include <canard/network/openflow/v13/detail/flow_mod_base.hpp>
#include <canard/network/openflow/v13/flow_entry.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class flow_delete
        : public flow_mod_detail::flow_mod_base<flow_delete>
    {
    public:
        static constexpr protocol::ofp_flow_mod_command command_type
            = protocol::OFPFC_DELETE;

        flow_delete(oxm_match_set match
                  , std::uint8_t const table_id
                  , v13::cookie_mask const cookie_mask
                  , std::uint32_t const out_port = protocol::OFPP_ANY
                  , std::uint32_t const out_group = protocol::OFPG_ANY
                  , std::uint32_t const xid = get_xid())
            : flow_mod_base{
                  std::move(match)
                , 0
                , cookie_mask.value()
                , cookie_mask.mask()
                , table_id
                , out_port
                , out_group
                , xid
              }
        {
        }

        flow_delete(oxm_match_set match
                  , std::uint8_t const table_id
                  , std::uint32_t const out_port = protocol::OFPP_ANY
                  , std::uint32_t const out_group = protocol::OFPG_ANY
                  , std::uint32_t const xid = get_xid())
            : flow_delete{
                  std::move(match)
                , table_id
                , v13::cookie_mask{0, 0}
                , out_port
                , out_group
                , xid
              }
        {
        }

        auto cookie() const noexcept
            -> std::uint64_t
        {
            return ofp_flow_mod().cookie;
        }

        auto cookie_mask() const noexcept
            -> std::uint64_t
        {
            return ofp_flow_mod().cookie_mask;
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return ofp_flow_mod().table_id;
        }

        auto out_port() const noexcept
            -> std::uint32_t
        {
            return ofp_flow_mod().out_port;
        }

        auto out_group() const noexcept
            -> std::uint32_t
        {
            return ofp_flow_mod().out_group;
        }

    private:
        friend flow_mod_base;

        flow_delete(v13_detail::ofp_flow_mod const& flow_mod
                  , oxm_match_set&& match
                  , instruction_set&& instructions)
            : flow_mod_base{flow_mod, std::move(match), std::move(instructions)}
        {
        }
    };


    class flow_delete_strict
        : public flow_mod_detail::flow_mod_base<flow_delete_strict>
    {
    public:
        static constexpr protocol::ofp_flow_mod_command command_type
            = protocol::OFPFC_DELETE_STRICT;

        flow_delete_strict(
                  flow_entry entry
                , std::uint8_t const table_id
                , std::uint32_t const out_port = protocol::OFPP_ANY
                , std::uint32_t const out_group = protocol::OFPG_ANY
                , std::uint32_t const xid = get_xid())
            : flow_mod_base{
                  std::move(entry).match()
                , entry.priority()
                , entry.cookie()
                , std::numeric_limits<std::uint64_t>::max()
                , table_id
                , out_port
                , out_group
                , xid
              }
        {
        }

        flow_delete_strict(
                  oxm_match_set match
                , std::uint16_t const priority
                , std::uint8_t const table_id
                , v13::cookie_mask const& cookie_mask
                , std::uint32_t const out_port = protocol::OFPP_ANY
                , std::uint32_t const out_group = protocol::OFPG_ANY
                , std::uint32_t const xid = get_xid())
            : flow_mod_base{
                  std::move(match)
                , priority
                , cookie_mask.value()
                , cookie_mask.mask()
                , table_id
                , out_port
                , out_group
                , xid
              }
        {
        }

        flow_delete_strict(
                  oxm_match_set match
                , std::uint16_t const priority
                , std::uint8_t const table_id
                , std::uint32_t const out_port = protocol::OFPP_ANY
                , std::uint32_t const out_group = protocol::OFPG_ANY
                , std::uint32_t const xid = get_xid())
            : flow_delete_strict{
                  std::move(match)
                , priority
                , table_id
                , v13::cookie_mask{0, 0}
                , out_port
                , out_group
                , xid
              }
        {
        }

        auto priority() const noexcept
            -> std::uint16_t
        {
            return ofp_flow_mod().priority;
        }

        auto cookie() const noexcept
            -> std::uint64_t
        {
            return ofp_flow_mod().cookie;
        }

        auto cookie_mask() const noexcept
            -> std::uint64_t
        {
            return ofp_flow_mod().cookie_mask;
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return ofp_flow_mod().table_id;
        }

        auto out_port() const noexcept
            -> std::uint32_t
        {
            return ofp_flow_mod().out_port;
        }

        auto out_group() const noexcept
            -> std::uint32_t
        {
            return ofp_flow_mod().out_group;
        }

        auto id() const
            -> flow_entry_id
        {
            return flow_entry_id{match(), priority()};
        }

    private:
        friend flow_mod_base;

        flow_delete_strict(
                  v13_detail::ofp_flow_mod const& flow_mod
                , oxm_match_set&& match
                , instruction_set&& instructions)
            : flow_mod_base{flow_mod, std::move(match), std::move(instructions)}
        {
        }
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_FLOW_DELETE_HPP
