#ifndef CANARD_NET_OFP_DETAIL_BASIC_PROTOCOL_TYPE_HPP
#define CANARD_NET_OFP_DETAIL_BASIC_PROTOCOL_TYPE_HPP

#include <cstdint>
#include <utility>
#include <boost/operators.hpp>
#include <canard/network/openflow/validator.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class T>
  constexpr auto get_min_length(T*) noexcept
    -> std::uint16_t
  {
    return sizeof(typename T::raw_ofp_type);
  };

  template <class T>
  class basic_protocol_type
    : private boost::equality_comparable<T>
  {
  public:
    static constexpr auto min_length() noexcept
      -> std::uint16_t
    {
      return get_min_length(static_cast<T*>(nullptr));
    }

    template <class Validator>
    void validate(Validator&& validator) const
    {
      derived()->validate_impl(std::forward<Validator>(validator));
    }

    template <class Container>
    auto encode(Container& container) const
      -> Container&
    {
      derived()->encode_impl(container);
      return container;
    }

    template <class Iterator>
    static auto decode(Iterator& first, Iterator last)
      -> T
    {
      return T::decode_impl(first, last);
    }

    template <class... Args>
    static auto create(Args&&... args)
      -> T
    {
      return validation::validate(T(std::forward<Args>(args)...));
    }

    friend auto operator==(
        T const& lhs, T const& rhs) noexcept(noexcept(lhs.equal_impl(rhs)))
      -> bool
    {
      return lhs.equal_impl(rhs);
    }

    friend auto equivalent(
        T const& lhs, T const& rhs) noexcept(noexcept(lhs.equivalent_impl(rhs)))
      -> bool
    {
      return lhs.equivalent_impl(rhs);
    }

  private:
    auto derived() const noexcept
      -> T const*
    {
      return static_cast<T const*>(this);
    }
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_BASIC_PROTOCOL_TYPE_HPP
