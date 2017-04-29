#ifndef CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP
#define CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP

#include <cstdint>
#include <utility>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v10/common/port.hpp>
#include <canard/net/ofp/v10/detail/basic_fixed_length_message.hpp>
#include <canard/net/ofp/v10/detail/basic_message.hpp>
#include <canard/net/ofp/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

  class features_request
    : public v10_detail::basic_fixed_length_message<features_request>
  {
  public:
    using ofp_type = protocol::ofp_header;

    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_FEATURES_REQUEST;

    explicit features_request(std::uint32_t const xid = get_xid()) noexcept
      : header_{protocol::OFP_VERSION, message_type, sizeof(ofp_type), xid}
    {
    }

    auto header() const noexcept
      -> protocol::ofp_header
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
    : public v10_detail::basic_message<features_reply>
  {
  public:
    using ofp_type = protocol::ofp_switch_features;
    using ports_type = ofp::list<port>;

    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_FEATURES_REPLY;

    features_reply(
          std::uint64_t const dpid
        , std::uint32_t const n_buffers
        , std::uint8_t const n_tables
        , std::uint32_t const capabilities
        , std::uint32_t const actions
        , ports_type ports
        , std::uint32_t const xid = get_xid())
      : switch_features_{
            protocol::ofp_header{
                protocol::OFP_VERSION
              , message_type
              , ports.calc_ofp_length(sizeof(ofp_type))
              , xid
            }
          , dpid
          , n_buffers
          , n_tables
          , { 0, 0, 0 }
          , capabilities
          , actions
        }
      , ports_(std::move(ports))
    {
    }

    features_reply(
          features_request const& request
        , std::uint64_t const dpid
        , std::uint32_t const n_buffers
        , std::uint8_t const n_tables
        , std::uint32_t const capabilities
        , std::uint32_t const actions
        , ports_type ports)
      : features_reply{
            dpid, n_buffers, n_tables, capabilities, actions
          , std::move(ports), request.xid()
        }
    {
    }

    features_reply(features_reply const&) = default;

    features_reply(features_reply&& other)
      : switch_features_(other.switch_features_)
      , ports_(other.extract_ports())
    {
    }

    auto operator=(features_reply const& other)
      -> features_reply&
    {
      return operator=(features_reply{other});
    }

    auto operator=(features_reply&& other)
      -> features_reply&
    {
      auto tmp = std::move(other);
      std::swap(switch_features_, tmp.switch_features_);
      ports_.swap(tmp.ports_);
      return *this;
    }

    auto header() const noexcept
      -> protocol::ofp_header
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

    auto capabilities() const noexcept
      -> std::uint32_t
    {
      return switch_features_.capabilities;
    }

    auto actions() const noexcept
      -> std::uint32_t
    {
      return switch_features_.actions;
    }

    auto ports() const noexcept
      -> ports_type const&
    {
      return ports_;
    }

    auto extract_ports()
      -> ports_type
    {
      auto tmp = ports_type{};
      tmp.swap(ports_);
      switch_features_.header.length = sizeof(ofp_type);
      return tmp;
    }

  private:
    friend basic_message;

    static constexpr bool is_fixed_length_message = false;

    friend constexpr auto additonally_check_message_length(
          v10_detail::basic_message_tag<features_reply>
        , std::uint16_t const length) noexcept
      -> bool
    {
      return (length - sizeof(ofp_type)) % port::min_length() == 0;
    }

    friend basic_message::basic_protocol_type;

    features_reply(ofp_type const& features, ports_type&& ports)
      : switch_features_(features)
      , ports_(std::move(ports))
    {
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, switch_features_);
      ports_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> features_reply
    {
      auto const features = detail::decode<ofp_type>(first, last);

      auto const ports_length = features.header.length - sizeof(ofp_type);
      last = std::next(first, ports_length);

      auto ports = ports_type::decode(first, last);

      return features_reply{features, std::move(ports)};
    }

    auto equal_impl(features_reply const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(switch_features_, rhs.switch_features_)
          && ports_ == rhs.ports_;
    }

  private:
    ofp_type switch_features_;
    ports_type ports_;
  };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_SWITCH_FEATURES_HPP
