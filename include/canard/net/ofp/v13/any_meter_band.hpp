#ifndef CANARD_NET_OFP_V13_ANY_METER_BAND_HPP
#define CANARD_NET_OFP_V13_ANY_METER_BAND_HPP

#include <canard/net/ofp/detail/any_type.hpp>
#include <canard/net/ofp/v13/decoder/meter_band_decoder.hpp>

#include <cstdint>
#include <boost/variant/static_visitor.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  namespace any_meter_band_detail {

    template <class Derived>
    class base
    {
    public:
      CANARD_NET_OFP_DECL auto rate() const noexcept
        -> std::uint32_t;
      CANARD_NET_OFP_DECL auto burst_size() const noexcept
        -> std::uint32_t;

    private:
      CANARD_NET_OFP_DECL auto derived() const noexcept
        -> Derived const&;
    };

    struct rate_visitor : boost::static_visitor<std::uint32_t>
    {
      template <class T>
      auto operator()(T const& t) const noexcept
        -> result_type
      { return t.rate(); }
    };

    struct burst_size_visitor : boost::static_visitor<std::uint32_t>
    {
      template <class T>
      auto operator()(T const& t) const noexcept
        -> result_type
      { return t.burst_size(); }
    };

    template <class Derived>
    auto base<Derived>::rate() const noexcept
      -> std::uint32_t
    {
      return derived().visit(rate_visitor{});
    }

    template <class Derived>
    auto base<Derived>::burst_size() const noexcept
      -> std::uint32_t
    {
      return derived().visit(burst_size_visitor{});
    }

    template <class Derived>
    auto base<Derived>::derived() const noexcept
      -> Derived const&
    {
      return *static_cast<Derived const*>(this);
    }

  } // namespace any_meter_band_detail


  using any_meter_band = detail::any_type<
    meter_band_decoder, any_meter_band_detail::base
  >;

  template <class T>
  auto any_cast(any_meter_band& meter_band)
    -> T&
  {
    return detail::any_cast<T>(meter_band);
  }

  template <class T>
  auto any_cast(any_meter_band const& meter_band)
    -> T const&
  {
    return detail::any_cast<T>(meter_band);
  }

  template <class T>
  auto any_cast(any_meter_band* const meter_band)
    -> T*
  {
    return detail::any_cast<T>(meter_band);
  }

  template <class T>
  auto any_cast(any_meter_band const* const meter_band)
    -> T const*
  {
    return detail::any_cast<T>(meter_band);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)

#include <canard/net/ofp/v13/impl/any_meter_band.ipp>

#elif defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace net {
namespace ofp {

  extern template class detail::any_type<
    v13::meter_band_decoder, v13::any_meter_band_detail::base
  >;
  extern template class v13::any_meter_band_detail::base<
    detail::any_type<v13::meter_band_decoder, v13::any_meter_band_detail::base>
  >;

} // namespace ofp
} // namespace net
} // namespace canard

#endif

#endif // CANARD_NET_OFP_V13_ANY_METER_BAND_HPP
