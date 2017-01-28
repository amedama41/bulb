#ifndef CANARD_NET_OFP_V13_ACTIONS_SET_FIELD_HPP
#define CANARD_NET_OFP_V13_ACTIONS_SET_FIELD_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/padding.hpp>
#include <canard/network/openflow/v13/common/oxm_header.hpp>
#include <canard/network/openflow/v13/common/oxm_match_field.hpp>
#include <canard/network/openflow/v13/detail/basic_action.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace actions {

    class basic_set_field
        : public detail::v13::basic_action<basic_set_field>
    {
    protected:
        basic_set_field() = default;

    public:
        using raw_ofp_type = protocol::ofp_action_set_field;

        static constexpr protocol::ofp_action_type action_type
            = protocol::OFPAT_SET_FIELD;

        static constexpr auto type() noexcept
            -> protocol::ofp_action_type
        {
            return action_type;
        }

        static auto extract_oxm_header(raw_ofp_type const& set_field) noexcept
            -> v13::oxm_header
        {
            auto it = set_field.field;
            return v13::oxm_header::decode(it, it + sizeof(set_field.field));
        }

    private:
        friend basic_action;

        static constexpr bool is_fixed_length_action = false;
    };


    template <class OXMMatchField>
    class set_field
        : public basic_set_field
        , public detail::basic_protocol_type<set_field<OXMMatchField>>
    {
        static constexpr std::size_t base_size = offsetof(raw_ofp_type, field);

    public:
        using oxm_match_field = OXMMatchField;
        using value_type = typename oxm_match_field::value_type;

        explicit set_field(value_type const& value)
            : field_{value}
        {
        }

        static constexpr auto oxm_type() noexcept
            -> std::uint32_t
        {
            return oxm_match_field::oxm_type();
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return detail::v13::exact_length(base_size + field_.length());
        }

        auto value() const noexcept
            -> value_type const&
        {
            return field_.oxm_value();
        }

        static auto create_from_match_field(oxm_match_field field)
            -> set_field
        {
            return set_field{std::move(field)};
        }

        static auto validate_set_field(raw_ofp_type const& set_field) noexcept
            -> char const*
        {
            auto const oxm_header = extract_oxm_header(set_field);

            if (auto const error_msg = oxm_match_field::validate_header(
                        oxm_header.to_ofp_type())) {
                return error_msg;
            }
            if (set_field.len != detail::v13::exact_length(
                        sizeof(raw_ofp_type) + oxm_header.oxm_length())) {
                return "invalid set_field length";
            }
            return nullptr;
        }

    private:
        explicit set_field(oxm_match_field&& field)
            : field_(std::move(field))
        {
        }

        friend detail::basic_protocol_type<set_field>;

        friend constexpr auto get_min_length(set_field*) noexcept
            -> std::uint16_t
        {
            return detail::v13::exact_length(
                    base_size + oxm_match_field::min_length());
        }

        template <class Validator>
        void validate_impl(Validator validator) const
        {
            field_.validate(validator);
            if (field_.oxm_has_mask()) {
                throw std::runtime_error{"invalid oxm_hasmask"};
            }
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, std::uint16_t{type()});
            detail::encode(container, length());
            field_.encode(container);
            detail::encode_byte_array(
                      container
                    , detail::padding
                    , detail::v13::padding_length(base_size + field_.length()));
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> set_field
        {
            std::advance(first, sizeof(raw_ofp_type::type));
            auto const length = detail::decode<std::uint16_t>(first, last);
            auto field = oxm_match_field::decode(first, last);
            std::advance(first, length - (base_size + field.length()));
            return set_field{std::move(field)};
        }

        auto equal_impl(set_field const& rhs) const noexcept
            -> bool
        {
            return field_ == rhs.field_;
        }

        auto equivalent_impl(set_field const& rhs) const noexcept
            -> bool
        {
            return field_ == rhs.field_;
        }

    private:
        oxm_match_field field_;
    };

    using set_eth_dst        = set_field<oxm_match_fields::eth_dst>;
    using set_eth_src        = set_field<oxm_match_fields::eth_src>;
    using set_eth_type       = set_field<oxm_match_fields::eth_type>;
    using set_vlan_vid       = set_field<oxm_match_fields::vlan_vid>;
    using set_vlan_pcp       = set_field<oxm_match_fields::vlan_pcp>;
    using set_ip_dscp        = set_field<oxm_match_fields::ip_dscp>;
    using set_ip_ecn         = set_field<oxm_match_fields::ip_ecn>;
    using set_ip_proto       = set_field<oxm_match_fields::ip_proto>;
    using set_ipv4_src       = set_field<oxm_match_fields::ipv4_src>;
    using set_ipv4_dst       = set_field<oxm_match_fields::ipv4_dst>;
    using set_tcp_src        = set_field<oxm_match_fields::tcp_src>;
    using set_tcp_dst        = set_field<oxm_match_fields::tcp_dst>;
    using set_udp_src        = set_field<oxm_match_fields::udp_src>;
    using set_udp_dst        = set_field<oxm_match_fields::udp_dst>;
    using set_sctp_src       = set_field<oxm_match_fields::sctp_src>;
    using set_sctp_dst       = set_field<oxm_match_fields::sctp_dst>;
    using set_icmpv4_type    = set_field<oxm_match_fields::icmpv4_type>;
    using set_icmpv4_code    = set_field<oxm_match_fields::icmpv4_code>;
    using set_arp_op         = set_field<oxm_match_fields::arp_op>;
    using set_arp_spa        = set_field<oxm_match_fields::arp_spa>;
    using set_arp_tpa        = set_field<oxm_match_fields::arp_tpa>;
    using set_arp_sha        = set_field<oxm_match_fields::arp_sha>;
    using set_arp_tha        = set_field<oxm_match_fields::arp_tha>;
    using set_ipv6_src       = set_field<oxm_match_fields::ipv6_src>;
    using set_ipv6_dst       = set_field<oxm_match_fields::ipv6_dst>;
    using set_ipv6_flabel    = set_field<oxm_match_fields::ipv6_flabel>;
    using set_icmpv6_type    = set_field<oxm_match_fields::icmpv6_type>;
    using set_icmpv6_code    = set_field<oxm_match_fields::icmpv6_code>;
    using set_ipv6_nd_target = set_field<oxm_match_fields::ipv6_nd_target>;
    using set_ipv6_nd_sll    = set_field<oxm_match_fields::ipv6_nd_sll>;
    using set_ipv6_nd_tll    = set_field<oxm_match_fields::ipv6_nd_tll>;
    using set_mpls_label     = set_field<oxm_match_fields::mpls_label>;
    using set_mpls_tc        = set_field<oxm_match_fields::mpls_tc>;
    using set_mpls_bos       = set_field<oxm_match_fields::mpls_bos>;
    using set_pbb_isid       = set_field<oxm_match_fields::pbb_isid>;
    using set_tunnel_id      = set_field<oxm_match_fields::tunnel_id>;

} // namespace actions

using default_set_field_list = std::tuple<
      actions::set_eth_dst
    , actions::set_eth_src
    , actions::set_eth_type
    , actions::set_vlan_vid
    , actions::set_vlan_pcp
    , actions::set_ip_dscp
    , actions::set_ip_ecn
    , actions::set_ip_proto
    , actions::set_ipv4_src
    , actions::set_ipv4_dst
    , actions::set_tcp_src
    , actions::set_tcp_dst
    , actions::set_udp_src
    , actions::set_udp_dst
    , actions::set_sctp_src
    , actions::set_sctp_dst
    , actions::set_icmpv4_type
    , actions::set_icmpv4_code
    , actions::set_arp_op
    , actions::set_arp_spa
    , actions::set_arp_tpa
    , actions::set_arp_sha
    , actions::set_arp_tha
    , actions::set_ipv6_src
    , actions::set_ipv6_dst
    , actions::set_ipv6_flabel
    , actions::set_icmpv6_type
    , actions::set_icmpv6_code
    , actions::set_ipv6_nd_target
    , actions::set_ipv6_nd_sll
    , actions::set_ipv6_nd_tll
    , actions::set_mpls_label
    , actions::set_mpls_tc
    , actions::set_mpls_bos
    , actions::set_pbb_isid
    , actions::set_tunnel_id
>;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTIONS_SET_FIELD_HPP
