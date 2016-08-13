#ifndef CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP
#define CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP

#include <cstdint>
#include <vector>
#include <canard/network/protocol/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    namespace v10_detail {

        template <class T>
        class basic_openflow_message
        {
        private:
            auto header() const
                -> v10_detail::ofp_header
            {
                return static_cast<T const*>(this)->header();
            }

        public:
            auto version() const
                -> std::uint8_t
            {
                return header().version;
            }

            auto type() const
                -> protocol::ofp_type
            {
                return protocol::ofp_type(header().type);
            }

            auto length() const
                -> std::uint16_t
            {
                return header().length;
            }

            auto xid() const
                -> std::uint32_t
            {
                return header().xid;
            }

            auto encode() const
                -> std::vector<unsigned char>
            {
                auto buf = std::vector<unsigned char>{};
                buf.reserve(length());
                return static_cast<T const*>(this)->encode(buf);
            }
        };

    } // namespace v10_detail

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_BASIC_OPENFLOW_MESSAGE_HPP
