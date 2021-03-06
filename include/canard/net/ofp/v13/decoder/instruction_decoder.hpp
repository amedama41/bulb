#ifndef CANARD_NET_OFP_V13_INSTRUCTION_DECODER_HPP
#define CANARD_NET_OFP_V13_INSTRUCTION_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/exception.hpp>
#include <canard/net/ofp/v13/instructions.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct instruction_decoder
  {
    using header_type = protocol::ofp_instruction;
    using type_id = std::uint16_t;
    using decode_type_list = default_instruction_list;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    static_assert(
          std::tuple_size<decode_type_list>::value == 6
        , "not match to the number of instruction types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const instruction
        = detail::decode_without_consumption<header_type>(first, last);

      if (std::distance(first, last) < instruction.len) {
        throw exception{
            protocol::bad_request_code::bad_len
          , "too small data size for instruction"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      switch (instruction.type) {

#     define CANARD_NET_OFP_V13_INSTRUCTION_CASE(z, N, _) \
      using instruction ## N = std::tuple_element<N, decode_type_list>::type; \
      case instruction ## N::type(): \
        if (!instruction ## N::is_valid_instruction_length(instruction)) { \
          throw exception{ \
              protocol::bad_instruction_code::bad_len \
            , "invalid instruction length" \
          } << CANARD_NET_OFP_ERROR_INFO(); \
        } \
        return function(instruction ## N::decode(first, last));

      BOOST_PP_REPEAT(6, CANARD_NET_OFP_V13_INSTRUCTION_CASE, _)

#     undef CANARD_NET_OFP_V13_INSTRUCTION_CASE

      default:
          throw exception{
            protocol::bad_instruction_code::unknown_inst, "unknown instruction"
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

#endif // CANARD_NET_OFP_V13_INSTRUCTION_DECODER_HPP
