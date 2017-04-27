#ifndef CANARD_NET_OFP_DETAIL_VARIANT_HPP
#define CANARD_NET_OFP_DETAIL_VARIANT_HPP

#include <memory>
#include <utility>

#if defined(CANARD_NET_OFP_USE_MAPBOX_VARIANT)

# include <mapbox/variant.hpp>

namespace canard {
namespace net {
namespace ofp {

  using bad_get = mapbox::util::bad_variant_access;

  namespace detail {

    using mapbox::util::variant;

    template <class TypeList>
    struct make_variant_over;
    template <template <class...> class TypeList, class... Types>
    struct make_variant_over<TypeList<Types...>>
    {
      using type = variant<Types...>;
    };

    template <class F, class... Variants>
    auto apply_visitor(F&& f, Variants&&... vs)
      -> decltype(mapbox::util::apply_visitor(
            std::forward<F>(f), std::forward<Variants>(vs)...))
    {
      return mapbox::util::apply_visitor(
          std::forward<F>(f), std::forward<Variants>(vs)...);
    }

    template <class ResultType, class T>
    auto get(T& v)
      -> decltype(mapbox::util::get<ResultType>(v))
    {
      return mapbox::util::get<ResultType>(v);
    }

    template <class ResultType, class T>
    auto get(T const& v)
      -> decltype(mapbox::util::get<ResultType>(v))
    {
      return mapbox::util::get<ResultType>(v);
    }

    template <class ResultType, class T>
    auto get(T* const v)
      -> decltype(std::addressof(mapbox::util::get<ResultType>(*v)))
    {
      if (!v->template is<ResultType>()) {
        return nullptr;
      }
      return std::addressof(mapbox::util::get<ResultType>(*v));
    }

    template <class ResultType, class T>
    auto get(T const* const v)
      -> decltype(std::addressof(mapbox::util::get<ResultType>(*v)))
    {
      if (!v->template is<ResultType>()) {
        return nullptr;
      }
      return std::addressof(mapbox::util::get<ResultType>(*v));
    }

  } // namespace detail

} // namespace ofp
} // namespace net
} // namespace canard

#else

# include <boost/variant/apply_visitor.hpp>
# include <boost/variant/get.hpp>
# include <boost/variant/variant.hpp>

namespace canard {
namespace net {
namespace ofp {

  using bad_get = boost::bad_get;

  namespace detail {

    using boost::variant;

    using boost::make_variant_over;

    template <class F, class... Variants>
    auto apply_visitor(F&& f, Variants&&... vs)
      -> decltype(boost::apply_visitor(
            std::forward<F>(f), std::forward<Variants>(vs)...))
    {
      return boost::apply_visitor(
          std::forward<F>(f), std::forward<Variants>(vs)...);
    }

    template <class ResultType, class T>
    auto get(T& v)
      -> decltype(boost::get<ResultType>(v))
    {
      return boost::get<ResultType>(v);
    }

    template <class ResultType, class T>
    auto get(T const& v)
      -> decltype(boost::get<ResultType>(v))
    {
      return boost::get<ResultType>(v);
    }

    template <class ResultType, class T>
    auto get(T* const v)
      -> decltype(boost::get<ResultType>(v))
    {
      return boost::get<ResultType>(v);
    }

    template <class ResultType, class T>
    auto get(T const* const v)
      -> decltype(boost::get<ResultType>(v))
    {
      return boost::get<ResultType>(v);
    }

  } // namespace detail

} // namespace ofp
} // namespace net
} // namespace canard

#endif

#endif // CANARD_NET_OFP_DETAIL_VARIANT_HPP
