#ifndef CANARD_NET_OFP_V10_BAISC_STATS_HPP
#define CANARD_NET_OFP_V10_BAISC_STATS_HPP

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v10/detail/basic_message.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {
namespace statistics {
namespace stats_detail {

  template <class StatsType>
  struct stats_message_type;

  template <>
  struct stats_message_type<protocol::ofp_stats_request>
  {
    static constexpr protocol::ofp_type value = protocol::OFPT_STATS_REQUEST;
  };

  template <>
  struct stats_message_type<protocol::ofp_stats_reply>
  {
    static constexpr protocol::ofp_type value = protocol::OFPT_STATS_REPLY;
  };

  template <class T, class StatsType>
  class basic_stats
    : public v10_detail::basic_message<T>
  {
    using base_t = v10_detail::basic_message<T>;

  public:
    using raw_ofp_type = StatsType;

    static constexpr protocol::ofp_type message_type
      = stats_message_type<raw_ofp_type>::value;

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return static_cast<T const*>(this)->stats().header;
    }

    constexpr static auto stats_type() noexcept
      -> protocol::ofp_stats_types
    {
      return T::stats_type_value;
    }

    auto flags() const noexcept
      -> std::uint16_t
    {
      return static_cast<T const*>(this)->stats().flags;
    }

    using base_t::validate_header;

    static auto validate_header(raw_ofp_type const& stats) noexcept
      -> char const*
    {
      if (stats.type != stats_type()) {
        return "invalid stats type";
      }
      if (!is_valid_stats_length(stats)) {
        return "invalid stats length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_stats_length(
        raw_ofp_type const& stats) noexcept
      -> bool
    {
      return T::is_valid_stats_length_impl(stats.header.length);
    }

  private:
    friend base_t;

    static constexpr bool is_fixed_length_message = false;

    friend constexpr auto min_message_length(
        v10_detail::basic_message_tag<T>) noexcept
      -> std::uint16_t
    {
      return sizeof(raw_ofp_type);
    }
  };


  template <class T, class StatsType>
  class empty_body_stats
    : public basic_stats<T, StatsType>
  {
    using base_t = basic_stats<T, StatsType>;

  public:
    using raw_ofp_type = typename base_t::raw_ofp_type;

  protected:
    empty_body_stats(
        std::uint16_t const flags, std::uint32_t const xid) noexcept
      : stats_{
            protocol::ofp_header{
              protocol::OFP_VERSION, T::message_type, sizeof(raw_ofp_type), xid
            }
          , T::stats_type_value
          , flags
        }
    {
    }

    explicit empty_body_stats(raw_ofp_type const& stats) noexcept
      : stats_(stats)
    {
    }

  private:
    friend typename base_t::basic_message::basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, stats_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      return T{detail::decode<raw_ofp_type>(first, last)};
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(stats_, rhs.stats_);
    }

    friend base_t;

    auto stats() const noexcept
      -> raw_ofp_type const&
    {
      return stats_;
    }

    static constexpr auto is_valid_stats_length_impl(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length == sizeof(raw_ofp_type);
    }

  private:
    raw_ofp_type stats_;
  };


  template <class T, class BodyType, class StatsType>
  class single_element_stats
    : public basic_stats<T, StatsType>
  {
    using base_t = basic_stats<T, StatsType>;

  public:
    using raw_ofp_type = typename base_t::raw_ofp_type;
    using raw_ofp_stats_type = BodyType;

  protected:
    single_element_stats(
          std::uint16_t const flags
        , raw_ofp_stats_type const& body
        , std::uint32_t const xid) noexcept
      : stats_{
            protocol::ofp_header{
                protocol::OFP_VERSION
              , T::message_type
              , sizeof(raw_ofp_type) + sizeof(raw_ofp_stats_type)
              , xid
            }
          , T::stats_type_value
          , flags
        }
      , body_(body)
    {
    }

    single_element_stats(
        raw_ofp_type const& stats, raw_ofp_stats_type const& body) noexcept
      : stats_(stats)
      , body_(body)
    {
    }

    auto body() const noexcept
      -> raw_ofp_stats_type const&
    {
      return body_;
    }

  private:
    friend typename base_t::basic_message::basic_protocol_type;

    friend constexpr auto get_min_length(
        detail::basic_protocol_type_tag<T>) noexcept
      -> std::uint16_t
    {
      return sizeof(raw_ofp_type) + sizeof(raw_ofp_stats_type);
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, stats_);
      detail::encode(container, body_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      auto const stats = detail::decode<raw_ofp_type>(first, last);
      return T{stats, detail::decode<raw_ofp_stats_type>(first, last)};
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(stats_, rhs.stats_)
          && detail::memcmp(body_, rhs.body_);
    }

    friend base_t;

    auto stats() const noexcept
      -> StatsType const&
    {
      return stats_;
    }

    static constexpr auto is_valid_stats_length_impl(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length == sizeof(raw_ofp_type) + sizeof(raw_ofp_stats_type);
    }

  private:
    raw_ofp_type stats_;
    raw_ofp_stats_type body_;
  };


  template <class T, class BodyType, class StatsType>
  class array_body_stats
    : public basic_stats<T, StatsType>
  {
    using base_t = basic_stats<T, StatsType>;
    using elem_type = typename std::remove_all_extents<BodyType>::type;

  public:
    using raw_ofp_type = typename base_t::raw_ofp_type;
    using body_type = ofp::list<elem_type>;

    auto body() const noexcept
      -> body_type const&
    {
      return body_;
    }

    auto extract_body()
      -> body_type
    {
      auto body = body_type{};
      body.swap(body_);
      stats_.header.length = sizeof(raw_ofp_type);
      return body;
    }

  protected:
    array_body_stats(
          std::uint16_t const flags
        , body_type&& body
        , std::uint32_t const xid)
      : stats_{
            protocol::ofp_header{
                protocol::OFP_VERSION
              , T::message_type
              , body.calc_ofp_length(sizeof(raw_ofp_type))
              , xid
            }
          , T::stats_type_value
          , flags
        }
      , body_(std::move(body))
    {
    }

    array_body_stats(raw_ofp_type const& stats, body_type&& body)
      : stats_(stats)
      , body_(std::move(body))
    {
    }

    array_body_stats(array_body_stats const&) = default;

    array_body_stats(array_body_stats&& other)
      : stats_(other.stats_)
      , body_(other.extract_body())
    {
    }

    auto operator=(array_body_stats const& other)
      -> array_body_stats&
    {
      return operator=(array_body_stats{other});
    }

    auto operator=(array_body_stats&& other)
      -> array_body_stats&
    {
      auto tmp = std::move(other);
      std::swap(stats_, tmp.stats_);
      body_.swap(tmp.body_);
      return *this;
    }

  private:
    friend typename base_t::basic_message::basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, stats_);
      body_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      auto const stats = detail::decode<raw_ofp_type>(first, last);
      auto const body_length = stats.header.length - sizeof(raw_ofp_type);
      last = std::next(first, body_length);

      auto body = body_type::decode(first, last);

      return T{stats, std::move(body)};
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(stats_, rhs.stats_)
          && body_ == rhs.body_;
    }

    friend base_t;

    auto stats() const noexcept
      -> raw_ofp_type const&
    {
      return stats_;
    }

    static constexpr auto is_valid_stats_length_impl(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length >= sizeof(raw_ofp_type)
          && T::is_valid_stats_body_length(length - sizeof(raw_ofp_type));
    }

  private:
    raw_ofp_type stats_;
    body_type body_;
  };


  template <class StatsType, class T, class BodyType>
  using basic_stats_t = typename std::conditional<
      std::is_same<BodyType, void>::value
    , empty_body_stats<T, StatsType>
    , typename std::conditional<
          std::is_array<BodyType>::value
        , array_body_stats<T, BodyType, StatsType>
        , single_element_stats<T, BodyType, StatsType>
      >::type
  >::type;


  template <class T, class BodyType>
  class basic_stats_request
    : public basic_stats_t<protocol::ofp_stats_request, T, BodyType>
  {
  protected:
    using base_type = basic_stats_t<protocol::ofp_stats_request, T, BodyType>;

    using base_type::base_type;
  };


  template <class T, class BodyType>
  class basic_stats_reply
    : public basic_stats_t<protocol::ofp_stats_reply, T, BodyType>
  {
  protected:
    using base_type = basic_stats_t<protocol::ofp_stats_reply, T, BodyType>;

    using base_type::base_type;
  };

} // namespace stats_detail
} // namespace statistics
} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_BAISC_STATS_HPP
