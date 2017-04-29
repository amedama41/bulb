#ifndef CANARD_NET_OFP_V10_ACTIONS_SET_VLAN_VID_HPP
#define CANARD_NET_OFP_V10_ACTIONS_SET_VLAN_VID_HPP

#include <cstdint>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>
#include <boost/fusion/support/pair.hpp>
#include <canard/net/ofp/v10/common/match_fields.hpp>
#include <canard/net/ofp/v10/detail/basic_fixed_length_action.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/detail/fusion_adaptor.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace actions {

  namespace set_field_detail {

    using protocol::action_type;

    template <action_type ActionType, class OFPAction>
    struct ofp_action_info
    {
      static constexpr protocol::action_type action_type = ActionType;
      using ofp_type = OFPAction;
    };

    template <class MatchField, action_type ActionType, class OFPAction>
    using pair
      = boost::fusion::pair<MatchField, ofp_action_info<ActionType, OFPAction>>;

    using set_field_info_table = boost::fusion::map<
        pair<match_fields::eth_src  , action_type::set_dl_src   , protocol::ofp_action_dl_addr>
      , pair<match_fields::eth_dst  , action_type::set_dl_dst   , protocol::ofp_action_dl_addr>
      , pair<match_fields::vlan_vid , action_type::set_vlan_vid , protocol::ofp_action_vlan_vid>
      , pair<match_fields::vlan_pcp , action_type::set_vlan_pcp , protocol::ofp_action_vlan_pcp>
      , pair<match_fields::ip_dscp  , action_type::set_nw_tos   , protocol::ofp_action_nw_tos>
      , pair<match_fields::ipv4_src , action_type::set_nw_src   , protocol::ofp_action_nw_addr>
      , pair<match_fields::ipv4_dst , action_type::set_nw_dst   , protocol::ofp_action_nw_addr>
      , pair<match_fields::tcp_src  , action_type::set_tp_src   , protocol::ofp_action_tp_port>
      , pair<match_fields::tcp_dst  , action_type::set_tp_dst   , protocol::ofp_action_tp_port>
    >;

    template <class MatchField>
    struct set_field_info
    {
      using ofp_action_info = typename boost::fusion::result_of::value_at_key<
        set_field_info_table, MatchField
      >::type;

      static constexpr protocol::action_type action_type
        = ofp_action_info::action_type;

      using ofp_type = typename ofp_action_info::ofp_type;
    };

    template <class ValueType, class SetFieldInfo>
    auto to_ofp_action(ValueType value, SetFieldInfo) noexcept
      -> typename SetFieldInfo::ofp_type
    {
      using ofp_type = typename SetFieldInfo::ofp_type;
      return ofp_type{SetFieldInfo::action_type, sizeof(ofp_type), value};
    }

    inline auto to_ofp_action(
        std::uint8_t const value, set_field_info<match_fields::ip_dscp>) noexcept
      -> set_field_info<match_fields::ip_dscp>::ofp_type
    {
      using info = set_field_info<match_fields::ip_dscp>;
      return info::ofp_type{
          info::action_type
        , sizeof(info::ofp_type)
        , std::uint8_t(std::uint32_t{value} << 2)
        , { 0, 0, 0 }
      };
    }

    template <class SetFieldInfo>
    auto to_ofp_action(
        boost::asio::ip::address_v4 const& value, SetFieldInfo) noexcept
      -> typename SetFieldInfo::ofp_type
    {
      using ofp_type = typename SetFieldInfo::ofp_type;
      return ofp_type{
          SetFieldInfo::action_type
        , sizeof(ofp_type)
        , std::uint32_t(value.to_ulong())
      };
    }

    template <class SetFieldInfo>
    auto to_ofp_action(
        canard::mac_address const& value, SetFieldInfo) noexcept
      -> typename SetFieldInfo::ofp_type
    {
      using ofp_type = typename SetFieldInfo::ofp_type;
      auto const& bytes = value.to_bytes();
      return ofp_type{
          SetFieldInfo::action_type
        , sizeof(ofp_type)
        , { bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5] }
        , { 0, 0, 0, 0, 0, 0 }
      };
    }

    template <class OFPAction>
    auto access(OFPAction const& action_set_field) noexcept
      -> typename boost::fusion::result_of::value_at_c<OFPAction, 2>::type
    {
      return boost::fusion::at_c<2>(action_set_field);
    }

    inline auto access(
        protocol::ofp_action_nw_tos const& set_nw_tos) noexcept
      -> typename boost::fusion::result_of::value_at_c<
           protocol::ofp_action_nw_tos, 2
         >::type
    {
      return std::uint32_t{boost::fusion::at_c<2>(set_nw_tos)} >> 2;
    }

    inline auto access(
        protocol::ofp_action_dl_addr const& set_dl_addr) noexcept
      -> canard::mac_address
    {
      return canard::mac_address{set_dl_addr.dl_addr};
    }

    inline auto access(protocol::ofp_action_nw_addr const& set_nw_addr)
      -> boost::asio::ip::address_v4
    {
      return boost::asio::ip::address_v4{set_nw_addr.nw_addr};
    }

  } // namespace set_field_detail


  template <class MatchField>
  class set_field
    : public detail::v10::basic_fixed_length_action<set_field<MatchField>>
  {
    using base_t
      = detail::v10::basic_fixed_length_action<set_field<MatchField>>;
    using value_type = typename MatchField::value_type;
    using set_field_info = set_field_detail::set_field_info<MatchField>;

  public:
    using ofp_type = typename set_field_info::ofp_type;

    static constexpr protocol::action_type action_type
      = set_field_info::action_type;

    explicit set_field(value_type const& value) noexcept
      : set_field_(set_field_detail::to_ofp_action(value, set_field_info{}))
    {
    }

    explicit set_field(MatchField const& field) noexcept
      : set_field{field.value()}
    {
    }

    auto value() const noexcept
      -> value_type
    {
      return set_field_detail::access(set_field_);
    };

    auto match_field() const noexcept
      -> MatchField
    {
      return MatchField{value()};
    }

  private:
    friend base_t;

    explicit set_field(ofp_type const& action) noexcept
      : set_field_(action)
    {
    }

    auto ofp_action() const noexcept
      -> ofp_type const&
    {
      return set_field_;
    }

    void validate_action() const
    {
      match_detail::validate(value(), typename MatchField::field_type{});
    }

    auto is_equivalent_action(set_field const& rhs) const noexcept
      -> bool
    {
      return value() == rhs.value();
    }

  private:
    ofp_type set_field_;
  };

  using set_eth_src = set_field<match_fields::eth_src>;
  using set_eth_dst = set_field<match_fields::eth_dst>;
  using set_vlan_vid = set_field<match_fields::vlan_vid>;
  using set_vlan_pcp = set_field<match_fields::vlan_pcp>;
  using set_ip_dscp = set_field<match_fields::ip_dscp>;
  using set_ipv4_src = set_field<match_fields::ipv4_src>;
  using set_ipv4_dst = set_field<match_fields::ipv4_dst>;
  using set_tcp_dst = set_field<match_fields::tcp_dst>;
  using set_tcp_src = set_field<match_fields::tcp_src>;
  using set_udp_dst = set_field<match_fields::udp_dst>;
  using set_udp_src = set_field<match_fields::udp_src>;

} // namespace actions
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTIONS_SET_VLAN_VID_HPP
