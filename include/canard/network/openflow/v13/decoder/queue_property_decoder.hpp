#ifndef CANARD_NET_OFP_V13_QUEUE_PROPERTY_DECODER_HPP
#define CANARD_NET_OFP_V13_QUEUE_PROPERTY_DECODER_HPP

#include <iterator>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/queue_properties.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

struct queue_property_decoder
{
    using header_type = v13_detail::ofp_queue_prop_header;
    using decode_type_list = default_queue_property_list;

    static_assert(
              std::tuple_size<decode_type_list>::value == 2
            , "not match to the number of queue property types");

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
        -> ReturnType
    {
        auto it = first;
        auto const prop_header = detail::decode<header_type>(it, last);

        if (std::distance(first, last) < prop_header.len) {
            throw std::runtime_error{"queue property length is too big"};
        }

        switch (prop_header.property) {
#       define CANARD_NET_OFP_V13_QUEUE_PROPERTY_CASE(z, N, _) \
        using property ## N = std::tuple_element<N, decode_type_list>::type; \
        case property ## N::queue_property: \
            return function(property ## N::decode(first, last));
        BOOST_PP_REPEAT(2, CANARD_NET_OFP_V13_QUEUE_PROPERTY_CASE, _)
#       undef  CANARD_NET_OFP_V13_QUEUE_PROPERTY_CASE
        default:
            throw std::runtime_error{"unknwon queue property"};
        }
    }
};

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_QUEUE_PROPERTY_DECODER_HPP
