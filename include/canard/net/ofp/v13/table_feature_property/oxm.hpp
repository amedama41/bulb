#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_OXM_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_OXM_HPP

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/type_traits/is_all_constructible.hpp>
#include <canard/net/ofp/v13/detail/basic_table_feature_property.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/openflow.hpp>
#include <canard/net/ofp/v13/table_feature_property/id/oxm_id.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace table_feature_properties {

  namespace oxm_detail {

    template <class T>
    class basic_prop_oxm
      : public detail::v13::basic_table_feature_property<T>
    {
      using base_t = detail::v13::basic_table_feature_property<T>;

    public:
      using raw_ofp_type = protocol::ofp_table_feature_prop_oxm;
      using oxm_ids_type = ofp::list<oxm_id>;
      using iterator = oxm_ids_type::iterator;
      using const_iterator = oxm_ids_type::const_iterator;

    private:
      template <class... OXMIDs>
      using enable_if_is_all_constructible_t = typename std::enable_if<
        type_traits::is_all_constructible<
          oxm_ids_type::value_type, OXMIDs...
        >::value
      >::type;

    public:
      explicit basic_prop_oxm(oxm_ids_type oxm_ids)
        : table_feature_prop_oxm_{
              base_t::type()
            , oxm_ids.calc_ofp_length(sizeof(raw_ofp_type))
          }
        , oxm_ids_(std::move(oxm_ids))
      {
      }

      template <
          class... OXMIDs
        , class = enable_if_is_all_constructible_t<OXMIDs...>
      >
      explicit basic_prop_oxm(OXMIDs&&... oxm_ids)
        : basic_prop_oxm{oxm_ids_type{std::forward<OXMIDs>(oxm_ids)...}}
      {
      }

      basic_prop_oxm(basic_prop_oxm const&) = default;

      basic_prop_oxm(basic_prop_oxm&& other)
        : table_feature_prop_oxm_(other.table_feature_prop_oxm_)
        , oxm_ids_(other.extract_oxm_ids())
      {
      }

      auto operator=(basic_prop_oxm const& other)
        -> basic_prop_oxm&
      {
        return operator=(basic_prop_oxm{other});
      }

      auto operator=(basic_prop_oxm&& other)
        -> basic_prop_oxm&
      {
        auto tmp = std::move(other);
        std::swap(table_feature_prop_oxm_, tmp.table_feature_prop_oxm_);
        oxm_ids_.swap(tmp.oxm_ids_);
        return *this;
      }

      auto length() const noexcept
        -> std::uint16_t
      {
        return table_feature_prop_oxm_.length;
      }

      auto oxm_ids() const noexcept
        -> oxm_ids_type const&
      {
        return oxm_ids_;
      }

      auto extract_oxm_ids()
        -> oxm_ids_type
      {
        auto oxm_ids = oxm_ids_type{};
        oxm_ids.swap(oxm_ids_);
        table_feature_prop_oxm_.length = sizeof(raw_ofp_type);
        return oxm_ids;
      }

      auto begin() noexcept
        -> iterator
      {
        return oxm_ids_.begin();
      }

      auto begin() const noexcept
        -> const_iterator
      {
        return oxm_ids_.begin();
      }

      auto end() noexcept
        -> iterator
      {
        return oxm_ids_.end();
      }

      auto end() const noexcept
        -> const_iterator
      {
        return oxm_ids_.end();
      }

    protected:
      basic_prop_oxm(
            raw_ofp_type const& table_feature_prop_oxm
          , oxm_ids_type&& oxm_ids)
        : table_feature_prop_oxm_(table_feature_prop_oxm)
        , oxm_ids_(std::move(oxm_ids))
      {
      }

    private:
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
          detail::encode(container, table_feature_prop_oxm_);
          boost::for_each(
              oxm_ids_, [&](oxm_id const& id) { id.encode(container); });
      }

      template <class Iterator>
      static auto decode_impl(Iterator& first, Iterator last)
        -> T
      {
        auto const property = detail::decode<raw_ofp_type>(first, last);

        last = std::next(first, property.length - sizeof(raw_ofp_type));
        auto oxm_ids = oxm_ids_type::decode(first, last);

        return T{property, std::move(oxm_ids)};
      }

      auto equal_impl(T const& rhs) const noexcept
        -> bool
      {
        return oxm_ids_ == rhs.oxm_ids_;
      }

      auto equivalent_impl(T const& rhs) const noexcept
        -> bool
      {
        if (oxm_ids_.size() != rhs.oxm_ids_.size()) {
          return false;
        }
        auto const rhs_ids_end = rhs.oxm_ids_.end();
        for (auto const& lhs_id : oxm_ids_) {
          using const_reference = oxm_ids_type::const_reference;
          if (boost::find_if(
                rhs.oxm_ids_
                , [&lhs_id](const_reference rhs_id)
                  { return equivalent(lhs_id, rhs_id); })
              == rhs_ids_end) {
            return false;
          }
        }
        return true;
      }

    private:
      raw_ofp_type table_feature_prop_oxm_;
      oxm_ids_type oxm_ids_;
    };

  } // namespace oxm_detail


  class match
    : public oxm_detail::basic_prop_oxm<match>
  {
  public:
    static constexpr protocol::ofp_table_feature_prop_type prop_type
      = protocol::OFPTFPT_MATCH;

    using basic_prop_oxm::basic_prop_oxm;

    friend basic_prop_oxm;
  };

  class wildcards
    : public oxm_detail::basic_prop_oxm<wildcards>
  {
  public:
    static constexpr protocol::ofp_table_feature_prop_type prop_type
      = protocol::OFPTFPT_WILDCARDS;

    using basic_prop_oxm::basic_prop_oxm;

    friend basic_prop_oxm;
  };

  class write_setfield
    : public oxm_detail::basic_prop_oxm<write_setfield>
  {
  public:
    static constexpr protocol::ofp_table_feature_prop_type prop_type
      = protocol::OFPTFPT_WRITE_SETFIELD;

    using basic_prop_oxm::basic_prop_oxm;

    friend basic_prop_oxm;
  };

  class write_setfield_miss
    : public oxm_detail::basic_prop_oxm<write_setfield_miss>
  {
  public:
    static constexpr protocol::ofp_table_feature_prop_type prop_type
      = protocol::OFPTFPT_WRITE_SETFIELD_MISS;

    using basic_prop_oxm::basic_prop_oxm;

    friend basic_prop_oxm;
  };


  class apply_setfield
    : public oxm_detail::basic_prop_oxm<apply_setfield>
  {
  public:
    static constexpr protocol::ofp_table_feature_prop_type prop_type
      = protocol::OFPTFPT_APPLY_SETFIELD;

    using basic_prop_oxm::basic_prop_oxm;

    friend basic_prop_oxm;
  };


  class apply_setfield_miss
    : public oxm_detail::basic_prop_oxm<apply_setfield_miss>
  {
  public:
    static constexpr protocol::ofp_table_feature_prop_type prop_type
      = protocol::OFPTFPT_APPLY_SETFIELD_MISS;

    using basic_prop_oxm::basic_prop_oxm;

    friend basic_prop_oxm;
  };

} // namespace table_feature_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_OXM_HPP
