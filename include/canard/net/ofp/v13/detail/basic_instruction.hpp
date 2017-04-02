#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP

#include <cstdint>
#include <type_traits>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  namespace basic_instruction_detail {

    inline auto is_valid_length(
          std::uint16_t const length, std::uint16_t const min_length
        , std::true_type) noexcept
      -> bool
    {
      return length == min_length;
    }

    inline auto is_valid_length(
          std::uint16_t const length, std::uint16_t const min_length
        , std::false_type) noexcept
      -> bool
    {
      return length >= min_length;
    }

  } // namespace basic_instruction_detail


  template <class T>
  class basic_instruction
    : public detail::basic_protocol_type<T>
  {
  protected:
    basic_instruction() = default;

  public:
    using ofp_header_type = ofp::v13::protocol::ofp_instruction;

    static constexpr auto type() noexcept
      -> ofp::v13::protocol::ofp_instruction_type
    {
      return T::instruction_type;
    }

    static auto validate_header(ofp_header_type const& instruction) noexcept
      -> char const*
    {
      if (instruction.type != type()) {
        return "invalid instruction type";
      }
      if (!is_valid_instruction_length(instruction)) {
        return "invalid instruction length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_instruction_length(
        ofp_header_type const& header) noexcept
      -> bool
    {
      using is_fixed_length
        = std::integral_constant<bool, T::is_fixed_length_instruction>;
      return basic_instruction_detail::is_valid_length(
          header.len, T::min_length(), is_fixed_length{});
    }
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_HPP
