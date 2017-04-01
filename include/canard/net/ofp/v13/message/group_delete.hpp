#ifndef CANARD_NET_OFP_V13_GROUP_DELETE_HPP
#define CANARD_NET_OFP_V13_GROUP_DELETE_HPP

#include <cstdint>
#include <utility>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/detail/group_mod_base.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    class group_delete
        : public detail::v13::group_mod_base<group_delete>
    {
    public:
        static protocol::ofp_group_mod_command const command_type
            = protocol::OFPGC_DELETE;

        explicit group_delete(
                  std::uint32_t const group_id
                , std::uint32_t const xid = get_xid())
            : group_mod_base{group_id, xid}
        {
        }

    private:
        friend group_mod_base;

        group_delete(raw_ofp_type const& group_mod, buckets_type&& buckets)
            : group_mod_base{group_mod, std::move(buckets)}
        {
        }
    };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_GROUP_DELETE_HPP
