#ifndef CANARD_NET_OFP_V10_FLOW_ENTRY_ADAPTOR_HPP
#define CANARD_NET_OFP_V10_FLOW_ENTRY_ADAPTOR_HPP

#include <cstdint>
#include <canard/network/protocol/openflow/v10/flow_entry.hpp>
#include <canard/network/protocol/openflow/v10/match_set.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace v10_detail {

    template <class T, class FlowEntry>
    class flow_entry_adaptor
    {
    public:
        auto match() const noexcept
            -> match_set
        {
            return match_set{ofp_match()};
        }

        auto priority() const noexcept
            -> std::uint16_t
        {
            return base_flow_entry().priority;
        }

        auto id() const
            -> v10::flow_entry_id
        {
            return v10::flow_entry_id{
                static_cast<T const*>(this)->match(), priority()
            };
        }

        auto cookie() const noexcept
            -> std::uint64_t
        {
            return base_flow_entry().cookie;
        }

        auto duration_sec() const noexcept
            -> std::uint32_t
        {
            return base_flow_entry().duration_sec;
        }

        auto duration_nsec() const noexcept
            -> std::uint32_t
        {
            return base_flow_entry().duration_nsec;
        }

        auto elapsed_time() const noexcept
            -> v10::elapsed_time
        {
            return v10::elapsed_time{duration_sec(), duration_nsec()};
        }

        auto idle_timeout() const noexcept
            -> std::uint16_t
        {
            return base_flow_entry().idle_timeout;
        }

        auto hard_timeout() const noexcept
            -> std::uint16_t
        {
            return base_flow_entry().hard_timeout;
        }

        auto timeouts() const noexcept
            -> v10::timeouts
        {
            return v10::timeouts{idle_timeout(), hard_timeout()};
        }

        auto packet_count() const noexcept
            -> std::uint64_t
        {
            return base_flow_entry().packet_count;
        }

        auto byte_count() const noexcept
            -> std::uint64_t
        {
            return base_flow_entry().byte_count;
        }

        auto counters() const noexcept
            -> v10::counters
        {
            return v10::counters{packet_count(), byte_count()};
        }

    private:
        auto base_flow_entry() const noexcept
            -> FlowEntry const&
        {
            return static_cast<T const*>(this)->ofp_flow_entry();
        }
    };

} // namespace v10_detail
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_FLOW_ENTRY_ADAPTOR_HPP
