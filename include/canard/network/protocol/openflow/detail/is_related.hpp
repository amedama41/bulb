#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_IS_RELATED_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_IS_RELATED_HPP

#include <type_traits>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    namespace is_related_detail {

        template <class T>
        using remove_cvref_t = typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type;

    } // namespace is_related_detail

    template <class T, class... U>
    struct is_related : std::false_type {};

    template <class T, class U>
    struct is_related<T, U> : std::is_same<
          is_related_detail::remove_cvref_t<T>
        , is_related_detail::remove_cvref_t<U>
    > {};

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_IS_RELATED_HPP
