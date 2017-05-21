#ifndef CANARD_NET_OFP_V13_OXM_MATCH_FIELD_DECODER_HPP
#define CANARD_NET_OFP_V13_OXM_MATCH_FIELD_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/v13/common/oxm_header.hpp>
#include <canard/net/ofp/v13/common/oxm_match_field.hpp>
#include <canard/net/ofp/v13/exception.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct oxm_match_field_decoder
  {
    using header_type = std::uint32_t;
    using type_id = std::uint32_t;
    using decode_type_list = default_oxm_match_field_list;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    static_assert(
          std::tuple_size<decode_type_list>::value == 40
        , "not match to the number of oxm match types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const oxm_header
        = v13::oxm_header::decode_without_consumption(first, last);

      if (std::distance(first, last)
          < std::uint16_t(oxm_header.length() + oxm_header.oxm_length())) {
        throw exception{
            protocol::bad_request_code::bad_len
          , "too small data size for oxm_match_field"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      switch (oxm_header.oxm_type()) {

#     define CANARD_NET_OFP_V13_MATCH_FIELD_CASE(z, N, _) \
      using field ## N = std::tuple_element<N, decode_type_list>::type; \
      case field ## N::oxm_type(): \
        if (!field ## N::is_valid_oxm_match_field_length(oxm_header)) { \
          throw exception{ \
              protocol::bad_match_code::bad_len \
            , "invalid oxm_match_field length" \
          } << CANARD_NET_OFP_ERROR_INFO(); \
        } \
        return function(field ## N::decode(first, last));

      BOOST_PP_REPEAT(40, CANARD_NET_OFP_V13_MATCH_FIELD_CASE, _)

#     undef CANARD_NET_OFP_V13_MATCH_FIELD_CASE

      default:
        throw exception{
          protocol::bad_match_code::bad_field, "unknown oxm_match_field"
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

#endif // CANARD_NET_OFP_V13_OXM_MATCH_FIELD_DECODER_HPP
