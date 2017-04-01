#ifndef CANARD_NET_OFP_V13_ACTION_SET_HPP
#define CANARD_NET_OFP_V13_ACTION_SET_HPP

#include <canard/net/ofp/detail/config.hpp>

#include <cstdint>
#include <canard/net/ofp/detail/set_base.hpp>
#include <canard/net/ofp/v13/action_order.hpp>
#include <canard/net/ofp/v13/any_action.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class action_set
    : public detail::set_base<action_set, std::uint64_t, any_action>
  {
  public:
    using set_base::set_base;

  private:
    friend set_base;

    using set_info = set_base::default_set_info;

    template <class Action>
    static constexpr auto key_impl() noexcept
      -> key_type
    {
      return action_order<Action>::value;
    }

    template <class Action>
    static auto get_key_impl(Action const& action) noexcept
      -> key_type
    {
      return get_order(action);
    }

    template <class T>
    static auto cast_impl(const_reference action)
      -> T const&
    {
      return v13::any_cast<T>(action);
    }
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  extern template class detail::set_base<
    v13::action_set, std::uint64_t, v13::any_action
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#else
# include <canard/net/ofp/v13/impl/action_set.ipp>
#endif

#endif // CANARD_NET_OFP_V13_ACTION_SET_HPP
