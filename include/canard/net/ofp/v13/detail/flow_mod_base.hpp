#ifndef CANARD_NET_OFP_V13_FLOW_MOD_BASE_HPP
#define CANARD_NET_OFP_V13_FLOW_MOD_BASE_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/any_instruction.hpp>
#include <canard/net/ofp/v13/common/oxm_match.hpp>
#include <canard/net/ofp/v13/detail/basic_message.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/detail/length_utility.hpp>
#include <canard/net/ofp/v13/exception.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace flow_mod_detail {

  template <class FlowMod>
  class flow_mod_base
    : public detail::v13::basic_message<FlowMod>
  {
    using base_t = detail::v13::basic_message<FlowMod>;

  public:
    static constexpr protocol::ofp_type message_type = protocol::OFPT_FLOW_MOD;

    using ofp_type = protocol::ofp_flow_mod;
    using instructions_type = ofp::list<any_instruction>;

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return flow_mod_.header;
    }

    auto match() const noexcept
      -> oxm_match const&
    {
      return match_;
    }

    auto extract_match()
      -> oxm_match
    {
      auto match = std::move(match_);
      flow_mod_.header.length
        -= (match.byte_length() - oxm_match::min_byte_length());
      return match;
    }

    auto instructions() const noexcept
      -> instructions_type const&
    {
      return instructions_;
    }

    auto extract_instructions()
      -> instructions_type
    {
      auto instructions = instructions_type{};
      instructions.swap(instructions_);
      flow_mod_.header.length = sizeof(ofp_type) + match_.byte_length();
      return instructions;
    }

  protected:
    flow_mod_base(
          oxm_match&& match
        , std::uint16_t const priority
        , std::uint64_t const cookie
        , std::uint64_t const cookie_mask
        , instructions_type&& instructions
        , std::uint8_t const table_id
        , std::uint16_t const idle_timeout
        , std::uint16_t const hard_timeout
        , std::uint16_t const flags
        , std::uint32_t const buffer_id
        , std::uint32_t const xid)
      : flow_mod_{
            protocol::ofp_header{
                base_t::version()
              , base_t::type()
              , instructions.calc_ofp_length(
                  match.calc_ofp_length(sizeof(ofp_type)))
              , xid
            }
          , cookie
          , cookie_mask
          , table_id
          , FlowMod::command_type
          , idle_timeout
          , hard_timeout
          , priority
          , buffer_id
          , 0
          , 0
          , flags
          , { 0, 0 }
        }
      , match_(std::move(match))
      , instructions_(std::move(instructions))
    {
    }

    flow_mod_base(
          oxm_match&& match
        , std::uint16_t const priority
        , std::uint64_t const cookie
        , std::uint64_t const cookie_mask
        , std::uint8_t const table_id
        , std::uint32_t const out_port
        , std::uint32_t const out_group
        , std::uint32_t const xid)
      : flow_mod_{
            protocol::ofp_header{
                base_t::version()
              , base_t::type()
              , match.calc_ofp_length(sizeof(ofp_type))
              , xid
            }
          , cookie
          , cookie_mask
          , table_id
          , FlowMod::command_type
          , 0
          , 0
          , priority
          , 0
          , out_port
          , out_group
          , 0
          , { 0, 0 }
        }
      , match_(std::move(match))
      , instructions_{}
    {
    }

    flow_mod_base(
          ofp_type const& flow_mod
        , oxm_match&& match
        , instructions_type&& instructions)
      : flow_mod_(flow_mod)
      , match_(std::move(match))
      , instructions_(std::move(instructions))
    {
    }

    flow_mod_base(flow_mod_base const&) = default;

    flow_mod_base(flow_mod_base&& other)
      : flow_mod_(other.flow_mod_)
      , match_(other.extract_match())
      , instructions_(other.extract_instructions())
    {
    }

    auto operator=(flow_mod_base const& other)
      -> flow_mod_base&
    {
      return operator=(flow_mod_base{other});
    }

    auto operator=(flow_mod_base&& other)
      -> flow_mod_base&
    {
      auto tmp = std::move(other);
      std::swap(flow_mod_, tmp.flow_mod_);
      match_.swap(tmp.match_);
      instructions_.swap(tmp.instructions_);
      return *this;
    }

    auto ofp_flow_mod() const noexcept
      -> protocol::ofp_flow_mod const&
    {
      return flow_mod_;
    }

  private:
    friend base_t;

    static constexpr bool is_fixed_length_message = false;

    friend typename base_t::basic_protocol_type;

    friend constexpr auto get_min_length(
        detail::basic_protocol_type_tag<FlowMod>) noexcept
      -> std::uint16_t
    {
      return sizeof(ofp_type) + oxm_match::min_byte_length();
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, flow_mod_);
      match_.encode(container);
      instructions_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> FlowMod
    {
      auto const flow_mod = detail::decode<ofp_type>(first, last);
      last = std::next(first, flow_mod.header.length - sizeof(ofp_type));

      auto const ofp_match
        = detail::decode_without_consumption<protocol::ofp_match>(first, last);
      oxm_match::validate_header(ofp_match);
      if (std::distance(first, last)
          < detail::v13::exact_length(ofp_match.length)) {
        throw exception{
            protocol::bad_request_code::bad_len
          , "too small data size for oxm_match"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      auto match = oxm_match::decode(first, last);

      auto instructions = instructions_type::decode(first, last);

      return FlowMod{
        flow_mod, std::move(match), std::move(instructions)
      };
    }

    auto equal_impl(FlowMod const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(flow_mod_, rhs.flow_mod_)
          && match_ == rhs.match_
          && instructions_ == rhs.instructions_;
    }

  private:
    ofp_type flow_mod_;
    oxm_match match_;
    instructions_type instructions_;
  };

} // namespace flow_mod_detail
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_FLOW_MOD_BASE_HPP
