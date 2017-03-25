#ifndef CANARD_NET_OFP_DETAIL_EXCEPTION_BASE_HPP
#define CANARD_NET_OFP_DETAIL_EXCEPTION_BASE_HPP

#include <cstdint>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <boost/exception/exception.hpp>
#include <boost/throw_exception.hpp>
#include <boost/utility/string_ref.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  namespace exception_base_detail {

    enum ofp_ex_error_code : std::uint16_t {
      bad_type,
      bad_length,
      bad_value,
    };

    template <class CharT, class Traits>
    auto operator<<(
        std::basic_ostream<CharT, Traits>& os, ofp_ex_error_code const& code)
      -> std::basic_ostream<CharT, Traits>&
    {
      switch (code) {
      case ofp_ex_error_code::bad_type:
        return os << "BAD_TYPE";
      case ofp_ex_error_code::bad_length:
        return os << "BAD_LENGTH";
      case ofp_ex_error_code::bad_value:
        return os << "BAD_VALUE";
      default:
        return os << std::hex << std::setfill('0') << std::setw(4) << code;
      }
    }

  } // namespace exception_base_detail


  class exception_base
    : public std::runtime_error
    , virtual public boost::exception
  {
  public:
    using ex_error_code = exception_base_detail::ofp_ex_error_code;

    exception_base(
          std::uint32_t const error_type
        , std::uint16_t const error_code
        , std::string const& what_arg)
      : std::runtime_error{what_arg}
      , error_type_(error_type)
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

  protected:
    template <class Dispatcher, class ExErrorType>
    static auto make_what_arg(
          std::uint32_t const error_type, std::uint16_t const error_code
        , boost::string_ref const& what_arg)
      -> std::string
    {
      if (error_type & 0xffff0000) {
        std::ostringstream oss;
        oss
          << what_arg << ": "
          << static_cast<ExErrorType>(error_type) << "_"
          << static_cast<ex_error_code>(error_code);
        return oss.str();
      }
      else {
        return Dispatcher::dispatch(
            error_type, what_arg_maker{error_code, what_arg});
      }
    }

    template <class ErrorCode>
    static auto make_what_arg(
        ErrorCode const& error_code, boost::string_ref const& what_arg)
      -> std::string
    {
      std::ostringstream oss;
      oss << what_arg << ": " << error_code;
      return oss.str();
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
          << ','
          << "0x" << std::hex << std::setfill('0') << std::setw(4) << error_code;
        return oss.str();
      }

      std::uint16_t error_code;
      boost::string_ref what_arg;
    };

  private:
    std::uint32_t error_type_;
    std::uint16_t error_code_;
  };

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_EXCEPTION_BASE_HPP
