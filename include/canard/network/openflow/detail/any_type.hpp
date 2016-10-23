#ifndef CANARD_NET_OFP_DETAIL_ANY_TYPE_HPP
#define CANARD_NET_OFP_DETAIL_ANY_TYPE_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/operators.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <canard/network/openflow/detail/type_list.hpp>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Derived> class empty_any_type_base {};

  template <class Decoder, template <class> class Base = empty_any_type_base>
  class any_type
    : public Base<any_type<Decoder, Base>>
    , private boost::equality_comparable<any_type<Decoder, Base>>
  {
  public:
    using header_type = typename Decoder::header_type;
    using type_id = typename Decoder::type_id;
    using type_list = typename Decoder::decode_type_list;
    static constexpr std::uint16_t header_size = Decoder::header_size;

  private:
    using inner_type_list = detail::to_type_list_t<type_list>;
    template <class T>
    using containable_if_t = typename std::enable_if<
      boost::mpl::contains<inner_type_list, typename std::decay<T>::type>::value
    >::type;

  public:
    static constexpr auto min_length() noexcept
      -> std::uint16_t
    {
      return min_element_t<min_length_t>::value;
    }

    static constexpr auto min_byte_length() noexcept
      -> std::uint16_t
    {
      return min_element_t<min_byte_length_t>::value;
    }

    template <class T, class = containable_if_t<T>>
    any_type(T&& t)
      : variant_(std::forward<T>(t))
    {
    }

    template <class T, class = containable_if_t<T>>
    auto operator=(T&& t)
      -> any_type&
    {
      variant_ = std::forward<T>(t);
      return *this;
    }

    CANARD_NET_OFP_DECL auto type() const noexcept
      -> type_id;

    CANARD_NET_OFP_DECL auto length() const noexcept
      -> std::uint16_t;

    CANARD_NET_OFP_DECL auto byte_length() const noexcept
      -> std::uint16_t;

    CANARD_NET_OFP_DECL auto index() const noexcept
      -> std::size_t;

    template <class Visitor>
    auto visit(Visitor&& visitor)
      -> typename std::remove_reference<Visitor>::type::result_type
    {
      return boost::apply_visitor(std::forward<Visitor>(visitor), variant_);
    }

    template <class Visitor>
    auto visit(Visitor&& visitor) const
      -> typename std::remove_reference<Visitor>::type::result_type
    {
      return boost::apply_visitor(std::forward<Visitor>(visitor), variant_);
    }

    template <class Validator>
    void validate(Validator validator) const
    {
      visit(detail::validation_visitor<Validator>{validator});
    }

    template <class Container>
    auto encode(Container& container) const
      -> Container&
    {
      return visit(detail::encoding_visitor<Container>{container});
    }

    template <class Iterator>
    static auto decode(Iterator& first, Iterator last)
      -> any_type
    {
      return Decoder::template decode<any_type>(first, last, to_any{});
    }

    friend auto operator==(any_type const& lhs, any_type const& rhs) noexcept
      -> bool
    {
      return lhs.equal_impl(rhs);
    }

    template <class T, class = containable_if_t<T>>
    friend auto operator==(any_type const& lhs, T const& rhs) noexcept
      -> bool
    {
      if (auto const v = lhs.template ptr_any_cast<T>()) {
        return *v == rhs;
      }
      return false;
    }

    template <class T, class = containable_if_t<T>>
    friend auto operator==(T const& lhs, any_type const& rhs) noexcept
      -> bool
    {
      return rhs == lhs;
    }

    template <class T, class = containable_if_t<T>>
    friend auto operator!=(any_type const& lhs, T const& rhs) noexcept
      -> bool
    {
      return !(lhs == rhs);
    }

    template <class T, class = containable_if_t<T>>
    friend auto operator!=(T const& lhs, any_type const& rhs) noexcept
      -> bool
    {
      return !(rhs == lhs);
    }

    friend auto equivalent(any_type const& lhs, any_type const& rhs) noexcept
      -> bool
    {
      return lhs.equivalent_impl(rhs);
    }

    template <class T, class = containable_if_t<T>>
    friend auto equivalent(any_type const& lhs, T const& rhs) noexcept
      -> bool
    {
      if (auto const v = lhs.template ptr_any_cast<T>()) {
        return equivalent(*v, rhs);
      }
      return false;
    }

    template <class T, class = containable_if_t<T>>
    friend auto equivalent(T const& lhs, any_type const& rhs) noexcept
      -> bool
    {
      return equivalent(rhs, lhs);
    }

    template <class T, class D, template <class> class B>
    friend auto any_cast(any_type<D, B>&)
      -> T&;

    template <class T, class D, template <class> class B>
    friend auto any_cast(any_type<D, B> const&)
      -> T const&;

    template <class T, class D, template <class> class B>
    friend auto any_cast(any_type<D, B>*)
      -> T*;

    template <class T, class D, template <class> class B>
    friend auto any_cast(any_type<D, B> const*)
      -> T const*;

  private:
    template <template <class> class F>
    using min_element_t = typename boost::mpl::deref<
      typename boost::mpl::min_element<
          typename boost::mpl::transform_view<
            inner_type_list, F<boost::mpl::placeholders::_>
          >::type
      >::type
    >::type;

    template <class T>
    struct min_length_t
      : boost::mpl::integral_c<std::uint16_t, T::min_length()>
    {};

    template <class T>
    struct min_byte_length_t
      : boost::mpl::integral_c<std::uint16_t, T::min_byte_length()>
    {};

    CANARD_NET_OFP_DECL auto equal_impl(any_type const&) const noexcept
      -> bool;

    CANARD_NET_OFP_DECL auto equivalent_impl(any_type const&) const noexcept
      -> bool;

    template <class T>
    auto ref_any_cast()
      -> T&
    {
      return boost::get<T>(variant_);
    }

    template <class T>
    auto ref_any_cast() const
      -> T const&
    {
      return boost::get<T>(variant_);
    }

    template <class T>
    auto ptr_any_cast()
      -> T*
    {
      return boost::get<T>(std::addressof(variant_));
    }

    template <class T>
    auto ptr_any_cast() const
      -> T const*
    {
      return boost::get<T>(std::addressof(variant_));
    }

    struct to_any
    {
      template <class T>
      auto operator()(T&& t) const
        -> any_type
      {
        return any_type{std::forward<T>(t)};
      }
    };

  private:
    using variant_t = typename boost::make_variant_over<inner_type_list>::type;
    variant_t variant_;
  };

  template <class T, class Decoder, template <class> class Base>
  auto any_cast(any_type<Decoder, Base>& any)
    -> T&
  {
    return any.template ref_any_cast<T>();
  }

  template <class T, class Decoder, template <class> class Base>
  auto any_cast(any_type<Decoder, Base> const& any)
    -> T const&
  {
    return any.template ref_any_cast<T>();
  }

  template <class T, class Decoder, template <class> class Base>
  auto any_cast(any_type<Decoder, Base>* const any)
    -> T*
  {
    return any->template ptr_any_cast<T>();
  }

  template <class T, class Decoder, template <class> class Base>
  auto any_cast(any_type<Decoder, Base> const* const any)
    -> T const*
  {
    return any->template ptr_any_cast<T>();
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/openflow/detail/impl/any_type.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_TYPE_HPP
