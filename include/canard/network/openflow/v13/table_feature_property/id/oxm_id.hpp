#ifndef CANARD_NET_OFP_V13_OXM_ID_HPP
#define CANARD_NET_OFP_V13_OXM_ID_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/common/oxm_header.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class oxm_id
    : public detail::basic_protocol_type<oxm_id>
  {
  public:
    using oxm_header_type = std::uint32_t;
    using raw_ofp_type = oxm_header_type;
    using raw_ofp_exp_type = protocol::ofp_oxm_experimenter_header;

    explicit oxm_id(oxm_header_type const oxm_header) noexcept
      : oxm_header_{oxm_header, 0}
    {
    }

    oxm_id(protocol::ofp_oxm_class const oxm_class
         , std::uint8_t const oxm_field
         , bool const oxm_hasmask
         , std::uint8_t const oxm_length) noexcept
      : oxm_header_{
            oxm_header_ops::oxm_header(
                oxm_class, oxm_field, oxm_hasmask, oxm_length)
          , 0
        }
    {
    }

    oxm_id(std::uint8_t const oxm_field
         , bool const oxm_hasmask
         , std::uint8_t const oxm_length
         , std::uint32_t const experimenter_id) noexcept
      : oxm_header_{
            oxm_header_ops::oxm_header(
                protocol::OFPXMC_EXPERIMENTER, oxm_field, oxm_hasmask, oxm_length)
          , experimenter_id
        }
    {
    }

    auto oxm_class() const noexcept
      -> std::uint16_t
    {
      return oxm_header_ops::oxm_class(oxm_header_.oxm_header);
    }

    auto oxm_field() const noexcept
      -> std::uint8_t
    {
      return oxm_header_ops::oxm_field(oxm_header_.oxm_header);
    }

    auto oxm_type() const noexcept
      -> std::uint32_t
    {
      return oxm_header_ops::oxm_type(oxm_header_.oxm_header);
    }

    auto oxm_hasmask() const noexcept
      -> bool
    {
      return oxm_header_ops::oxm_hasmask(oxm_header_.oxm_header);
    }

    auto oxm_length() const noexcept
      -> std::uint8_t
    {
      return oxm_header_ops::oxm_length(oxm_header_.oxm_header);
    }

    auto oxm_header() const noexcept
      -> oxm_header_type
    {
      return oxm_header_.oxm_header;
    }

    auto is_experimenter() const noexcept
      -> bool
    {
      return oxm_class() == protocol::OFPXMC_EXPERIMENTER;
    }

    auto experimenter_id() const noexcept
      -> std::uint32_t
    {
      return oxm_header_.experimenter;
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return is_experimenter() ? sizeof(raw_ofp_exp_type) : sizeof(raw_ofp_type);
    };

  private:
    oxm_id(raw_ofp_type const oxm_header
         , std::uint32_t const experimenter_id) noexcept
      : oxm_header_{oxm_header, experimenter_id}
    {
    }

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      if (is_experimenter()) {
        detail::encode(container, oxm_header_);
      }
      else {
        detail::encode(container, oxm_header_.oxm_header);
      }
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> oxm_id
    {
      auto const header = detail::decode<raw_ofp_type>(first, last);
      auto experimenter_id = std::uint32_t{0};
      if (oxm_header_ops::oxm_class(header) == protocol::OFPXMC_EXPERIMENTER) {
        if (std::distance(first, last) < sizeof(experimenter_id)) {
          throw exception{
              protocol::bad_request_code::bad_len
            , "too small data size for oxm_id with experimenter id"
          } << CANARD_NET_OFP_ERROR_INFO();
        }
        experimenter_id = detail::decode<std::uint32_t>(first, last);
      }
      return oxm_id{header, experimenter_id};
    }

    auto equal_impl(oxm_id const& rhs) const noexcept
      -> bool
    {
      if (is_experimenter()) {
        return detail::memcmp(oxm_header_, rhs.oxm_header_);
      }
      else {
        return oxm_header_.oxm_header == rhs.oxm_header_.oxm_header;
      }
    }

    auto equivalent_impl(oxm_id const& rhs) const noexcept
      -> bool
    {
      if (is_experimenter()) {
        return detail::memcmp(oxm_header_, rhs.oxm_header_);
      }
      else {
        return oxm_header_.oxm_header == rhs.oxm_header_.oxm_header;
      }
    }

  private:
    raw_ofp_exp_type oxm_header_;
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OXM_ID_HPP
