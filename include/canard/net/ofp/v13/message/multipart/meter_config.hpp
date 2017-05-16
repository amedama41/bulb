#ifndef CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_CONFIG_HPP
#define CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_CONFIG_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/any_meter_band.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/detail/basic_multipart.hpp>
#include <canard/net/ofp/v13/exception.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

  class meter_config
    : public detail::basic_protocol_type<meter_config>
  {
  public:
    using ofp_type = protocol::ofp_meter_config;
    using bands_type = ofp::list<v13::any_meter_band>;

    meter_config(
          std::uint32_t const meter_id
        , std::uint16_t const flags
        , bands_type bands)
      : meter_config_{
            bands.calc_ofp_length(sizeof(ofp_type))
          , flags
          , meter_id
        }
      , bands_(std::move(bands))
    {
    }

    meter_config(meter_config const&) = default;

    meter_config(meter_config&& other) noexcept
      : meter_config_(other.meter_config_)
      , bands_(other.extract_bands())
    {
    }

    auto operator=(meter_config const& other)
      -> meter_config&
    {
      return operator=(meter_config{other});
    }

    auto operator=(meter_config&& other)
      -> meter_config&
    {
      auto tmp = std::move(other);
      swap(tmp);
      return *this;
    }

    void swap(meter_config& other)
    {
      std::swap(meter_config_, other.meter_config_);
      bands_.swap(other.bands_);
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return meter_config_.length;
    }

    auto meter_id() const noexcept
      -> std::uint32_t
    {
      return meter_config_.meter_id;
    }

    auto flags() const noexcept
      -> std::uint16_t
    {
      return meter_config_.flags;
    }

    auto bands() const noexcept
      -> bands_type const&
    {
      return bands_;
    }

    auto extract_bands() noexcept
      -> bands_type
    {
      auto bands = bands_type{};
      bands.swap(bands_);
      meter_config_.length = sizeof(ofp_type);
      return bands;
    }

  private:
    meter_config(ofp_type const& meter_config, bands_type&& bands) noexcept
      : meter_config_(meter_config)
      , bands_(std::move(bands))
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container&& container) const
    {
      detail::encode(container, meter_config_);
      bands_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> meter_config
    {
      auto const config = detail::decode<ofp_type>(first, last);
      if (config.length < sizeof(ofp_type)) {
        throw v13::exception{
            v13::exception::ex_error_type::bad_multipart_element
          , v13::exception::ex_error_code::bad_length
          , "too small meter_config length"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      auto const bands_length = config.length - sizeof(ofp_type);
      if (std::distance(first, last) < bands_length) {
        throw v13::exception{
            protocol::bad_request_code::bad_len
          , "too small data size for meter_config"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      last = std::next(first, bands_length);

      auto bands = bands_type::decode(first, last);

      return meter_config{config, std::move(bands)};
    }

    auto equal_impl(meter_config const& other) const noexcept
      -> bool
    {
      return detail::memcmp(meter_config_, other.meter_config_)
          && bands_ == other.bands_;
    }

  private:
    ofp_type meter_config_;
    bands_type bands_;
  };

  class meter_config_request
    : public multipart_detail::basic_multipart_request<
        meter_config_request, protocol::ofp_meter_multipart_request
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_METER_CONFIG;

    meter_config_request(
          std::uint32_t const meter_id
        , std::uint32_t const xid = get_xid()) noexcept
      : basic_multipart_request{0, body_type{meter_id, { 0, 0, 0, 0 }}, xid}
    {
    }

    auto meter_id() const noexcept
      -> std::uint32_t
    {
      return body().meter_id;
    }

  private:
    friend basic_multipart_request::base_type;

    meter_config_request(
          ofp_type const& multipart_request
        , body_type const& meter_multipart_request) noexcept
      : basic_multipart_request{multipart_request, meter_multipart_request}
    {
    }
  };

  class meter_config_reply
    : public multipart_detail::basic_multipart_reply<
        meter_config_reply, meter_config[]
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_METER_CONFIG;

    meter_config_reply(
          body_type meter_config
        , std::uint16_t const flags = 0
        , std::uint32_t const xid = get_xid())
      : basic_multipart_reply{flags, std::move(meter_config), xid}
    {
    }

  private:
    friend basic_multipart_reply::base_type;

    static constexpr bool is_fixed_length_element = false;

    meter_config_reply(
        ofp_type const& multipart_reply, body_type&& meter_config) noexcept
      : basic_multipart_reply{multipart_reply, std::move(meter_config)}
    {
    }
  };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_CONFIG_HPP
