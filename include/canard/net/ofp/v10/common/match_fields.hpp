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
#include <canard/net/ofp/validator.hpp>
#include <canard/net/ofp/v10/detail/fusion_adaptor.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  namespace match_detail {

    using protocol::flow_wildcards;

    template <flow_wildcards Field>
    using field_type = std::integral_constant<flow_wildcards, Field>;

    template <flow_wildcards Field, int MemberIndex>
    using pair
      = boost::fusion::pair<field_type<Field>, boost::mpl::int_<MemberIndex>>;

    using match_field_index_table = boost::fusion::map<
    //  ofp_match field type               ofp_match member index
    //+----------------------------------+------------------------+
        pair<flow_wildcards::in_port     , 1>
      , pair<flow_wildcards::dl_src      , 2>
      , pair<flow_wildcards::dl_dst      , 3>
      , pair<flow_wildcards::dl_vlan     , 4>
      , pair<flow_wildcards::dl_vlan_pcp , 5>
      , pair<flow_wildcards::dl_type     , 7>
      , pair<flow_wildcards::nw_tos      , 8>
      , pair<flow_wildcards::nw_proto    , 9>
      , pair<flow_wildcards::nw_src_all  , 11>
      , pair<flow_wildcards::nw_dst_all  , 12>
      , pair<flow_wildcards::tp_src      , 13>
      , pair<flow_wildcards::tp_dst      , 14>
    //+----------------------------------+------------------------+
    >;

    template <flow_wildcards Field>
    using member_index = typename boost::fusion::result_of::value_at_key<
      match_field_index_table, field_type<Field>
    >::type;

    template <flow_wildcards>
    struct mask_info;

    template <>
    struct mask_info<flow_wildcards::nw_src_all>
    {
      static constexpr std::uint32_t shift = flow_wildcards::nw_src_shift;
      static constexpr std::uint32_t mask = flow_wildcards::nw_src_mask;
    };

    template <>
    struct mask_info<flow_wildcards::nw_dst_all>
    {
      static constexpr std::uint32_t shift = flow_wildcards::nw_dst_shift;
      static constexpr std::uint32_t mask = flow_wildcards::nw_dst_mask;
    };

    template <class T, class FieldType>
    void validate(T const, FieldType) noexcept
    {
    }

    inline void validate(
        std::uint16_t const in_port, field_type<flow_wildcards::in_port>)
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
        std::uint16_t const vlan_vid, field_type<flow_wildcards::dl_vlan>)
    {
      if (vlan_vid > 0x0fff && vlan_vid != protocol::OFP_VLAN_NONE) {
        throw std::runtime_error{"invalid vlan vid"};
      }
    }

    inline void validate(
        std::uint8_t const vlan_pcp, field_type<flow_wildcards::dl_vlan_pcp>)
    {
      if (vlan_pcp > 0x07) {
        throw std::runtime_error{"invalid vlan pcp"};
      }
    }

    inline auto get_dl_addr(
          protocol::ofp_match& match
        , field_type<flow_wildcards::dl_dst>) noexcept
      -> std::uint8_t(&)[protocol::OFP_ETH_ALEN]
    {
      return match.dl_dst;
    }

    inline auto get_dl_addr(
          protocol::ofp_match const& match
        , field_type<flow_wildcards::dl_dst>) noexcept
      -> std::uint8_t const(&)[protocol::OFP_ETH_ALEN]
    {
      return match.dl_dst;
    }

    inline auto get_dl_addr(
          protocol::ofp_match& match
        , field_type<flow_wildcards::dl_src>) noexcept
      -> std::uint8_t(&)[protocol::OFP_ETH_ALEN]
    {
      return match.dl_src;
    }

    inline auto get_dl_addr(
          protocol::ofp_match const& match
        , field_type<flow_wildcards::dl_src>) noexcept
      -> std::uint8_t const(&)[protocol::OFP_ETH_ALEN]
    {
      return match.dl_src;
    }

  } // namespace match_detail


  class match;


  namespace match_fields {

    template <protocol::flow_wildcards Field>
    class match_field
      : private boost::equality_comparable<match_field<Field>>
    {
      using member_index = match_detail::member_index<Field>;

    public:
      using field_type = match_detail::field_type<Field>;
      using value_type = typename boost::fusion::result_of::value_at<
        protocol::ofp_match, member_index
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
        match_detail::validate(value(), field_type{});
      }

      static auto create(value_type const value)
        -> match_field
      {
        return validation::validate(match_field{value});
      }

      template <protocol::flow_wildcards Field2>
      friend auto operator==(
          match_field<Field2> const&, match_field<Field2> const&) noexcept
        -> bool;

    private:
      explicit match_field(protocol::ofp_match const& match)
        : value_(boost::fusion::at<member_index>(match))
      {
      }

      auto equal_impl(match_field const& rhs) const noexcept
        -> bool
      {
        return value_ == rhs.value_;
      }

      friend match;

      void set_value(protocol::ofp_match& match) const noexcept
      {
        boost::fusion::at<member_index>(match) = value_;
        match.wildcards &= ~field_type::value;
      }

      static auto is_wildcard(protocol::ofp_match const& match) noexcept
        -> bool
      {
        return match.wildcards & field_type::value;
      }

      static auto create_from_match(protocol::ofp_match const& match) noexcept
        -> match_field
      {
        return match_field{match};
      }

      static void erase_from_match(protocol::ofp_match& match) noexcept
      {
        boost::fusion::at<member_index>(match) = 0;
        match.wildcards |= field_type::value;
      }

    private:
      value_type value_;
    };

    template <>
    inline match_field<protocol::flow_wildcards::nw_tos>::match_field(
        value_type dscp) noexcept
      : value_(std::uint32_t{dscp} << 2)
    {
    }

    template <>
    inline auto
    match_field<protocol::flow_wildcards::nw_tos>::value() const noexcept
      -> value_type
    {
      return std::uint32_t{value_} >> 2;
    }

    template <protocol::flow_wildcards Field>
    auto operator==(
        match_field<Field> const& lhs, match_field<Field> const& rhs) noexcept
      -> bool
    {
      return lhs.equal_impl(rhs);
    }

    template <protocol::flow_wildcards Field>
    auto equivalent(
        match_field<Field> const& lhs, match_field<Field> const& rhs) noexcept
      -> bool
    {
      return lhs.value() == rhs.value();
    }


    template <protocol::flow_wildcards Field>
    class dl_addr_match_field
      : private boost::equality_comparable<dl_addr_match_field<Field>>
    {
    public:
      using field_type = match_detail::field_type<Field>;
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
      friend match;

      void set_value(protocol::ofp_match& match) const noexcept
      {
        std::memcpy(
              match_detail::get_dl_addr(match, field_type{})
            , value().to_bytes().data()
            , value().to_bytes().size());
        match.wildcards &= ~field_type::value;
      }

      static auto is_wildcard(protocol::ofp_match const& match) noexcept
        -> bool
      {
        return match.wildcards & field_type::value;
      }


      static auto create_from_match(
          protocol::ofp_match const& match) noexcept
        -> dl_addr_match_field
      {
        return dl_addr_match_field{
          value_type{match_detail::get_dl_addr(match, field_type{})}
        };
      }

      static void erase_from_match(protocol::ofp_match& match) noexcept
      {
        std::memset(
              match_detail::get_dl_addr(match, field_type{})
            , 0, protocol::OFP_ETH_ALEN);
        match.wildcards |= field_type::value;
      }

    private:
      value_type value_;
    };

    template <protocol::flow_wildcards Field>
    auto operator==(
          dl_addr_match_field<Field> const& lhs
        , dl_addr_match_field<Field> const& rhs) noexcept
      -> bool
    {
      return lhs.value() == rhs.value();
    }

    template <protocol::flow_wildcards Field>
    auto equivalent(
          dl_addr_match_field<Field> const& lhs
        , dl_addr_match_field<Field> const& rhs) noexcept
      -> bool
    {
      return lhs == rhs;
    }


    template <protocol::flow_wildcards Field>
    class nw_addr_match_field
      : private boost::equality_comparable<nw_addr_match_field<Field>>
    {
      using member_index = match_detail::member_index<Field>;
      using mask_info = match_detail::mask_info<Field>;

    public:
      using field_type = match_detail::field_type<Field>;
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
        return validation::validate(nw_addr_match_field{value, prefix_length});
      }

    private:
      explicit nw_addr_match_field(protocol::ofp_match const& match)
        : value_{boost::fusion::at<member_index>(match)}
        , wildcard_bit_count_(
            (match.wildcards & mask_info::mask) >> mask_info::shift)
      {
      }

      friend match;

      void set_value(protocol::ofp_match& match) const noexcept
      {
        boost::fusion::at<member_index>(match) = value().to_ulong();
        match.wildcards &= ~mask_info::mask;
        match.wildcards
          |= (std::uint32_t{wildcard_bit_count_} << mask_info::shift);
      }

      static auto is_wildcard(protocol::ofp_match const& match) noexcept
        -> bool
      {
        return match.wildcards & field_type::value;
      }

      static auto create_from_match(protocol::ofp_match const& match)
        -> nw_addr_match_field
      {
        return nw_addr_match_field{match};
      }

      static void erase_from_match(protocol::ofp_match& match) noexcept
      {
        boost::fusion::at<member_index>(match) = 0;
        match.wildcards |= field_type::value;
      }

    private:
      value_type value_;
      std::uint8_t wildcard_bit_count_;
    };

    template <protocol::flow_wildcards Field>
    auto operator==(
          nw_addr_match_field<Field> const& lhs
        , nw_addr_match_field<Field> const& rhs) noexcept
      -> bool
    {
      return lhs.wildcard_bit_count() == rhs.wildcard_bit_count()
          && lhs.value() == rhs.value();
    }

    template <protocol::flow_wildcards Field>
    auto equivalent(
          nw_addr_match_field<Field> const& lhs
        , nw_addr_match_field<Field> const& rhs) noexcept
      -> bool
    {
      return lhs.wildcard_bit_count() == rhs.wildcard_bit_count()
          && (lhs.value().to_ulong() >> lhs.wildcard_bit_count())
            == (rhs.value().to_ulong() >> rhs.wildcard_bit_count());
    }


    using in_port = match_field<protocol::flow_wildcards::in_port>;
    using eth_src = dl_addr_match_field<protocol::flow_wildcards::dl_src>;
    using eth_dst = dl_addr_match_field<protocol::flow_wildcards::dl_dst>;
    using vlan_vid = match_field<protocol::flow_wildcards::dl_vlan>;
    using vlan_pcp = match_field<protocol::flow_wildcards::dl_vlan_pcp>;
    using eth_type = match_field<protocol::flow_wildcards::dl_type>;
    using ip_dscp = match_field<protocol::flow_wildcards::nw_tos>;
    using ip_proto = match_field<protocol::flow_wildcards::nw_proto>;
    using ipv4_src = nw_addr_match_field<protocol::flow_wildcards::nw_src_all>;
    using ipv4_dst = nw_addr_match_field<protocol::flow_wildcards::nw_dst_all>;
    using arp_spa = nw_addr_match_field<protocol::flow_wildcards::nw_src_all>;
    using arp_tpa = nw_addr_match_field<protocol::flow_wildcards::nw_dst_all>;
    using tcp_src = match_field<protocol::flow_wildcards::tp_src>;
    using tcp_dst = match_field<protocol::flow_wildcards::tp_dst>;
    using udp_src = match_field<protocol::flow_wildcards::tp_src>;
    using udp_dst = match_field<protocol::flow_wildcards::tp_dst>;
    using icmpv4_type = match_field<protocol::flow_wildcards::tp_src>;
    using icmpv4_code = match_field<protocol::flow_wildcards::tp_dst>;

  } // namespace match_fields

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MATCH_FIELDS_HPP
