#ifndef CANARD_NET_OFP_V10_MESSAGES_FLOW_REMOVED_HPP
#define CANARD_NET_OFP_V10_MESSAGES_FLOW_REMOVED_HPP

#include <cstdint>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v10/common/match.hpp>
#include <canard/net/ofp/v10/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/flow_entry.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

  class flow_removed
    : public v10_detail::basic_fixed_length_message<flow_removed>
  {
  public:
    using ofp_type = protocol::ofp_flow_removed;

    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_FLOW_REMOVED;

    flow_removed(
          v10::match const& match
        , std::uint16_t const priority
        , std::uint64_t const cookie
        , protocol::ofp_flow_removed_reason const reason
        , v10::elapsed_time const elapsed_time
        , std::uint16_t const idle_timeout
        , v10::counters const& counters
        , std::uint32_t const xid = get_xid())
      : flow_removed_{
            protocol::ofp_header{
              protocol::OFP_VERSION, message_type, sizeof(ofp_type), xid
            }
          , match.ofp_match()
          , cookie
          , priority
          , std::uint8_t(reason)
          , { 0 }
          , elapsed_time.duration_sec()
          , elapsed_time.duration_nsec()
          , idle_timeout
          , { 0, 0 }
          , counters.packet_count()
          , counters.byte_count()
        }
    {
    }

    flow_removed(
          flow_entry const& entry
        , protocol::ofp_flow_removed_reason const reason
        , v10::elapsed_time const elapsed_time
        , std::uint16_t idle_timeout
        , v10::counters const& counters
        , std::uint32_t const xid = get_xid())
      : flow_removed{
            entry.match(), entry.priority(), entry.cookie()
          , reason, elapsed_time, idle_timeout, counters, xid
        }
    {
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return flow_removed_.header;
    }

    auto match() const noexcept
      -> v10::match
    {
      return v10::match{flow_removed_.match};
    }

    auto priority() const noexcept
      -> std::uint16_t
    {
      return flow_removed_.priority;
    }

    auto cookie() const noexcept
      -> std::uint64_t
    {
      return flow_removed_.cookie;
    }

    auto reason() const noexcept
      -> protocol::ofp_flow_removed_reason
    {
      return protocol::ofp_flow_removed_reason(flow_removed_.reason);
    }

    auto idle_timeout() const noexcept
      -> std::uint16_t
    {
      return flow_removed_.idle_timeout;
    }

    auto duration_sec() const noexcept
      -> std::uint32_t
    {
      return flow_removed_.duration_sec;
    }

    auto duration_nsec() const noexcept
      -> std::uint32_t
    {
      return flow_removed_.duration_nsec;
    }

    auto elapsed_time() const noexcept
      -> v10::elapsed_time
    {
      return v10::elapsed_time{duration_sec(), duration_nsec()};
    }

    auto packet_count() const noexcept
      -> std::uint64_t
    {
      return flow_removed_.packet_count;
    }

    auto byte_count() const noexcept
      -> std::uint64_t
    {
      return flow_removed_.byte_count;
    }

    auto counters() const noexcept
      -> v10::counters
    {
      return v10::counters{packet_count(), byte_count()};
    }

  private:
    friend basic_fixed_length_message;

    explicit flow_removed(ofp_type const& removed) noexcept
      : flow_removed_(removed)
    {
    }

    auto ofp_message() const noexcept
      -> ofp_type const&
    {
      return flow_removed_;
    }

  private:
    ofp_type flow_removed_;
  };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_FLOW_REMOVED_HPP
