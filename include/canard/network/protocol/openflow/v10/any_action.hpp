#ifndef CANARD_NET_OFP_V10_ANY_ACTION_HPP
#define CANARD_NET_OFP_V10_ANY_ACTION_HPP

#include <canard/network/protocol/openflow/detail/any_action.hpp>
#include <canard/network/protocol/openflow/v10/actions.hpp>
#include <canard/network/protocol/openflow/v10/decoder/action_decoder.hpp>
#include <canard/network/protocol/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    using any_action = detail::any_action<action_decoder>;

    template <class T>
    auto any_cast(any_action const& action)
        -> T const&
    {
        return detail::any_cast<T>(action);
    }

    template <class T>
    auto any_cast(any_action const* const action)
        -> T const*
    {
        return detail::any_cast<T>(action);
    }

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ANY_ACTION_HPP
