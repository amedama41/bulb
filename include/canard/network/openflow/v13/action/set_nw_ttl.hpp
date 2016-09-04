#ifndef CANARD_NET_OFP_V13_ACTIONS_SET_NW_TTL_HPP
#define CANARD_NET_OFP_V13_ACTIONS_SET_NW_TTL_HPP

#include <cstdint>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/detail/basic_action.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class set_nw_ttl
        : public detail::v13::basic_action<
            set_nw_ttl, v13_detail::ofp_action_nw_ttl
          >
    {
    public:
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

        friend auto operator==(
                set_nw_ttl const& lhs, set_nw_ttl const& rhs) noexcept
            -> bool
        {
            return detail::memcmp(lhs.action_nw_ttl_, rhs.action_nw_ttl_);
        }

    private:
        friend basic_action;

        explicit set_nw_ttl(raw_ofp_type const& action_nw_ttl) noexcept
            : action_nw_ttl_(action_nw_ttl)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return action_nw_ttl_;
        }

        template <class Validator>
        void validate_impl(Validator) const
        {
        }

    private:
        raw_ofp_type action_nw_ttl_;
    };

    inline auto equivalent(
            set_nw_ttl const& lhs, set_nw_ttl const& rhs) noexcept
        -> bool
    {
        return lhs.ttl() == rhs.ttl();
    }

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_SET_NW_TTL_HPP
