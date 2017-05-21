#ifndef CANARD_NET_OFP_DETAIL_BASIC_PROTOCOL_TYPE_HPP
#define CANARD_NET_OFP_DETAIL_BASIC_PROTOCOL_TYPE_HPP

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/validator.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class T>
  struct basic_protocol_type_tag {};

  namespace bpt_detail {

    template <class T>
    constexpr auto get_min_length(basic_protocol_type_tag<T>) noexcept
      -> std::uint16_t
    {
      return sizeof(typename T::ofp_type);
    }

    constexpr auto exclude_padding(...) noexcept
      -> bool
    {
      return false;
    }

    template <class T>
    using does_length_exclude_padding_length = std::integral_constant<
      bool, exclude_padding(basic_protocol_type_tag<T>{})
    >;

    constexpr auto exact_length(std::uint16_t const length) noexcept
      -> std::uint16_t
    {
      return (length + 7) / 8 * 8;
    }

    constexpr auto padding_length(std::uint16_t const length) noexcept
      -> std::uint16_t
    {
      return bpt_detail::exact_length(length) - length;
    }

    template <class Container, class T>
    void optionally_encode_padding_impl(
        Container& container, T const& t, std::true_type)
    {
      constexpr std::uint8_t const padding[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      detail::encode_byte_array(
          container, padding, bpt_detail::padding_length(t.length()));
    }

    template <class Container, class T>
    void optionally_encode_padding_impl(
        Container&, T const&, std::false_type) noexcept
    {
    }

    template <class Container, class T>
    void optionally_encode_padding(Container& container, T const& t)
    {
      bpt_detail::optionally_encode_padding_impl(
          container, t, does_length_exclude_padding_length<T>{});
    }

    template <class Iterator, class T>
    void optionally_decode_padding_impl(
        Iterator& first, Iterator, T const& t, std::true_type)
    {
      std::advance(first, bpt_detail::padding_length(t.length()));
    }

    template <class Iterator, class T>
    void optionally_decode_padding_impl(
        Iterator&, Iterator, T const&, std::false_type) noexcept
    {
    }

    template <class Iterator, class T>
    void optionally_decode_padding(Iterator& first, Iterator last, T const& t)
    {
      bpt_detail::optionally_decode_padding_impl(
          first, last, t, does_length_exclude_padding_length<T>{});
    }

    constexpr auto calc_byte_length_impl(
        std::uint16_t const length, std::true_type) noexcept
      -> std::uint16_t
    {
      return bpt_detail::exact_length(length);
    }

    constexpr auto calc_byte_length_impl(
        std::uint16_t const length, std::false_type) noexcept
      -> std::uint16_t
    {
      return length;
    }

    template <class T>
    constexpr auto calc_byte_length(std::uint16_t const length) noexcept
      -> std::uint16_t
    {
      return bpt_detail::calc_byte_length_impl(
          length, does_length_exclude_padding_length<T>{});
    }

    template <class T, class... Args>
    auto public_construct(Args&&... args)
      -> T
    {
      return T(std::forward<Args>(args)...);
    }

  } // namespace bpt_detail

  template <class T>
  class basic_protocol_type
    : private boost::equality_comparable<T>
  {
  public:
    static constexpr auto min_length() noexcept
      -> std::uint16_t
    {
      using bpt_detail::get_min_length;
      return get_min_length(basic_protocol_type_tag<T>{});
    }

    static constexpr auto min_byte_length() noexcept
      -> std::uint16_t
    {
      return bpt_detail::calc_byte_length<T>(min_length());
    }

    auto byte_length() const noexcept
      -> std::uint16_t
    {
      return bpt_detail::calc_byte_length<T>(derived().length());
    }

    template <class Validator>
    void validate(Validator&& validator) const
    {
      derived().validate_impl(std::forward<Validator>(validator));
    }

    template <class Container>
    auto encode(Container& container) const
      -> Container&
    {
      derived().encode_impl(container);
      bpt_detail::optionally_encode_padding(container, derived());
      return container;
    }

    template <class Iterator>
    static auto decode(Iterator& first, Iterator last)
      -> T
    {
      auto t = T::decode_impl(first, last);
      bpt_detail::optionally_decode_padding(first, last, t);
      return t;
    }

    template <class Iterator>
    static auto decode_without_consumption(Iterator first, Iterator last)
      -> T
    {
      auto t = T::decode_impl(first, last);
      bpt_detail::optionally_decode_padding(first, last, t);
      return t;
    }

    template <class... Args>
    static auto create(Args&&... args)
      -> T
    {
      // construct through non-member function not to use private constructors
      return validation::validate(
          bpt_detail::public_construct<T>(std::forward<Args>(args)...));
    }

    friend auto operator==(
        T const& lhs, T const& rhs) noexcept(noexcept(lhs.is_equal(rhs)))
      -> bool
    {
      return lhs.is_equal(rhs);
    }

    friend auto equivalent(
        T const& lhs, T const& rhs) noexcept(noexcept(lhs.is_equivalent(rhs)))
      -> bool
    {
      return lhs.is_equivalent(rhs);
    }

  private:
    auto derived() const noexcept
      -> T const&
    {
      return static_cast<T const&>(*this);
    }

    auto is_equal(T const& rhs) const
        noexcept(noexcept(derived().equal_impl(rhs)))
      -> bool
    {
      return derived().equal_impl(rhs);
    }

    auto is_equivalent(T const& rhs) const
        noexcept(noexcept(derived().equivalent_impl(rhs)))
      -> bool
    {
      return derived().equivalent_impl(rhs);
    }
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_BASIC_PROTOCOL_TYPE_HPP
