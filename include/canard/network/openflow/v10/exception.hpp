#ifndef CANARD_NET_OFP_V10_EXCEPTION_HPP
#define CANARD_NET_OFP_V10_EXCEPTION_HPP

#include <cstdint>
#include <iomanip>
#include <ostream>
#include <boost/utility/string_ref.hpp>
#include <canard/network/openflow/detail/exception_base.hpp>
#include <canard/network/openflow/v10/dispatcher/error_type_dispatcher.hpp>
#include <canard/network/openflow/v10/error_code.hpp>
#include <canard/network/openflow/v10/io/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  namespace exception_detail {

    enum ofp_ex_error_type : std::uint32_t {
      bad_stats_element = 0x00010000,
      bad_multipart_element = bad_stats_element,
      bad_match_field = 0x00020000,
      bad_packet_queue = 0x00030000,
      bad_queue_property = 0x00040000,
    };

    template <class CharT, class Traits>
    auto operator<<(
          std::basic_ostream<CharT, Traits>& os
        , ofp_ex_error_type const& type)
      -> std::basic_ostream<CharT, Traits>&
    {
      switch (type) {
      case ofp_ex_error_type::bad_stats_element:
        return os << "STATS_ELEMENT";
      case ofp_ex_error_type::bad_match_field:
        return os << "MATCH_FIELD";
      case ofp_ex_error_type::bad_packet_queue:
        return os << "PACKET_QUEUE";
      case ofp_ex_error_type::bad_queue_property:
        return os << "QUEUE_PROPERTY";
      default:
        return os << std::hex << std::setfill('0') << std::setw(8) << type;
      }
    }

  } // namespace exception_detail


  class exception
    : public ofp::detail::exception_base
  {
    using base_t = ofp::detail::exception_base;

  public:
    using ex_error_type = exception_detail::ofp_ex_error_type;

    exception(
          std::uint32_t const error_type
        , std::uint16_t const error_code
        , boost::string_ref const& what_arg)
      : base_t{
            error_type
          , error_code
          , make_what_arg<v10::error_type_dispatcher, ex_error_type>(
              error_type, error_code, what_arg)
        }
    {
    }

    template <class ErrorCode>
    exception(
          ErrorCode const& error_code
        , boost::string_ref const& what_arg)
      : base_t{
            v10::error_code<ErrorCode>::error_type
          , std::uint16_t(error_code)
          , make_what_arg(error_code, what_arg)
        }
    {
    }
  };

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_EXCEPTION_HPP
