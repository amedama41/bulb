#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_PUSH_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_PUSH_HPP

#include <cstdint>
#include <canard/net/ofp/v13/detail/basic_fixed_length_action.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  class basic_action_push
    : public basic_fixed_length_action<T>
  {
    using base_t = basic_fixed_length_action<T>;

  public:
    using ofp_type = ofp::v13::protocol::ofp_action_push;

    auto ethertype() const noexcept
      -> std::uint16_t
    {
      return action_push_.ethertype;
    }

  protected:
    explicit basic_action_push(std::uint16_t const ethertype) noexcept
      : action_push_{T::action_type, base_t::length(), ethertype, { 0, 0 }}
    {
    }

    explicit basic_action_push(ofp_type const& action_push) noexcept
      : action_push_(action_push)
    {
    }

  private:
    friend base_t;

    auto ofp_action() const noexcept
      -> ofp_type const&
    {
      return action_push_;
    }

    auto is_equivalent_action(T const& rhs) const noexcept
      -> bool
    {
      return ethertype() == rhs.ethertype();
    }

  private:
    ofp_type action_push_;
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_PUSH_HPP
