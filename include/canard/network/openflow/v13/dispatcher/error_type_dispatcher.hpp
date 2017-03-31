#ifndef CANARD_NET_OFP_V13_DISPATCHER_ERROR_TYPE_DISPATCHER_HPP
#define CANARD_NET_OFP_V13_DISPATCHER_ERROR_TYPE_DISPATCHER_HPP

#include <tuple>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <canard/network/openflow/v13/error_code.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct error_type_dispatcher
  {
    template <class Function>
    static auto dispatch(std::uint16_t const error_type, Function function)
      -> decltype(function(error_type))
    {
      using error_code_list = default_error_code_list;
      static_assert(
            std::tuple_size<error_code_list>::value == 14
          , "not match to the number of error_code types");

      switch (error_type) {

#     define CANARD_NET_OFP_V13_DISPATCHER_ERROR_TYPE_CASE(z, N, _) \
      using error_code ## N = std::tuple_element<N, error_code_list>::type; \
      case error_code ## N::error_type: \
        return function( \
            error_type, boost::mpl::identity<error_code ## N::type>{}); \

      BOOST_PP_REPEAT(14, CANARD_NET_OFP_V13_DISPATCHER_ERROR_TYPE_CASE, _)

#     undef CANARD_NET_OFP_V13_DISPATCHER_ERROR_TYPE_CASE

      default:
        return function(error_type);
      }
    }
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_DISPATCHER_ERROR_TYPE_DISPATCHER_HPP
