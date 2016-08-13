#ifndef CANARD_NET_OFP_V13_ACTION_LIST_HPP
#define CANARD_NET_OFP_V13_ACTION_LIST_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <canard/network/openflow/detail/action_list.hpp>
#include <canard/network/openflow/v13/any_action.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    using action_list
        = detail::action_list<any_action, v13_detail::ofp_action_header>;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {
namespace detail {

    extern template class action_list<
        ofp::v13::any_action, ofp::v13::v13_detail::ofp_action_header
    >;

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

# endif
#endif

#endif // CANARD_NET_OFP_V13_ACTION_LIST_HPP