#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <boost/operators.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/is_same_value_type.hpp>
#include <canard/network/openflow/validator.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

    template <class T, class OFPAction>
    class basic_action
        : private boost::equality_comparable<T>
    {
    protected:
        using raw_ofp_type = OFPAction;

        basic_action() = default;

    public:
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

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            return detail::encode(container, base_action());
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> T
        {
            return T{detail::decode<raw_ofp_type>(first, last)};
        }

        template <class... Args>
        static auto create(Args&&... args)
            -> T
        {
            return validation::validate(T(std::forward<Args>(args)...));
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

        template <class Validator>
        void validate(Validator validator) const
        {
            static_cast<T const*>(this)->validate_impl(validator);
        }

    private:
        auto base_action() const noexcept
            -> raw_ofp_type const&
        {
            return static_cast<T const*>(this)->ofp_action();
        }
    };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_ACTION_HPP
