#ifndef CANARD_NET_OFP_V13_ACTIONS_PUSH_PBB_HPP
#define CANARD_NET_OFP_V13_ACTIONS_PUSH_PBB_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/v13/detail/basic_action_push.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class push_pbb
        : public detail::v13::basic_action_push<push_pbb>
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_PUSH_PBB;

        push_pbb() noexcept
            : basic_action_push{0x88e7}
        {
        }

    private:
        friend basic_action_push::basic_fixed_length_action;

        explicit push_pbb(raw_ofp_type const& ofp_action) noexcept
            : basic_action_push(ofp_action)
        {
        }

        void validate_action() const
        {
            if (ethertype() != 0x88e7) {
                throw std::runtime_error{"invalid ethertype"};
            }
        }
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_PUSH_PBB_HPP
