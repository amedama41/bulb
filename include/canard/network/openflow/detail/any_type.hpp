#ifndef CANARD_NET_OFP_DETAIL_ANY_TYPE_HPP
#define CANARD_NET_OFP_DETAIL_ANY_TYPE_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/type_list.hpp>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class T>
  struct get_decoder;

  template <template <class> class T, class Decoder>
  struct get_decoder<T<Decoder>>
  {
    using type = Decoder;
  };

  template <class T>
  using get_decoder_t = typename get_decoder<T>::type;


  template <class Derived, class Decoder = get_decoder_t<Derived>>
  class any_type
    : public detail::basic_protocol_type<Derived>
  {
  public:
    using header_type = typename Decoder::header_type;
    using type_list = typename Decoder::decode_type_list;

  private:
    using inner_type_list = detail::to_type_list_t<type_list>;
    template <class T>
    using containable_if_t = typename std::enable_if<
      boost::mpl::contains<inner_type_list, typename std::decay<T>::type>::value
    >::type;

  public:
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

    CANARD_NET_OFP_DECL auto length() const noexcept
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

    template <class T, class = containable_if_t<T>>
    friend auto operator==(Derived const& lhs, T const& rhs) noexcept
      -> bool
    {
      if (auto const v = lhs.template ptr_any_cast<T>()) {
        return *v == rhs;
      }
      return false;
    }

    template <class T, class = containable_if_t<T>>
    friend auto operator==(T const& lhs, Derived const& rhs) noexcept
      -> bool
    {
      return rhs == lhs;
    }

    template <class T, class = containable_if_t<T>>
    friend auto operator!=(Derived const& lhs, T const& rhs) noexcept
      -> bool
    {
      return !(lhs == rhs);
    }

    template <class T, class = containable_if_t<T>>
    friend auto operator!=(T const& lhs, Derived const& rhs) noexcept
      -> bool
    {
      return !(rhs == lhs);
    }

    template <class T, class = containable_if_t<T>>
    friend auto equivalent(Derived const& lhs, T const& rhs) noexcept
      -> bool
    {
      if (auto const v = lhs.template ptr_any_cast<T>()) {
        return equivalent(*v, rhs);
      }
      return false;
    }

    template <class T, class = containable_if_t<T>>
    friend auto equivalent(T const& lhs, Derived const& rhs) noexcept
      -> bool
    {
      return equivalent(rhs, lhs);
    }

    template <class T, class D1, class D2>
    friend auto any_cast(any_type<D1, D2>&)
      -> T&;

    template <class T, class D1, class D2>
    friend auto any_cast(any_type<D1, D2> const&)
      -> T const&;

    template <class T, class D1, class D2>
    friend auto any_cast(any_type<D1, D2>*)
      -> T*;

    template <class T, class D1, class D2>
    friend auto any_cast(any_type<D1, D2> const*)
      -> T const*;

  private:
    friend detail::basic_protocol_type<Derived>;

    template <class LHS, class RHS>
    struct less_min_length
    {
      using type = boost::mpl::bool_<(LHS::min_length() < RHS::min_length())>;
    };

    friend constexpr auto get_min_length(Derived*) noexcept
      -> std::uint16_t
    {
      return boost::mpl::deref<
        typename boost::mpl::min_element<
            any_type::inner_type_list
          , any_type::less_min_length<
              boost::mpl::placeholders::_1, boost::mpl::placeholders::_2
            >
        >::type
      >::type::min_length();
    }

    template <class Validator>
    void validate_impl(Validator validator) const
    {
      visit(detail::validation_visitor<Validator>{validator});
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      visit(detail::encoding_visitor<Container>{container});
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> Derived
    {
      return Decoder::template decode<Derived>(first, last, to_any{});
    }

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
        -> Derived
      {
        return Derived{std::forward<T>(t)};
      }
    };

  private:
    using variant_t = typename boost::make_variant_over<inner_type_list>::type;
    variant_t variant_;
  };

  template <class T, class Derived, class Decoder>
  auto any_cast(any_type<Derived, Decoder>& any)
    -> T&
  {
    return any.template ref_any_cast<T>();
  }

  template <class T, class Derived, class Decoder>
  auto any_cast(any_type<Derived, Decoder> const& any)
    -> T const&
  {
    return any.template ref_any_cast<T>();
  }

  template <class T, class Derived, class Decoder>
  auto any_cast(any_type<Derived, Decoder>* const any)
    -> T*
  {
    return any->template ptr_any_cast<T>();
  }

  template <class T, class Derived, class Decoder>
  auto any_cast(any_type<Derived, Decoder> const* const any)
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
