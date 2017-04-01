#ifndef CANARD_NET_OFP_V13_ACTIONS_POP_MPLS_HPP
#define CANARD_NET_OFP_V13_ACTIONS_POP_MPLS_HPP

#include <cstdint>
#include <canard/network/openflow/v13/detail/basic_fixed_length_action.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class pop_mpls
        : public detail::v13::basic_fixed_length_action<pop_mpls>
    {
    public:
        using raw_ofp_type = protocol::ofp_action_pop_mpls;

        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_POP_MPLS;

        explicit pop_mpls(std::uint16_t const ethertype) noexcept
            : action_pop_mpls_{
                  action_type
                , length()
                , ethertype
                , { 0, 0 }
              }
        {
        }

        auto ethertype() const noexcept
            -> std::uint16_t
        {
            return action_pop_mpls_.ethertype;
        }

        static auto ipv4() noexcept
            -> pop_mpls
        {
            return pop_mpls{0x0800};
        }

        static auto unicast() noexcept
            -> pop_mpls
        {
            return pop_mpls{0x8847};
        }

        static auto multicast() noexcept
            -> pop_mpls
        {
            return pop_mpls{0x8848};
        }

    private:
        friend basic_fixed_length_action;

        explicit pop_mpls(raw_ofp_type const& action_pop_mpls) noexcept
            : action_pop_mpls_(action_pop_mpls)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return action_pop_mpls_;
        }

        void validate_action() const
        {
        }

        auto is_equivalent_action(pop_mpls const& rhs) const noexcept
            -> bool
        {
            return ethertype() == rhs.ethertype();
        }

    private:
        raw_ofp_type action_pop_mpls_;
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_POP_MPLS_HPP
