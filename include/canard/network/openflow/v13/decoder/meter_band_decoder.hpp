#ifndef CANARD_NET_OFP_V13_METER_BAND_DECODER_HPP
#define CANARD_NET_OFP_V13_METER_BAND_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/meter_bands.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct meter_band_decoder
  {
    using header_type = protocol::ofp_meter_band_header;
    using type_id = std::uint16_t;
    using decode_type_list = default_meter_band_list;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    static_assert(
          std::tuple_size<decode_type_list>::value == 2
        , "not match to the number of meter band types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const meter_band_header
        = detail::decode_without_consumption<header_type>(first, last);

      if (std::distance(first, last) < meter_band_header.len) {
        throw std::runtime_error{"too small data size for meter band"};
      }

      switch (meter_band_header.type) {

#     define CANARD_NET_OFP_V13_METER_BAND_CASE(z, N, _) \
      using meter_band ## N = std::tuple_element<N, decode_type_list>::type; \
      case meter_band ## N::type(): \
        if (!meter_band ## N::is_valid_meter_band_length(meter_band_header)) { \
          throw std::runtime_error{"invalid meter band length"}; \
        } \
        return function(meter_band ## N::decode(first, last));

      BOOST_PP_REPEAT(2, CANARD_NET_OFP_V13_METER_BAND_CASE, _)

#     undef CANARD_NET_OFP_V13_METER_BAND_CASE

      default:
        throw std::runtime_error{"unknwon meter band"};
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

#endif // CANARD_NET_OFP_V13_METER_BAND_DECODER_HPP
