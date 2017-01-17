#ifndef CANARD_NET_OFP_V13_HELLO_ELEMENTS_UNKNOWN_ELEMENT_HPP
#define CANARD_NET_OFP_V13_HELLO_ELEMENTS_UNKNOWN_ELEMENT_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace hello_elements {

  class unknown_element
    : public detail::basic_protocol_type<unknown_element>
  {
  public:
    using raw_ofp_type = protocol::ofp_hello_elem_header;
    using data_type = std::vector<unsigned char>;

    explicit unknown_element(std::uint16_t const type)
      : header_{type, sizeof(raw_ofp_type)}
      , data_{}
    {
    }

    unknown_element(std::uint16_t const type, data_type data)
      : header_{type, std::uint16_t(sizeof(raw_ofp_type) + data.size())}
      , data_(std::move(data))
    {
    }

    unknown_element(unknown_element const&) = default;

    unknown_element(unknown_element&& other)
      : header_(other.header_)
      , data_(other.extract_data())
    {
    }

    auto operator=(unknown_element const& other)
      -> unknown_element&
    {
      return operator=(unknown_element{other});
    }

    auto operator=(unknown_element&& other)
      -> unknown_element&
    {
      auto tmp = std::move(other);
      std::swap(header_, tmp.header_);
      data_.swap(tmp.data_);
      return *this;
    }

    auto type() const noexcept
      -> protocol::ofp_hello_elem_type
    {
      return protocol::ofp_hello_elem_type(header_.type);
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return header_.length;
    }

    auto data() const noexcept
      -> data_type const&
    {
      return data_;
    }

    auto extract_data()
      -> data_type
    {
      auto data = data_type{};
      data.swap(data_);
      header_.length = sizeof(raw_ofp_type);
      return data;
    }

    static void validate_header(protocol::ofp_hello_elem_header const& header)
    {
      if (header.length < sizeof(raw_ofp_type)) {
        throw std::runtime_error{"hello_element length is too small"};
      }
    }

  private:
    unknown_element(raw_ofp_type const& header, data_type&& data)
      : header_(header)
      , data_(std::move(data))
    {
    }

    friend basic_protocol_type;

    friend constexpr auto exclude_padding(unknown_element*) noexcept
      -> bool
    {
      return true;
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, header_);
      detail::encode_byte_array(container, data_.data(), data_.size());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> unknown_element
    {
      auto const header = detail::decode<raw_ofp_type>(first, last);

      auto const data_length = header.length - sizeof(raw_ofp_type);
      auto data = data_type(first, std::next(first, data_length));
      std::advance(first, data_length);

      return unknown_element(header, std::move(data));
    }

    auto equal_impl(unknown_element const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(header_, rhs.header_)
          && data_ == rhs.data_;
    }

    auto equivalent_impl(unknown_element const& rhs) const noexcept
      -> bool
    {
      return *this == rhs;
    }

  private:
    raw_ofp_type header_;
    data_type data_;
  };

} // namespace hello_elements
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_HELLO_ELEMENTS_UNKNOWN_ELEMENT_HPP
