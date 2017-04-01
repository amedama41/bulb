#ifndef CANARD_NET_OFP_V13_ACTION_DECODER_HPP
#define CANARD_NET_OFP_V13_ACTION_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/actions.hpp>
#include <canard/network/openflow/v13/decoder/set_field_decoder.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct action_decoder
  {
    using header_type = protocol::ofp_action_header;
    using type_id = std::uint16_t;
    using decode_type_list = default_all_action_list;
    using non_set_field_action_type_list = default_action_list;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    static_assert(
          std::tuple_size<non_set_field_action_type_list>::value == 15
        , "not match to the number of action types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const action_header
        = detail::decode_without_consumption<header_type>(first, last);

      if (std::distance(first, last) < action_header.len) {
        throw exception{
          protocol::bad_request_code::bad_len, "too small data size for action"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      switch (action_header.type) {

#     define CANARD_NET_OFP_V13_ACTION_CASE(z, N, _) \
      using action ## N \
        = std::tuple_element<N, non_set_field_action_type_list>::type; \
      case action ## N::action_type: \
        if (!action ## N::is_valid_action_length(action_header)) { \
          throw exception{ \
            protocol::bad_action_code::bad_len, "invalid action length" \
          } << CANARD_NET_OFP_ERROR_INFO(); \
        } \
        return function(action ## N::decode(first, last));

      BOOST_PP_REPEAT(15, CANARD_NET_OFP_V13_ACTION_CASE, _)

#     undef CANARD_NET_OFP_V13_ACTION_CASE

      case protocol::OFPAT_SET_FIELD:
        if (action_header.len < set_field_decoder::header_size) {
          throw exception{
            protocol::bad_action_code::bad_len, "invalid action length"
          } << CANARD_NET_OFP_ERROR_INFO();
        }
        return set_field_decoder::decode<ReturnType>(
            first, last, std::move(function));
      default:
        throw exception{
          protocol::bad_action_code::bad_type, "unknown action"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
    }

    template <class ReturnType, class Iterator, class Function>
    static auto decode_without_consumption(
        Iterator first, Iterator last, Function function)
      -> ReturnType
    {
      return decode<ReturnType>(first, last, function);
    }
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTION_DECODER_HPP
