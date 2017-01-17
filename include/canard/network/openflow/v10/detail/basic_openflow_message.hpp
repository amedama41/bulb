#ifndef CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP
#define CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP

#include <cstdint>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    namespace v10_detail {

        template <class T>
        class basic_openflow_message
            : public detail::basic_protocol_type<T>
        {
        private:
            auto header() const
                -> protocol::ofp_header
            {
                return static_cast<T const*>(this)->header();
            }

        public:
            static constexpr auto version() noexcept
                -> std::uint8_t
            {
                return protocol::OFP_VERSION;
            }

            static constexpr auto type() noexcept
                -> protocol::ofp_type
            {
                return T::message_type;
            }

            auto length() const noexcept
                -> std::uint16_t
            {
                return header().length;
            }

            auto xid() const noexcept
                -> std::uint32_t
            {
                return header().xid;
            }
        };

    } // namespace v10_detail

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP
