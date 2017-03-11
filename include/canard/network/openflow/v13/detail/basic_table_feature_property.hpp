#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_TABLE_FEATURE_PROPERTY_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_TABLE_FEATURE_PROPERTY_HPP

#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  class basic_table_feature_property
    : public detail::basic_protocol_type<T>
  {
    using base_t = detail::basic_protocol_type<T>;

  protected:
    basic_table_feature_property() = default;

  public:
    using ofp_header_type = ofp::v13::protocol::ofp_table_feature_prop_header;

    static constexpr auto type() noexcept
      -> ofp::v13::protocol::ofp_table_feature_prop_type
    {
      return T::prop_type;
    }

    static auto validate_header(ofp_header_type const& header) noexcept
      -> char const*
    {
      if (header.type != type()) {
        return "invalid table_feature_property type";
      }
      if (!is_valid_table_feature_property_length(header)) {
        return "invalid table_feature_property length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_table_feature_property_length(
        ofp_header_type const& header) noexcept
      -> bool
    {
      return header.length >= T::min_length();
    }
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_TABLE_FEATURE_PROPERTY_HPP
