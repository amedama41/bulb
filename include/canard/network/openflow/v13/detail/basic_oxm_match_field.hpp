#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_OXM_MATCH_FIELD_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_OXM_MATCH_FIELD_HPP

#include <cstddef>
#include <cstdint>
#include <array>
#include <stdexcept>
#include <type_traits>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/optional/optional.hpp>
#include <boost/range/algorithm_ext/for_each.hpp>
#include <canard/network/openflow/detail/as_byte_range.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

    namespace basic_oxm_match_field_detail {

        template <class Mask, class T>
        auto all_of(Mask const& mask, T t) noexcept
            -> bool
        {
            return boost::algorithm::all_of(
                      detail::as_byte_range(mask)
                    , [=](std::uint8_t const e) { return e == t; });
        }

        template <std::size_t N, class T>
        auto all_of(std::array<std::uint8_t, N> const& mask, T t) noexcept
            -> bool
        {
            return boost::algorithm::all_of(
                    mask, [=](std::uint8_t const e) { return e == t; });
        }

        template <class T>
        void validate_mask(T const& value, T const& mask)
        {
            boost::for_each(
                      detail::as_byte_range(value)
                    , detail::as_byte_range(mask)
                    , [](std::uint8_t const v, std::uint8_t const m) {
                if (v & ~m) {
                    throw std::runtime_error{"invalid wildcard"};
                }
            });
        }

    } // namespace basic_oxm_match_field_detail

    template <class T, class OXMMatchFieldTraits>
    class basic_oxm_match_field
        : public detail::basic_protocol_type<T>
    {
    public:
        using oxm_header_type = std::uint32_t;
        using value_type = typename OXMMatchFieldTraits::value_type;

        static constexpr auto oxm_class() noexcept
            -> ofp::v13::protocol::ofp_oxm_class
        {
            return T::oxm_class_value;
        }

        static constexpr auto oxm_field() noexcept
            -> std::uint8_t
        {
            return T::oxm_field_value;
        }

        static constexpr auto oxm_type() noexcept
            -> std::uint32_t
        {
            return std::uint32_t{oxm_class()} << 7 | oxm_field();
        }

        auto oxm_has_mask() const noexcept
            -> bool
        {
            return static_cast<bool>(oxm_mask());
        }

        auto oxm_length() const noexcept
            -> std::uint8_t
        {
            return oxm_has_mask() ? value_length() * 2 : value_length();
        }

        auto oxm_header() const noexcept
            -> oxm_header_type
        {
            return (oxm_type() << 9)
                | (oxm_header_type{oxm_has_mask()} << 8)
                | oxm_length();
        }

        auto oxm_value() const noexcept
            -> value_type const&
        {
            return value_;
        }

        auto oxm_mask() const noexcept
            -> boost::optional<value_type> const&
        {
            return mask_;
        }

        static constexpr auto type() noexcept
            -> std::uint32_t
        {
            return oxm_type();
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return sizeof(oxm_header_type) + oxm_length();
        }

        auto is_wildcard() const noexcept
            -> bool
        {
            return oxm_has_mask()
                ? basic_oxm_match_field_detail::all_of(raw_mask(), 0)
                : false;
        }

        auto is_exact() const noexcept
            -> bool
        {
            return oxm_has_mask()
                ? basic_oxm_match_field_detail::all_of(raw_mask(), 0xff)
                : true;
        }

        static void validate_oxm_header(oxm_header_type const oxm_header)
        {
            if ((oxm_header >> 16) != oxm_class()) {
                throw std::runtime_error{"invalid oxm class"};
            }
            if (((oxm_header >> 9) & 0x7f) == oxm_field()) {
                throw std::runtime_error{"invalid oxm field"};
            }
            auto const expected_length = (oxm_header & 0x00000100)
                ? value_length() * 2 : value_length();
            if ((oxm_header & 0xff) != expected_length) {
                throw std::runtime_error{"invalid oxm length"};
            }
        }

    protected:
        using oxm_value_type = typename OXMMatchFieldTraits::oxm_value_type;

        explicit basic_oxm_match_field(value_type const& value)
            : value_(value)
            , mask_{boost::none}
        {
        }

        basic_oxm_match_field(value_type const& value, value_type const& mask)
            : value_(value)
            , mask_{mask}
        {
        }

    private:
        using byte_order = std::integral_constant<
            bool, OXMMatchFieldTraits::needs_byte_order_conversion
        >;

        auto raw_value() const noexcept
            -> oxm_value_type
        {
            return OXMMatchFieldTraits::to_oxm_value(oxm_value());
        }

        auto raw_mask() const noexcept
            -> oxm_value_type
        {
            return OXMMatchFieldTraits::to_oxm_value(*oxm_mask());
        }

        friend detail::basic_protocol_type<T>;

        friend constexpr auto get_min_length(T*) noexcept
            -> std::uint16_t
        {
            return sizeof(oxm_header_type) + value_length();
        }

        static constexpr auto value_length() noexcept
            -> std::uint16_t
        {
            return sizeof(oxm_value_type);
        }

        template <class Validator>
        void validate_impl(Validator) const
        {
            T::validate_value(oxm_value());
            if (oxm_has_mask()) {
                basic_oxm_match_field_detail::validate_mask(
                        raw_value(), raw_mask());
            }
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, oxm_header());
            detail::encode(
                    container, raw_value(), value_length(), byte_order{});
            if (oxm_has_mask()) {
                detail::encode(
                        container, raw_mask(), value_length(), byte_order{});
            }
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> T
        {
            auto const oxm_header
                = detail::decode<oxm_header_type>(first, last);
            auto const value = detail::decode<oxm_value_type>(
                      first, last, value_length(), byte_order{});
            if (oxm_header & 0x00000100) {
                auto const mask = detail::decode<oxm_value_type>(
                          first, last, value_length(), byte_order{});
                return T{value, mask};
            }
            else {
                return T{value};
            }
        }

        auto equal_impl(T const& rhs) const noexcept
            -> bool
        {
            return oxm_mask() == rhs.oxm_mask()
                && oxm_value() == rhs.oxm_value();
        }

        auto equivalent_impl(T const& rhs) const noexcept
            -> bool
        {
            if (is_wildcard()) {
                return rhs.is_wildcard();
            }
            if (is_exact()) {
                return rhs.is_exact()
                    && oxm_value() == rhs.oxm_value();
            }
            return oxm_mask() == rhs.oxm_mask()
                && oxm_value() == rhs.oxm_value();
        }

    private:
        value_type value_;
        boost::optional<value_type> mask_;
    };

} // namespace detail
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_OXM_MATCH_FIELD_HPP
