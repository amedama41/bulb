#ifndef CANARD_NET_OFP_V10_MATCH_SET_HPP
#define CANARD_NET_OFP_V10_MATCH_SET_HPP

#include <tuple>
#include <type_traits>
#include <utility>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/query/all.hpp>
#include <boost/operators.hpp>
#include <boost/optional/optional.hpp>
#include <canard/network/openflow/detail/is_related.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v10/common/match_fields.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    class match_set
        : boost::equality_comparable<match_set>
    {
        using raw_ofp_type = v10_detail::ofp_match;

    public:
        template <
              class... MatchFields
            , typename std::enable_if<
                       !detail::is_related<match_set, MatchFields...>::value
                    && !detail::is_related<raw_ofp_type, MatchFields...>::value
              >::type* = nullptr
        >
        explicit match_set(MatchFields&&... fields) noexcept
            : match_{protocol::OFPFW_ALL, 0}
        {
            set_impl(std::forward<MatchFields>(fields)...);
        }

        explicit match_set(raw_ofp_type const& match) noexcept
            : match_(match)
        {
        }

        auto empty() const noexcept
            -> bool
        {
            constexpr std::uint32_t nw_addr_mask
                = protocol::OFPFW_NW_SRC_ALL | protocol::OFPFW_NW_DST_ALL;
            constexpr std::uint32_t all_mask_wo_nw_addr
                = protocol::OFPFW_ALL
                & ~(protocol::OFPFW_NW_SRC_MASK | protocol::OFPFW_NW_DST_MASK);

            return ((wildcards() & nw_addr_mask) == nw_addr_mask)
                && ((wildcards() & all_mask_wo_nw_addr) == all_mask_wo_nw_addr);
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
        }

        auto wildcards() const noexcept
            -> std::uint32_t
        {
            return match_.wildcards;
        }

        template <class MatchField>
        auto get() const noexcept
            -> boost::optional<MatchField>
        {
            if (MatchField::is_wildcard(ofp_match())) {
                return boost::none;
            }
            return MatchField::create_from_match(ofp_match());
        }

        template <class MatchField>
        void add(MatchField&& field) noexcept
        {
            set(std::forward<MatchField>(field));
        }

        template <class MatchField>
        void set(MatchField&& field) noexcept
        {
            std::forward<MatchField>(field).set_value(match_);
        }

        template <class MatchField>
        void erase() noexcept
        {
            MatchField::erase_from_match(match_);
        }

        auto ofp_match() const noexcept
            -> raw_ofp_type const&
        {
            return match_;
        }

    private:
        void set_impl() noexcept
        {
        }

        template <class MatchField, class... MatchFields>
        void set_impl(MatchField&& field, MatchFields&&... fields) noexcept
        {
            set(std::forward<MatchField>(field));
            set_impl(std::forward<MatchFields>(fields)...);
        }

    private:
        raw_ofp_type match_;
    };

    inline auto operator==(match_set const& lhs, match_set const& rhs) noexcept
        -> bool
    {
        return detail::memcmp(lhs.ofp_match(), rhs.ofp_match());
    }

    namespace match_set_detail {

        struct equivalent_impl
        {
            template <class FieldType>
            auto operator()(FieldType*) const noexcept
                -> bool
            {
                if (auto const lhs_field = lhs->get<FieldType>()) {
                    return equivalent(*lhs_field, *rhs->get<FieldType>());
                }
                return true;
            }

            operator bool() const noexcept
            {
                if ((lhs->wildcards() & protocol::OFPFW_ALL)
                        != (rhs->wildcards() & protocol::OFPFW_ALL)) {
                    return false;
                }

                using match_field_pointer_list = std::tuple<
                      match::in_port*
                    , match::eth_src*, match::eth_dst*
                    , match::vlan_vid*, match::vlan_pcp*
                    , match::eth_type*
                    , match::ip_dscp*, match::ip_proto*
                    , match::ipv4_src*, match::ipv4_dst*
                    , match::tcp_src*, match::tcp_dst*
                >;
                return boost::fusion::all(match_field_pointer_list{}, *this);
            }

            match_set const* lhs;
            match_set const* rhs;
        };

    } // namespace match_set_detail

    inline auto equivalent(match_set const& lhs, match_set const& rhs) noexcept
        -> bool
    {
        return match_set_detail::equivalent_impl{&lhs, &rhs};
    }

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MATCH_SET_HPP
