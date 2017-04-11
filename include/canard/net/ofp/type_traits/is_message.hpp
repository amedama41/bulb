#ifndef CANARD_NET_OFP_TYPE_TRAITS_IS_MESSAGE_HPP
#define CANARD_NET_OFP_TYPE_TRAITS_IS_MESSAGE_HPP

#include <utility>
#include <type_traits>

namespace canard {
namespace net {
namespace ofp {
namespace type_traits {

  namespace is_message_detail {

    template <class T>
    auto is_message_impl(T const& t) noexcept
      -> decltype(
            (void)(t.version(), t.type(), t.length(), t.xid())
          , std::true_type{});

    auto is_message_impl(...) noexcept
      -> std::false_type;

  } // namespace is_message_detail

  template <class T>
  struct is_message
    : decltype(is_message_detail::is_message_impl(std::declval<T>()))
  {
  };

} // namespace type_traits
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_TYPE_TRAITS_IS_MESSAGE_HPP
