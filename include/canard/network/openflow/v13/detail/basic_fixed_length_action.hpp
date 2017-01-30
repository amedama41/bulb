#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_ACTION_HPP

#include <cstdint>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/detail/basic_action.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  class basic_fixed_length_action
    : public basic_action<T>
  {
    using base_t = basic_action<T>;

  public:
    static constexpr auto length() noexcept
      -> std::uint16_t
    {
      return sizeof(typename T::raw_ofp_type);
    }

  protected:
    basic_fixed_length_action() = default;

  private:
    auto derived() const noexcept
      -> T const&
    {
      return *static_cast<T const*>(this);
    }

    friend base_t;

    static constexpr bool is_fixed_length_action = true;

    friend typename base_t::basic_protocol_type;

    template <class Validator>
    void validate_impl(Validator const&) const
    {
      return derived().validate_action();
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, derived().ofp_action());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      return T{detail::decode<typename T::raw_ofp_type>(first, last)};
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(derived().ofp_action(), rhs.derived().ofp_action());
    }

    auto equivalent_impl(T const& rhs) const noexcept
      -> bool
    {
      return derived().is_equivalent_action(rhs);
    }
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_ACTION_HPP
