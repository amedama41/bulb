#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_SET_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_SET_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/set_base.hpp>
#include <canard/network/openflow/v13/any_table_feature_property.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class table_feature_property_set
    : public detail::set_base<
        table_feature_property_set, std::uint16_t, any_table_feature_property
      >
  {
  public:
    using set_base::set_base;

  private:
    friend set_base;

    using set_info = set_base::default_set_info;

    template <class Property>
    static constexpr auto key_impl() noexcept
      -> key_type
    {
      return Property::type();
    }

    template <class Property>
    static auto get_key_impl(Property const& property) noexcept
      -> key_type
    {
      return property.type();
    }

    template <class T>
    static auto cast_impl(const_reference property)
      -> T const&
    {
      return v13::any_cast<T>(property);
    }
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  extern template class detail::set_base<
      v13::table_feature_property_set
    , std::uint16_t
    , v13::any_table_feature_property
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#else
# include <canard/network/openflow/v13/impl/table_feature_property_set.ipp>
#endif

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_SET_HPP
