#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_ANY_OXM_MATCH_FIELD_HPP

#include <cstdint>
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <canard/mpl/adapted/std_tuple.hpp>
#include <canard/network/protocol/openflow/detail/construct.hpp>
#include <canard/network/protocol/openflow/detail/is_related.hpp>
#include <canard/network/protocol/openflow/detail/visitors.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    template <class OXMMatchFieldDecoder>
    class any_oxm_match_field
        : private boost::equality_comparable<
              any_oxm_match_field<OXMMatchFieldDecoder>
          >
    {
        using oxm_match_field_type_list
            = typename OXMMatchFieldDecoder::oxm_match_field_type_list;
        using oxm_match_field_variant = typename boost::make_variant_over<
            oxm_match_field_type_list
        >::type;

    public:
        using type_list = oxm_match_field_type_list;

        template <
              class OXMMatchField
            , class = typename std::enable_if<
                !detail::is_related<any_oxm_match_field, OXMMatchField>::value
              >::type
        >
        any_oxm_match_field(OXMMatchField&& field)
            : variant_(std::forward<OXMMatchField>(field))
        {
        }

        template <
              class OXMMatchField
            , class = typename std::enable_if<
                !detail::is_related<any_oxm_match_field, OXMMatchField>::value
              >::type
        >
        auto operator=(OXMMatchField&& field)
            -> any_oxm_match_field&
        {
            variant_ = std::forward<OXMMatchField>(field);
            return *this;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            auto visitor = detail::length_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto oxm_class() const noexcept
            -> std::uint16_t
        {
            auto visitor = detail::oxm_class_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto oxm_field() const noexcept
            -> std::uint8_t
        {
            auto visitor = detail::oxm_field_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto oxm_type() const noexcept
            -> std::uint32_t
        {
            auto visitor = detail::oxm_type_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto oxm_header() const noexcept
            -> std::uint32_t
        {
            auto visitor = detail::oxm_header_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto oxm_has_mask() const noexcept
            -> bool
        {
            auto visitor = detail::oxm_has_mask_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto oxm_length() const noexcept
            -> std::uint8_t
        {
            auto visitor = detail::oxm_length_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto is_wildcard() const noexcept
            -> bool
        {
            auto visitor = detail::is_wildcard_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto is_exact() const noexcept
            -> bool
        {
            auto visitor = detail::is_exact_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            auto visitor = detail::encoding_visitor<Container>{container};
            return boost::apply_visitor(visitor, variant_);
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> any_oxm_match_field
        {
            return OXMMatchFieldDecoder::
                template decode<any_oxm_match_field>(
                          first, last
                        , detail::construct<any_oxm_match_field>{});
        }

        friend auto operator==(
                  any_oxm_match_field const& lhs
                , any_oxm_match_field const& rhs)
            -> bool
        {
            return lhs.equal_impl(rhs);
        }

        friend auto equivalent(
                  any_oxm_match_field const& lhs
                , any_oxm_match_field const& rhs) noexcept
            -> bool
        {
            return lhs.equivalent_impl(rhs);
        }

        template <class T, class Decoder>
        friend auto any_cast(any_oxm_match_field<Decoder> const&)
            -> T const&;

        template <class T, class Decoder>
        friend auto any_cast(any_oxm_match_field<Decoder> const*)
            -> T const*;

    private:
        auto equal_impl(any_oxm_match_field const& rhs) const
            -> bool
        {
            return variant_ == rhs.variant_;
        }

        auto equivalent_impl(
                any_oxm_match_field const& rhs) const noexcept
            -> bool
        {
            auto visitor = detail::equivalent_visitor{};
            return boost::apply_visitor(visitor, variant_, rhs.variant_);
        }

        oxm_match_field_variant variant_;
    };

    template <class T, class OXMMatchFieldDecoder>
    auto any_cast(any_oxm_match_field<OXMMatchFieldDecoder> const& field)
        -> T const&
    {
        return boost::get<T>(field.variant_);
    }

    template <class T, class OXMMatchFieldDecoder>
    auto any_cast(any_oxm_match_field<OXMMatchFieldDecoder> const* field)
        -> T const*
    {
        return boost::get<T>(&field->variant_);
    }

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_ANY_OXM_MATCH_FIELD_HPP
