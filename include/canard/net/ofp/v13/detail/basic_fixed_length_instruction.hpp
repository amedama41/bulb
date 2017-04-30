#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_INSTRUCTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_INSTRUCTION_HPP

#include <cstdint>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/v13/detail/basic_instruction.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  class basic_fixed_length_instruction
    : public basic_instruction<T>
  {
    using base_t = basic_instruction<T>;

  protected:
    basic_fixed_length_instruction() = default;

  public:
    auto length() const noexcept
      -> std::uint16_t
    {
      return T::min_length();
    }

  private:
    auto derived() const noexcept
      -> T const&
    {
      return *static_cast<T const*>(this);
    }

    friend base_t;

    static constexpr bool is_fixed_length_instruction = true;

    friend typename base_t::basic_protocol_type;

    template <class Validator>
    void validate_impl(Validator) const
    {
      static_cast<T const*>(this)->validate_instruction();
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, derived().ofp_instruction());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      return T{detail::decode<typename T::ofp_type>(first, last)};
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(
          derived().ofp_instruction(), rhs.derived().ofp_instruction());
    }

    auto equivalent_impl(T const& rhs) const noexcept
      -> bool
    {
      return static_cast<T const*>(this)->is_equivalent_instruction(rhs);
    }
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_FIXED_LENGTH_INSTRUCTION_HPP
