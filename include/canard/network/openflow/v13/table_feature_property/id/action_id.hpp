#ifndef CANARD_NET_OFP_V13_ACTION_ID_HPP
#define CANARD_NET_OFP_V13_ACTION_ID_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <canard/network/openflow/data_type.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class action_id
    : public detail::basic_protocol_type<action_id>
  {
    static constexpr std::uint16_t base_size
      = offsetof(protocol::ofp_action_header, pad);

  public:
    using raw_ofp_type = protocol::ofp_action_header;
    using raw_ofp_exp_type = protocol::ofp_action_experimenter_header;
    using data_type = ofp::data_type;

    explicit action_id(std::uint16_t const type) noexcept
      : action_header_{type, base_size, 0}
      , data_{}
    {
    }

    action_id(std::uint32_t const experimenter_id, data_type data) noexcept
      : action_header_{
            protocol::OFPAT_EXPERIMENTER
          , ofp::calc_ofp_length(data, sizeof(raw_ofp_exp_type))
          , experimenter_id
        }
      , data_(std::move(data))
    {
    }

    action_id(action_id const&) = default;

    action_id(action_id&& other) noexcept
      : action_header_(other.action_header_)
      , data_(other.extract_data())
    {
    }

    auto operator=(action_id const&)
      -> action_id& = default;

    auto operator=(action_id&& other) noexcept
      -> action_id&
    {
      auto tmp = std::move(other);
      std::swap(action_header_, tmp.action_header_);
      data_.swap(tmp.data_);
      return *this;
    }

    auto type() const noexcept
      -> std::uint16_t
    {
      return action_header_.type;
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return action_header_.len;
    }

    auto is_experimenter() const noexcept
      -> bool
    {
      return type() == protocol::OFPAT_EXPERIMENTER;
    }

    auto experimenter_id() const noexcept
      -> std::uint32_t
    {
      return action_header_.experimenter;
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
      action_header_.len -= data.size();
      return data;
    }

  private:
    action_id(raw_ofp_exp_type const& header, data_type&& data) noexcept
      : action_header_(header)
      , data_(std::move(data))
    {
    }

    friend basic_protocol_type;

    friend constexpr auto get_min_length(
        detail::basic_protocol_type_tag<action_id>) noexcept
      -> std::uint16_t
    {
      return base_size;
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      if (is_experimenter()) {
        detail::encode(container, action_header_);
      }
      else {
        detail::encode(
            container, action_header_, detail::copy_size<base_size>{});
      }
      detail::encode_byte_array(container, data_.data(), data_.size());
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> action_id
    {
      auto header = detail::decode<raw_ofp_exp_type>(
          first, last, detail::copy_size<base_size>{});
      if (header.len < base_size) {
        throw exception{
            protocol::table_features_failed_code::bad_len
          , "too small action_id length"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      auto rest_size = std::uint16_t(header.len - base_size);
      if (std::distance(first, last) < rest_size) {
        throw exception{
            protocol::bad_request_code::bad_len
          , "too small data size for action_id"
        } << CANARD_NET_OFP_ERROR_INFO();
      }

      if (header.type == protocol::OFPAT_EXPERIMENTER) {
        if (rest_size < sizeof(header.experimenter)) {
          throw exception{
              protocol::bad_request_code::bad_len
            , "too small data size for action_id with experimenter id"
          } << CANARD_NET_OFP_ERROR_INFO();
        }
        header.experimenter = detail::decode<std::uint32_t>(first, last);
        rest_size -= sizeof(header.experimenter);
      }

      auto data = ofp::decode_data(first, rest_size);

      return action_id{header, std::move(data)};
    }

    auto equal_impl(action_id const& rhs) const noexcept
      -> bool
    {
      auto const cmp_size
        = is_experimenter() ? sizeof(raw_ofp_exp_type) : base_size;
      return (std::memcmp(&action_header_, &rhs.action_header_, cmp_size) == 0)
          && data_ == rhs.data_;
    }

    auto equivalent_impl(action_id const& rhs) const noexcept
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
    raw_ofp_exp_type action_header_;
    data_type data_;
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTION_ID_HPP
