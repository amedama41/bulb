#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_GENERIC_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_GENERIC_ACTION_HPP

#include <canard/network/openflow/v13/detail/basic_action.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

    template <class T>
    class basic_generic_action
        : public basic_action<T, ofp::v13::v13_detail::ofp_action_header>
    {
        using base_type
            = basic_action<T, ofp::v13::v13_detail::ofp_action_header>;

    public:
        using raw_ofp_type = typename base_type::raw_ofp_type;

    protected:
        basic_generic_action() noexcept
            : action_header_{
                  T::action_type
                , base_type::length()
                , { 0, 0, 0, 0 }
              }
        {
        }

        explicit basic_generic_action(
                raw_ofp_type const& action_header) noexcept
            : action_header_(action_header)
        {
        }

    private:
        friend base_type;

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return action_header_;
        }

        void validate_action() const
        {
        }

        auto is_equivalent_action(T const&) const noexcept
            -> bool
        {
            return true;
        }

    private:
        raw_ofp_type action_header_;
    };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_GENERIC_ACTION_HPP
