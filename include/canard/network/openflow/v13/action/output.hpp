#ifndef CANARD_NET_OFP_V13_ACTIONS_OUTPUT_HPP
#define CANARD_NET_OFP_V13_ACTIONS_OUTPUT_HPP

#include <cstdint>
#include <stdexcept>
#include <canard/network/openflow/v13/detail/basic_action.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class output
        : public detail::v13::basic_action<
            output, protocol::ofp_action_output
          >
    {
    public:
        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_OUTPUT;

        explicit output(
                  std::uint32_t const port_no
                , std::uint16_t const max_length
                    = protocol::OFPCML_NO_BUFFER) noexcept
            : action_output_{
                  action_type
                , length()
                , port_no
                , max_length
                , { 0, 0, 0, 0, 0, 0 }
              }
        {
        }

        auto port_no() const noexcept
            -> std::uint32_t
        {
            return action_output_.port;
        }

        auto max_length() const noexcept
            -> std::uint16_t
        {
            return action_output_.max_len;
        }

        static auto to_controller(
                std::uint16_t const max_length
                    = protocol::OFPCML_NO_BUFFER) noexcept
            -> output
        {
            return output{protocol::OFPP_CONTROLLER, max_length};
        }

    private:
        friend basic_action;

        explicit output(raw_ofp_type const& action_output) noexcept
            : action_output_(action_output)
        {
        }

        auto ofp_action() const noexcept
            -> raw_ofp_type const&
        {
            return action_output_;
        }

        void validate_action() const
        {
            if (port_no() == 0 || port_no() == protocol::OFPP_ANY) {
                throw std::runtime_error{"invalid port_no"};
            }
            if (max_length() > protocol::OFPCML_MAX
                    && max_length() != protocol::OFPCML_NO_BUFFER) {
                throw std::runtime_error{"invalid max_length"};
            }
        }

        auto is_equivalent_action(output const& rhs) const noexcept
            -> bool
        {
            return port_no() == protocol::OFPP_CONTROLLER
                ? (rhs.port_no() == protocol::OFPP_CONTROLLER
                        && max_length() == rhs.max_length())
                : port_no() == rhs.port_no();
        }

    private:
        raw_ofp_type action_output_;
    };

} // namespace actions
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_OUTPUT_HPP
