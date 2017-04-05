#ifndef CANARD_NET_OFP_V10_MATCH_HPP
#define CANARD_NET_OFP_V10_MATCH_HPP

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/query/all.hpp>
#include <boost/optional/optional.hpp>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/type_traits/conjuction.hpp>
#include <canard/net/ofp/v10/common/match_fields.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  namespace match_detail {

    template <protocol::flow_wildcards Field>
    auto is_match_field_impl(match_fields::match_field<Field> const&) noexcept
      -> std::true_type;

    template <protocol::flow_wildcards Field>
    auto is_match_field_impl(
        match_fields::dl_addr_match_field<Field> const&) noexcept
      -> std::true_type;

    template <protocol::flow_wildcards Field>
    auto is_match_field_impl(
        match_fields::nw_addr_match_field<Field> const&) noexcept
      -> std::true_type;

    auto is_match_field_impl(...) noexcept -> std::false_type;

    template <class T>
    struct is_match_field : decltype(is_match_field_impl(std::declval<T>()))
    {};

  } // namespace match_detail

  class match
    : public detail::basic_protocol_type<match>
  {
    template <class... Ts>
    using enable_if_is_all_match_field = typename std::enable_if<
         sizeof...(Ts)
      && ofp::type_traits::conjuction<match_detail::is_match_field<Ts>...>::value
    >::type;

  public:
    using raw_ofp_type = protocol::ofp_match;

    match() noexcept
      : match_{protocol::OFPFW_ALL, 0}
    {
    }

    template <
      class... MatchFields, class = enable_if_is_all_match_field<MatchFields...>
    >
    explicit match(MatchFields&&... fields) noexcept
      : match_{protocol::OFPFW_ALL, 0}
    {
      struct dummy_type {};
      dummy_type const unused[] = {
        (set(std::forward<MatchFields>(fields)), dummy_type{})...
      };
      static_cast<void>(unused);
    }

    explicit match(raw_ofp_type const& match) noexcept
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

    static constexpr auto length() noexcept
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
    friend basic_protocol_type;

    template <class Validator>
    void validate_impl(Validator)
    {
      // TODO
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, match_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> match
    {
      return match{detail::decode<raw_ofp_type>(first, last)};
    }

    auto equal_impl(match const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(match_, rhs.match_);
    }

    auto equivalent_impl(match const& rhs) const noexcept
      -> bool
    {
      return equivalent_for_each_field{this, &rhs};
    }

    struct equivalent_for_each_field
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
            match_fields::in_port*
          , match_fields::eth_src*, match_fields::eth_dst*
          , match_fields::vlan_vid*, match_fields::vlan_pcp*
          , match_fields::eth_type*
          , match_fields::ip_dscp*, match_fields::ip_proto*
          , match_fields::ipv4_src*, match_fields::ipv4_dst*
          , match_fields::tcp_src*, match_fields::tcp_dst*
        >;
        return boost::fusion::all(match_field_pointer_list{}, *this);
      }

      match const* lhs;
      match const* rhs;
    };

  private:
    raw_ofp_type match_;
  };

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MATCH_HPP
