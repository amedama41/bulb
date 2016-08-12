#ifndef CANARD_NETWORK_OPENFLOW_V13_IMPL_ACTION_SET_IPP
#define CANARD_NETWORK_OPENFLOW_V13_IMPL_ACTION_SET_IPP

#include <canard/network/protocol/openflow/detail/config.hpp>
#include <canard/network/protocol/openflow/v13/action_set.hpp>

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <array>
#include <iterator>
#include <unordered_set>
#include <utility>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/fill.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/lower_bound.hpp>
#include <canard/network/protocol/openflow/v13/action_list.hpp>
#include <canard/network/protocol/openflow/v13/action_order.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {

  namespace action_set_detail {

    class action_set_info
    {
      struct action_info
      {
        action_list::const_iterator pos;
        std::size_t num_actions;
      };

    public:
      action_set_info(action_list const& actions) noexcept
        : it_end_(actions.end())
      {
        boost::fill(info_map_, action_info{it_end_, 0});
        for (auto it = actions.begin(); it != it_end_; ++it) {
          auto& action_info = info_map_[it->index()];
          if (action_info.num_actions++ == 0) {
            action_info.pos = it;
          }
        }
      }

      auto has_equivalent_action(
          action_list::const_reference action) const noexcept
        -> bool
      {
        auto const& info = info_map_[action.index()];
        if (info.pos == it_end_) {
          return false;
        }
        if (!equivalent(action, *info.pos)) {
          return false;
        }
        return true;
      }

      void set_next_pos(action_list::const_reference action) noexcept
      {
        auto const index = action.index();
        auto& info = info_map_[index];
        if (info.num_actions-- > 0) {
          info.pos = std::find_if(
                std::next(info.pos), it_end_
              , [index](action_list::const_reference action)
                { return action.index() == index; });
        }
        else {
          info.pos = it_end_;
        }
      }

    public:
      std::array<action_info, action_list::value_type::number_of_types>
        info_map_;
      action_list::const_iterator it_end_;
    };

  } // namespace write_actions_detail

  auto action_set::begin() const noexcept
    -> const_iterator
  {
    return actions_.begin();
  }

  auto action_set::end() const noexcept
    -> const_iterator
  {
    return actions_.end();
  }

  auto action_set::cbegin() const noexcept
    -> const_iterator
  {
    return actions_.cbegin();
  }

  auto action_set::cend() const noexcept
    -> const_iterator
  {
    return actions_.cend();
  }

  auto action_set::rbegin() const noexcept
    -> const_reverse_iterator
  {
    return actions_.rbegin();
  }

  auto action_set::rend() const noexcept
    -> const_reverse_iterator
  {
    return actions_.rend();
  }

  auto action_set::crbegin() const noexcept
    -> const_reverse_iterator
  {
    return actions_.crbegin();
  }

  auto action_set::crend() const noexcept
    -> const_reverse_iterator
  {
    return actions_.crend();
  }

  auto action_set::empty() const noexcept
    -> bool
  {
    return actions_.empty();
  }

  auto action_set::size() const noexcept
    -> size_type
  {
    return actions_.size();
  }

  auto action_set::max_size() const noexcept
    -> size_type
  {
    return actions_.max_size();
  }

  auto action_set::at(key_type const act_order) const
    -> const_reference
  {
    auto const it = find(act_order);
    if (it == actions_.end()) {
      throw std::out_of_range{"not found specified action"};
    }
    return *it;
  }

  auto action_set::erase(const_iterator it)
    -> const_iterator
  {
    return actions_.erase(it);
  }

  void action_set::swap(action_set& other) noexcept
  {
    actions_.swap(other.actions_);
  }

  void action_set::clear() noexcept
  {
    return actions_.clear();
  }

  auto action_set::find(key_type const act_order) const
    -> const_iterator
  {
    return boost::find_if(actions_, [=](const_reference e) {
        return get_order(e) == act_order;
    });
  }

  auto action_set::length() const
    -> std::uint16_t
  {
    return actions_.length();
  }

  auto action_set::to_list() const&
    -> action_list const&
  {
    return actions_;
  }

  auto action_set::to_list() &&
    -> action_list
  {
    return actions_;
  }

  auto action_set::is_action_set(action_list const& actions)
    -> bool
  {
    auto action_order_set = std::unordered_set<key_type>{};
    action_order_set.reserve(actions.size());
    for (auto const& action : actions) {
      if (!action_order_set.insert(get_order(action)).second) {
        return false;
      }
    }
    return true;
  }

  auto action_set::equivalent_as_action_set(
      action_list const& lhs, action_list const& rhs) noexcept
    -> bool
  {
    auto rhs_action_set_info
      = action_set_detail::action_set_info{rhs};
    for (auto const& lhs_action : lhs) {
      if (!rhs_action_set_info.has_equivalent_action(lhs_action)) {
        return false;
      }
      rhs_action_set_info.set_next_pos(lhs_action);
    }
    return true;
  }

  auto action_set::non_const_lower_bound(key_type const act_order)
    -> action_list::iterator
  {
    return boost::lower_bound(
          actions_
        , act_order
        , [](const_reference e, key_type const act_order)
          { return get_order(e) < act_order; });
  }

  auto action_set::equal_impl(action_set const& rhs) const
    -> bool
  {
    return to_list() == rhs.to_list();
  }

  auto action_set::equivalent_impl(action_set const& rhs) const noexcept
    -> bool
  {
    return boost::equal(
          *this, rhs
        , [](const_reference lhs_action, const_reference rhs_action)
          { return equivalent(lhs_action, rhs_action); });
  }

} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_V13_IMPL_ACTION_SET_IPP
