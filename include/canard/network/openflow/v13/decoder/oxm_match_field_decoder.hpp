#ifndef CANARD_NET_OFP_V13_OXM_MATCH_FIELD_DECODER_HPP
#define CANARD_NET_OFP_V13_OXM_MATCH_FIELD_DECODER_HPP

#include <cstdint>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/v13/common/oxm_header.hpp>
#include <canard/network/openflow/v13/common/oxm_match_field.hpp>

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

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
        -> ReturnType
    {
        auto it = first;
        auto const oxm_header = v13::oxm_header::decode(it, last);

        if (std::distance(it, last) < oxm_header.oxm_length()) {
            throw std::runtime_error{"too small data size for oxm_match_field"};
        }

        switch (oxm_header.oxm_type()) {
#       define CANARD_NET_OFP_V13_MATCH_FIELD_CASE(z, N, _) \
        using oxm_match_field ## N \
            = std::tuple_element<N, decode_type_list>::type; \
        case oxm_match_field ## N::oxm_type(): \
            if (!oxm_match_field ## N \
                    ::is_valid_oxm_match_field_length(oxm_header)) { \
                throw std::runtime_error{"invalid oxm_match_field length"}; \
            } \
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
