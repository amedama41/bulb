#ifndef CANARD_NET_OFP_V13_GROUP_MOD_HPP
#define CANARD_NET_OFP_V13_GROUP_MOD_HPP

#include <canard/network/openflow/v13/message/group_add.hpp>
#include <canard/network/openflow/v13/message/group_delete.hpp>
#include <canard/network/openflow/v13/message/group_modify.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

using messages::group_add;
using messages::group_modify;
using messages::group_delete;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_GROUP_MOD_HPP
