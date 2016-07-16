#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_IS_SAME_VALUE_TYPE_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_IS_SAME_VALUE_TYPE_HPP

#include <type_traits>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    namespace is_same_value_type_detail {

        template <class T>
        using remove_cvref_t = typename std::remove_cv<
            typename std::remove_reference<T>::type
        >::type;

    } // namespace is_same_value_type_detail

    template <class T, class U>
    struct is_same_value_type
        : std::is_same<
            is_same_value_type_detail::remove_cvref_t<T>
          , is_same_value_type_detail::remove_cvref_t<U>
          >
    {
    };

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_IS_SAME_VALUE_TYPE_HPP
