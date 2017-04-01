#ifndef CANARD_NET_OFP_V13_ACTIONS_DECREMENT_NW_TTL_HPP
#define CANARD_NET_OFP_V13_ACTIONS_DECREMENT_NW_TTL_HPP

#include <canard/net/ofp/v13/detail/basic_generic_action.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class decrement_nw_ttl
        : public detail::v13::basic_generic_action<decrement_nw_ttl>
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_DEC_NW_TTL;

        decrement_nw_ttl() = default;

    private:
        friend basic_generic_action::basic_fixed_length_action;

        explicit decrement_nw_ttl(raw_ofp_type const& ofp_action) noexcept
            : basic_generic_action{ofp_action}
        {
        }
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_DECREMENT_NW_TTL_HPP
