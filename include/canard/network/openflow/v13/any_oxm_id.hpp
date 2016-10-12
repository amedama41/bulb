#ifndef CANARD_NET_OFP_V13_ANY_OXM_ID_HPP
#define CANARD_NET_OFP_V13_ANY_OXM_ID_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/detail/impl/any_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/table_feature_property/id/oxm_id.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct oxm_id_decoder
  {
    using header_type = std::uint32_t;
    using type_id = std::uint32_t;
    using decode_type_list = std::tuple<oxm_id, oxm_experimenter_id>;

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto it = first;
      auto const header = detail::decode<header_type>(it, last);

      if ((header >> 16) == protocol::OFPXMC_EXPERIMENTER) {
        // oxm_experimenter_id::validate_oxm_header(header);
        return function(oxm_experimenter_id::decode(first, last));
      }
      else {
        // oxm_id::validate_oxm_header(header);
        return function(oxm_id::decode(first, last));
      }
    }
  };

  using any_oxm_id = detail::any_type<oxm_id_decoder>;

  template <class T>
  auto any_cast(any_oxm_id& oxm_id)
    -> T&
  {
    return detail::any_cast<T>(oxm_id);
  }

  template <class T>
  auto any_cast(any_oxm_id const& oxm_id)
    -> T const&
  {
    return detail::any_cast<T>(oxm_id);
  }

  template <class T>
  auto any_cast(any_oxm_id* const oxm_id)
    -> T*
  {
    return detail::any_cast<T>(oxm_id);
  }

  template <class T>
  auto any_cast(any_oxm_id const* const oxm_id)
    -> T const*
  {
    return detail::any_cast<T>(oxm_id);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ANY_OXM_ID_HPP
