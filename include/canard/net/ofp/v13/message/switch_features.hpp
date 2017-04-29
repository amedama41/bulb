#ifndef CANARD_NET_OFP_V13_MESSAGES_SWITCH_FEATURES_HPP
#define CANARD_NET_OFP_V13_MESSAGES_SWITCH_FEATURES_HPP

#include <cstdint>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  class features_request
    : public detail::v13::basic_fixed_length_message<features_request>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_FEATURES_REQUEST;

    using ofp_type = protocol::ofp_header;

    features_request(std::uint32_t const xid = get_xid()) noexcept
      : header_{version(), type(), sizeof(ofp_type), xid}
    {
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return header_;
    }

  private:
    friend basic_fixed_length_message;

    explicit features_request(ofp_type const& header) noexcept
      : header_(header)
    {
    }

    auto ofp_message() const noexcept
      -> ofp_type const&
    {
      return header_;
    }

  private:
    ofp_type header_;
  };


  class features_reply
    : public detail::v13::basic_fixed_length_message<features_reply>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_FEATURES_REPLY;

    using ofp_type = protocol::ofp_switch_features;

    features_reply(
          features_request const& request
        , std::uint64_t const dpid
        , std::uint32_t const n_buffers
        , std::uint8_t const n_tables
        , std::uint8_t const auxiliary_id
        , std::uint32_t const capabilities) noexcept
      : switch_features_{
            protocol::ofp_header{
              version(), type(), sizeof(ofp_type), request.xid()
            }
          , dpid
          , n_buffers
          , n_tables
          , auxiliary_id
          , { 0, 0 }
          , capabilities
          , 0
        }
    {
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return switch_features_.header;
    }

    auto datapath_id() const noexcept
      -> std::uint64_t
    {
      return switch_features_.datapath_id;
    }

    auto num_buffers() const noexcept
      -> std::uint32_t
    {
      return switch_features_.n_buffers;
    }

    auto num_tables() const noexcept
      -> std::uint8_t
    {
      return switch_features_.n_tables;
    }

    auto auxiliary_id() const noexcept
      -> std::uint8_t
    {
      return switch_features_.auxiliary_id;
    }

    auto capabilities() const noexcept
      -> std::uint32_t
    {
      return switch_features_.capabilities;
    }

  private:
    friend basic_fixed_length_message;

    explicit features_reply(ofp_type const& switch_features) noexcept
      : switch_features_(switch_features)
    {
    }

    auto ofp_message() const noexcept
      -> ofp_type const&
    {
      return switch_features_;
    }

  private:
    ofp_type switch_features_;
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_SWITCH_FEATURES_HPP
