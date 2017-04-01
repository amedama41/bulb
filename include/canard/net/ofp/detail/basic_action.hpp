#ifndef CANARD_NET_OFP_DETAIL_BASIC_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_BASIC_ACTION_HPP

#include <cstdint>
#include <type_traits>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  namespace basic_action_detail {

    constexpr auto is_valid_length(
          std::uint16_t const length
        , std::uint16_t const min_length, std::true_type) noexcept
      -> bool
    {
      return length == min_length;
    }

    constexpr auto is_valid_length(
          std::uint16_t const length
        , std::uint16_t const min_length, std::false_type) noexcept
      -> bool
    {
      return length >= min_length;
    }

  } // namespace basic_action_detail


  template <class T, class HeaderType, class ActionType>
  class basic_action
    : public detail::basic_protocol_type<T>
  {
    using base_t = detail::basic_protocol_type<T>;

  protected:
    basic_action() = default;

  public:
    using ofp_header_type = HeaderType;

    static constexpr auto type() noexcept
      -> ActionType
    {
      return T::action_type;
    }

    static auto validate_header(ofp_header_type const& header) noexcept
      -> char const*
    {
      if (header.type != type()) {
        return "invalid action type";
      }
      if (!is_valid_action_length(header)) {
        return "invalid action length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_action_length(
        ofp_header_type const& header) noexcept
      -> bool
    {
      using is_fixed_length
        = std::integral_constant<bool, T::is_fixed_length_action>;
      return basic_action_detail::is_valid_length(
          header.len, sizeof(typename T::raw_ofp_type), is_fixed_length{});
    }
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_BASIC_ACTION_HPP
