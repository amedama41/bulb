#ifndef CANARD_NET_OFP_V10_EXCEPTION_HPP
#define CANARD_NET_OFP_V10_EXCEPTION_HPP

#include <cstdint>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <boost/exception/exception.hpp>
#include <boost/throw_exception.hpp>
#include <boost/utility/string_ref.hpp>
#include <canard/network/openflow/v10/dispatcher/error_type_dispatcher.hpp>
#include <canard/network/openflow/v10/error_code.hpp>
#include <canard/network/openflow/v10/io/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  class exception
    : public std::runtime_error
    , virtual public boost::exception
  {
  public:
    enum ex_error_type : std::uint32_t {
      bad_stats_element = 0x00010000,
      bad_multipart_element = bad_stats_element,
      bad_match_field = 0x00020000,
      bad_packet_queue = 0x00030000,
      bad_queue_property = 0x00040000,
    };

    enum ex_error_code : std::uint16_t {
      bad_type,
      bad_length,
      bad_value
    };

    exception(
          std::uint32_t const error_type
        , std::uint16_t const error_code
        , boost::string_ref const& what_arg)
      : std::runtime_error{make_what_arg(error_type, error_code, what_arg)}
      , error_type_(error_type)
      , error_code_(error_code)
    {
    }

    template <class ErrorCode>
    exception(
          ErrorCode const& error_code
        , boost::string_ref const& what_arg)
      : std::runtime_error{make_what_arg(error_code, what_arg)}
      , error_type_(v10::error_code<ErrorCode>::error_type)
      , error_code_(error_code)
    {
    }

    auto error_type() const noexcept
      -> std::uint32_t
    {
      return error_type_;
    }

    auto error_code() const noexcept
      -> std::uint16_t
    {
      return error_code_;
    }

  private:
    struct what_arg_maker
    {
      template <class ErrorCode>
      auto operator()(std::uint16_t, ErrorCode) const
        -> std::string
      {
        return make_what_arg(
            static_cast<typename ErrorCode::type>(error_code), what_arg);
      }

      auto operator()(std::uint16_t const error_type) const
        -> std::string
      {
        std::ostringstream oss;
        oss
          << what_arg << ": "
          << "0x" << std::hex << std::setfill('0') << std::setw(4) << error_type
          << '-'
          << "0x" << std::hex << std::setfill('0') << std::setw(4) << error_code;
        return oss.str();
      }

      std::uint16_t error_code;
      boost::string_ref what_arg;
    };

    static auto make_what_arg(
          std::uint32_t const error_type, std::uint16_t const error_code
        , boost::string_ref const& what_arg)
      -> std::string
    {
      if (error_type & 0xffff0000) {
        return make_what_arg_from_ex_error(
              static_cast<ex_error_type>(error_type)
            , static_cast<ex_error_code>(error_code)
            , what_arg);
      }
      else {
        return error_type_dispatcher::dispatch(
              error_type
            , what_arg_maker{error_code, what_arg});
      }
    }

    template <class ErrorCode>
    static auto make_what_arg(
          ErrorCode const& error_code
        , boost::string_ref const& what_arg)
      -> std::string
    {
      std::ostringstream oss;
      oss << what_arg << ": " << error_code;
      return oss.str();
    }

    static auto make_what_arg_from_ex_error(
        ex_error_type const, ex_error_code const, boost::string_ref const&)
      -> std::string;

  private:
    std::uint32_t error_type_;
    std::uint16_t error_code_;
  };

  template <class CharT, class Traits>
  auto operator<<(
        std::basic_ostream<CharT, Traits>& os
      , exception::ex_error_type const& error_type)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (error_type) {
    case exception::bad_stats_element:
      return os << "stats_element";
    case exception::bad_match_field:
      return os << "match_field";
    case exception::bad_packet_queue:
      return os << "packet_queue";
    case exception::bad_queue_property:
      return os << "queue_property";
    default:
      return os << std::hex << std::setfill('0') << std::setw(8) << error_type;
    }
  }

  template <class CharT, class Traits>
  auto operator<<(
        std::basic_ostream<CharT, Traits>& os
      , exception::ex_error_code const& error_code)
    -> std::basic_ostream<CharT, Traits>&
  {
    switch (error_code) {
    case exception::bad_type:
      return os << "bad type";
    case exception::bad_length:
      return os << "bad length";
    case exception::bad_value:
      return os << "bad value";
    default:
      return os << std::hex << std::setfill('0') << std::setw(4) << error_code;
    }
  }

  inline auto exception::make_what_arg_from_ex_error(
        exception::ex_error_type const error_type
      , exception::ex_error_code const error_code
      , boost::string_ref const& what_arg)
    -> std::string
  {
    std::ostringstream oss;
    oss << what_arg << ": " << error_type << '-' << error_code;
    return oss.str();
  }

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_EXCEPTION_HPP
