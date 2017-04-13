#ifndef CANARD_NET_OFP_V13_MESSAGES_ERROR_HPP
#define CANARD_NET_OFP_V13_MESSAGES_ERROR_HPP

#include <cstdint>
#include <iterator>
#include <limits>
#include <utility>
#include <type_traits>
#include <boost/utility/string_ref.hpp>
#include <canard/net/ofp/data_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/type_traits/is_message.hpp>
#include <canard/net/ofp/v13/detail/basic_message.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/error_code.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  class error
    : public detail::v13::basic_message<error>
  {
  public:
    using raw_ofp_type = protocol::ofp_error_msg;
    using data_type = ofp::data_type;

    static constexpr protocol::ofp_type message_type = protocol::OFPT_ERROR;

  private:
    static constexpr std::uint16_t max_data_size
      = std::numeric_limits<std::uint16_t>::max() - sizeof(raw_ofp_type);

    template <class Message>
    using enable_if_is_message
      = typename std::enable_if<type_traits::is_message<Message>::value>::type;

  public:
    error(protocol::error_type const type
        , std::uint16_t const code
        , data_type data
        , std::uint32_t const xid = get_xid()) noexcept
      : error_msg_{
            protocol::ofp_header{
                version()
              , error::type()
              , ofp::calc_ofp_length(data, sizeof(raw_ofp_type))
              , xid
            }
          , std::uint16_t(type)
          , code
        }
      , data_(std::move(data))
    {
    }

    template <class Message, class = enable_if_is_message<Message>>
    error(protocol::error_type const type
        , std::uint16_t const code
        , std::uint16_t const data_size
        , Message const& msg)
    : error{type, code, create_data(msg, data_size), msg.xid()}
    {
    }

    template <class ErrorCode>
    error(ErrorCode const code
        , data_type data
        , std::uint32_t const xid = get_xid()) noexcept
      : error{
          v13::error_code<ErrorCode>::error_type, code, std::move(data), xid
        }
    {
    }

    template <
      class ErrorCode, class Message, class = enable_if_is_message<Message>
    >
    error(ErrorCode const code
        , std::uint16_t const data_size
        , Message const& msg)
      : error{code, create_data(msg, data_size), msg.xid()}
    {
    }

    template <
      class ErrorCode, class Message, class = enable_if_is_message<Message>
    >
    error(ErrorCode const code
        , Message const& msg)
      : error{code, create_data(msg, max_data_size), msg.xid()}
    {
    }

    error(error const&) = default;

    error(error&& other) noexcept
      : error_msg_(other.error_msg_)
      , data_(std::move(other.data_))
    {
      other.error_msg_.header.length = sizeof(raw_ofp_type);
    }

    auto operator=(error const& other)
      -> error&
    {
      return operator=(error{other});
    }

    auto operator=(error&& other) noexcept
      -> error&
    {
      auto tmp = std::move(other);
      std::swap(error_msg_, tmp.error_msg_);
      data_.swap(tmp.data_);
      return *this;
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return error_msg_.header;
    }

    auto error_type() const noexcept
      -> protocol::ofp_error_type
    {
      return protocol::ofp_error_type(error_msg_.type);
    }

    auto error_code() const noexcept
      -> std::uint16_t
    {
      return error_msg_.code;
    }

    auto data() const noexcept
      -> data_type const&
    {
      return data_;
    }

    auto data_length() const noexcept
      -> std::uint16_t
    {
      return data_.size();
    }

    auto extract_data() noexcept
      -> data_type
    {
      auto data = data_type{};
      data.swap(data_);
      error_msg_.header.length = sizeof(raw_ofp_type);
      return data;
    }

    auto failed_request_header() const
      -> protocol::ofp_header
    {
      return detail::decode_without_consumption<protocol::ofp_header>(
          data_.data(), data_.data() + data_.size());
    }

    static auto hello_failed(
          protocol::hello_failed_code const code
        , boost::string_ref const data
        , std::uint32_t const xid)
      -> error
    {
      return error{
          protocol::OFPET_HELLO_FAILED
        , std::uint16_t(code)
        , data_type{data.begin(), data.end()}
        , xid
      };
    }

  private:
    error(raw_ofp_type const& error_msg, data_type&& data) noexcept
      : error_msg_(error_msg)
      , data_(std::move(data))
    {
    }

    friend basic_message;

    static constexpr bool is_fixed_length_message = false;

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, error_msg_);
      detail::encode_byte_array(container, data_.data(), data_.size());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> error
    {
      auto const error_msg = detail::decode<raw_ofp_type>(first, last);

      auto const data_length = error_msg.header.length - sizeof(raw_ofp_type);
      auto data = ofp::decode_data(first, data_length);

      return error{error_msg, std::move(data)};
    }

    template <class Message>
    static auto create_data(Message const& msg, std::uint16_t data_size)
      -> data_type
    {
      auto data = data_type{};
      data.reserve(msg.length());
      msg.encode(data);

      if (data_size > max_data_size) {
        data_size = max_data_size;
      }
      if (data.size() > data_size) {
        data.resize(data_size);
      }

      return data;
    }

  private:
    raw_ofp_type error_msg_;
    data_type data_;
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_ERROR_HPP
