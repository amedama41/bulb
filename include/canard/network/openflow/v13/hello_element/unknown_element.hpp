#ifndef CANARD_NET_OFP_V13_HELLO_ELEMENTS_UNKNOWN_ELEMENT_HPP
#define CANARD_NET_OFP_V13_HELLO_ELEMENTS_UNKNOWN_ELEMENT_HPP

#include <cstdint>
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace hello_elements {

  class unknown_element
  {
  public:
    explicit unknown_element(std::uint16_t const type)
      : header_{type, sizeof(header_)}
    {
    }

    auto type() const
      -> protocol::ofp_hello_elem_type
    {
      return protocol::ofp_hello_elem_type(header_.type);
    }

    auto length() const
      -> std::uint16_t
    {
      return sizeof(header_) + data_.size();
    }

    template <class Container>
    auto encode(Container& container) const
      -> Container&
    {
      detail::encode(container, header_);
      return detail::encode_byte_array(container, data_.data(), data_.size());
    }

  public:
    template <class Iterator>
    static auto decode(Iterator& first, Iterator last)
      -> unknown_element
    {
      auto const header
        = detail::decode<v13_detail::ofp_hello_elem_header>(first, last);

      auto data = std::vector<unsigned char>(header.length - sizeof(header));
      std::copy_n(first, data.size(), data.begin());
      std::advance(first, data.size());

      std::advance(first, v13_detail::padding_length(header.length));

      return unknown_element(header, std::move(data));
    }

  private:
    unknown_element(
          v13_detail::ofp_hello_elem_header const& header
        , std::vector<unsigned char> data)
      : header_(header)
      , data_(std::move(data))
    {
      if (header.length != sizeof(header) + data.size()) {
        throw 1;
      }
    }

  private:
    v13_detail::ofp_hello_elem_header header_;
    std::vector<unsigned char> data_;
  };

} // namespace hello_elements
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_HELLO_ELEMENTS_UNKNOWN_ELEMENT_HPP
