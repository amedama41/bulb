#ifndef CANARD_NET_OFP_V13_INSTRUCTION_ID_HPP
#define CANARD_NET_OFP_V13_INSTRUCTION_ID_HPP

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/data_type.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class instruction_id
    : public detail::basic_protocol_type<instruction_id>
  {
    static constexpr std::uint16_t base_size
      = sizeof(protocol::ofp_instruction);

  public:
    using raw_ofp_type = protocol::ofp_instruction;
    using raw_ofp_exp_type = protocol::ofp_instruction_experimenter;
    using data_type = ofp::data_type;

    explicit instruction_id(std::uint16_t const type) noexcept
      : instruction_header_{type, base_size, 0}
      , data_{}
    {
    }

    instruction_id(std::uint32_t const experimenter_id, data_type data) noexcept
      : instruction_header_{
            protocol::OFPIT_EXPERIMENTER
          , ofp::calc_ofp_length(data, sizeof(raw_ofp_exp_type))
          , experimenter_id
        }
      , data_(std::move(data))
    {
    }

    instruction_id(instruction_id const&) = default;

    instruction_id(instruction_id&& other) noexcept
      : instruction_header_(other.instruction_header_)
      , data_(other.extract_data())
    {
    }

    auto operator=(instruction_id const&)
      -> instruction_id& = default;

    auto operator=(instruction_id&& other) noexcept
      -> instruction_id&
    {
      auto tmp = std::move(other);
      std::swap(instruction_header_, tmp.instruction_header_);
      data_.swap(tmp.data_);
      return *this;
    }

    auto type() const noexcept
      -> std::uint16_t
    {
      return instruction_header_.type;
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return instruction_header_.len;
    }

    auto is_experimenter() const noexcept
      -> bool
    {
      return type() == protocol::OFPIT_EXPERIMENTER;
    }

    auto experimenter_id() const noexcept
      -> std::uint32_t
    {
      return instruction_header_.experimenter;
    }

    auto data() const noexcept
      -> data_type const&
    {
      return data_;
    }

    auto extract_data() noexcept
      -> data_type
    {
      auto data = data_type{};
      data.swap(data_);
      instruction_header_.len -= data.size();
      return data;
    }

  private:
    instruction_id(raw_ofp_exp_type const& header, data_type&& data) noexcept
      : instruction_header_(header)
      , data_(std::move(data))
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      if (is_experimenter()) {
        detail::encode(container, instruction_header_);
      }
      else {
        detail::encode(
            container, instruction_header_, detail::copy_size<base_size>{});
      }
      detail::encode_byte_array(container, data_.data(), data_.size());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> instruction_id
    {
      auto header = detail::decode<raw_ofp_exp_type>(
          first, last, detail::copy_size<base_size>{});
      if (header.len < base_size) {
        throw std::runtime_error{"too small instruction_id length"};
      }

      auto rest_size = std::uint16_t(header.len - base_size);
      if (std::distance(first, last) < rest_size) {
        throw std::runtime_error{"too short byte length"};
      }

      if (header.type == protocol::OFPIT_EXPERIMENTER) {
        if (rest_size < sizeof(header.experimenter)) {
          throw std::runtime_error{"invalid instruction_id length"};
        }
        header.experimenter = detail::decode<std::uint32_t>(first, last);
        rest_size -= sizeof(header.experimenter);
      }

      auto data = ofp::decode_data(first, rest_size);

      return instruction_id{header, std::move(data)};
    }

    auto equal_impl(instruction_id const& rhs) const noexcept
      -> bool
    {
      auto const cmp_size
        = is_experimenter() ? sizeof(raw_ofp_exp_type) : base_size;
      return (std::memcmp(
            &instruction_header_, &rhs.instruction_header_, cmp_size) == 0)
          && data_ == rhs.data_;
    }

    auto equivalent_impl(instruction_id const& rhs) const noexcept
      -> bool
    {
      if (is_experimenter()) {
        return rhs.is_experimenter()
            && experimenter_id() == rhs.experimenter_id()
            && data() == rhs.data();
      }
      else {
        return type() == rhs.type();
      }
    }

  private:
    raw_ofp_exp_type instruction_header_;
    data_type data_;
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTION_ID_HPP
