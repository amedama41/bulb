#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_NEXT_TABLES_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_NEXT_TABLES_HPP

#include <cstddef>
#include <cstdint>
#include <array>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace table_feature_properties {

    template <class T>
    class basic_prop_next_tables
        : public detail::basic_protocol_type<basic_prop_next_tables<T>>
    {
    public:
        static constexpr protocol::ofp_table_feature_prop_type prop_type
            = T::prop_type;

        using raw_ofp_type = protocol::ofp_table_feature_prop_next_tables;
        using next_table_ids_type = std::vector<std::uint8_t>;
        using iterator = next_table_ids_type::iterator;
        using const_iterator = next_table_ids_type::const_iterator;

        explicit basic_prop_next_tables(next_table_ids_type table_ids)
            : table_feature_prop_next_tables_{
                  prop_type
                , calc_length(table_ids)
              }
            , next_table_ids_(std::move(table_ids))
        {
        }

        explicit basic_prop_next_tables(
                std::initializer_list<std::uint8_t> table_ids)
            : basic_prop_next_tables{next_table_ids_type(table_ids)}
        {
        }

        basic_prop_next_tables(basic_prop_next_tables const&) = default;

        basic_prop_next_tables(basic_prop_next_tables&& other)
            : table_feature_prop_next_tables_(
                    other.table_feature_prop_next_tables_)
            , next_table_ids_(other.extract_next_table_ids())
        {
        }

        auto operator=(basic_prop_next_tables const& other)
            -> basic_prop_next_tables&
        {
            return operator=(basic_prop_next_tables{other});
        }

        auto operator=(basic_prop_next_tables&& other)
            -> basic_prop_next_tables&
        {
            auto tmp = std::move(other);
            std::swap(table_feature_prop_next_tables_
                    , tmp.table_feature_prop_next_tables_);
            next_table_ids_.swap(tmp.next_table_ids_);
            return *this;
        }

        static constexpr auto type() noexcept
            -> protocol::ofp_table_feature_prop_type
        {
            return prop_type;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return table_feature_prop_next_tables_.length;
        }

        auto next_table_ids() const noexcept
            -> next_table_ids_type const&
        {
            return next_table_ids_;
        }

        auto extract_next_table_ids()
            -> next_table_ids_type
        {
            auto next_table_ids = next_table_ids_type{};
            next_table_ids.swap(next_table_ids_);
            table_feature_prop_next_tables_.length = sizeof(raw_ofp_type);
            return next_table_ids;
        }

        auto begin() noexcept
            -> iterator
        {
            return next_table_ids_.begin();
        }

        auto begin() const noexcept
            -> const_iterator
        {
            return next_table_ids_.begin();
        }

        auto end() noexcept
            -> iterator
        {
            return next_table_ids_.end();
        }

        auto end() const noexcept
            -> const_iterator
        {
            return next_table_ids_.end();
        }

    private:
        basic_prop_next_tables(
                  raw_ofp_type const& table_feature_prop_next_tables
                , next_table_ids_type&& next_table_ids)
            : table_feature_prop_next_tables_(table_feature_prop_next_tables)
            , next_table_ids_(std::move(next_table_ids))
        {
        }

        static auto calc_length(next_table_ids_type const& table_ids)
            -> std::uint16_t
        {
            constexpr auto max_length
                = std::numeric_limits<std::uint16_t>::max();
            auto const table_ids_length
                = table_ids.size() * sizeof(next_table_ids_type::value_type);
            if (table_ids_length > max_length - sizeof(raw_ofp_type)) {
                throw std::runtime_error{"too many table_ids"};
            }
            return sizeof(raw_ofp_type) + table_ids_length;
        }

        friend detail::basic_protocol_type<basic_prop_next_tables>;

        friend constexpr auto exclude_padding(basic_prop_next_tables*) noexcept
          -> bool
        {
          return true;
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, table_feature_prop_next_tables_);
            detail::encode_byte_array(
                    container, next_table_ids_.data(), next_table_ids_.size());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> basic_prop_next_tables
        {
            auto const property = detail::decode<raw_ofp_type>(first, last);

            last = std::next(first, property.length - sizeof(raw_ofp_type));
            auto next_table_ids = next_table_ids_type(first, last);
            first = last;

            return basic_prop_next_tables{property, std::move(next_table_ids)};
        }

        auto equal_impl(basic_prop_next_tables const& rhs) const noexcept
            -> bool
        {
            return next_table_ids_ == rhs.next_table_ids_;
        }

        auto equivalent_impl(basic_prop_next_tables const& rhs) const noexcept
            -> bool
        {
            if (next_table_ids_.size() != rhs.next_table_ids_.size()) {
                return false;
            }
            using value_type = next_table_ids_type::value_type;
            constexpr auto array_size
                = std::size_t{std::numeric_limits<value_type>::max() + 1};
            auto bitmap = std::array<bool, array_size>{};
            boost::for_each(
                      next_table_ids_
                    , [&bitmap](value_type id) { bitmap[id] = true; });
            return boost::algorithm::all_of(
                      rhs.next_table_ids_
                    , [&bitmap](value_type id) { return bitmap[id]; });
        }

    private:
        raw_ofp_type table_feature_prop_next_tables_;
        next_table_ids_type next_table_ids_;
    };

    namespace next_tables_detail {

        struct next_tables
        {
            static protocol::ofp_table_feature_prop_type const prop_type
                = protocol::OFPTFPT_NEXT_TABLES;
        };

        struct next_tables_miss
        {
            static protocol::ofp_table_feature_prop_type const prop_type
                = protocol::OFPTFPT_NEXT_TABLES_MISS;
        };

    } // namespace next_tables_detail

    using next_tables = basic_prop_next_tables<next_tables_detail::next_tables>;
    using next_tables_miss
        = basic_prop_next_tables<next_tables_detail::next_tables_miss>;

} // namespace table_feature_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_NEXT_TABLES_HPP
