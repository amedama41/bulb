#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

    template <class T, class OFPAction>
    class basic_action
        : public detail::basic_protocol_type<T>
    {
    protected:
        basic_action() = default;

    public:
        using raw_ofp_type = OFPAction;

        static constexpr auto type() noexcept
            -> ofp::v13::protocol::ofp_action_type
        {
            return T::action_type;
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
        }

        static void validate_action_header(
                ofp::v13::v13_detail::ofp_action_header const& header)
        {
            if (header.type != T::action_type) {
                throw std::runtime_error{"invalid action type"};
            }
            if (header.len != sizeof(raw_ofp_type)) {
                throw std::runtime_error{"invalid action length"};
            }
        }

    private:
        auto base_action() const noexcept
            -> raw_ofp_type const&
        {
            return static_cast<T const*>(this)->ofp_action();
        }

        friend detail::basic_protocol_type<T>;

        template <class Validator>
        void validate_impl(Validator const&) const
        {
            return static_cast<T const*>(this)->validate_action();
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

        auto equivalent_impl(T const& rhs) const noexcept
            -> bool
        {
            return static_cast<T const*>(this)->is_equivalent_action(rhs);
        }
    };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP
