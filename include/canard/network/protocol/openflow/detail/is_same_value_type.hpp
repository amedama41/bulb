#ifndef CANARD_NET_OFP_DETAIL_IS_SAME_VALUE_TYPE_HPP
#define CANARD_NET_OFP_DETAIL_IS_SAME_VALUE_TYPE_HPP

#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
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
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IS_SAME_VALUE_TYPE_HPP
