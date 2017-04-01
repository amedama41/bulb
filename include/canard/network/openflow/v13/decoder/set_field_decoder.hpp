#ifndef CANARD_NET_OFP_V13_SET_FIELD_DECODER_HPP
#define CANARD_NET_OFP_V13_SET_FIELD_DECODER_HPP

#include <cstdint>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/action/set_field.hpp>
#include <canard/network/openflow/v13/common/oxm_header.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct set_field_decoder
  {
    using header_type = protocol::ofp_action_set_field;
    using type_id = std::uint32_t;
    using decode_type_list = default_set_field_list;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    static_assert(
          std::tuple_size<default_set_field_list>::value == 36
        , "not match to the number of set_field types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const set_field
        = detail::decode_without_consumption<header_type>(first, last);

      auto const oxm_header = v13::oxm_header::decode_without_consumption(
          set_field.field, set_field.field + sizeof(set_field.field));

      if (set_field.len != detail::v13::exact_length(
            header_size + oxm_header.oxm_length())) {
        throw exception{
          protocol::bad_action_code::bad_set_len, "invalid set_field length"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      switch (oxm_header.oxm_type()) {

#     define CANARD_NET_OFP_V13_SET_FIELD_CASE(z, N, _) \
      using set_field ## N = \
            std::tuple_element<N, default_set_field_list>::type; \
      case set_field ## N::oxm_type(): \
        if (!set_field ## N::oxm_match_field \
            ::is_valid_oxm_match_field_length(oxm_header)) { \
          throw exception{ \
              protocol::bad_action_code::bad_set_len \
            , "invalid set_field's oxm_match_field length" \
          } << CANARD_NET_OFP_ERROR_INFO(); \
        } \
        return function(set_field ## N::decode(first, last));

      BOOST_PP_REPEAT(36, CANARD_NET_OFP_V13_SET_FIELD_CASE, _)

#     undef CANARD_NET_OFP_V13_SET_FIELD_CASE

      default:
        throw exception{
            protocol::bad_action_code::bad_set_type
          , "unknwon set_field's oxm_match_field type"
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

#endif // CANARD_NET_OFP_V13_SET_FIELD_DECODER_HPP
