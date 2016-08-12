#ifndef CANARD_NETWORK_OPENFLOW_V13_ACTION_LIST_HPP
#define CANARD_NETWORK_OPENFLOW_V13_ACTION_LIST_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#include <canard/network/protocol/openflow/detail/action_list.hpp>
#include <canard/network/protocol/openflow/v13/any_action.hpp>
#include <canard/network/protocol/openflow/v13/openflow.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {

    using action_list
        = detail::action_list<any_action, v13_detail::ofp_action_header>;

} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    extern template class action_list<
        openflow::v13::any_action, openflow::v13::v13_detail::ofp_action_header
    >;

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

# endif
#endif

#endif // CANARD_NETWORK_OPENFLOW_V13_ACTION_LIST_HPP
