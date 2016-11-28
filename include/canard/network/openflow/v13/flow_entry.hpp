#ifndef CANARD_NET_OFP_V13_FLOW_ENTRY_HPP
#define CANARD_NET_OFP_V13_FLOW_ENTRY_HPP

#include <cstdint>
#include <utility>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/v13/any_instruction.hpp>
#include <canard/network/openflow/v13/common/oxm_match.hpp>
#include <canard/network/openflow/v13/utility/instruction_set.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    class flow_entry_id
    {
    public:
        flow_entry_id(oxm_match match, std::uint16_t const priority)
            : match_(std::move(match))
            , priority_(priority)
        {
        }

        auto match() const& noexcept
            -> oxm_match const&
        {
            return match_;
        }

        auto match() && noexcept
            -> oxm_match&&
        {
            return std::move(match_);
        }

        auto priority() const noexcept
            -> std::uint16_t
        {
            return priority_;
        }

        static auto table_miss()
            -> flow_entry_id
        {
            return flow_entry_id{oxm_match{}, 0};
        }

    private:
        oxm_match match_;
        std::uint16_t priority_;
    };


    class flow_entry
    {
    public:
        using instructions_type = ofp::list<any_instruction>;

        flow_entry(flow_entry_id identifer
                 , std::uint64_t const cookie
                 , instructions_type instructions)
            : identifier_(std::move(identifer))
            , cookie_(cookie)
            , instructions_(std::move(instructions))
        {
        }

        flow_entry(flow_entry_id identifer
                 , std::uint64_t const cookie
                 , instruction_set instructions)
            : identifier_(std::move(identifer))
            , cookie_(cookie)
            , instructions_(std::move(instructions).to_list())
        {
        }

        flow_entry(oxm_match match
                 , std::uint16_t const priority
                 , std::uint64_t const cookie
                 , instructions_type instructions)
            : identifier_{std::move(match), priority}
            , cookie_(cookie)
            , instructions_(std::move(instructions))
        {
        }

        flow_entry(oxm_match match
                 , std::uint16_t const priority
                 , std::uint64_t const cookie
                 , instruction_set instructions)
            : identifier_{std::move(match), priority}
            , cookie_(cookie)
            , instructions_(std::move(instructions).to_list())
        {
        }

        auto id() const& noexcept
            -> flow_entry_id const&
        {
            return identifier_;
        }

        auto id() && noexcept
            -> flow_entry_id&&
        {
            return std::move(identifier_);
        }

        auto match() const& noexcept
            -> oxm_match const&
        {
            return identifier_.match();
        }

        auto match() && noexcept
            -> oxm_match&&
        {
            return std::move(identifier_).match();
        }

        auto priority() const noexcept
            -> std::uint16_t
        {
            return id().priority();
        }

        auto cookie() const noexcept
            -> std::uint64_t
        {
            return cookie_;
        }

        auto instructions() const& noexcept
            -> instructions_type const&
        {
            return instructions_;
        }

        auto instructions() && noexcept
            -> instructions_type&&
        {
            return std::move(instructions_);
        }

        void instructions(instruction_set instructions)
        {
            instructions_ = std::move(instructions).to_list();
        }

    private:
        flow_entry_id identifier_;
        std::uint64_t cookie_;
        instructions_type instructions_;
    };


    class timeouts
    {
    public:
        constexpr timeouts(
                  std::uint16_t const idle_timeout
                , std::uint16_t const hard_timeout) noexcept
            : idle_timeout_(idle_timeout)
            , hard_timeout_(hard_timeout)
        {
        }

        constexpr auto idle_timeout() const noexcept
            -> std::uint16_t
        {
            return idle_timeout_;
        }

        constexpr auto hard_timeout() const noexcept
            -> std::uint16_t
        {
            return hard_timeout_;
        }

    private:
        std::uint16_t idle_timeout_;
        std::uint16_t hard_timeout_;
    };


    class counters
    {
    public:
        constexpr counters(
                  std::uint64_t const packet_count
                , std::uint64_t const byte_count) noexcept
            : packet_count_(packet_count)
            , byte_count_(byte_count)
        {
        }

        constexpr auto packet_count() const noexcept
            -> std::uint64_t
        {
            return packet_count_;
        }

        constexpr auto byte_count() const noexcept
            -> std::uint64_t
        {
            return byte_count_;
        }

    private:
        std::uint64_t packet_count_;
        std::uint64_t byte_count_;
    };


    class elapsed_time
    {
    public:
        constexpr elapsed_time(
                std::uint32_t const duration_sec
                , std::uint32_t const duration_nsec) noexcept
            : duration_sec_(duration_sec)
            , duration_nsec_(duration_nsec)
        {
        }

        constexpr auto duration_sec() const noexcept
            -> std::uint32_t
        {
            return duration_sec_;
        }

        constexpr auto duration_nsec() const noexcept
            -> std::uint32_t
        {
            return duration_nsec_;
        }

    private:
        std::uint32_t duration_sec_;
        std::uint32_t duration_nsec_;
    };


    class cookie_mask
    {
    public:
        constexpr cookie_mask(
                std::uint64_t const value, std::uint64_t const mask) noexcept
            : value_(value)
            , mask_(mask)
        {
        }

        constexpr auto value() const noexcept
            -> std::uint64_t
        {
            return value_;
        }

        constexpr auto mask() const noexcept
            -> std::uint64_t
        {
            return mask_;
        }

    private:
        std::uint64_t value_;
        std::uint64_t mask_;
    };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_FLOW_ENTRY_HPP
