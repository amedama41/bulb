#ifndef CANARD_NET_OFP_V13_MESSAGES_FLOW_MODIFY_HPP
#define CANARD_NET_OFP_V13_MESSAGES_FLOW_MODIFY_HPP

#include <cstdint>
#include <limits>
#include <utility>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/common/oxm_match.hpp>
#include <canard/net/ofp/v13/detail/flow_mod_base.hpp>
#include <canard/net/ofp/v13/flow_entry.hpp>
#include <canard/net/ofp/v13/openflow.hpp>
#include <canard/net/ofp/v13/utility/instruction_set.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  class flow_modify
    : public flow_mod_detail::flow_mod_base<flow_modify>
  {
  public:
    static constexpr protocol::ofp_flow_mod_command command_type
      = protocol::OFPFC_MODIFY;

    flow_modify(
          oxm_match match
        , std::uint8_t const table_id
        , instructions_type instructions
        , v13::cookie_mask const& cookie_mask
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_mod_base{
            std::move(match)
          , 0
          , cookie_mask.value()
          , cookie_mask.mask()
          , std::move(instructions)
          , table_id
          , 0
          , 0
          , std::uint16_t(reset_counter ? protocol::OFPFF_RESET_COUNTS : 0)
          , buffer_id
          , xid
        }
    {
    }

    flow_modify(
          oxm_match match
        , std::uint8_t const table_id
        , instruction_set instructions
        , v13::cookie_mask const& cookie_mask
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_modify{
            std::move(match)
          , table_id
          , std::move(instructions).to_list()
          , cookie_mask
          , reset_counter
          , buffer_id
          , xid
        }
    {
    }

    flow_modify(
          oxm_match match
        , std::uint8_t const table_id
        , instructions_type instructions
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_modify{
            std::move(match)
          , table_id
          , std::move(instructions)
          , v13::cookie_mask{0, 0}
          , reset_counter
          , buffer_id
          , xid
        }
    {
    }

    flow_modify(
          oxm_match match
        , std::uint8_t const table_id
        , instruction_set instructions
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_modify{
            std::move(match)
          , table_id
          , std::move(instructions)
          , v13::cookie_mask{0, 0}
          , reset_counter
          , buffer_id
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

    auto buffer_id() const noexcept
      -> std::uint32_t
    {
      return ofp_flow_mod().buffer_id;
    }

    auto reset_counter() const noexcept
      -> bool
    {
      return ofp_flow_mod().flags & protocol::OFPFF_RESET_COUNTS;
    }

  private:
    friend flow_mod_base;

    flow_modify(
          protocol::ofp_flow_mod const& flow_mod
        , oxm_match&& match
        , instructions_type&& instructions)
      : flow_mod_base{flow_mod, std::move(match), std::move(instructions)}
    {
    }
  };


  class flow_modify_strict
    : public flow_mod_detail::flow_mod_base<flow_modify_strict>
  {
  public:
    static constexpr protocol::ofp_flow_mod_command command_type
      = protocol::OFPFC_MODIFY_STRICT;

    flow_modify_strict(
          flow_entry entry
        , std::uint8_t const table_id
        , instructions_type instructions
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_mod_base{
            std::move(entry).match()
          , entry.priority()
          , entry.cookie()
          , std::numeric_limits<std::uint64_t>::max()
          , std::move(instructions)
          , table_id
          , 0
          , 0
          , std::uint16_t(reset_counter ? protocol::OFPFF_RESET_COUNTS : 0)
          , buffer_id
          , xid
        }
    {
    }

    flow_modify_strict(
          flow_entry entry
        , std::uint8_t const table_id
        , instruction_set instructions
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_modify_strict{
            std::move(entry)
          , table_id
          , std::move(instructions).to_list()
          , reset_counter
          , buffer_id
          , xid
        }
    {
    }

    flow_modify_strict(
          oxm_match match
        , std::uint16_t const priority
        , std::uint8_t const table_id
        , instructions_type instructions
        , v13::cookie_mask cookie_mask
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_mod_base{
            std::move(match)
          , priority
          , cookie_mask.value()
          , cookie_mask.mask()
          , std::move(instructions)
          , table_id
          , 0
          , 0
          , std::uint16_t(reset_counter ? protocol::OFPFF_RESET_COUNTS : 0)
          , buffer_id
          , xid
        }
    {
    }

    flow_modify_strict(
          oxm_match match
        , std::uint16_t const priority
        , std::uint8_t const table_id
        , instruction_set instructions
        , v13::cookie_mask cookie_mask
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_modify_strict{
            std::move(match)
          , priority
          , table_id
          , std::move(instructions).to_list()
          , cookie_mask
          , reset_counter
          , buffer_id
          , xid
        }
    {
    }

    flow_modify_strict(
          oxm_match match
        , std::uint16_t const priority
        , std::uint8_t const table_id
        , instructions_type instructions
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_modify_strict{
            std::move(match)
          , priority
          , table_id
          , std::move(instructions)
          , v13::cookie_mask{0, 0}
          , reset_counter
          , buffer_id
          , xid
        }
    {
    }

    flow_modify_strict(
          oxm_match match
        , std::uint16_t const priority
        , std::uint8_t const table_id
        , instruction_set instructions
        , bool const reset_counter = false
        , std::uint32_t const buffer_id = protocol::OFP_NO_BUFFER
        , std::uint32_t const xid = get_xid())
      : flow_modify_strict{
            std::move(match)
          , priority
          , table_id
          , std::move(instructions)
          , v13::cookie_mask{0, 0}
          , reset_counter
          , buffer_id
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

    auto buffer_id() const noexcept
      -> std::uint32_t
    {
      return ofp_flow_mod().buffer_id;
    }

    auto reset_counter() const noexcept
      -> bool
    {
      return ofp_flow_mod().flags & protocol::OFPFF_RESET_COUNTS;
    }

    auto entry() const
      -> flow_entry
    {
      return flow_entry{match(), priority(), cookie(), instructions()};
    }

  private:
    friend flow_mod_base;

    flow_modify_strict(
          protocol::ofp_flow_mod const& flow_mod
        , oxm_match&& match
        , instructions_type&& instructions)
      : flow_mod_base{flow_mod, std::move(match), std::move(instructions)}
    {
    }
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_FLOW_MODIFY_HPP
