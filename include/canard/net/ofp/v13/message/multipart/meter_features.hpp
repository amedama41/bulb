#ifndef CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_FEATURES_HPP
#define CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_FEATURES_HPP

#include <cstdint>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/detail/basic_multipart.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

  class meter_features_request
    : public multipart_detail::basic_multipart_request<
        meter_features_request, void
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_METER_FEATURES;

    explicit meter_features_request(
        std::uint32_t const xid = get_xid()) noexcept
      : basic_multipart_request{0, xid}
    {
    }

  private:
    friend basic_multipart_request::base_type;

    explicit meter_features_request(ofp_type const& multipart_request) noexcept
      : basic_multipart_request{multipart_request}
    {
    }
  };

  class meter_features_reply
    : public multipart_detail::basic_multipart_reply<
        meter_features_reply, protocol::ofp_meter_features
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_METER_FEATURES;

    meter_features_reply(
          std::uint32_t const max_meter
        , std::uint32_t const band_types
        , std::uint32_t const capabilities
        , std::uint8_t const max_bands
        , std::uint8_t const max_color
        , std::uint32_t const xid = get_xid()) noexcept
      : basic_multipart_reply{
            0
          , body_type{
                max_meter
              , band_types
              , capabilities
              , max_bands
              , max_color
              , { 0, 0 }
            }
          , xid
        }
    {
    }

    auto max_meter() const noexcept
      -> std::uint32_t
    {
      return body().max_meter;
    }

    auto band_types() const noexcept
      -> std::uint32_t
    {
      return body().band_types;
    }

    auto capabilities() const noexcept
      -> std::uint32_t
    {
      return body().capabilities;
    }

    auto max_bands() const noexcept
      -> std::uint8_t
    {
      return body().max_bands;
    }

    auto max_color() const noexcept
      -> std::uint8_t
    {
      return body().max_color;
    }

  private:
    friend basic_multipart_reply::base_type;

    meter_features_reply(
          ofp_type const& multipart_reply
        , body_type const& meter_features) noexcept
      : basic_multipart_reply{multipart_reply, meter_features}
    {
    }
  };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_FEATURES_HPP
