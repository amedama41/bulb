#ifndef CANARD_NET_OFP_EXCEPTION_HPP
#define CANARD_NET_OFP_EXCEPTION_HPP

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <boost/exception/exception.hpp>
#include <boost/throw_exception.hpp>
#include <boost/utility/string_ref.hpp>

namespace canard {
namespace net {
namespace ofp {

  class exception
    : public std::runtime_error
    , virtual public boost::exception
  {
  public:
    enum ex_error_type : std::uint32_t {
      bad_packet_queue = 0x00010000,
      bad_queue_property = 0x00020000,
      bad_stats_element = 0x00030000,
      bad_multipart_element = bad_stats_element,
      bad_hello_element = 0x00040000,
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
    static auto make_what_arg(
          std::uint32_t const error_type, std::uint16_t const error_code
        , boost::string_ref const& what_arg)
      -> std::string
    {
      std::ostringstream oss;
      oss
        << what_arg
        << ' '
        << '('
        << "0x" << std::hex << std::setfill('0') << std::setw(8) << error_type
        << ','
        << "0x" << std::hex << std::setfill('0') << std::setw(4) << error_code
        << ')';
      return oss.str();
    }

  private:
    std::uint32_t error_type_;
    std::uint16_t error_code_;
  };

} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_EXCEPTION_HPP
