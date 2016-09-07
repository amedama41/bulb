#ifndef CANARD_NET_OFP_DETAIL_TYPE_LIST_HPP
#define CANARD_NET_OFP_DETAIL_TYPE_LIST_HPP

#include <tuple>
#include <boost/mpl/advance_fwd.hpp>
#include <boost/mpl/at_fwd.hpp>
#include <boost/mpl/back_fwd.hpp>
#include <boost/mpl/begin_end_fwd.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/clear_fwd.hpp>
#include <boost/mpl/distance_fwd.hpp>
#include <boost/mpl/empty_fwd.hpp>
#include <boost/mpl/front_fwd.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/push_back_fwd.hpp>
#include <boost/mpl/push_front_fwd.hpp>
#include <boost/mpl/size_fwd.hpp>
#include <boost/mpl/size_t.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class T, class N>
  struct type_list_iterator
  {
    using category = boost::mpl::random_access_iterator_tag;
    using type = typename T::template element<N::value>::type;
    using next = type_list_iterator<T, boost::mpl::size_t<N::value + 1>>;
    using prior = type_list_iterator<T, boost::mpl::size_t<N::value - 1>>;
  };

  template <class T>
  struct type_list_iterator<T, boost::mpl::size_t<T::size>>
  {
    using category = boost::mpl::random_access_iterator_tag;
    using prior = type_list_iterator<T, boost::mpl::size_t<T::size - 1>>;
  };

  struct type_list_tag {};

  template <class... Ts>
  struct type_list
  {
    using tuple = std::tuple<Ts...>;
    template <std::size_t N>
    using element = std::tuple_element<N, tuple>;
    static constexpr std::size_t size = std::tuple_size<tuple>::value;

    using tag = type_list_tag;
  };

  template <class T>
  struct to_type_list;

  template <class T>
  using to_type_list_t = typename to_type_list<T>::type;

  template <class... Ts>
  struct to_type_list<std::tuple<Ts...>>
  {
    using type = type_list<Ts...>;
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

namespace boost {
namespace mpl {

  template <class TypeList, class N, class M>
  struct advance<canard::net::ofp::detail::type_list_iterator<TypeList, N>, M>
  {
    static_assert(TypeList::size <= N::value + M::value, "");

    using type = canard::net::ofp::detail::type_list_iterator<
      TypeList, boost::mpl::size_t<N::value + M::value>
    >;
  };

  template <class TypeList, class N1, class N2>
  struct distance<
      canard::net::ofp::detail::type_list_iterator<TypeList, N1>
    , canard::net::ofp::detail::type_list_iterator<TypeList, N2>
  >
    : public boost::mpl::long_<
        ((N2::value > N1::value)
          ? long(N2::value - N1::value) : -long(N1::value - N2::value))
      >
  {
  };

  template <>
  struct begin_impl<canard::net::ofp::detail::type_list_tag>
  {
    template <class T>
    struct apply
    {
      using type = canard::net::ofp::detail::type_list_iterator<
        T, boost::mpl::size_t<0>
      >;
    };
  };

  template <>
  struct end_impl<canard::net::ofp::detail::type_list_tag>
  {
    template <class T>
    struct apply
    {
      using type = canard::net::ofp::detail::type_list_iterator<
        T, boost::mpl::size_t<T::size>
      >;
    };
  };

  template <>
  struct size_impl<canard::net::ofp::detail::type_list_tag>
  {
    template <class T>
    struct apply
    {
      using type = boost::mpl::size_t<T::size>;
    };
  };

  template <>
  struct empty_impl<canard::net::ofp::detail::type_list_tag>
  {
    template <class T>
    struct apply
    {
      using type = boost::mpl::bool_<T::size == 0>;
    };
  };

  template <>
  struct front_impl<canard::net::ofp::detail::type_list_tag>
  {
    template <class T>
    struct apply
      : T::template element<0>
    {
    };
  };

  template <>
  struct back_impl<canard::net::ofp::detail::type_list_tag>
  {
    template <class T>
    struct apply
      : T::template element<T::size - 1>
    {
    };
  };

  template <>
  struct at_impl<canard::net::ofp::detail::type_list_tag>
  {
    template <class T, class N>
    struct apply
      : T::template element<N::value>
    {
    };
  };

  template <>
  struct clear_impl<canard::net::ofp::detail::type_list_tag>
  {
    template <class T>
    struct apply
    {
      using type = canard::net::ofp::detail::type_list<>;
    };
  };

  template <class... Ts, class T>
  struct push_back<canard::net::ofp::detail::type_list<Ts...>, T>
  {
    using type = canard::net::ofp::detail::type_list<Ts..., T>;
  };

  template <class... Ts, class T>
  struct push_front<canard::net::ofp::detail::type_list<Ts...>, T>
  {
    using type = canard::net::ofp::detail::type_list<T, Ts...>;
  };

} // namespace mpl
} // namespace boost

#endif // CANARD_NET_OFP_DETAIL_TYPE_LIST_HPP
