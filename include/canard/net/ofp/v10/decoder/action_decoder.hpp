#ifndef CANARD_NET_OFP_V10_ACTION_DECODER_HPP
#define CANARD_NET_OFP_V10_ACTION_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/v10/exception.hpp>
#include <canard/net/ofp/v10/actions.hpp>
#include <canard/net/ofp/v10/detail/byteorder.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  struct action_decoder
  {
    using header_type = protocol::ofp_action_header;
    using type_id = std::uint16_t;
    using decode_type_list = default_action_list;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    static_assert(
          std::tuple_size<decode_type_list>::value == 12
        , "not match to the number of action types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const action_header
        = detail::decode_without_consumption<header_type>(first, last);

      if (std::distance(first, last) < action_header.len) {
        throw v10::exception{
          protocol::OFPBRC_BAD_LEN, "too small data size for action"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      switch (action_header.type) {

#     define CANARD_NET_OFP_V10_ACTION_CASE(z, N, _) \
      using action ## N = std::tuple_element<N, decode_type_list>::type; \
      case action ## N::type(): \
        if (!action ## N::is_valid_action_length(action_header)) { \
          throw v10::exception{ \
            protocol::OFPBAC_BAD_LEN, "invalid action length" \
          } << CANARD_NET_OFP_ERROR_INFO(); \
        } \
        return function(action ## N::decode(first, last));

      BOOST_PP_REPEAT(12, CANARD_NET_OFP_V10_ACTION_CASE, _)

#     undef CANARD_NET_OFP_V10_ACTION_CASE

      default:
        throw v10::exception{
          protocol::OFPBAC_BAD_TYPE, "unknwon action type"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
    }

    template <class ReturnType, class Iterator, class Function>
    static auto decode_without_consumption(
        Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      return decode<ReturnType>(first, last, function);
    }
  };

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTION_DECODER_HPP
