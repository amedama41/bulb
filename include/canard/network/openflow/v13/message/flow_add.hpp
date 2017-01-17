#ifndef CANARD_NET_OFP_V13_MESSAGES_FLOW_ADD_HPP
#define CANARD_NET_OFP_V13_MESSAGES_FLOW_ADD_HPP

#include <cstdint>
#include <utility>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/common/oxm_match.hpp>
#include <canard/network/openflow/v13/detail/flow_entry_adaptor.hpp>
#include <canard/network/openflow/v13/detail/flow_mod_base.hpp>
#include <canard/network/openflow/v13/flow_entry.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class flow_add
        : public flow_mod_detail::flow_mod_base<flow_add>
        , public v13_detail::flow_entry_adaptor<
                flow_add, protocol::ofp_flow_mod
          >
    {
    public:
        static constexpr protocol::ofp_flow_mod_command command_type
            = protocol::OFPFC_ADD;

        flow_add(flow_entry&& entry
               , std::uint8_t const table_id
               , v13::timeouts const& timeouts
               , std::uint16_t const flags = 0
               , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
               , std::uint32_t const xid = get_xid())
            : flow_mod_base{
                  std::move(entry).match()
                , entry.priority()
                , entry.cookie()
                , 0
                , std::move(entry).instructions()
                , table_id
                , timeouts.idle_timeout()
                , timeouts.hard_timeout()
                , flags
                , buffer_id
                , xid
              }
        {
        }

        flow_add(flow_entry const& entry
               , std::uint8_t const table_id
               , v13::timeouts const& timeouts
               , std::uint16_t const flags = 0
               , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
               , std::uint32_t const xid = get_xid())
            : flow_add{
                flow_entry(entry), table_id, timeouts, flags, buffer_id, xid
              }
        {
        }

        flow_add(flow_entry entry
               , std::uint8_t const table_id
               , std::uint16_t const flags = 0
               , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
               , std::uint32_t const xid = get_xid())
            : flow_add{
                  std::move(entry)
                , table_id
                , v13::timeouts{0, 0}
                , flags
                , buffer_id
                , xid
              }
        {
        }

        auto buffer_id() const noexcept
            -> std::uint32_t
        {
            return ofp_flow_mod().buffer_id;
        }

        auto entry() const
            -> flow_entry
        {
            return flow_entry{match(), priority(), cookie(), instructions()};
        }

    private:
        friend flow_mod_base;

        flow_add(protocol::ofp_flow_mod const& flow_mod
               , oxm_match&& match
               , instructions_type&& instructions)
            : flow_mod_base{
                flow_mod, std::move(match), std::move(instructions)
              }
        {
        }

        friend flow_entry_adaptor;

        auto ofp_flow_entry() const noexcept
            -> protocol::ofp_flow_mod const&
        {
            return ofp_flow_mod();
        }
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_FLOW_ADD_HPP
