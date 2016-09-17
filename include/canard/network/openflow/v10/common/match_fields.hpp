#ifndef CANARD_NET_OFP_V10_MATCH_FIELDS_HPP
#define CANARD_NET_OFP_V10_MATCH_FIELDS_HPP

#include <cstdint>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>
#include <boost/mpl/int.hpp>
#include <boost/operators.hpp>
#include <canard/mac_address.hpp>
#include <canard/network/openflow/validator.hpp>
#include <canard/network/openflow/v10/detail/fusion_adaptor.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    namespace match_detail {

        template <std::uint32_t Field>
        using field_type = std::integral_constant<std::uint32_t, Field>;

        using match_field_index_table = boost::fusion::map<
          //  ofp_match field type                                          ofp_match member index
          //+-------------------------------------------------------------+------------------------+
              boost::fusion::pair<field_type<protocol::OFPFW_IN_PORT>     , boost::mpl::int_<1>>
            , boost::fusion::pair<field_type<protocol::OFPFW_DL_SRC>      , boost::mpl::int_<2>>
            , boost::fusion::pair<field_type<protocol::OFPFW_DL_DST>      , boost::mpl::int_<8>>
            , boost::fusion::pair<field_type<protocol::OFPFW_DL_VLAN>     , boost::mpl::int_<14>>
            , boost::fusion::pair<field_type<protocol::OFPFW_DL_VLAN_PCP> , boost::mpl::int_<15>>
            , boost::fusion::pair<field_type<protocol::OFPFW_DL_TYPE>     , boost::mpl::int_<17>>
            , boost::fusion::pair<field_type<protocol::OFPFW_NW_TOS>      , boost::mpl::int_<18>>
            , boost::fusion::pair<field_type<protocol::OFPFW_NW_PROTO>    , boost::mpl::int_<19>>
            , boost::fusion::pair<field_type<protocol::OFPFW_NW_SRC_ALL>  , boost::mpl::int_<22>>
            , boost::fusion::pair<field_type<protocol::OFPFW_NW_DST_ALL>  , boost::mpl::int_<23>>
            , boost::fusion::pair<field_type<protocol::OFPFW_TP_SRC>      , boost::mpl::int_<24>>
            , boost::fusion::pair<field_type<protocol::OFPFW_TP_DST>      , boost::mpl::int_<25>>
          //+-------------------------------------------------------------+------------------------+
        >;

        template <class T>
        struct mask_info;

        template <>
        struct mask_info<field_type<protocol::OFPFW_NW_SRC_ALL>>
        {
            static constexpr std::uint32_t shift = protocol::OFPFW_NW_SRC_SHIFT;
            static constexpr std::uint32_t mask = protocol::OFPFW_NW_SRC_MASK;
        };

        template <>
        struct mask_info<field_type<protocol::OFPFW_NW_DST_ALL>>
        {
            static constexpr std::uint32_t shift = protocol::OFPFW_NW_DST_SHIFT;
            static constexpr std::uint32_t mask = protocol::OFPFW_NW_DST_MASK;
        };

        template <class T, class FieldType>
        void validate(T const, FieldType) noexcept
        {
        }

        inline void validate(
                  std::uint16_t const in_port
                , field_type<protocol::OFPFW_IN_PORT>)
        {
            if (in_port == 0) {
                throw std::runtime_error{"in_port zero is invalid"};
            }
            if (in_port > protocol::OFPP_MAX
                    && in_port != protocol::OFPP_CONTROLLER
                    && in_port != protocol::OFPP_LOCAL) {
                throw std::runtime_error{"invalid in_port"};
            }
        }

        inline void validate(
                  std::uint16_t const vlan_vid
                , field_type<protocol::OFPFW_DL_VLAN>)
        {
            if (vlan_vid > 0x0fff && vlan_vid != protocol::OFP_VLAN_NONE) {
                throw std::runtime_error{"invalid vlan vid"};
            }
        }

        inline void validate(
                  std::uint8_t const vlan_pcp
                , field_type<protocol::OFPFW_DL_VLAN_PCP>)
        {
            if (vlan_pcp > 0x07) {
                throw std::runtime_error{"invalid vlan pcp"};
            }
        }

        inline auto get_dl_addr(
                  v10_detail::ofp_match& match
                , field_type<protocol::OFPFW_DL_DST>) noexcept
            -> std::uint8_t(&)[protocol::OFP_ETH_ALEN]
        {
            return match.dl_dst;
        }

        inline auto get_dl_addr(
                  v10_detail::ofp_match const& match
                , field_type<protocol::OFPFW_DL_DST>) noexcept
            -> std::uint8_t const(&)[protocol::OFP_ETH_ALEN]
        {
            return match.dl_dst;
        }

        inline auto get_dl_addr(
                  v10_detail::ofp_match& match
                , field_type<protocol::OFPFW_DL_SRC>) noexcept
            -> std::uint8_t(&)[protocol::OFP_ETH_ALEN]
        {
            return match.dl_src;
        }

        inline auto get_dl_addr(
                  v10_detail::ofp_match const& match
                , field_type<protocol::OFPFW_DL_SRC>) noexcept
            -> std::uint8_t const(&)[protocol::OFP_ETH_ALEN]
        {
            return match.dl_src;
        }

    } // namespace match_detail


    class match_set;


    namespace match {

        template <class FieldType>
        class match_field
            : private boost::equality_comparable<match_field<FieldType>>
        {
            using member_index = typename boost::fusion::result_of::value_at_key<
                match_detail::match_field_index_table, FieldType
            >::type;

        public:
            using field_type = FieldType;
            using value_type = typename boost::fusion::result_of::value_at<
                v10_detail::ofp_match, member_index
            >::type;

            explicit match_field(value_type const value) noexcept
                : value_(value)
            {
            }

            auto value() const noexcept
                -> value_type
            {
                return value_;
            }

            template <class Validator>
            void validate(Validator) const
            {
                match_detail::validate(value(), FieldType{});
            }

            static auto create(value_type const value)
                -> match_field
            {
                return validation::validate(match_field{value});
            }

            template <class FieldType2>
            friend auto operator==(
                      match_field<FieldType2> const&
                    , match_field<FieldType2> const&) noexcept
                -> bool;

        private:
            explicit match_field(v10_detail::ofp_match const& match)
                : value_(boost::fusion::at<member_index>(match))
            {
            }

            auto equal_impl(match_field const& rhs) const noexcept
                -> bool
            {
                return value_ == rhs.value_;
            }

            friend match_set;

            void set_value(v10_detail::ofp_match& match) const noexcept
            {
                boost::fusion::at<member_index>(match) = value_;
                match.wildcards &= ~FieldType::value;
            }

            static auto is_wildcard(v10_detail::ofp_match const& match) noexcept
                -> bool
            {
                return match.wildcards & FieldType::value;
            }

            static auto create_from_match(
                    v10_detail::ofp_match const& match) noexcept
                -> match_field
            {
                return match_field{match};
            }

            static void erase_from_match(v10_detail::ofp_match& match) noexcept
            {
                boost::fusion::at<member_index>(match) = 0;
                match.wildcards |= FieldType::value;
            }

        private:
            value_type value_;
        };

        template <>
        inline
        match_field<match_detail::field_type<protocol::OFPFW_NW_TOS>>
        ::match_field(value_type dscp) noexcept
            : value_(std::uint32_t{dscp} << 2)
        {
        }

        template <>
        inline auto
        match_field<match_detail::field_type<protocol::OFPFW_NW_TOS>>
        ::value() const noexcept
            -> value_type
        {
            return std::uint32_t{value_} >> 2;
        }

        template <class FieldType>
        auto operator==(
                  match_field<FieldType> const& lhs
                , match_field<FieldType> const& rhs) noexcept
            -> bool
        {
            return lhs.equal_impl(rhs);
        }

        template <class FieldType>
        auto equivalent(
                  match_field<FieldType> const& lhs
                , match_field<FieldType> const& rhs) noexcept
            -> bool
        {
            return lhs.value() == rhs.value();
        }


        template <class FieldType>
        class dl_addr_match_field
            : private boost::equality_comparable<dl_addr_match_field<FieldType>>
        {
        public:
            using field_type = FieldType;
            using value_type = canard::mac_address;

            explicit dl_addr_match_field(value_type const value)
                : value_(value)
            {
            }

            auto value() const noexcept
                -> value_type const
            {
                return value_;
            }

            template <class Validator>
            void validate(Validator) const
            {
            }

            static auto create(value_type const value)
                -> dl_addr_match_field
            {
                return validation::validate(dl_addr_match_field{value});
            }

        private:
            friend match_set;

            void set_value(v10_detail::ofp_match& match) const noexcept
            {
                std::memcpy(match_detail::get_dl_addr(match, FieldType{})
                          , value().to_bytes().data()
                          , value().to_bytes().size());
                match.wildcards &= ~FieldType::value;
            }

            static auto is_wildcard(v10_detail::ofp_match const& match) noexcept
                -> bool
            {
                return match.wildcards & FieldType::value;
            }


            static auto create_from_match(
                    v10_detail::ofp_match const& match) noexcept
                -> dl_addr_match_field
            {
                return dl_addr_match_field{
                    value_type{match_detail::get_dl_addr(match, FieldType{})}
                };
            }

            static void erase_from_match(v10_detail::ofp_match& match) noexcept
            {
                std::memset(match_detail::get_dl_addr(match, FieldType{})
                          , 0, protocol::OFP_ETH_ALEN);
                match.wildcards |= FieldType::value;
            }

        private:
            value_type value_;
        };

        template <class FieldType>
        auto operator==(
                  dl_addr_match_field<FieldType> const& lhs
                , dl_addr_match_field<FieldType> const& rhs) noexcept
            -> bool
        {
            return lhs.value() == rhs.value();
        }

        template <class FieldType>
        auto equivalent(
                  dl_addr_match_field<FieldType> const& lhs
                , dl_addr_match_field<FieldType> const& rhs) noexcept
            -> bool
        {
            return lhs == rhs;
        }


        template <class FieldType>
        class nw_addr_match_field
            : private boost::equality_comparable<nw_addr_match_field<FieldType>>
        {
            using member_index = typename boost::fusion::result_of::value_at_key<
                match_detail::match_field_index_table, FieldType
            >::type;

            using mask_info = match_detail::mask_info<FieldType>;

        public:
            using field_type = FieldType;
            using value_type = boost::asio::ip::address_v4;

            static constexpr std::uint8_t max_prefix_length = 32;

            explicit nw_addr_match_field(
                      value_type const value
                    , std::uint8_t const prefix_length = max_prefix_length)
                : value_(value)
                , wildcard_bit_count_(max_prefix_length - prefix_length)
            {
            }

            explicit nw_addr_match_field(
                      boost::asio::ip::address const value
                    , std::uint8_t const prefix_length = max_prefix_length)
                : nw_addr_match_field{value.to_v4(), prefix_length}
            {
            }

            auto value() const noexcept
                -> value_type
            {
                return value_;
            }

            auto prefix_length() const noexcept
                -> std::uint8_t
            {
                constexpr auto tmp = max_prefix_length; // avoid odr-used
                return max_prefix_length - std::min(wildcard_bit_count_, tmp);
            }

            auto wildcard_bit_count() const noexcept
                -> std::uint8_t
            {
                return wildcard_bit_count_;
            }

            template <class Validator>
            void validate(Validator) const
            {
                constexpr auto max_wildcard_bit_count
                    = mask_info::mask >> mask_info::shift;
                if (wildcard_bit_count_ > max_wildcard_bit_count) {
                    throw std::runtime_error{"invalid wildcard"};
                }
            }

            template <class IPAddress>
            static auto create(
                    IPAddress const value, std::uint8_t const prefix_length)
                -> nw_addr_match_field
            {
                if (prefix_length > max_prefix_length) {
                    throw std::runtime_error{"invalid prefix length"};
                }
                return validation::validate(
                        nw_addr_match_field{value, prefix_length});
            }

        private:
            explicit nw_addr_match_field(v10_detail::ofp_match const& match)
                : value_{boost::fusion::at<member_index>(match)}
                , wildcard_bit_count_(
                        (match.wildcards & mask_info::mask) >> mask_info::shift)
            {
            }

            friend match_set;

            void set_value(v10_detail::ofp_match& match) const noexcept
            {
                boost::fusion::at<member_index>(match) = value().to_ulong();
                match.wildcards &= ~mask_info::mask;
                match.wildcards
                    |= (std::uint32_t{wildcard_bit_count_} << mask_info::shift);
            }

            static auto is_wildcard(v10_detail::ofp_match const& match) noexcept
                -> bool
            {
                return match.wildcards & FieldType::value;
            }

            static auto create_from_match(v10_detail::ofp_match const& match)
                -> nw_addr_match_field
            {
                return nw_addr_match_field{match};
            }

            static void erase_from_match(v10_detail::ofp_match& match) noexcept
            {
                boost::fusion::at<member_index>(match) = 0;
                match.wildcards |= FieldType::value;
            }

        private:
            value_type value_;
            std::uint8_t wildcard_bit_count_;
        };

        template <class FieldType>
        auto operator==(
                  nw_addr_match_field<FieldType> const& lhs
                , nw_addr_match_field<FieldType> const& rhs) noexcept
            -> bool
        {
            return lhs.wildcard_bit_count() == rhs.wildcard_bit_count()
                && lhs.value() == rhs.value();
        }

        template <class FieldType>
        auto equivalent(
                  nw_addr_match_field<FieldType> const& lhs
                , nw_addr_match_field<FieldType> const& rhs) noexcept
            -> bool
        {
            return lhs.wildcard_bit_count() == rhs.wildcard_bit_count()
                && (lhs.value().to_ulong() >> lhs.wildcard_bit_count())
                == (rhs.value().to_ulong() >> rhs.wildcard_bit_count());
        }


        using in_port = match_field<match_detail::field_type<protocol::OFPFW_IN_PORT>>;
        using eth_src = dl_addr_match_field<match_detail::field_type<protocol::OFPFW_DL_SRC>>;
        using eth_dst = dl_addr_match_field<match_detail::field_type<protocol::OFPFW_DL_DST>>;
        using vlan_vid = match_field<match_detail::field_type<protocol::OFPFW_DL_VLAN>>;
        using vlan_pcp = match_field<match_detail::field_type<protocol::OFPFW_DL_VLAN_PCP>>;
        using eth_type = match_field<match_detail::field_type<protocol::OFPFW_DL_TYPE>>;
        using ip_dscp = match_field<match_detail::field_type<protocol::OFPFW_NW_TOS>>;
        using ip_proto = match_field<match_detail::field_type<protocol::OFPFW_NW_PROTO>>;
        using ipv4_src = nw_addr_match_field<match_detail::field_type<protocol::OFPFW_NW_SRC_ALL>>;
        using ipv4_dst = nw_addr_match_field<match_detail::field_type<protocol::OFPFW_NW_DST_ALL>>;
        using arp_spa = nw_addr_match_field<match_detail::field_type<protocol::OFPFW_NW_SRC_ALL>>;
        using arp_tpa = nw_addr_match_field<match_detail::field_type<protocol::OFPFW_NW_DST_ALL>>;
        using tcp_src = match_field<match_detail::field_type<protocol::OFPFW_TP_SRC>>;
        using tcp_dst = match_field<match_detail::field_type<protocol::OFPFW_TP_DST>>;
        using udp_src = match_field<match_detail::field_type<protocol::OFPFW_TP_SRC>>;
        using udp_dst = match_field<match_detail::field_type<protocol::OFPFW_TP_DST>>;
        using icmpv4_type = match_field<match_detail::field_type<protocol::OFPFW_TP_SRC>>;
        using icmpv4_code = match_field<match_detail::field_type<protocol::OFPFW_TP_DST>>;

    } // namespace match

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MATCH_FIELDS_HPP