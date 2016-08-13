#ifndef CANARD_NET_OFP_DETAIL_ADD_HELPER_HPP
#define CANARD_NET_OFP_DETAIL_ADD_HELPER_HPP

#include <utility>

namespace canard {
namespace net {
namespace ofp {

    namespace detail {

        template <class Container>
        struct add_helper
        {
            template <class T>
            void operator()(T&& t) const
            {
                container.add(std::forward<T>(t));
            }

            Container& container;
        };

    } // namespace detail

} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_ADD_HELPER_HPP
