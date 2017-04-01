#ifndef CANARD_NET_OFP_DETAIL_V13_METER_MOD_BASE_HPP
#define CANARD_NET_OFP_DETAIL_V13_METER_MOD_BASE_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/any_meter_band.hpp>
#include <canard/net/ofp/v13/detail/basic_message.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class MeterMod>
  class meter_mod_base
    : public ofp::detail::v13::basic_message<MeterMod>
  {
    using base_t = ofp::detail::v13::basic_message<MeterMod>;

  public:
    using raw_ofp_type = ofp::v13::protocol::ofp_meter_mod;
    using bands_type = ofp::list<ofp::v13::any_meter_band>;

    static constexpr ofp::v13::protocol::ofp_type message_type
      = ofp::v13::protocol::OFPT_METER_MOD;

    static constexpr auto command() noexcept
      -> std::uint16_t
    {
      return MeterMod::command_type;
    }

    auto header() const noexcept
      -> ofp::v13::protocol::ofp_header const&
    {
      return meter_mod_.header;
    }

    auto meter_id() const noexcept
      -> std::uint32_t
    {
      return meter_mod_.meter_id;
    }

    auto flags() const noexcept
      -> std::uint16_t
    {
      return meter_mod_.flags;
    }

    auto bands() const noexcept
      -> bands_type const&
    {
      return bands_;
    }

    auto extract_bands()
      -> bands_type
    {
      auto bands = bands_type{};
      bands.swap(bands_);
      meter_mod_.header.length = sizeof(raw_ofp_type);
      return std::move(bands);
    }

  protected:
    meter_mod_base(
          std::uint32_t const meter_id
        , std::uint16_t const flags, bands_type&& bands
        , std::uint32_t const xid)
      : meter_mod_{
            ofp::v13::protocol::ofp_header{
                base_t::version()
              , base_t::type()
              , bands.calc_ofp_length(sizeof(raw_ofp_type))
              , xid
            }
          , command()
          , flags
          , meter_id
        }
      , bands_(std::move(bands))
    {
    }

    meter_mod_base(std::uint32_t const meter_id, std::uint32_t const xid)
      : meter_mod_{
            ofp::v13::protocol::ofp_header{
                base_t::version()
              , base_t::type()
              , sizeof(raw_ofp_type)
              , xid
            }
          , command()
          , 0
          , meter_id
        }
      , bands_{}
    {
    }

    meter_mod_base(raw_ofp_type const& meter_mod, bands_type&& bands)
      : meter_mod_(meter_mod)
      , bands_(std::move(bands))
    {
    }

    meter_mod_base(meter_mod_base const&) = default;

    meter_mod_base(meter_mod_base&& other)
      : meter_mod_(other.meter_mod_)
      , bands_(other.extract_bands())
    {
    }

    auto operator=(meter_mod_base const& other)
      -> meter_mod_base&
    {
      return operator=(meter_mod_base{other});
    }

    auto operator=(meter_mod_base&& other)
      -> meter_mod_base&
    {
      auto tmp = std::move(other);
      std::swap(meter_mod_, tmp.meter_mod_);
      bands_.swap(tmp.bands_);
      return *this;
    }

    static auto equivalent_flag(
        std::uint16_t const lhs, std::uint16_t const rhs) noexcept
      -> bool
    {
      constexpr auto OFPMF_PKTPS
        = std::uint16_t{ofp::v13::protocol::OFPMF_PKTPS};
      constexpr auto rate_bit_mask
        = std::uint16_t{ofp::v13::protocol::OFPMF_KBPS | OFPMF_PKTPS};
      return (lhs & OFPMF_PKTPS) == (rhs & OFPMF_PKTPS)
          && (lhs & ~rate_bit_mask) == (rhs & ~rate_bit_mask);
    }

  private:
    auto derived() const noexcept
      -> MeterMod const&
    {
      return *static_cast<MeterMod const*>(this);
    }

    friend base_t;

    static constexpr bool is_fixed_length_message = false;

    friend typename base_t::basic_protocol_type;

    template <class Validator>
    void validate_impl(Validator const& validator) const
    {
      derived().validate_meter_mod(validator);
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      ofp::detail::encode(container, meter_mod_);
      bands_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> MeterMod
    {
      auto const meter_mod = ofp::detail::decode<raw_ofp_type>(first, last);
      auto const bands_length = meter_mod.header.length - sizeof(raw_ofp_type);
      last = std::next(first, bands_length);
      auto bands = bands_type::decode(first, last);
      return MeterMod{meter_mod, std::move(bands)};
    }

    auto equal_impl(MeterMod const& rhs) const noexcept
      -> bool
    {
      return ofp::detail::memcmp(meter_mod_, rhs.meter_mod_)
          && bands_ == rhs.bands_;
    }

    auto equivalent_impl(MeterMod const& rhs) const
      -> bool
    {
      return this->xid() == rhs.xid()
          && derived().is_equivalent_meter_mod(rhs);
    }

  private:
    raw_ofp_type meter_mod_;
    bands_type bands_;
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_METER_MOD_BASE_HPP
