#ifndef CANARD_NET_OFP_V10_ACTIONS_BASIC_ACTION_HPP
#define CANARD_NET_OFP_V10_ACTIONS_BASIC_ACTION_HPP

#include <cstdint>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace actions_detail {

    template <class T, class OFPAction>
    class basic_action
        : public detail::basic_protocol_type<T>
    {
    protected:
        basic_action() = default;

    public:
        using raw_ofp_type = OFPAction;

        static constexpr auto type() noexcept
            -> protocol::ofp_action_type
        {
            return T::action_type;
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
        }

        static auto validate_header(
                protocol::ofp_action_header const& header) noexcept
            -> char const*
        {
            if (header.type != T::action_type) {
                return "invalid action type";
            }
            if (header.len != sizeof(raw_ofp_type)) {
                return "invalid action length";
            }
            return nullptr;
        }

    private:
        friend detail::basic_protocol_type<T>;

        auto base_action() const noexcept
            -> raw_ofp_type const&
        {
            return static_cast<T const*>(this)->ofp_action();
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, base_action());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> T
        {
            return T{detail::decode<raw_ofp_type>(first, last)};
        }

        auto equal_impl(T const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(base_action(), rhs.base_action());
        }
    };

} // namespace actions_detail
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTIONS_BASIC_ACTION_HPP
