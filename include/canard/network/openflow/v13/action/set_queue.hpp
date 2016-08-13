#ifndef CANARD_NET_OFP_V13_ACTIONS_SET_QUEUE_HPP
#define CANARD_NET_OFP_V13_ACTIONS_SET_QUEUE_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/detail/basic_action.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class set_queue
        : public detail::v13::basic_action<
            set_queue, v13_detail::ofp_action_set_queue
          >
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_SET_QUEUE;

        explicit set_queue(std::uint32_t const queue_id) noexcept
            : action_set_queue_{
                  action_type
                , length()
                , queue_id
              }
        {
        }

        auto queue_id() const noexcept
            -> std::uint32_t
        {
            return action_set_queue_.queue_id;
        }

        friend auto operator==(set_queue const& lhs, set_queue const& rhs) noexcept
            -> bool
        {
            return detail::memcmp(lhs.action_set_queue_, rhs.action_set_queue_);
        }

    private:
        friend basic_action;

        explicit set_queue(raw_ofp_type const& action_set_queue) noexcept
            : action_set_queue_(action_set_queue)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return action_set_queue_;
        }

        static void validate_impl(set_queue const& action)
        {
            if (action.queue_id() == protocol::OFPQ_ALL) {
                throw std::runtime_error{"invalid queue_id"};
            }
        }

    private:
        raw_ofp_type action_set_queue_;
    };

    inline auto equivalent(set_queue const& lhs, set_queue const& rhs) noexcept
        -> bool
    {
        return lhs.queue_id() == rhs.queue_id();
    }

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_SET_QUEUE_HPP
