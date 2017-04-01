#ifndef CANARD_NET_OFP_DETAIL_IMPL_ANY_TYPE_HPP
#define CANARD_NET_OFP_DETAIL_IMPL_ANY_TYPE_HPP

#include <canard/net/ofp/detail/config.hpp>
#include <canard/net/ofp/detail/any_type.hpp>

#include <cstddef>
#include <cstdint>
#include <boost/variant/apply_visitor.hpp>
#include <canard/net/ofp/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Decoder, template <class> class Base>
  auto any_type<Decoder, Base>::type() const noexcept
    -> type_id
  {
    return visit(detail::type_visitor<type_id>{});
  }

  template <class Decoder, template <class> class Base>
  auto any_type<Decoder, Base>::length() const noexcept
    -> std::uint16_t
  {
    return visit(detail::length_visitor{});
  }

  template <class Decoder, template <class> class Base>
  auto any_type<Decoder, Base>::byte_length() const noexcept
    -> std::uint16_t
  {
    return visit(detail::byte_length_visitor{});
  }

  template <class Decoder, template <class> class Base>
  auto any_type<Decoder, Base>::index() const noexcept
    -> std::size_t
  {
    return variant_.which();
  }

  template <class Decoder, template <class> class Base>
  auto any_type<Decoder, Base>::equal_impl(
      any_type const& rhs) const noexcept
    -> bool
  {
    return variant_ == rhs.variant_;
  }

  template <class Decoder, template <class> class Base>
  auto any_type<Decoder, Base>::equivalent_impl(
      any_type const& rhs) const noexcept
    -> bool
  {
    auto visitor = detail::equivalent_visitor{};
    return boost::apply_visitor(visitor, variant_, rhs.variant_);
  }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_IMPL_ANY_TYPE_HPP
