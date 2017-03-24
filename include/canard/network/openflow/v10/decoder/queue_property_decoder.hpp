#ifndef CANARD_NET_OFP_V10_QUEUE_PROPERTY_DECODER_HPP
#define CANARD_NET_OFP_V10_QUEUE_PROPERTY_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/exception.hpp>
#include <canard/network/openflow/v10/openflow.hpp>
#include <canard/network/openflow/v10/queue_properties.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  struct queue_property_decoder
  {
    using header_type = protocol::ofp_queue_prop_header;
    using type_id = std::uint16_t;
    using decode_type_list = default_queue_property_list;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    static_assert(
          std::tuple_size<decode_type_list>::value == 1
        , "not match to the number of queue property types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const prop_header
        = detail::decode_without_consumption<header_type>(first, last);

      if (std::distance(first, last) < prop_header.len) {
        BOOST_THROW_EXCEPTION((v10::exception{
                protocol::OFPBRC_BAD_LEN
              , "too small data size for queue_property"
        }));
      }

      switch (prop_header.property) {

#     define CANARD_NET_OFP_V10_QUEUE_PROPERTY_CASE(z, N, _) \
      using property ## N = std::tuple_element<N, decode_type_list>::type; \
      case property ## N::queue_property: \
        if (!property ## N::is_valid_queue_property_length(prop_header)) { \
          BOOST_THROW_EXCEPTION((v10::exception{ \
                  v10::exception::bad_queue_property \
                , v10::exception::bad_length \
                , "invalid queue_property length" \
          })); \
        } \
        return function(property ## N::decode(first, last));

      BOOST_PP_REPEAT(1, CANARD_NET_OFP_V10_QUEUE_PROPERTY_CASE, _)

#     undef  CANARD_NET_OFP_V10_QUEUE_PROPERTY_CASE

      default:
        BOOST_THROW_EXCEPTION((v10::exception{
                v10::exception::bad_queue_property
              , v10::exception::bad_type
              , "unknwon queue_property"
        }));
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

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_QUEUE_PROPERTY_DECODER_HPP
