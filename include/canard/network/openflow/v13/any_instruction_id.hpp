#ifndef CANARD_NET_OFP_V13_ANY_INSTRUCTION_ID_HPP
#define CANARD_NET_OFP_V13_ANY_INSTRUCTION_ID_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <canard/network/openflow/detail/any_type.hpp>
#include <canard/network/openflow/detail/impl/any_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/table_feature_property/id/instruction_id.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  struct instruction_id_decoder
  {
    using header_type = v13_detail::ofp_instruction;
    using type_id = std::uint16_t;
    using decode_type_list
      = std::tuple<instruction_id, instruction_experimenter_id>;

    template <class ReturnType, class Iterator, class Function>
    static auto decode(Iterator& first, Iterator last, Function function)
      -> ReturnType
    {
      auto it = first;
      auto const header = detail::decode<header_type>(it, last);

      if (header.len > std::distance(first, last)) {
        throw std::runtime_error{"too large instruction_id length"};
      }

      if (header.type == protocol::OFPIT_EXPERIMENTER) {
        instruction_experimenter_id::validate_instruction_header(header);
        return function(instruction_experimenter_id::decode(first, last));
      }
      else {
        instruction_id::validate_instruction_header(header);
        return function(instruction_id::decode(first, last));
      }
    }
  };

  using any_instruction_id = detail::any_type<instruction_id_decoder>;

  template <class T>
  auto any_cast(any_instruction_id& instruction_id)
    -> T&
  {
    return detail::any_cast<T>(instruction_id);
  }

  template <class T>
  auto any_cast(any_instruction_id const& instruction_id)
    -> T const&
  {
    return detail::any_cast<T>(instruction_id);
  }

  template <class T>
  auto any_cast(any_instruction_id* const instruction_id)
    -> T*
  {
    return detail::any_cast<T>(instruction_id);
  }

  template <class T>
  auto any_cast(any_instruction_id const* const instruction_id)
    -> T const*
  {
    return detail::any_cast<T>(instruction_id);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ANY_INSTRUCTION_ID_HPP
