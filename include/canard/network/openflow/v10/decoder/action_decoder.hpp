#ifndef CANARD_NET_OFP_V10_ACTION_DECODER_HPP
#define CANARD_NET_OFP_V10_ACTION_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v10/actions.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

struct action_decoder
{
    using header_type = v10_detail::ofp_action_header;
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
        auto it = first;
        auto const action_header = detail::decode<header_type>(it, last);

        if (std::distance(first, last) < action_header.len) {
            throw std::runtime_error{"action length is too big"};
        }

        switch (action_header.type) {
#       define CANARD_NET_OFP_V10_ACTION_CASE(z, N, _) \
        using action ## N \
            = std::tuple_element<N, decode_type_list>::type; \
        case action ## N::action_type: \
            return function(action ## N::decode(first, last));
        BOOST_PP_REPEAT(12, CANARD_NET_OFP_V10_ACTION_CASE, _)
#       undef CANARD_NET_OFP_V10_ACTION_CASE
        default:
            throw std::runtime_error{"unknwon action type"};
        }
    }
};

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_ACTION_DECODER_HPP
