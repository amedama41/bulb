#ifndef CANARD_NET_OFP_V10_ACTIONS_ENQUEUE_HPP
#define CANARD_NET_OFP_V10_ACTIONS_ENQUEUE_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <canard/net/ofp/v10/detail/basic_fixed_length_action.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace actions {

  class enqueue
    : public detail::v10::basic_fixed_length_action<enqueue>
  {
  public:
    using ofp_type = protocol::ofp_action_enqueue;

    static constexpr protocol::ofp_action_type action_type
      = protocol::OFPAT_ENQUEUE;

    enqueue(std::uint32_t const queue_id, std::uint16_t const port_no) noexcept
      : enqueue_{
          action_type, sizeof(ofp_type), port_no, { 0, 0, 0, 0, 0, 0 }, queue_id
        }
    {
    }

    auto queue_id() const noexcept
      -> std::uint32_t
    {
      return enqueue_.queue_id;
    }

    auto port_no() const noexcept
      -> std::uint16_t
    {
      return enqueue_.port;
    }

  private:
    friend basic_fixed_length_action;

    explicit enqueue(ofp_type const& action_enqueue) noexcept
      : enqueue_(action_enqueue)
    {
    }

    auto ofp_action() const noexcept
      -> ofp_type const&
    {
      return enqueue_;
    }

    void validate_action() const
    {
      if (queue_id() == protocol::OFPQ_ALL) {
        throw std::runtime_error{"invalid queue_id"};
      }
      if (port_no() == 0
          || (port_no() > protocol::OFPP_MAX
            && port_no() != protocol::OFPP_IN_PORT)) {
        throw std::runtime_error{"invalid port_no"};
      }
    }

    auto is_equivalent_action(enqueue const& rhs) const noexcept
      -> bool
    {
      return queue_id() == rhs.queue_id()
          && port_no() == rhs.port_no();
    }

  private:
    ofp_type enqueue_;
  };

} // namespace actions
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTIONS_ENQUEUE_HPP
