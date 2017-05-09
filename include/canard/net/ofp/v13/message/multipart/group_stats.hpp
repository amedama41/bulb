#ifndef CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_STATS_HPP
#define CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_STATS_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/detail/basic_multipart.hpp>
#include <canard/net/ofp/v13/exception.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

  class bucket_counter
    : public detail::basic_protocol_type<bucket_counter>
  {
  public:
    using ofp_type = protocol::ofp_bucket_counter;

    bucket_counter(
          std::uint64_t const packet_count
        , std::uint64_t const byte_count) noexcept
      : bucket_counter_{packet_count, byte_count}
    {
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return sizeof(ofp_type);
    }

    auto packet_count() const noexcept
      -> std::uint64_t
    {
      return bucket_counter_.packet_count;
    }

    auto byte_count() const noexcept
      -> std::uint64_t
    {
      return bucket_counter_.byte_count;
    }

  private:
    explicit bucket_counter(ofp_type const& bucket_counter) noexcept
      : bucket_counter_(bucket_counter)
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, bucket_counter_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> bucket_counter
    {
      return bucket_counter{detail::decode<ofp_type>(first, last)};
    }

    auto equal_impl(bucket_counter const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(bucket_counter_, rhs.bucket_counter_);
    }

    auto equivalent_impl(bucket_counter const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(bucket_counter_, rhs.bucket_counter_);
    }

  private:
    ofp_type bucket_counter_;
  };

  class group_stats
    : public detail::basic_protocol_type<group_stats>
  {
  public:
    using ofp_type = protocol::ofp_group_stats;
    using bucket_stats_type = ofp::list<bucket_counter>;

    group_stats(
          std::uint32_t const group_id
        , std::uint32_t const ref_count
        , std::uint64_t const packet_count, std::uint64_t const byte_count
        , std::uint32_t const duration_sec, std::uint32_t const duration_nsec
        , bucket_stats_type bucket_stats)
      : group_stats_{
            bucket_stats.calc_ofp_length(sizeof(ofp_type))
          , { 0, 0 }
          , group_id
          , ref_count
          , { 0, 0, 0, 0 }
          , packet_count
          , byte_count
          , duration_sec
          , duration_nsec
        }
      , bucket_stats_(std::move(bucket_stats))
    {
    }

    group_stats(group_stats const&) = default;

    group_stats(group_stats&& other) noexcept
      : group_stats_(other.group_stats_)
      , bucket_stats_(other.extract_bucket_stats())
    {
    }

    auto operator=(group_stats const& other)
      -> group_stats&
    {
      return operator=(group_stats{other});
    }

    auto operator=(group_stats&& other)
      -> group_stats&
    {
      auto tmp = std::move(other);
      swap(tmp);
      return *this;
    }

    void swap(group_stats& other) noexcept
    {
      std::swap(group_stats_, other.group_stats_);
      bucket_stats_.swap(other.bucket_stats_);
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return group_stats_.length;
    }

    auto group_id() const noexcept
      -> std::uint32_t
    {
      return group_stats_.group_id;
    }

    auto ref_count() const noexcept
      -> std::uint32_t
    {
      return group_stats_.ref_count;
    }

    auto packet_count() const noexcept
      -> std::uint64_t
    {
      return group_stats_.packet_count;
    }

    auto byte_count() const noexcept
      -> std::uint64_t
    {
      return group_stats_.byte_count;
    }

    auto duration_sec() const noexcept
      -> std::uint32_t
    {
      return group_stats_.duration_sec;
    }

    auto duration_nsec() const noexcept
      -> std::uint32_t
    {
      return group_stats_.duration_nsec;
    }

    auto bucket_stats() const noexcept
      -> bucket_stats_type const&
    {
      return bucket_stats_;
    }

    auto extract_bucket_stats() noexcept
      -> bucket_stats_type
    {
      auto bucket_stats = bucket_stats_type{};
      bucket_stats.swap(bucket_stats_);
      group_stats_.length = sizeof(ofp_type);
      return bucket_stats;
    }

  private:
    group_stats(
        ofp_type const& group_stats, bucket_stats_type&& bucket_stats) noexcept
      : group_stats_(group_stats)
      , bucket_stats_(std::move(bucket_stats))
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, group_stats_);
      bucket_stats_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> group_stats
    {
      auto const stats = detail::decode<ofp_type>(first, last);
      if (stats.length < sizeof(ofp_type)) {
        throw v13::exception{
            v13::exception::ex_error_type::bad_multipart_element
          , v13::exception::ex_error_code::bad_length
          , "too small group_stats length"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      auto const bucket_stats_length = stats.length - sizeof(ofp_type);
      if (std::distance(first, last) < bucket_stats_length) {
        throw v13::exception{
            protocol::bad_request_code::bad_len
          , "too small data size for group_stats"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      last = std::next(first, bucket_stats_length);

      auto bucket_stats = bucket_stats_type::decode(first, last);

      return group_stats{stats, std::move(bucket_stats)};
    }

    auto equal_impl(group_stats const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(group_stats_, rhs.group_stats_)
          && bucket_stats_ == rhs.bucket_stats_;
    }

  private:
    ofp_type group_stats_;
    bucket_stats_type bucket_stats_;
  };

  class group_stats_request
    : public multipart_detail::basic_multipart_request<
        group_stats_request, protocol::ofp_group_stats_request
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_GROUP;

    group_stats_request(
          std::uint32_t const group_id
        , std::uint32_t const xid = get_xid()) noexcept
      : basic_multipart_request{0, {group_id, { 0, 0, 0, 0 }}, xid}
    {
    }

    auto group_id() const noexcept
      -> std::uint32_t
    {
      return body().group_id;
    }

  private:
    friend basic_multipart_request::base_type;

    group_stats_request(
          ofp_type const& multipart_request
        , body_type const& group_stats_request) noexcept
      : basic_multipart_request{multipart_request, group_stats_request}
    {
    }
  };

  class group_stats_reply
    : public multipart_detail::basic_multipart_reply<
        group_stats_reply, group_stats[]
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_GROUP;

    explicit group_stats_reply(
          body_type group_stats
        , std::uint16_t const flags = 0
        , std::uint32_t const xid = get_xid())
      : basic_multipart_reply{flags, std::move(group_stats), xid}
    {
    }

  private:
    friend basic_multipart_reply::base_type;

    static constexpr bool is_fixed_length_element = false;

    group_stats_reply(
        ofp_type const& multipart_reply, body_type&& group_stats)
      : basic_multipart_reply{multipart_reply, std::move(group_stats)}
    {
    }
  };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_STATS_HPP
