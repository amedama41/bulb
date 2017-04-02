#ifndef CANARD_NET_OFP_V13_MESSAGES_BASIC_MULTIPART_HPP
#define CANARD_NET_OFP_V13_MESSAGES_BASIC_MULTIPART_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/common/oxm_match.hpp>
#include <canard/net/ofp/v13/detail/basic_message.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/detail/length_utility.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {
namespace multipart_detail {

  template <class MultipartType>
  struct multipart_message_type;

  template <>
  struct multipart_message_type<protocol::ofp_multipart_request>
  {
    static constexpr protocol::ofp_type value
      = protocol::OFPT_MULTIPART_REQUEST;
  };

  template <>
  struct multipart_message_type<protocol::ofp_multipart_reply>
  {
    static constexpr protocol::ofp_type value
      = protocol::OFPT_MULTIPART_REPLY;
  };

  template <class T, class MultipartType>
  class basic_multipart
    : public detail::v13::basic_message<T>
  {
    using base_t = detail::v13::basic_message<T>;

  public:
    static constexpr protocol::ofp_type message_type
      = multipart_message_type<MultipartType>::value;

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return static_cast<T const*>(this)->multipart().header;
    }

    static constexpr auto multipart_type() noexcept
      -> protocol::ofp_multipart_type
    {
      return T::multipart_type_value;
    }

    auto flags() const noexcept
      -> std::uint16_t
    {
      return static_cast<T const*>(this)->multipart().flags;
    }

    using base_t::validate_header;

    static auto validate_header(MultipartType const& multipart) noexcept
      -> char const*
    {
      if (multipart.type != multipart_type()) {
        return "invalid multipart type";
      }
      if (!is_valid_multipart_length(multipart)) {
        return "invalid multipart length";
      }
      return nullptr;
    }

    static constexpr auto is_valid_multipart_length(
        MultipartType const& multipart) noexcept
      -> bool
    {
      return T::is_valid_multipart_length_impl(multipart.header.length);
    }

  private:
    friend base_t;

    static constexpr bool is_fixed_length_message = false;

    friend constexpr auto min_message_length(
        detail::v13::basic_message_tag<T>) noexcept
      -> std::uint16_t
    {
      return sizeof(MultipartType);
    }
  };


  template <class T, class MultipartType>
  class empty_body_multipart
    : public basic_multipart<T, MultipartType>
  {
    using base_t = basic_multipart<T, MultipartType>;

  public:
    using raw_ofp_type = MultipartType;

  protected:
    empty_body_multipart(
        std::uint16_t const flags, std::uint32_t const xid) noexcept
      : multipart_{
            protocol::ofp_header{
              base_t::version(), base_t::type(), sizeof(raw_ofp_type), xid
            }
          , T::multipart_type_value
          , flags
          , { 0, 0, 0, 0 }
        }
    {
    }

    explicit empty_body_multipart(raw_ofp_type const& multipart) noexcept
      : multipart_(multipart)
    {
    }

  private:
    friend base_t;

    auto multipart() const noexcept
      -> MultipartType const&
    {
      return multipart_;
    }

    static constexpr auto is_valid_multipart_length_impl(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length == base_t::min_length();
    }

    friend typename base_t::basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, multipart_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      return T{detail::decode<raw_ofp_type>(first, last)};
    }

  private:
    raw_ofp_type multipart_;
  };


  template <class T, class BodyType, class MultipartType>
  class single_element_multipart
    : public basic_multipart<T, MultipartType>
  {
    using base_t = basic_multipart<T, MultipartType>;

  public:
    using raw_ofp_type = MultipartType;
    using body_type = BodyType;

  protected:
    single_element_multipart(
          std::uint16_t const flags
        , body_type const& body
        , std::uint32_t const xid) noexcept
      : multipart_{
            protocol::ofp_header{
                base_t::version()
              , base_t::type()
              , sizeof(raw_ofp_type) + sizeof(body_type)
              , xid
            }
          , T::multipart_type_value
          , flags
          , { 0, 0, 0, 0 }
        }
      , body_(body)
    {
    }

    single_element_multipart(
        raw_ofp_type const& multipart, body_type const& body) noexcept
      : multipart_(multipart)
      , body_(body)
    {
    }

    auto body() const noexcept
      -> body_type const&
    {
      return body_;
    }

  private:
    friend basic_multipart<T, MultipartType>;

    auto multipart() const noexcept
      -> MultipartType const&
    {
      return multipart_;
    }

    static constexpr auto is_valid_multipart_length_impl(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length == base_t::min_length();
    }

    friend typename base_t::basic_protocol_type;

    friend constexpr auto get_min_length(
        detail::basic_protocol_type_tag<T>) noexcept
      -> std::uint16_t
    {
      return sizeof(raw_ofp_type) + sizeof(body_type);
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, multipart_);
      detail::encode(container, body_);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      auto const multipart = detail::decode<raw_ofp_type>(first, last);
      return T{multipart, detail::decode<body_type>(first, last)};
    }

  private:
    raw_ofp_type multipart_;
    body_type body_;
  };


  template <class T, class BodyType, class MultipartType>
  class single_element_with_match_multipart
    : public basic_multipart<T, MultipartType>
  {
    using base_t = basic_multipart<T, MultipartType>;

  public:
    using raw_ofp_type = MultipartType;
    using body_type = BodyType;

    auto match() const noexcept
      -> oxm_match const&
    {
      return match_;
    }

    auto extract_match()
      -> oxm_match
    {
      auto match = std::move(match_);
      multipart_.header.length = base_t::min_length();
      return match;
    }

  protected:
    single_element_with_match_multipart(
          std::uint16_t const flags
        , body_type const& body
        , oxm_match&& match
        , std::uint32_t const xid) noexcept
      : multipart_{
            protocol::ofp_header{
                base_t::version()
              , base_t::type()
              , match.calc_ofp_length(sizeof(raw_ofp_type) + sizeof(body_type))
              , xid
            }
          , T::multipart_type_value
          , flags
          , { 0, 0, 0, 0 }
        }
      , body_(body)
      , match_(std::move(match))
    {
    }

    single_element_with_match_multipart(
          raw_ofp_type const& multipart
        , body_type const& body
        , oxm_match&& match) noexcept
      : multipart_(multipart)
      , body_(body)
      , match_(std::move(match))
      {
      }

    single_element_with_match_multipart(
        single_element_with_match_multipart const&) = default;

    single_element_with_match_multipart(
        single_element_with_match_multipart&& other)
      : multipart_(other.multipart_)
      , body_(other.body_)
      , match_(other.extract_match())
    {
    }

    auto operator=(single_element_with_match_multipart const& other)
      -> single_element_with_match_multipart&
    {
      return operator=(single_element_with_match_multipart{other});
    }

    auto operator=(single_element_with_match_multipart&& other)
      -> single_element_with_match_multipart&
    {
      auto tmp = std::move(other);
      std::swap(multipart_, tmp.multipart_);
      std::swap(body_, tmp.body_);
      match_.swap(tmp.match_);
      return *this;
    }

    auto body() const noexcept
      -> body_type const&
    {
      return body_;
    }

  private:
    friend basic_multipart<T, MultipartType>;

    auto multipart() const noexcept
      -> MultipartType const&
    {
      return multipart_;
    }

    static constexpr auto is_valid_multipart_length_impl(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length >= base_t::min_length();
    }

    friend typename base_t::basic_protocol_type;

    friend constexpr auto get_min_length(
        detail::basic_protocol_type_tag<T>) noexcept
      -> std::uint16_t
    {
      return sizeof(raw_ofp_type)
        + sizeof(body_type) + oxm_match::min_byte_length();
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, multipart_);
      detail::encode(container, body_);
      match_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      auto const multipart = detail::decode<raw_ofp_type>(first, last);
      last = std::next(first, multipart.header.length - sizeof(raw_ofp_type));

      auto const body = detail::decode<body_type>(first, last);

      auto const ofp_match
        = detail::decode_without_consumption<protocol::ofp_match>(first, last);
      oxm_match::validate_header(ofp_match);
      if (std::distance(first, last)
          != v13_detail::exact_length(ofp_match.length)) {
        throw exception{
            protocol::bad_request_code::bad_len
          , "too small data size for oxm_match"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      auto match = oxm_match::decode(first, last);

      return T{multipart, body, std::move(match)};
    }

  private:
    raw_ofp_type multipart_;
    body_type body_;
    oxm_match match_;
  };


  template <class T, class BodyType, class MultipartType>
  class array_body_multipart
    : public basic_multipart<T, MultipartType>
  {
    using base_t = basic_multipart<T, MultipartType>;
    using elem_type = typename std::remove_all_extents<BodyType>::type;

  public:
    using raw_ofp_type = MultipartType;
    using body_type = ofp::list<elem_type>;
    using iterator = typename body_type::iterator;
    using const_iterator = typename body_type::const_iterator;

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
      multipart_.header.length = base_t::min_length();
      return body;
    }

    auto size() const noexcept
      -> std::size_t
    {
      return body_.size();
    }

    auto begin() noexcept
      -> iterator
    {
      return body_.begin();
    }

    auto begin() const noexcept
      -> const_iterator
    {
      return body_.begin();
    }

    auto end() noexcept
      -> iterator
    {
      return body_.end();
    }

    auto end() const noexcept
      -> const_iterator
    {
      return body_.end();
    }

  protected:
    array_body_multipart(
        std::uint16_t const flags, body_type&& body, std::uint32_t const xid)
      : multipart_{
            protocol::ofp_header{
                base_t::version()
              , base_t::type()
              , body.calc_ofp_length(sizeof(raw_ofp_type))
              , xid
            }
          , T::multipart_type_value
          , flags
          , { 0, 0, 0, 0 }
        }
      , body_(std::move(body))
    {
    }

    array_body_multipart(raw_ofp_type const& multipart, body_type&& body)
      : multipart_(multipart)
      , body_(std::move(body))
    {
    }

    array_body_multipart(array_body_multipart const&) = default;

    array_body_multipart(array_body_multipart&& other)
      : multipart_(other.multipart_)
      , body_(other.extract_body())
    {
    }

    auto operator=(array_body_multipart const& other)
      -> array_body_multipart&
    {
      return operator=(array_body_multipart{other});
    }

    auto operator=(array_body_multipart&& other)
      -> array_body_multipart&
    {
      auto tmp = std::move(other);
      std::swap(multipart_, tmp.multipart_);
      body_.swap(tmp.body_);
      return *this;
    }

  private:
    static constexpr auto is_valid_body_length(
        std::uint16_t const length) noexcept
      -> bool
    {
      return T::is_fixed_length_element
        ? length % elem_type::min_length() == 0
        : true;
    }

    friend basic_multipart<T, MultipartType>;

    auto multipart() const noexcept
      -> MultipartType const&
    {
      return multipart_;
    }

    static constexpr auto is_valid_multipart_length_impl(
        std::uint16_t const length) noexcept
      -> bool
    {
      return length >= base_t::min_length()
          && is_valid_body_length(length - base_t::min_length());
    }

    friend typename base_t::basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, multipart_);
      body_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      auto const multipart = detail::decode<raw_ofp_type>(first, last);
      auto const body_length = multipart.header.length - sizeof(raw_ofp_type);
      last = std::next(first, body_length);

      auto body = body_type::decode(first, last);

      return T{multipart, std::move(body)};
    }

  private:
    raw_ofp_type multipart_;
    body_type body_;
  };


  template <class MultipartType, class T, class BodyType, bool HasMatch>
  using basic_multipart_t = typename std::conditional<
      std::is_same<BodyType, void>::value
    , empty_body_multipart<T, MultipartType>
    , typename std::conditional<
          std::is_array<BodyType>::value
        , array_body_multipart<T, BodyType, MultipartType>
        , typename std::conditional<
              HasMatch
            , single_element_with_match_multipart<T, BodyType, MultipartType>
            , single_element_multipart<T, BodyType, MultipartType>
          >::type
      >::type
  >::type;

  template <class T, class BodyType, bool HasMatch = false>
  class basic_multipart_request
  : public basic_multipart_t<
      protocol::ofp_multipart_request, T, BodyType, HasMatch
    >
  {
  protected:
    using base_type = basic_multipart_t<
      protocol::ofp_multipart_request, T, BodyType, HasMatch
    >;

  public:
    using base_type::base_type;
  };


  template <class T, class BodyType, bool HasMatch = false>
  class basic_multipart_reply
  : public basic_multipart_t<
      protocol::ofp_multipart_reply, T, BodyType, HasMatch
    >
  {
  protected:
    using base_type = basic_multipart_t<
      protocol::ofp_multipart_reply, T, BodyType, HasMatch
    >;

  public:
    using base_type::base_type;
  };

} // namespace multipart_detail
} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_BASIC_MULTIPART_HPP
