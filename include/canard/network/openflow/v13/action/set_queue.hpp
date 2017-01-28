#ifndef CANARD_NET_OFP_V13_ACTIONS_SET_QUEUE_HPP
#define CANARD_NET_OFP_V13_ACTIONS_SET_QUEUE_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/v13/detail/basic_fixed_length_action.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class set_queue
        : public detail::v13::basic_fixed_length_action<set_queue>
    {
    public:
        using raw_ofp_type = protocol::ofp_action_set_queue;

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

    private:
        friend basic_fixed_length_action;

        explicit set_queue(raw_ofp_type const& action_set_queue) noexcept
            : action_set_queue_(action_set_queue)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return action_set_queue_;
        }

        void validate_action() const
        {
            if (queue_id() == protocol::OFPQ_ALL) {
                throw std::runtime_error{"invalid queue_id"};
            }
        }

        auto is_equivalent_action(set_queue const& rhs) const noexcept
            -> bool
        {
            return queue_id() == rhs.queue_id();
        }

    private:
        raw_ofp_type action_set_queue_;
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_SET_QUEUE_HPP
