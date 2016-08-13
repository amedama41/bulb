#ifndef CANARD_NET_OFP_V10_QUEUE_PROPERTY_DECODER_HPP
#define CANARD_NET_OFP_V10_QUEUE_PROPERTY_DECODER_HPP

#include <iterator>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/protocol/openflow/detail/decode.hpp>
#include <canard/network/protocol/openflow/v10/detail/byteorder.hpp>
#include <canard/network/protocol/openflow/v10/openflow.hpp>
#include <canard/network/protocol/openflow/v10/queue_properties.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

struct queue_property_decoder
{
    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
        -> ReturnType
    {
        auto it = first;
        auto const prop_header
            = detail::decode<v10_detail::ofp_queue_prop_header>(it, last);

        if (std::distance(first, last) < prop_header.len) {
            throw std::runtime_error{"queue property length is too big"};
        }

        switch (prop_header.property) {
#       define CANARD_NET_OFP_V10_QUEUE_PROPERTY_CASE(z, N, _) \
        using property ## N \
            = std::tuple_element<N, default_queue_property_list>::type; \
        case property ## N::queue_property: \
            return function(property ## N::decode(first, last));
        static_assert(
                  std::tuple_size<default_queue_property_list>::value == 1
                , "not match to the number of queue property types");
        BOOST_PP_REPEAT(1, CANARD_NET_OFP_V10_QUEUE_PROPERTY_CASE, _)
#       undef  CANARD_NET_OFP_V10_QUEUE_PROPERTY_CASE
        default:
            throw std::runtime_error{"unknwon queue property"};
        }
    }
};

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_QUEUE_PROPERTY_DECODER_HPP
