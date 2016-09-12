#ifndef CANARD_NET_OFP_V10_ACTIONS_ENQUEUE_HPP
#define CANARD_NET_OFP_V10_ACTIONS_ENQUEUE_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/v10/detail/basic_action.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace actions {

    class enqueue
        : public actions_detail::basic_action<
                enqueue, v10_detail::ofp_action_enqueue
          >
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_ENQUEUE;

        enqueue(std::uint32_t const queue_id
              , std::uint16_t const port_no) noexcept
            : enqueue_{
                  action_type
                , sizeof(raw_ofp_type)
                , port_no
                , { 0, 0, 0, 0, 0, 0 }
                , queue_id
              }
        {
        }

        auto queue_id() const noexcept
            -> std::uint32_t
        {
            return enqueue_.queue_id;
        }

        auto port_no() const noexcept
            -> std::uint16_t
        {
            return enqueue_.port;
        }

    private:
        friend basic_action;
        friend basic_protocol_type;

        explicit enqueue(raw_ofp_type const& action_enqueue) noexcept
            : enqueue_(action_enqueue)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return enqueue_;
        }

        template <class Validator>
        void validate_impl(Validator) const
        {
            if (queue_id() == protocol::OFPQ_ALL) {
                throw std::runtime_error{"invalid queue_id"};
            }
            if (port_no() == 0
                    || (port_no() > protocol::OFPP_MAX
                        && port_no() != protocol::OFPP_IN_PORT)) {
                throw std::runtime_error{"invalid port_no"};
            }
        }

        auto equivalent_impl(enqueue const& rhs) const noexcept
            -> bool
        {
            return queue_id() == rhs.queue_id()
                && port_no() == rhs.port_no();
        }

    private:
        raw_ofp_type enqueue_;
    };

} // namespace actions
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTIONS_ENQUEUE_HPP
