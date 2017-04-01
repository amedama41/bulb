#ifndef CANARD_NET_OFP_V13_HELLO_ELEMENT_DECODER_HPP
#define CANARD_NET_OFP_V13_HELLO_ELEMENT_DECODER_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/hello_elements.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  namespace hello_element_decoder_detail {

    template <class Tuple, class T>
    struct tuple_append;

    template <class... Types, class T>
    struct tuple_append<std::tuple<Types...>, T>
    {
      using type = std::tuple<Types..., T>;
    };

    template <class Tuple, class T>
    using tuple_append_t = typename tuple_append<Tuple, T>::type;

  } // namespace hello_element_decoder_detail

  struct hello_element_decoder
  {
    using hello_element_list = default_hello_element_list;
    using unknwon_type = hello_elements::unknown_element;

    static_assert(
          std::tuple_size<hello_element_list>::value == 1
        , "not match to the number of hello element types");

    using header_type = protocol::ofp_hello_elem_header;
    using type_id = std::uint16_t;
    using decode_type_list = hello_element_decoder_detail::tuple_append_t<
      hello_element_list, unknwon_type
    >;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const helem_header
        = detail::decode_without_consumption<header_type>(first, last);

      if (std::distance(first, last) < helem_header.length) {
        throw exception{
            protocol::bad_request_code::bad_len
          , "too small data size for hello_element"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      switch (helem_header.type) {

#     define CANARD_NET_OFP_V13_HELLO_ELEMENT_DECODE_CASE(z, N, _) \
      using helem ## N = std::tuple_element<N, hello_element_list>::type; \
      case helem ## N::type(): \
        if (!helem ## N::is_valid_hello_element_length(helem_header)) { \
          throw exception{ \
              exception::ex_error_type::bad_hello_element \
            , exception::ex_error_code::bad_length \
            , "invalid hello_element length" \
          } << CANARD_NET_OFP_ERROR_INFO(); \
        } \
        return function(helem ## N::decode(first, last));

      BOOST_PP_REPEAT(1, CANARD_NET_OFP_V13_HELLO_ELEMENT_DECODE_CASE, _)

#     undef CANARD_NET_OFP_V13_HELLO_ELEMENT_DECODE_CASE

      default:
        return function(unknwon_type::decode(first, last));
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

#endif // CANARD_NET_OFP_V13_HELLO_ELEMENT_DECODER_HPP
