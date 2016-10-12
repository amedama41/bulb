#ifndef CANARD_NET_OFP_V13_ANY_ACTION_ID_HPP
#define CANARD_NET_OFP_V13_ANY_ACTION_ID_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/detail/impl/any_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/table_feature_property/id/action_id.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct action_id_decoder
  {
    using header_type = v13_detail::ofp_action_header;
    using type_id = std::uint16_t;
    using decode_type_list = std::tuple<action_id, action_experimenter_id>;

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto it = first;
      auto const header
        = detail::decode<header_type>(it, last, offsetof(header_type, pad));

      if (header.len > std::distance(first, last)) {
        throw std::runtime_error{"too large action_id length"};
      }

      if (header.type == protocol::OFPAT_EXPERIMENTER) {
        action_experimenter_id::validate_action_header(header);
        return function(action_experimenter_id::decode(first, last));
      }
      else {
        action_id::validate_action_header(header);
        return function(action_id::decode(first, last));
      }
    }
  };

  using any_action_id = detail::any_type<action_id_decoder>;

  template <class T>
  auto any_cast(any_action_id& action_id)
    -> T&
  {
    return detail::any_cast<T>(action_id);
  }

  template <class T>
  auto any_cast(any_action_id const& action_id)
    -> T const&
  {
    return detail::any_cast<T>(action_id);
  }

  template <class T>
  auto any_cast(any_action_id* const action_id)
    -> T*
  {
    return detail::any_cast<T>(action_id);
  }

  template <class T>
  auto any_cast(any_action_id const* const action_id)
    -> T const*
  {
    return detail::any_cast<T>(action_id);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ANY_ACTION_ID_HPP
