#ifndef CANARD_NET_OFP_V10_FLOW_MODE_DECODER_HPP
#define CANARD_NET_OFP_V10_FLOW_MODE_DECODER_HPP

#include <cstdint>
#include <tuple>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v10/exception.hpp>
#include <canard/network/openflow/v10/message/flow_mod.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

  struct flow_mod_decoder
  {
    using header_type = protocol::ofp_flow_mod;
    using type_id = std::uint16_t;
    using decode_type_list = std::tuple<
        messages::flow_add
      , messages::flow_modify
      , messages::flow_modify_strict
      , messages::flow_delete
      , messages::flow_delete_strict
    >;
    static constexpr std::uint16_t header_size = sizeof(header_type);

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto const flow_mod
        = detail::decode_without_consumption<header_type>(first, last);

      switch (flow_mod.command) {

#     define CANARD_NET_OFP_V10_FLOW_MOD_CASE(z, N, _) \
      using flow_mod ## N = std::tuple_element<N, decode_type_list>::type; \
      case flow_mod ## N::command(): \
        return function(flow_mod ## N::decode(first, last));

      BOOST_PP_REPEAT(5, CANARD_NET_OFP_V10_FLOW_MOD_CASE, _)

#     undef CANARD_NET_OFP_V10_FLOW_MOD_CASE

      default:
        throw v10::exception{
          protocol::OFPFMFC_BAD_COMMAND, "unknwon flow_mod command"
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

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_FLOW_MODE_DECODER_HPP
