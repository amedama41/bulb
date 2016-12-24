#ifndef CANARD_NET_OFP_V13_OXM_MATCH_FIELD_SET_HPP
#define CANARD_NET_OFP_V13_OXM_MATCH_FIELD_SET_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <boost/container/flat_map.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <canard/network/openflow/detail/set_base.hpp>
#include <canard/network/openflow/v13/any_oxm_match_field.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class oxm_match_field_set
    : public detail::set_base<
        oxm_match_field_set, std::uint32_t, any_oxm_match_field
      >
  {
  public:
    using set_base::set_base;

  private:
    friend set_base;

    class set_info
    {
      using field_set_type
        = boost::container::flat_map<key_type, const_iterator>;

    public:
      explicit set_info(list_type const& fields)
        : field_set_{}
        , is_valid_set_{}
      {
        field_set_.reserve(fields.size());
        auto const it_end = fields.cend();
        for (auto it = fields.cbegin(); it != it_end; ++it) {
          // TODO prerequisite checking
          if (!field_set_.emplace(get_key_impl(*it), it).second) {
            is_valid_set_ = false;
            return;
          }
        }
        is_valid_set_ = true;
      }

      explicit operator bool() const noexcept
      {
        return is_valid_set_;
      }

      friend auto operator==(set_info const& lhs, set_info const& rhs) noexcept
        -> bool
      {
        using namespace boost::adaptors;
        return boost::equal(
              lhs.field_set_ | map_values | filtered(is_not_wildcard{})
            , rhs.field_set_ | map_values | filtered(is_not_wildcard{})
            , [](const_iterator lhs_it, const_iterator rhs_it)
              { return equivalent(*lhs_it, *rhs_it); });
      }

    private:
      struct is_not_wildcard
      {
        auto operator()(const_iterator it) const noexcept
          -> bool
        { return !it->is_wildcard(); }
      };

    private:
      field_set_type field_set_;
      bool is_valid_set_;
    };

    template <class OXMMatchField>
    static constexpr auto key_impl() noexcept
      -> key_type
    {
      return OXMMatchField::oxm_type();
    };

    template <class OXMMatchField>
    static auto get_key_impl(OXMMatchField const& field) noexcept
      -> key_type
    {
      return field.oxm_type();
    }

    template <class T>
    static auto cast_impl(const_reference field)
      -> T const&
    {
      return v13::any_cast<T>(field);
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
    v13::oxm_match_field_set, std::uint32_t, v13::any_oxm_match_field
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#else
# include <canard/network/openflow/v13/impl/oxm_match_field_set.ipp>
#endif

#endif // CANARD_NET_OFP_V13_OXM_MATCH_FIELD_SET_HPP
