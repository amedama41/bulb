#ifndef CANARD_NET_OFP_V13_ACTIONS_SET_NW_TTL_HPP
#define CANARD_NET_OFP_V13_ACTIONS_SET_NW_TTL_HPP

#include <cstdint>
#include <canard/net/ofp/v13/detail/basic_fixed_length_action.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class set_nw_ttl
        : public detail::v13::basic_fixed_length_action<set_nw_ttl>
    {
    public:
        using raw_ofp_type = protocol::ofp_action_nw_ttl;

        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_SET_NW_TTL;

        explicit set_nw_ttl(std::uint8_t const nw_ttl) noexcept
            : action_nw_ttl_{
                  action_type
                , length()
                , nw_ttl
                , { 0, 0, 0 }
              }
        {
        }

        auto ttl() const noexcept
            -> std::uint8_t
        {
            return action_nw_ttl_.nw_ttl;
        }

    private:
        friend basic_fixed_length_action;

        explicit set_nw_ttl(raw_ofp_type const& action_nw_ttl) noexcept
            : action_nw_ttl_(action_nw_ttl)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return action_nw_ttl_;
        }

        void validate_action() const
        {
        }

        auto is_equivalent_action(set_nw_ttl const& rhs) const noexcept
            -> bool
        {
            return ttl() == rhs.ttl();
        }

    private:
        raw_ofp_type action_nw_ttl_;
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_SET_NW_TTL_HPP
