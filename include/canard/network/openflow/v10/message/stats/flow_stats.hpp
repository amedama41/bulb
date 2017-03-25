#ifndef CANARD_NET_OFP_V10_MESSAGES_STATISTICS_FLOW_STATS_HPP
#define CANARD_NET_OFP_V10_MESSAGES_STATISTICS_FLOW_STATS_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v10/exception.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v10/action_list.hpp>
#include <canard/network/openflow/v10/common/match.hpp>
#include <canard/network/openflow/v10/detail/basic_stats.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/flow_entry.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {
namespace statistics {

    class flow_stats
        : public detail::basic_protocol_type<flow_stats>
    {
    public:
        using raw_ofp_type = protocol::ofp_flow_stats;

        flow_stats(v10::flow_entry entry
                 , std::uint8_t const table_id
                 , v10::timeouts const& timeouts
                 , v10::elapsed_time const& elapsed_time
                 , v10::counters const& counters)
            : flow_stats_{
                  entry.actions().calc_ofp_length(sizeof(raw_ofp_type))
                , table_id
                , 0
                , entry.match().ofp_match()
                , elapsed_time.duration_sec()
                , elapsed_time.duration_nsec()
                , entry.priority()
                , timeouts.idle_timeout()
                , timeouts.hard_timeout()
                , { 0, 0, 0, 0, 0, 0 }
                , entry.cookie()
                , counters.packet_count()
                , counters.byte_count()
              }
            , actions_(std::move(entry).actions())
        {
        }

        flow_stats(flow_stats const&) = default;

        flow_stats(flow_stats&& other)
            : flow_stats_(other.flow_stats_)
            , actions_(other.extract_actions())
        {
        }

        auto operator=(flow_stats const& other)
            -> flow_stats&
        {
            return operator=(flow_stats{other});
        }

        auto operator=(flow_stats&& other)
            -> flow_stats&
        {
            auto tmp = std::move(other);
            swap(tmp);
            return *this;
        }

        void swap(flow_stats& other)
        {
            std::swap(flow_stats_, other.flow_stats_);
            actions_.swap(other.actions_);
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return flow_stats_.length;
        }

        auto match() const noexcept
            -> v10::match
        {
            return v10::match{flow_stats_.match};
        }

        auto priority() const noexcept
            -> std::uint16_t
        {
            return flow_stats_.priority;
        }

        auto id() const noexcept
            -> v10::flow_entry_id
        {
            return v10::flow_entry_id{match(), priority()};
        }

        auto cookie() const noexcept
            -> std::uint64_t
        {
            return flow_stats_.cookie;
        }

        auto actions() const noexcept
            -> action_list const&
        {
            return actions_;
        }

        auto extract_actions()
            -> action_list
        {
            auto actions = action_list{};
            actions.swap(actions_);
            flow_stats_.length = sizeof(raw_ofp_type);
            return actions;
        }

        auto entry() const
            -> flow_entry
        {
            return flow_entry{match(), priority(), cookie(), actions()};
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return flow_stats_.table_id;
        }

        auto idle_timeout() const noexcept
            -> std::uint16_t
        {
            return flow_stats_.idle_timeout;
        }

        auto hard_timeout() const noexcept
            -> std::uint16_t
        {
            return flow_stats_.hard_timeout;
        }

        auto timeouts() const noexcept
            -> v10::timeouts
        {
            return v10::timeouts{idle_timeout(), hard_timeout()};
        }

        auto duration_sec() const noexcept
            -> std::uint32_t
        {
            return flow_stats_.duration_sec;
        }

        auto duration_nsec() const noexcept
            -> std::uint32_t
        {
            return flow_stats_.duration_nsec;
        }

        auto elapsed_time() const noexcept
            -> v10::elapsed_time
        {
            return v10::elapsed_time{duration_sec(), duration_nsec()};
        }

        auto packet_count() const noexcept
            -> std::uint64_t
        {
            return flow_stats_.packet_count;
        }

        auto byte_count() const noexcept
            -> std::uint64_t
        {
            return flow_stats_.byte_count;
        }

        auto counters() const noexcept
            -> v10::counters
        {
            return v10::counters{packet_count(), byte_count()};
        }

    private:
        flow_stats(raw_ofp_type const& stats, action_list&& actions)
            : flow_stats_(stats)
            , actions_(std::move(actions))
        {
        }

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, flow_stats_);
            actions_.encode(container);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> flow_stats
        {
            auto const stats = detail::decode<raw_ofp_type>(first, last);
            if (stats.length < sizeof(raw_ofp_type)) {
                BOOST_THROW_EXCEPTION((v10::exception{
                          v10::exception::ex_error_type::bad_stats_element
                        , v10::exception::ex_error_code::bad_length
                        , "flow_stats length is too small"
                }));
            }
            auto const actions_length = stats.length - sizeof(raw_ofp_type);
            if (std::distance(first, last) < actions_length) {
                BOOST_THROW_EXCEPTION((v10::exception{
                          protocol::OFPBRC_BAD_LEN
                        , "too small data size for flow_stats"
                }));
            }
            last = std::next(first, actions_length);

            auto actions = action_list::decode(first, last);

            return flow_stats{stats, std::move(actions)};
        }

        auto equal_impl(flow_stats const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(flow_stats_, rhs.flow_stats_)
                && actions_ == rhs.actions_;
        }

    private:
        raw_ofp_type flow_stats_;
        action_list actions_;
    };


    class flow_stats_request
        : public stats_detail::basic_stats_request<
                flow_stats_request, protocol::ofp_flow_stats_request
          >
    {
    public:
        static constexpr protocol::ofp_stats_types stats_type_value
            = protocol::OFPST_FLOW;

        flow_stats_request(
                  v10::match const& match
                , std::uint8_t const table_id
                , std::uint16_t const out_port = protocol::OFPP_NONE
                , std::uint32_t const xid = get_xid()) noexcept
            : basic_stats_request{
                  0
                , raw_ofp_stats_type{
                      match.ofp_match()
                    , table_id
                    , 0
                    , out_port
                  }
                , xid
              }
        {
        }

        auto match() const noexcept
            -> v10::match
        {
            return v10::match{body().match};
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return body().table_id;
        }

        auto out_port() const noexcept
            -> std::uint16_t
        {
            return body().out_port;
        }

    private:
        friend basic_stats_request::base_type;

        flow_stats_request(
                  raw_ofp_type const& stats_request
                , raw_ofp_stats_type const& flow_stats) noexcept
            : basic_stats_request{stats_request, flow_stats}
        {
        }
    };


    class flow_stats_reply
        : public stats_detail::basic_stats_reply<flow_stats_reply, flow_stats[]>
    {
    public:
        static constexpr protocol::ofp_stats_types stats_type_value
            = protocol::OFPST_FLOW;

        explicit flow_stats_reply(
                  body_type flow_stats
                , std::uint16_t const flags = 0
                , std::uint32_t const xid = get_xid())
            : basic_stats_reply{flags, std::move(flow_stats), xid}
        {
        }

    private:
        friend basic_stats_reply::base_type;

        flow_stats_reply(
                raw_ofp_type const& stats_reply, body_type&& flow_stats)
            : basic_stats_reply{stats_reply, std::move(flow_stats)}
        {
        }

        static constexpr auto is_valid_stats_body_length(
                std::uint16_t const length) noexcept
            -> bool
        {
            return true;
        }
    };

} // namespace statistics
} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_STATISTICS_FLOW_STATS_HPP
