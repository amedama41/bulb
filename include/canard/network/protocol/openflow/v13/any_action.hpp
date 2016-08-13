#ifndef CANARD_NETWORK_OPENFLOW_V13_ANY_ACTION_HPP
#define CANARD_NETWORK_OPENFLOW_V13_ANY_ACTION_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#include <cstdint>
#include <canard/network/protocol/openflow/detail/any_action.hpp>
#include <canard/network/protocol/openflow/v13/action_order.hpp>
#include <canard/network/protocol/openflow/v13/decoder/action_decoder.hpp>
#include <canard/network/protocol/openflow/v13/openflow.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {

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

    template <>
    struct action_order<any_action>
    {
        CANARD_NET_OFP_DECL static auto get_value(any_action const&)
            -> std::uint64_t;
    };

} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)

#include <canard/network/protocol/openflow/v13/impl/any_action.ipp>

#elif defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    extern template class any_action<openflow::v13::action_decoder>;

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif

#endif // CANARD_NETWORK_OPENFLOW_V13_ANY_ACTION_HPP
