#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_ACTIONS_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_ACTIONS_HPP

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
#include <canard/net/ofp/v13/table_feature_property/id/action_id.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace table_feature_properties {

  namespace actions_detail {

    template <class T>
    class basic_prop_actions
      : public detail::v13::basic_table_feature_property<T>
    {
      using base_t = detail::v13::basic_table_feature_property<T>;

    public:
      using raw_ofp_type = protocol::ofp_table_feature_prop_actions;
      using action_ids_type = ofp::list<action_id>;
      using iterator = action_ids_type::iterator;
      using const_iterator = action_ids_type::const_iterator;

    private:
      template <class... ActionIDs>
      using enable_if_is_all_constructible_t = typename std::enable_if<
        type_traits::is_all_constructible<
          action_ids_type::value_type, ActionIDs...
        >::value
      >::type;

    public:
      explicit basic_prop_actions(action_ids_type action_ids)
        : table_feature_prop_actions_{
              base_t::type()
            , action_ids.calc_ofp_length(sizeof(raw_ofp_type))
          }
        , action_ids_(std::move(action_ids))
      {
      }

      template <
          class... ActionIDs
        , class = enable_if_is_all_constructible_t<ActionIDs...>
      >
      explicit basic_prop_actions(ActionIDs&&... action_ids)
        : basic_prop_actions{
            action_ids_type{std::forward<ActionIDs>(action_ids)...}
          }
      {
      }

      basic_prop_actions(basic_prop_actions const&) = default;

      basic_prop_actions(basic_prop_actions&& other)
        : table_feature_prop_actions_(other.table_feature_prop_actions_)
        , action_ids_(other.extract_action_ids())
      {
      }

      auto operator=(basic_prop_actions const& other)
        -> basic_prop_actions&
      {
        return operator=(basic_prop_actions{other});
      }

      auto operator=(basic_prop_actions&& other)
        -> basic_prop_actions&
      {
        auto tmp = std::move(other);
        std::swap(table_feature_prop_actions_, tmp.table_feature_prop_actions_);
        action_ids_.swap(tmp.action_ids_);
        return *this;
      }

      auto length() const noexcept
        -> std::uint16_t
      {
        return table_feature_prop_actions_.length;
      }

      auto action_ids() const noexcept
        -> action_ids_type const&
      {
        return action_ids_;
      }

      auto extract_action_ids()
        -> action_ids_type
      {
        auto action_ids = action_ids_type{};
        action_ids.swap(action_ids_);
        table_feature_prop_actions_.length = sizeof(raw_ofp_type);
        return action_ids;
      }

      auto begin() noexcept
        -> iterator
      {
        return action_ids_.begin();
      }

      auto begin() const noexcept
        -> const_iterator
      {
        return action_ids_.begin();
      }

      auto end() noexcept
        -> iterator
      {
        return action_ids_.end();
      }

      auto end() const noexcept
        -> const_iterator
      {
        return action_ids_.end();
      }

    protected:
      basic_prop_actions(
            raw_ofp_type const& table_feature_prop_actions
          , action_ids_type&& action_ids)
        : table_feature_prop_actions_(table_feature_prop_actions)
        , action_ids_(std::move(action_ids))
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
        detail::encode(container, table_feature_prop_actions_);
        boost::for_each(
            action_ids_, [&](action_id const& id) { id.encode(container); });
      }

      template <class Iterator>
      static auto decode_impl(Iterator& first, Iterator last)
        -> T
      {
        auto const property = detail::decode<raw_ofp_type>(first, last);

        last = std::next(first, property.length - sizeof(raw_ofp_type));
        auto action_ids = action_ids_type::decode(first, last);

        return T{property, std::move(action_ids)};
      }

      auto equal_impl(T const& rhs) const noexcept
        -> bool
      {
        return action_ids_ == rhs.action_ids_;
      }

      auto equivalent_impl(T const& rhs) const noexcept
        -> bool
      {
        if (action_ids_.size() != rhs.action_ids_.size()) {
          return false;
        }
        auto const rhs_ids_end = rhs.action_ids_.end();
        for (auto const& lhs_id : action_ids_) {
          using const_reference = action_ids_type::const_reference;
          if (boost::find_if(
                  rhs.action_ids_
                , [&lhs_id](const_reference rhs_id)
                  { return equivalent(lhs_id, rhs_id); })
              == rhs_ids_end) {
            return false;
          }
        }
        return true;
      }

    private:
      raw_ofp_type table_feature_prop_actions_;
      action_ids_type action_ids_;
    };

  } // namespace actions_detail


  class write_actions
    : public actions_detail::basic_prop_actions<write_actions>
  {
  public:
    static protocol::ofp_table_feature_prop_type const prop_type
      = protocol::OFPTFPT_WRITE_ACTIONS;

    using basic_prop_actions::basic_prop_actions;

    friend basic_prop_actions;
  };

  class write_actions_miss
    : public actions_detail::basic_prop_actions<write_actions_miss>
  {
  public:
    static protocol::ofp_table_feature_prop_type const prop_type
      = protocol::OFPTFPT_WRITE_ACTIONS_MISS;

    using basic_prop_actions::basic_prop_actions;

    friend basic_prop_actions;
  };

  class apply_actions
    : public actions_detail::basic_prop_actions<apply_actions>
  {
  public:
    static protocol::ofp_table_feature_prop_type const prop_type
      = protocol::OFPTFPT_APPLY_ACTIONS;

    using basic_prop_actions::basic_prop_actions;

    friend basic_prop_actions;
  };

  class apply_actions_miss
    : public actions_detail::basic_prop_actions<apply_actions_miss>
  {
  public:
    static protocol::ofp_table_feature_prop_type const prop_type
      = protocol::OFPTFPT_APPLY_ACTIONS_MISS;

    using basic_prop_actions::basic_prop_actions;

    friend basic_prop_actions;
  };

} // namespace table_feature_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_ACTIONS_HPP
