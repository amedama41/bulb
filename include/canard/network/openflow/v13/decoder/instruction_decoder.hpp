#ifndef CANARD_NET_OFP_V13_INSTRUCTION_DECODER_HPP
#define CANARD_NET_OFP_V13_INSTRUCTION_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/instructions.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

struct instruction_decoder
{
    using header_type = v13_detail::ofp_instruction;
    using type_id = std::uint16_t;
    using decode_type_list = default_instruction_list;

    static_assert(
              std::tuple_size<decode_type_list>::value == 6
            , "not match to the number of instruction types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
        -> ReturnType
    {
        auto it = first;
        auto const instruction = detail::decode<header_type>(it, last);

        if (std::distance(first, last) < instruction.len) {
            throw std::runtime_error{"instruction length is too big"};
        }

        switch (instruction.type) {
#       define CANARD_NET_OFP_V13_INSTRUCTION_CASE(z, N, _) \
        using instruction ## N \
            = std::tuple_element<N, decode_type_list>::type; \
        case instruction ## N::instruction_type: \
            return function(instruction ## N::decode(first, last));
        BOOST_PP_REPEAT(6, CANARD_NET_OFP_V13_INSTRUCTION_CASE, _)
#       undef CANARD_NET_OFP_V13_INSTRUCTION_CASE
        default:
            throw std::runtime_error{"unknown instruction type"};
        }
    }
};

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTION_DECODER_HPP
