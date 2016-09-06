#ifndef CANARD_NET_OFP_V13_OXM_MATCH_FIELD_DECODER_HPP
#define CANARD_NET_OFP_V13_OXM_MATCH_FIELD_DECODER_HPP

#include <cstdint>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/common/oxm_match_field.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

struct oxm_match_field_decoder
{
    using header_type = std::uint32_t;
    using decode_type_list = default_oxm_match_field_list;

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
        -> ReturnType
    {
        auto it = first;
        auto const oxm_header = detail::decode<header_type>(it, last);

        if (std::distance(it, last) < (oxm_header & 0xff)) {
            throw std::runtime_error{"oxm length is too big"};
        }

        switch (oxm_header >> 9) {
#       define CANARD_NET_OFP_V13_MATCH_FIELD_CASE(z, N, _) \
        using oxm_match_field ## N \
            = std::tuple_element<N, decode_type_list>::type; \
        case oxm_match_field ## N::oxm_type(): \
            return function(oxm_match_field ## N::decode(first, last));
        static_assert(
                  std::tuple_size<decode_type_list>::value == 40
                , "not match to the number of oxm match types");
        BOOST_PP_REPEAT(40, CANARD_NET_OFP_V13_MATCH_FIELD_CASE, _)
#       undef CANARD_NET_OFP_V13_MATCH_FIELD_CASE
        default:
            throw std::runtime_error{"unknwon oxm type"};
        }
    }
};

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OXM_MATCH_FIELD_DECODER_HPP
