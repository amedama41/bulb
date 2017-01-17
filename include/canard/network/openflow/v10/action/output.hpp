#ifndef CANARD_NET_OFP_V10_ACTIONS_OUTPUT_HPP
#define CANARD_NET_OFP_V10_ACTIONS_OUTPUT_HPP

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/v10/detail/basic_action.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace actions {

    class output
        : public actions_detail::basic_action<
                output, protocol::ofp_action_output
          >
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_OUTPUT;

        explicit output(
                  std::uint16_t const port
                , std::uint16_t const max_len
                    = std::numeric_limits<std::uint16_t>::max()) noexcept
            : action_output_{
                  action_type
                , sizeof(raw_ofp_type)
                , port
                , max_len
              }
        {
        }

        auto port_no() const noexcept
            -> std::uint16_t
        {
            return action_output_.port;
        }

        auto max_length() const noexcept
            -> std::uint16_t
        {
            return action_output_.max_len;
        }

        static auto to_controller(
                std::uint16_t const max_len
                    = std::numeric_limits<std::uint16_t>::max()) noexcept
            -> output
        {
            return output{protocol::OFPP_CONTROLLER, max_len};
        }

    private:
        friend basic_action;
        friend basic_protocol_type;

        explicit output(raw_ofp_type const output) noexcept
            : action_output_(output)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return action_output_;
        }

        template <class Validator>
        void validate_impl(Validator) const
        {
            if (port_no() == 0 || port_no() == protocol::OFPP_NONE) {
                throw std::runtime_error{"invalid port_no"};
            }
        }

        auto equivalent_impl(output const& rhs) const noexcept
            -> bool
        {
            if (port_no() == protocol::OFPP_CONTROLLER) {
                return rhs.port_no() == protocol::OFPP_CONTROLLER
                    && max_length() == rhs.max_length();
            }
            return port_no() == rhs.port_no();
        }

    private:
        raw_ofp_type action_output_;
    };

} // namespace actions
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTIONS_OUTPUT_HPP
