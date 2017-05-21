#ifndef CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_STATS_HPP
#define CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_STATS_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/detail/basic_multipart.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/exception.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

  class meter_band_stats
    : public detail::basic_protocol_type<meter_band_stats>
  {
  public:
    using ofp_type = protocol::ofp_meter_band_stats;

    meter_band_stats(
          std::uint64_t const packet_band_count
        , std::uint64_t const byte_band_count) noexcept
      : meter_band_stats_{packet_band_count, byte_band_count}
    {
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return sizeof(ofp_type);
    }

    auto packet_band_count() const noexcept
      -> std::uint64_t
    {
      return meter_band_stats_.packet_band_count;
    }

    auto byte_band_count() const noexcept
      -> std::uint64_t
    {
      return meter_band_stats_.byte_band_count;
    }

  private:
    explicit meter_band_stats(ofp_type const& meter_band_stats) noexcept
      : meter_band_stats_(meter_band_stats)
    {
    }

    friend basic_protocol_type;

    template <class Validator>
    void validate_impl(Validator&&) const noexcept
    {
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, meter_band_stats_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> meter_band_stats
    {
      return meter_band_stats{detail::decode<ofp_type>(first, last)};
    }

    auto equal_impl(meter_band_stats const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(meter_band_stats_, rhs.meter_band_stats_);
    }

    auto equivalent_impl(meter_band_stats const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(meter_band_stats_, rhs.meter_band_stats_);
    }

  private:
    ofp_type meter_band_stats_;
  };

  class meter_stats
    : public detail::basic_protocol_type<meter_stats>
  {
  public:
    using ofp_type = protocol::ofp_meter_stats;
    using band_stats_type = ofp::list<meter_band_stats>;

    meter_stats(
          std::uint32_t const meter_id
        , std::uint32_t const flow_count
        , std::uint64_t const packet_in_count, std::uint64_t const byte_in_count
        , std::uint32_t const duration_sec, std::uint32_t const duration_nsec
        , band_stats_type band_stats)
      : meter_stats_{
            meter_id
          , band_stats.calc_ofp_length(sizeof(ofp_type))
          , { 0, 0, 0, 0, 0, 0 }
          , flow_count
          , packet_in_count
          , byte_in_count
          , duration_sec
          , duration_nsec
        }
      , band_stats_(std::move(band_stats))
    {
    }

    meter_stats(meter_stats const&) = default;

    meter_stats(meter_stats&& other) noexcept
      : meter_stats_(other.meter_stats_)
      , band_stats_(other.extract_band_stats())
    {
    }

    auto operator=(meter_stats const& other)
      -> meter_stats&
    {
      return operator=(meter_stats{other});
    }

    auto operator=(meter_stats&& other) noexcept
      -> meter_stats&
    {
      auto tmp = std::move(other);
      swap(tmp);
      return *this;
    }

    void swap(meter_stats& other) noexcept
    {
      std::swap(meter_stats_, other.meter_stats_);
      band_stats_.swap(other.band_stats_);
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return meter_stats_.len;
    }

    auto meter_id() const noexcept
      -> std::uint32_t
    {
      return meter_stats_.meter_id;
    }

    auto flow_count() const noexcept
      -> std::uint32_t
    {
      return meter_stats_.flow_count;
    }

    auto packet_in_count() const noexcept
      -> std::uint64_t
    {
      return meter_stats_.packet_in_count;
    }

    auto byte_in_count() const noexcept
      -> std::uint64_t
    {
      return meter_stats_.byte_in_count;
    }

    auto duration_sec() const noexcept
      -> std::uint32_t
    {
      return meter_stats_.duration_sec;
    }

    auto duration_nsec() const noexcept
      -> std::uint32_t
    {
      return meter_stats_.duration_nsec;
    }

    auto band_stats() const noexcept
      -> band_stats_type const&
    {
      return band_stats_;
    }

    auto extract_band_stats() noexcept
      -> band_stats_type
    {
      auto band_stats = band_stats_type{};
      band_stats.swap(band_stats_);
      meter_stats_.len = sizeof(ofp_type);
      return band_stats;
    }

  private:
    meter_stats(
        ofp_type const& meter_stats, band_stats_type&& band_stats) noexcept
      : meter_stats_(meter_stats)
      , band_stats_(std::move(band_stats))
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, meter_stats_);
      band_stats_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> meter_stats
    {
      auto const stats = detail::decode<ofp_type>(first, last);
      if (stats.len < sizeof(ofp_type)) {
        throw v13::exception{
            v13::exception::ex_error_type::bad_multipart_element
          , v13::exception::ex_error_code::bad_length
          , "too small meter_stats length"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      auto const band_stats_length
        = std::uint16_t(stats.len - sizeof(ofp_type));
      if (std::distance(first, last) < band_stats_length) {
        throw v13::exception{
            protocol::bad_request_code::bad_len
          , "too small data size for meter_stats"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      last = std::next(first, band_stats_length);

      auto band_stats = band_stats_type::decode(first, last);
      return meter_stats{stats, std::move(band_stats)};
    }

    auto equal_impl(meter_stats const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(meter_stats_, rhs.meter_stats_)
          && band_stats_ == rhs.band_stats_;
    }

  private:
    ofp_type meter_stats_;
    band_stats_type band_stats_;
  };

  class meter_stats_request
    : public multipart_detail::basic_multipart_request<
        meter_stats_request, protocol::ofp_meter_multipart_request
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_METER;

    meter_stats_request(
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

    meter_stats_request(
          ofp_type const& multipart_request
        , body_type const& meter_multipart_request) noexcept
      : basic_multipart_request{multipart_request, meter_multipart_request}
    {
    }
  };

  class meter_stats_reply
    : public multipart_detail::basic_multipart_reply<
        meter_stats_reply, meter_stats[]
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_METER;

    meter_stats_reply(
          body_type meter_stats
        , std::uint16_t const flags = 0
        , std::uint32_t const xid = get_xid())
      : basic_multipart_reply{flags, std::move(meter_stats), xid}
    {
    }

  private:
    friend basic_multipart_reply::base_type;

    static constexpr bool is_fixed_length_element = false;

    meter_stats_reply(
        ofp_type const& multipart_reply, body_type&& meter_stats) noexcept
      : basic_multipart_reply{multipart_reply, std::move(meter_stats)}
    {
    }
  };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGE_MULTIPART_METER_STATS_HPP
