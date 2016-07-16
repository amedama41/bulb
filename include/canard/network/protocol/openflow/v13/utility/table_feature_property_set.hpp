#ifndef CANARD_NETWORK_OPENFLOW_V13_TABLE_FEATURE_PROPERTY_SET_HPP
#define CANARD_NETWORK_OPENFLOW_V13_TABLE_FEATURE_PROPERTY_SET_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <map>
#include <stdexcept>
#include <utility>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/numeric.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <canard/network/protocol/openflow/detail/add_helper.hpp>
#include <canard/network/protocol/openflow/detail/visitors.hpp>
#include <canard/network/protocol/openflow/v13/detail/visitors.hpp>
#include <canard/network/protocol/openflow/v13/table_feature_property/table_feature_property.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {

    class table_feature_property_set
    {
        using property_map
            = std::map<std::uint16_t, table_feature_properties::variant>;

    public:
        using value_type = property_map::mapped_type;
        using reference = value_type const&;
        using const_reference = value_type const&;
        using iterator = boost::range_iterator<
            boost::select_second_const_range<property_map> const
        >::type;
        using const_iterator = boost::range_iterator<
            boost::select_second_const_range<property_map> const
        >::type;

        template <class... Properties>
        table_feature_property_set(Properties&&... properties)
            : properties_{}
        {
            add_impl(std::forward<Properties>(properties)...);
        }

        void swap(table_feature_property_set& other)
        {
            properties_.swap(other.properties_);
        }

        template <class Property>
        void add(Property&& property)
        {
            auto const type = property.type();
            auto const it = properties_.lower_bound(type);
            if (it != properties_.end() && !properties_.key_comp()(type, it->first)) {
                it->second = std::forward<Property>(property);
            }
            else {
                properties_.emplace_hint(it, type, std::forward<Property>(property));
            }
        }

        auto length() const
            -> std::uint16_t
        {
            using boost::adaptors::transformed;
            return boost::accumulate(
                      *this | transformed([](const_reference prop) {
                          auto const visitor
                              = v13_detail::calculating_exact_length_visitor{};
                          return boost::apply_visitor(visitor, prop);
                      })
                    , std::uint16_t{0});
        }

        auto size() const noexcept
            -> std::size_t
        {
            return properties_.size();
        }

        auto begin() const noexcept
            -> const_iterator
        {
            return boost::begin(properties_ | boost::adaptors::map_values);
        }

        auto end() const noexcept
            -> const_iterator
        {
            return boost::end(properties_ | boost::adaptors::map_values);
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            boost::for_each(*this, [&](const_reference prop) {
                auto const visitor = detail::encoding_visitor<Container>{container};
                boost::apply_visitor(visitor, prop);
            });
            return container;
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> table_feature_property_set
        {
            auto properties = table_feature_property_set{};
            while (std::distance(first, last)
                    >= sizeof(v13_detail::ofp_table_feature_prop_header)) {
                table_feature_properties::decode<void>(
                          first, last
                        , detail::add_helper<table_feature_property_set>{properties});
            }
            if (first != last) {
                throw std::runtime_error{"invalid properties length"};
            }
            return properties;
        }

    private:
        void add_impl()
        {
        }

        template <class Property, class... Properties>
        void add_impl(Property&& prop, Properties&&... properties)
        {
            add(std::forward<Property>(prop));
            add_impl(std::forward<Properties>(properties)...);
        }

    private:
        property_map properties_;
    };

} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_V13_TABLE_FEATURE_PROPERTY_SET_HPP