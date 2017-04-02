#ifndef CANARD_NET_OFP_V10_MESSAGES_FLOW_ADD_HPP
#define CANARD_NET_OFP_V10_MESSAGES_FLOW_ADD_HPP

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

  class flow_add
    : public flow_mod_detail::flow_mod_base<flow_add>
  {
  public:
    static constexpr protocol::ofp_flow_mod_command command_type
      = protocol::OFPFC_ADD;

    flow_add(
          v10::match const& match
        , std::uint16_t const priority
        , std::uint64_t const cookie
        , action_list actions
        , v10::timeouts const& timeouts
        , std::uint16_t const flags = 0
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_mod_base{
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

    flow_add(
          flow_entry entry
        , v10::timeouts const& timeouts
        , std::uint16_t const flags = 0
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_add{
            entry.id().match(), entry.id().priority()
          , entry.cookie()
          , std::move(entry).actions()
          , timeouts, flags, buffer_id, xid
        }
    {
    }

    flow_add(
          flow_entry entry
        , std::uint16_t const flags = 0
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_add{
            entry.id().match(), entry.id().priority()
          , entry.cookie()
          , std::move(entry).actions()
          , v10::timeouts{0, 0}, flags, buffer_id, xid
        }
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
      return flow_entry{id(), cookie(), actions()};
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

  private:
    friend flow_mod_base;

    flow_add(raw_ofp_type const& flow_mod, action_list&& actions)
      : flow_mod_base{flow_mod, std::move(actions)}
    {
    }
  };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_FLOW_ADD_HPP
