#ifndef CANARD_NET_OFP_DETAIL_ANY_QUEUE_PROPERTY_HPP
#define CANARD_NET_OFP_DETAIL_ANY_QUEUE_PROPERTY_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/size.hpp>
#include <boost/operators.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <canard/network/openflow/detail/is_related.hpp>
#include <canard/network/openflow/detail/min_base_size_element.hpp>
#include <canard/network/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

    template <class QueuePropertyList, class QueuePropertyDecoder>
    class any_queue_property
        : private boost::equality_comparable<
            any_queue_property<QueuePropertyList, QueuePropertyDecoder>
          >
    {
        using queue_property_variant
            = typename boost::make_variant_over<QueuePropertyList>::type;

        template <class QueueProperty>
        using containable_if_t = typename std::enable_if<
            boost::mpl::contains<QueuePropertyList, QueueProperty>::value
        >::type;

    public:
        static constexpr std::size_t number_of_types
            = boost::mpl::size<QueuePropertyList>::type::value;
        static constexpr std::size_t min_base_size
            = detail::min_base_size_element<QueuePropertyList>::value;

        template <
              class QueueProperty
            , typename std::enable_if<
                  !detail::is_related<any_queue_property, QueueProperty>::value
              >::type* = nullptr
        >
        any_queue_property(QueueProperty&& property)
            : variant_(std::forward<QueueProperty>(property))
        {
        }

        template <
              class QueueProperty
            , typename std::enable_if<
                  !detail::is_related<any_queue_property, QueueProperty>::value
              >::type* = nullptr
        >
        auto operator=(QueueProperty&& property)
            -> any_queue_property&
        {
            variant_ = std::forward<QueueProperty>(property);
            return *this;
        }

        auto property() const noexcept
            -> std::uint16_t
        {
            auto visitor = detail::property_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            auto visitor = detail::length_visitor{};
            return boost::apply_visitor(visitor, variant_);
        }

        auto index() const noexcept
            -> std::size_t
        {
            return variant_.which();
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
            -> any_queue_property
        {
            return QueuePropertyDecoder::template decode<any_queue_property>(
                    first, last, to_any_queue_property{});
        }

        friend auto operator==(
                  any_queue_property const& lhs
                , any_queue_property const& rhs)
            -> bool
        {
            return lhs.variant_ == rhs.variant_;
        }

        template <class QueueProperty, class = containable_if_t<QueueProperty>>
        friend auto operator==(
                  any_queue_property const& lhs
                , QueueProperty const& rhs) noexcept
            -> bool
        {
            if (auto const property
                    = boost::get<QueueProperty>(std::addressof(lhs.variant_))) {
                return *property == rhs;
            }
            return false;
        }

        template <class QueueProperty, class = containable_if_t<QueueProperty>>
        friend auto operator==(
                  QueueProperty const& lhs
                , any_queue_property const& rhs) noexcept
            -> bool
        {
            if (auto const property
                    = boost::get<QueueProperty>(std::addressof(rhs.variant_))) {
                return lhs == *property;
            }
            return false;
        }

        template <class QueueProperty, class = containable_if_t<QueueProperty>>
        friend auto operator!=(
                  any_queue_property const& lhs
                , QueueProperty const& rhs) noexcept
            -> bool
        {
            return !(lhs == rhs);
        }

        template <class QueueProperty, class = containable_if_t<QueueProperty>>
        friend auto operator!=(
                  QueueProperty const& lhs
                , any_queue_property const& rhs) noexcept
            -> bool
        {
            return !(lhs == rhs);
        }

        friend auto equivalent(
                  any_queue_property const& lhs
                , any_queue_property const& rhs) noexcept
            -> bool
        {
            auto visitor = detail::equivalent_visitor{};
            return boost::apply_visitor(visitor, lhs.variant_, rhs.variant_);
        }

        template <class QueueProperty, class = containable_if_t<QueueProperty>>
        friend auto equivalent(
                  any_queue_property const& lhs
                , QueueProperty const& rhs) noexcept
            -> bool
        {
            if (auto const prop
                    = boost::get<QueueProperty>(std::addressof(lhs.variant_))) {
                return equivalent(*prop, rhs);
            }
            return false;
        }

        template <class QueueProperty, class = containable_if_t<QueueProperty>>
        friend auto equivalent(
                  QueueProperty const& lhs
                , any_queue_property const& rhs) noexcept
            -> bool
        {
            if (auto const prop
                    = boost::get<QueueProperty>(std::addressof(rhs.variant_))) {
                return equivalent(lhs, *prop);
            }
            return false;
        }

        template <class T, class PropertyList, class Decorder>
        friend auto any_cast(any_queue_property<PropertyList, Decorder> const&)
            -> T const&;

        template <class T, class PropertyList, class Decorder>
        friend auto any_cast(any_queue_property<PropertyList, Decorder> const*)
            -> T const*;

    private:
        struct to_any_queue_property
        {
            template <class QueueProperty>
            auto operator()(QueueProperty&& property) const
                -> any_queue_property
            {
                return any_queue_property{
                    std::forward<QueueProperty>(property)
                };
            }
        };

    private:
        queue_property_variant variant_;
    };

    template <class T, class QueuePropertyList, class QueuePropertyDecoder>
    auto any_cast(
            any_queue_property<QueuePropertyList, QueuePropertyDecoder> const& property)
        -> T const&
    {
        return boost::get<T>(property.variant_);
    }

    template <class T, class QueuePropertyList, class QueuePropertyDecoder>
    auto any_cast(
            any_queue_property<QueuePropertyList, QueuePropertyDecoder> const* const property)
        -> T const*
    {
        return boost::get<T>(std::addressof(property->variant_));
    }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_ANY_QUEUE_PROPERTY_HPP
