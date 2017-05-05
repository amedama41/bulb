#ifndef CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_DESCRIPTION_HPP
#define CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_DESCRIPTION_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/common/bucket.hpp>
#include <canard/net/ofp/v13/detail/basic_multipart.hpp>
#include <canard/net/ofp/v13/exception.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

  class group_description
    : public detail::basic_protocol_type<group_description>
  {
  public:
    using ofp_type = protocol::ofp_group_desc;
    using buckets_type = ofp::list<v13::bucket>;

    group_description(
        std::uint32_t const group_id
        , std::uint8_t const group_type
        , buckets_type buckets)
      : group_desc_{
            buckets.calc_ofp_length(sizeof(ofp_type))
          , group_type
          , 0
          , group_id
        }
      , buckets_(std::move(buckets))
    {
    }

    group_description(group_description const&) = default;

    group_description(group_description&& other)
      : group_desc_(other.group_desc_)
      , buckets_(other.extract_buckets())
    {
    }

    auto operator=(group_description const& other)
      -> group_description&
    {
      return operator=(group_description{other});
    }

    auto operator=(group_description&& other)
      -> group_description&
    {
      auto tmp = std::move(other);
      swap(tmp);
      return *this;
    }

    void swap(group_description& other)
    {
      std::swap(group_desc_, other.group_desc_);
      buckets_.swap(other.buckets_);
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return group_desc_.length;
    }

    auto group_id() const noexcept
      -> std::uint32_t
    {
      return group_desc_.group_id;
    }

    auto group_type() const noexcept
      -> std::uint8_t
    {
      return group_desc_.type;
    }

    auto buckets() const noexcept
      -> buckets_type const&
    {
      return buckets_;
    }

    auto extract_buckets()
      -> buckets_type
    {
      auto buckets = buckets_type{};
      buckets.swap(buckets_);
      group_desc_.length = sizeof(ofp_type);
      return buckets;
    }

  private:
    group_description(ofp_type const& group_desc, buckets_type&& buckets)
      : group_desc_(group_desc)
      , buckets_(std::move(buckets))
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, group_desc_);
      buckets_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> group_description
    {
      auto const group_desc = detail::decode<ofp_type>(first, last);
      if (group_desc.length < sizeof(ofp_type)) {
        throw v13::exception{
            v13::exception::ex_error_type::bad_multipart_element
          , v13::exception::ex_error_code::bad_length
          , "too small group_description length"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      auto const buckets_length = group_desc.length - sizeof(ofp_type);
      if (std::distance(first, last) < buckets_length) {
        throw v13::exception{
            protocol::bad_request_code::bad_len
          , "too small data size for group_description"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      last = std::next(first, buckets_length);

      auto buckets = buckets_type::decode(first, last);

      return group_description{group_desc, std::move(buckets)};
    }

    auto equal_impl(group_description const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(group_desc_, rhs.group_desc_)
          && buckets_ == rhs.buckets_;
    }

  private:
    ofp_type group_desc_;
    buckets_type buckets_;
  };

  class group_description_request
    : public multipart_detail::basic_multipart_request<
        group_description_request, void
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_GROUP_DESC;

    explicit group_description_request(
        std::uint32_t const xid = get_xid()) noexcept
      : basic_multipart_request{0, xid}
    {
    }

  private:
    friend basic_multipart_request::base_type;

    explicit group_description_request(
        ofp_type const& multipart_request) noexcept
      : basic_multipart_request{multipart_request}
    {
    }
  };

  class group_description_reply
    : public multipart_detail::basic_multipart_reply<
      group_description_reply, group_description[]
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_GROUP_DESC;

    explicit group_description_reply(
          body_type group_description
        , std::uint16_t const flags = 0
        , std::uint32_t const xid = get_xid())
      : basic_multipart_reply{flags, std::move(group_description), xid}
    {
    }

  private:
    friend basic_multipart_reply::base_type;

    static constexpr bool is_fixed_length_element = false;

    group_description_reply(
        ofp_type const& multipart_reply, body_type&& group_description)
      : basic_multipart_reply{multipart_reply, std::move(group_description)}
    {
    }
  };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_DESCRIPTION_HPP
