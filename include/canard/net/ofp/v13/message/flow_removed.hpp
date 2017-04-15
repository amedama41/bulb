#ifndef CANARD_NET_OFP_V13_MESSAGES_FLOW_REMOVED_HPP
#define CANARD_NET_OFP_V13_MESSAGES_FLOW_REMOVED_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/common/oxm_match.hpp>
#include <canard/net/ofp/v13/detail/basic_message.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/detail/flow_entry_adaptor.hpp>
#include <canard/net/ofp/v13/detail/length_utility.hpp>
#include <canard/net/ofp/v13/exception.hpp>
#include <canard/net/ofp/v13/flow_entry.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  class flow_removed
    : public detail::v13::basic_message<flow_removed>
    , public v13_detail::flow_entry_adaptor<
        flow_removed, protocol::ofp_flow_removed
      >
  {
    static constexpr std::size_t base_flow_removed_size
      = sizeof(protocol::ofp_flow_removed) + oxm_match::min_byte_length();

  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_FLOW_REMOVED;

    using raw_ofp_type = protocol::ofp_flow_removed;

    flow_removed(
          oxm_match match
        , std::uint16_t const priority
        , std::uint64_t const cookie
        , protocol::ofp_flow_removed_reason const reason
        , std::uint8_t const table_id
        , v13::elapsed_time const& elapsed_time
        , v13::timeouts const& timeouts
        , v13::counters const& counters
        , std::uint32_t const xid = get_xid())
      : flow_removed_{
            protocol::ofp_header{
                version()
              , type()
              , match.calc_ofp_length(sizeof(raw_ofp_type))
              , xid
            }
          , cookie
          , priority
          , std::uint8_t(reason)
          , table_id
          , elapsed_time.duration_sec()
          , elapsed_time.duration_nsec()
          , timeouts.idle_timeout()
          , timeouts.hard_timeout()
          , counters.packet_count()
          , counters.byte_count()
        }
      , match_(std::move(match))
    {
    }

    flow_removed(
          flow_entry entry
        , protocol::ofp_flow_removed_reason const reason
        , std::uint8_t const table_id
        , v13::elapsed_time const& elapsed_time
        , v13::timeouts const& timeouts
        , v13::counters const& counters
        , std::uint32_t const xid = get_xid())
      : flow_removed{
            std::move(entry).match()
          , entry.priority(), entry.cookie()
          , reason
          , table_id
          , elapsed_time
          , timeouts
          , counters
          , xid
        }
    {
    }

    flow_removed(flow_removed const&) = default;

    flow_removed(flow_removed&& other)
      : flow_removed_(other.flow_removed_)
      , match_(other.extract_match())
    {
    }

    auto operator=(flow_removed const& other)
      -> flow_removed&
    {
      return operator=(flow_removed{other});
    }

    auto operator=(flow_removed&& other)
      -> flow_removed&
    {
      auto tmp = std::move(other);
      flow_removed_ = tmp.flow_removed_;
      match_.swap(tmp.match_);
      return *this;
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return flow_removed_.header;
    }

    auto reason() const noexcept
      -> protocol::ofp_flow_removed_reason
    {
      return protocol::ofp_flow_removed_reason(flow_removed_.reason);
    }

    auto match() const noexcept
      -> oxm_match const&
    {
      return match_;
    }

    auto extract_match()
      -> oxm_match
    {
      auto match = oxm_match{};
      match.swap(match_);
      flow_removed_.header.length = base_flow_removed_size;
      return match;
    }

  private:
    flow_removed(raw_ofp_type const& fremoved, oxm_match&& match)
      : flow_removed_(fremoved)
      , match_(std::move(match))
    {
    }

    friend basic_message;

    static constexpr bool is_fixed_length_message = false;

    friend basic_protocol_type;

    friend constexpr auto get_min_length(
        detail::basic_protocol_type_tag<flow_removed>) noexcept
      -> std::uint16_t
    {
      return flow_removed::base_flow_removed_size;
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, flow_removed_);
      match_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> flow_removed
    {
      auto const fremoved = detail::decode<raw_ofp_type>(first, last);

      auto const match_length = fremoved.header.length - sizeof(raw_ofp_type);
      last = std::next(first, match_length);

      auto const ofp_match
        = detail::decode_without_consumption<protocol::ofp_match>(first, last);
      oxm_match::validate_header(ofp_match);
      if (detail::v13::exact_length(ofp_match.length) != match_length) {
        throw exception{
            protocol::bad_request_code::bad_len
          , "too small data size for oxm_match"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      auto match = oxm_match::decode(first, last);

      return flow_removed{fremoved, std::move(match)};
    }

    auto equal_impl(flow_removed const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(flow_removed_, rhs.flow_removed_)
          && match_ == rhs.match_;
    }

    friend flow_entry_adaptor;

    auto ofp_flow_entry() const noexcept
      -> protocol::ofp_flow_removed const&
    {
      return flow_removed_;
    }

  private:
    raw_ofp_type flow_removed_;
    oxm_match match_;
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_FLOW_REMOVED_HPP
