#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP

#include <cstdint>
#include <type_traits>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

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


    template <class T>
    class basic_action
    {
    protected:
        basic_action() = default;

    public:
        using ofp_header_type = ofp::v13::protocol::ofp_action_header;

        static constexpr auto type() noexcept
            -> ofp::v13::protocol::ofp_action_type
        {
            return T::action_type;
        }

        static auto validate_header(ofp_header_type const& header) noexcept
            -> char const*
        {
            if (header.type != type()) {
                return "invalid action type";
            }
            using is_fixed_length
                = std::integral_constant<bool, T::is_fixed_length_action>;
            if (!basic_action_detail::is_valid_length(
                          header.len, sizeof(typename T::raw_ofp_type)
                        , is_fixed_length{})) {
                return "invalid action length";
            }
            return nullptr;
        }
    };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP
