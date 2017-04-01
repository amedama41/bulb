#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_DECODER_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/table_feature_properties.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

#define CANARD_NET_OFP_NUM_TABLE_FEATURE_PROPERTIES 14

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct table_feature_property_decoder
  {
    using header_type = protocol::ofp_table_feature_prop_header;
    using type_id = std::uint16_t;
    using decode_type_list = default_table_feature_property_list;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    static_assert(
          std::tuple_size<decode_type_list>::value
            == CANARD_NET_OFP_NUM_TABLE_FEATURE_PROPERTIES
        , "not match to the number of table_feature_property types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const header
        = detail::decode_without_consumption<header_type>(first, last);

      if (std::distance(first, last) < header.length) {
        throw exception{
            protocol::bad_request_code::bad_len
          , "too small data size for table_feature_property"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      switch (header.type) {

#     define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_CASE(z, N, _) \
      using property ## N = std::tuple_element<N, decode_type_list>::type; \
      case property ## N::type(): \
        if (!property ## N::is_valid_table_feature_property_length(header)) { \
          throw exception{ \
              protocol::table_features_failed_code::bad_len \
            , "invalid table_feature_property length" \
          } << CANARD_NET_OFP_ERROR_INFO(); \
        } \
        return function(property ## N::decode(first, last));

      BOOST_PP_REPEAT(
            CANARD_NET_OFP_NUM_TABLE_FEATURE_PROPERTIES
          , CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_CASE
          , _)

#     undef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_CASE

      default:
        throw exception{
            protocol::table_features_failed_code::bad_type
          , "unknwon table feature property type"
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

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTY_DECODER_HPP
