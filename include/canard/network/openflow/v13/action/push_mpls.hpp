#ifndef CANARD_NET_OFP_V13_ACTIONS_PUSH_MPLS_HPP
#define CANARD_NET_OFP_V13_ACTIONS_PUSH_MPLS_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/v13/detail/basic_action_push.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class push_mpls
        : public detail::v13::basic_action_push<push_mpls>
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_PUSH_MPLS;

        explicit push_mpls(std::uint16_t const ethertype) noexcept
            : basic_action_push{ethertype}
        {
        }

        static auto unicast() noexcept
            -> push_mpls
        {
            return push_mpls{0x8847};
        }

        static auto multicast() noexcept
            -> push_mpls
        {
            return push_mpls{0x8848};
        }

    private:
        friend basic_action;

        explicit push_mpls(raw_ofp_type const& ofp_action) noexcept
            : basic_action_push{ofp_action}
        {
        }

        template <class Validator>
        void validate_impl(Validator) const
        {
            if (ethertype() != 0x8847 && ethertype() != 0x8848) {
                throw std::runtime_error{"invalid ethertype"};
            }
        }
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_PUSH_MPLS_HPP
