#ifndef CANARD_NET_OFP_V13_MESSAGES_METER_MOD_HPP
#define CANARD_NET_OFP_V13_MESSAGES_METER_MOD_HPP

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/detail/meter_mod_base.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/utility/meter_band_set.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  class meter_add
    : public detail::v13::meter_mod_base<meter_add>
  {
    static constexpr protocol::ofp_meter_mod_command command_type
      = protocol::OFPMC_ADD;

  public:
    meter_add(
          std::uint32_t const meter_id
        , std::uint16_t const flags, bands_type bands
        , std::uint32_t const xid = get_xid())
      : meter_mod_base{meter_id, flags, std::move(bands), xid}
    {
    }

  private:
    friend meter_mod_base;

    meter_add(raw_ofp_type const& meter_mod, bands_type&& bands)
      : meter_mod_base{meter_mod, std::move(bands)}
    {
    }

    template <class Validator>
    void validate_meter_mod(Validator const& validator) const
    {
      if (meter_id() == 0 || meter_id() == protocol::OFPM_ALL) {
        throw std::runtime_error{"invalid meter_id"};
      }
      if (!(flags() & protocol::OFPMF_BURST)) {
        bands().validate([](bands_type::const_reference e) {
            if (e.burst_size() != 0) {
              throw std::runtime_error{"burst_size must be zero"};
            }
        });
      }
      validator(bands());
    }

    auto is_equivalent_meter_mod(meter_add const& rhs) const
      -> bool
    {
      return meter_id() == rhs.meter_id()
          && equivalent_flag(flags(), rhs.flags())
          && meter_band_set::equivalent_as_set(bands(), rhs.bands());
    }
  };

  class meter_modify
    : public detail::v13::meter_mod_base<meter_modify>
  {
    static constexpr protocol::ofp_meter_mod_command command_type
      = protocol::OFPMC_ADD;

  public:
    meter_modify(
          std::uint32_t const meter_id
        , std::uint16_t const flags, bands_type bands
        , std::uint32_t const xid = get_xid())
      : meter_mod_base{meter_id, flags, std::move(bands), xid}
    {
    }

  private:
    friend meter_mod_base;

    meter_modify(raw_ofp_type const& meter_mod, bands_type&& bands)
      : meter_mod_base{meter_mod, std::move(bands)}
    {
    }

    template <class Validator>
    void validate_meter_mod(Validator const& validator) const
    {
      if (meter_id() == 0 || meter_id() == protocol::OFPM_ALL) {
        throw std::runtime_error{"invalid meter_id"};
      }
      if (!(flags() & protocol::OFPMF_BURST)) {
        bands().validate([](bands_type::const_reference e) {
            if (e.burst_size() != 0) {
              throw std::runtime_error{"burst_size must be zero"};
            }
        });
      }
      validator(bands());
    }

    auto is_equivalent_meter_mod(meter_modify const& rhs) const
      -> bool
    {
      return meter_id() == rhs.meter_id()
          && equivalent_flag(flags(), rhs.flags())
          && meter_band_set::equivalent_as_set(bands(), rhs.bands());
    }
  };

  class meter_delete
    : public detail::v13::meter_mod_base<meter_delete>
  {
    static constexpr protocol::ofp_meter_mod_command command_type
      = protocol::OFPMC_DELETE;

  public:
    explicit meter_delete(
        std::uint32_t const meter_id, std::uint32_t const xid = get_xid())
      : meter_mod_base{meter_id, xid}
    {
    }

  private:
    friend meter_mod_base;

    meter_delete(raw_ofp_type const& meter_mod, bands_type&& bands)
      : meter_mod_base{meter_mod, std::move(bands)}
    {
    }

    template <class Validator>
    void validate_meter_mod(Validator const& validator) const
    {
      if (meter_id() == 0) {
        throw std::runtime_error{"invalid meter_id"};
      }
    }

    auto is_equivalent_meter_mod(meter_delete const& rhs) const
      -> bool
    {
      return meter_id() == rhs.meter_id();
    }
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_METER_MOD_HPP
