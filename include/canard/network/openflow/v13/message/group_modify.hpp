#ifndef CANARD_NET_OFP_V13_GROUP_MODIFY_HPP
#define CANARD_NET_OFP_V13_GROUP_MODIFY_HPP

#include <cstdint>
#include <utility>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/detail/group_mod_base.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class group_modify
        : public detail::v13::group_mod_base<group_modify>
    {
    public:
        static protocol::ofp_group_mod_command const command_type
            = protocol::OFPGC_MODIFY;

        group_modify(
                  std::uint32_t const group_id
                , std::uint8_t const group_type
                , buckets_type buckets
                , std::uint32_t const xid = get_xid())
            : group_mod_base{group_id, group_type, std::move(buckets), xid}
        {
        }

    private:
        friend group_mod_base;

        group_modify(raw_ofp_type const& group_mod, buckets_type&& buckets)
            : group_mod_base{group_mod, std::move(buckets)}
        {
        }
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_GROUP_MODIFY_HPP
