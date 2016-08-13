#ifndef CANARD_NET_OFP_DETAIL_CONSTRUCT_HPP
#define CANARD_NET_OFP_DETAIL_CONSTRUCT_HPP

#include <utility>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

    template <class To>
    struct construct
    {
        template <class T>
        auto operator()(T&& t) const
            -> To
        {
            return To(std::forward<T>(t));
        }
    };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_CONSTRUCT_HPP
