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
#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/container/vector.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/v13/detail/basic_table_feature_property.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace table_feature_properties {

  namespace next_tables_detail {

    template <class T>
    class basic_prop_next_tables
      : public detail::v13::basic_table_feature_property<T>
    {
      using base_t = detail::v13::basic_table_feature_property<T>;

    public:
      using ofp_type = protocol::ofp_table_feature_prop_next_tables;
      using next_table_ids_type = boost::container::vector<std::uint8_t>;

      basic_prop_next_tables()
        : table_feature_prop_next_tables_{base_t::type(), sizeof(ofp_type)}
        , next_table_ids_{}
      {
      }

      explicit basic_prop_next_tables(next_table_ids_type table_ids)
        : table_feature_prop_next_tables_{
              base_t::type()
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
        : table_feature_prop_next_tables_(other.table_feature_prop_next_tables_)
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
        std::swap(
              table_feature_prop_next_tables_
            , tmp.table_feature_prop_next_tables_);
        next_table_ids_.swap(tmp.next_table_ids_);
        return *this;
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
        table_feature_prop_next_tables_.length = sizeof(ofp_type);
        return next_table_ids;
      }

    protected:
      basic_prop_next_tables(
            ofp_type const& table_feature_prop_next_tables
          , next_table_ids_type&& next_table_ids)
        : table_feature_prop_next_tables_(table_feature_prop_next_tables)
        , next_table_ids_(std::move(next_table_ids))
      {
      }

    private:
      static auto calc_length(next_table_ids_type const& table_ids)
        -> std::uint16_t
      {
        constexpr auto max_length = std::numeric_limits<std::uint16_t>::max();
        auto const table_ids_length
          = table_ids.size() * sizeof(next_table_ids_type::value_type);
        if (table_ids_length > max_length - sizeof(ofp_type)) {
          throw std::runtime_error{"too many table_ids"};
        }
        return sizeof(ofp_type) + table_ids_length;
      }

      friend typename base_t::basic_protocol_type;

      friend constexpr auto exclude_padding(
          detail::basic_protocol_type_tag<T>) noexcept
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
        -> T
      {
        auto const property = detail::decode<ofp_type>(first, last);

        last = std::next(first, property.length - sizeof(ofp_type));
        auto next_table_ids = next_table_ids_type(first, last);
        first = last;

        return T{property, std::move(next_table_ids)};
      }

      auto equal_impl(T const& rhs) const noexcept
        -> bool
      {
        return next_table_ids_ == rhs.next_table_ids_;
      }

      auto equivalent_impl(T const& rhs) const noexcept
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
      ofp_type table_feature_prop_next_tables_;
      next_table_ids_type next_table_ids_;
    };

  } // namespace next_tables_detail


  class next_tables
    : public next_tables_detail::basic_prop_next_tables<next_tables>
  {
  public:
    static protocol::ofp_table_feature_prop_type const prop_type
      = protocol::OFPTFPT_NEXT_TABLES;

    using basic_prop_next_tables::basic_prop_next_tables;

    friend basic_prop_next_tables;
  };

  class next_tables_miss
    : public next_tables_detail::basic_prop_next_tables<next_tables_miss>
  {
  public:
    static protocol::ofp_table_feature_prop_type const prop_type
      = protocol::OFPTFPT_NEXT_TABLES_MISS;

    using basic_prop_next_tables::basic_prop_next_tables;

    friend basic_prop_next_tables;
  };

} // namespace table_feature_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_NEXT_TABLES_HPP
