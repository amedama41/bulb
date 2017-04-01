#ifndef CANARD_NET_OFP_V13_MESSAGES_MULTIPART_FLOW_STATS_HPP
#define CANARD_NET_OFP_V13_MESSAGES_MULTIPART_FLOW_STATS_HPP

#include <cstdint>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/v13/any_instruction.hpp>
#include <canard/network/openflow/v13/common/oxm_match.hpp>
#include <canard/network/openflow/v13/detail/basic_multipart.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/flow_entry_adaptor.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/flow_entry.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

    class flow_stats
        : public detail::basic_protocol_type<flow_stats>
        , public v13_detail::flow_entry_adaptor<
                flow_stats, protocol::ofp_flow_stats
          >
    {
    public:
        using raw_ofp_type = protocol::ofp_flow_stats;
        using instructions_type = ofp::list<any_instruction>;

        flow_stats(v13::flow_entry entry
                 , std::uint8_t const table_id
                 , std::uint16_t const flags
                 , v13::timeouts const& timeouts
                 , v13::elapsed_time const& elapsed_time
                 , v13::counters const& counters)
            : flow_stats_{
                  entry.instructions().calc_ofp_length(
                          entry.match().calc_ofp_length(sizeof(raw_ofp_type)))
                , table_id
                , 0
                , elapsed_time.duration_sec()
                , elapsed_time.duration_nsec()
                , entry.priority()
                , timeouts.idle_timeout()
                , timeouts.hard_timeout()
                , flags
                , { 0, 0, 0, 0 }
                , entry.cookie()
                , counters.packet_count()
                , counters.byte_count()
              }
            , match_(std::move(entry).match())
            , instructions_(std::move(entry).instructions())
        {
        }

        flow_stats(flow_stats const&) = default;

        flow_stats(flow_stats&& other)
            : flow_stats_(other.flow_stats_)
            , match_(other.extract_match())
            , instructions_(other.extract_instructions())
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
            match_.swap(other.match_);
            instructions_.swap(other.instructions_);
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return flow_stats_.length;
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
            flow_stats_.length = base_size + instructions_.length();
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
            flow_stats_.length = sizeof(raw_ofp_type) + match_.byte_length();
            return instructions;
        }

        auto entry() const
            -> flow_entry
        {
            return flow_entry{match(), priority(), cookie(), instructions()};
        }

    private:
        flow_stats(raw_ofp_type const& stats
                 , oxm_match&& match
                 , instructions_type&& instructions)
            : flow_stats_(stats)
            , match_(std::move(match))
            , instructions_(std::move(instructions))
        {
        }

        friend flow_entry_adaptor;

        auto ofp_flow_entry() const noexcept
            -> protocol::ofp_flow_stats const&
        {
            return flow_stats_;
        }

        friend basic_protocol_type;

        static constexpr std::uint16_t base_size
            = sizeof(raw_ofp_type) + oxm_match::min_byte_length();

        friend constexpr auto get_min_length(
                detail::basic_protocol_type_tag<flow_stats>) noexcept
            -> std::uint16_t
        {
            return flow_stats::base_size;
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, flow_stats_);
            match_.encode(container);
            instructions_.encode(container);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> flow_stats
        {
            auto const stats = detail::decode<raw_ofp_type>(first, last);
            if (stats.length < base_size) {
                throw exception{
                      exception::ex_error_type::bad_multipart_element
                    , exception::ex_error_code::bad_length
                    , "too small flow_stats length"
                } << CANARD_NET_OFP_ERROR_INFO();
            }
            auto const rest_length = stats.length - sizeof(raw_ofp_type);
            if (std::distance(first, last) < rest_length) {
                throw exception{
                      protocol::bad_request_code::bad_len
                    , "too small data size for flow_stats"
                } << CANARD_NET_OFP_ERROR_INFO();
            }
            last = std::next(first, rest_length);

            auto const ofp_match
                = detail::decode_without_consumption<protocol::ofp_match>(
                        first, last);
            oxm_match::validate_header(ofp_match);
            if (std::distance(first, last)
                    < v13_detail::exact_length(ofp_match.length)) {
                throw exception{
                      protocol::bad_request_code::bad_len
                    , "too small data size for oxm_match"
                } << CANARD_NET_OFP_ERROR_INFO();
            }
            auto match = oxm_match::decode(first, last);

            auto instructions = instructions_type::decode(first, last);

            return flow_stats{stats, std::move(match), std::move(instructions)};
        }

    private:
        raw_ofp_type flow_stats_;
        oxm_match match_;
        instructions_type instructions_;
    };


    class flow_stats_request
        : public multipart_detail::basic_multipart_request<
              flow_stats_request, protocol::ofp_flow_stats_request, true
          >
    {
    public:
        static constexpr protocol::ofp_multipart_type multipart_type_value
            = protocol::OFPMP_FLOW;

        flow_stats_request(
                  flow_entry entry
                , std::uint8_t const table_id
                , std::uint32_t const out_port = protocol::OFPP_ANY
                , std::uint32_t const out_group = protocol::OFPG_ANY
                , std::uint32_t const xid = get_xid())
            : basic_multipart_request{
                  0
                , body_type{
                      table_id
                    , { 0, 0, 0 }
                    , out_port
                    , out_group
                    , { 0, 0, 0, 0 }
                    , entry.cookie()
                    , std::numeric_limits<std::uint64_t>::max()
                  }
                , std::move(std::move(entry).match())
                , xid
              }
        {
        }

        flow_stats_request(
                  oxm_match match
                , std::uint8_t const table_id
                , v13::cookie_mask const& cookie_mask
                , std::uint32_t const out_port = protocol::OFPP_ANY
                , std::uint32_t const out_group = protocol::OFPG_ANY
                , std::uint32_t const xid = get_xid())
            : basic_multipart_request{
                  0
                , body_type{
                      table_id
                    , { 0, 0, 0 }
                    , out_port
                    , out_group
                    , { 0, 0, 0, 0 }
                    , cookie_mask.value()
                    , cookie_mask.mask()
                  }
                , std::move(match)
                , xid
              }
        {
        }

        flow_stats_request(
                  oxm_match match
                , std::uint8_t const table_id
                , std::uint32_t const out_port = protocol::OFPP_ANY
                , std::uint32_t const out_group = protocol::OFPG_ANY
                , std::uint32_t const xid = get_xid())
            : flow_stats_request{
                  std::move(match)
                , table_id
                , v13::cookie_mask{0, 0}
                , out_port
                , out_group
                , xid
              }
        {
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return body().table_id;
        }

        auto cookie() const noexcept
            -> std::uint64_t
        {
            return body().cookie;
        }

        auto cookie_mask() const noexcept
            -> std::uint64_t
        {
            return body().cookie_mask;
        }

        auto out_port() const noexcept
            -> std::uint32_t
        {
            return body().out_port;
        }

        auto out_group() const noexcept
            -> std::uint32_t
        {
            return body().out_group;
        }

    private:
        friend basic_multipart_request::base_type;

        flow_stats_request(
                  raw_ofp_type const& multipart_request
                , body_type const& flow_stats_request
                , oxm_match&& match)
            : basic_multipart_request{
                multipart_request, flow_stats_request, std::move(match)
              }
        {
        }
    };


    class flow_stats_reply
        : public multipart_detail::basic_multipart_reply<
              flow_stats_reply, flow_stats[]
          >
    {
    public:
        static constexpr protocol::ofp_multipart_type multipart_type_value
            = protocol::OFPMP_FLOW;

        explicit flow_stats_reply(
                  body_type flow_stats
                , std::uint16_t const flags = 0
                , std::uint32_t const xid = get_xid())
            : basic_multipart_reply{flags, std::move(flow_stats), xid}
        {
        }

    private:
        friend basic_multipart_reply::base_type;

        static constexpr bool is_fixed_length_element = false;

        flow_stats_reply(
                  raw_ofp_type const& multipart_reply
                , body_type&& flow_stats)
            : basic_multipart_reply{multipart_reply, std::move(flow_stats)}
        {
        }
    };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_MULTIPART_FLOW_STATS_HPP
