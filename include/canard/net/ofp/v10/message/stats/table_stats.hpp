#ifndef CANARD_NET_OFP_V10_MESSAGES_STATISTICS_TABLE_STATS_HPP
#define CANARD_NET_OFP_V10_MESSAGES_STATISTICS_TABLE_STATS_HPP

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <utility>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/utility/string_ref.hpp>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v10/detail/basic_stats.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {
namespace statistics {

  class table_stats
    : public detail::basic_protocol_type<table_stats>
  {
  public:
    using ofp_type = protocol::ofp_table_stats;

    table_stats(
          std::uint8_t const table_id
        , boost::string_ref name
        , std::uint32_t const wildcards
        , std::uint32_t const max_entries
        , std::uint32_t const active_count
        , std::uint64_t const lookup_count
        , std::uint64_t const matched_count)
      : table_stats_{
            table_id
          , { 0, 0, 0 }
          , { 0 }
          , wildcards
          , max_entries
          , active_count
          , lookup_count
          , matched_count
        }
    {
      auto const name_size
        = std::min(name.size(), sizeof(table_stats_.name) - 1);
      using boost::adaptors::sliced;
      boost::copy(name | sliced(0, name_size), table_stats_.name);
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return sizeof(ofp_type);
    }

    auto table_id() const noexcept
      -> std::uint8_t
    {
      return table_stats_.table_id;
    }

    auto name() const
      -> boost::string_ref
    {
      return table_stats_.name;
    }

    auto wildcards() const noexcept
      -> std::uint32_t
    {
      return table_stats_.wildcards;
    }

    auto max_entries() const noexcept
      -> std::uint32_t
    {
      return table_stats_.max_entries;
    }

    auto active_count() const noexcept
      -> std::uint32_t
    {
      return table_stats_.active_count;
    }

    auto lookup_count() const noexcept
      -> std::uint64_t
    {
      return table_stats_.lookup_count;
    }

    auto matched_count() const noexcept
      -> std::uint64_t
    {
      return table_stats_.matched_count;
    }

  private:
    explicit table_stats(ofp_type const& table_stats) noexcept
      : table_stats_(table_stats)
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, table_stats_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> table_stats
    {
      return table_stats{detail::decode<ofp_type>(first, last)};
    }

    auto equal_impl(table_stats const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(table_stats_, rhs.table_stats_);
    }

  private:
    ofp_type table_stats_;
  };


  class table_stats_request
    : public stats_detail::basic_stats_request<table_stats_request, void>
  {
  public:
    static constexpr protocol::ofp_stats_types stats_type_value
      = protocol::OFPST_TABLE;

    explicit table_stats_request(
        std::uint32_t const xid = get_xid()) noexcept
      : basic_stats_request{0, xid}
    {
    }

  private:
    friend basic_stats_request::base_type;

    explicit table_stats_request(ofp_type const& stats_request) noexcept
      : basic_stats_request{stats_request}
    {
    }
  };


  class table_stats_reply
    : public stats_detail::basic_stats_reply<table_stats_reply, table_stats[]>
  {
  public:
    static constexpr protocol::ofp_stats_types stats_type_value
      = protocol::OFPST_TABLE;

    explicit table_stats_reply(
          body_type table_stats
        , std::uint16_t const flags = 0
        , std::uint32_t const xid = get_xid())
      : basic_stats_reply{flags, std::move(table_stats), xid}
    {
    }

  private:
    friend basic_stats_reply::base_type;

    table_stats_reply(ofp_type const& stats_reply, body_type&& table_stats)
      : basic_stats_reply{stats_reply, std::move(table_stats)}
    {
    }

    static constexpr auto is_valid_stats_body_length(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length % body_type::value_type::min_length() == 0;
    }
  };

} // namespace statistics
} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_STATISTICS_TABLE_STATS_HPP
