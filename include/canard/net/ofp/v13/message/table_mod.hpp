#ifndef CANARD_NET_OFP_V13_MESSAGES_TABLE_MOD_HPP
#define CANARD_NET_OFP_V13_MESSAGES_TABLE_MOD_HPP

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

  class table_mod
    : public detail::v13::basic_fixed_length_message<table_mod>
  {
  public:
    static constexpr protocol::ofp_type message_type
      = protocol::OFPT_TABLE_MOD;

    using ofp_type = protocol::ofp_table_mod;

    table_mod(
          std::uint8_t const table_id
        , std::uint32_t const config
        , std::uint32_t const xid = get_xid())
      : table_mod_{
            protocol::ofp_header{version(), type(), sizeof(ofp_type), xid}
          , table_id
          , { 0, 0, 0 }
          , config
        }
    {
    }

    auto header() const noexcept
      -> protocol::ofp_header const&
    {
      return table_mod_.header;
    }

    auto table_id() const noexcept
      -> std::uint8_t
    {
      return table_mod_.table_id;
    }

    auto config() const noexcept
      -> std::uint32_t
    {
      return table_mod_.config;
    }

  private:
    friend basic_fixed_length_message;

    explicit table_mod(ofp_type const& table_mod) noexcept
      : table_mod_(table_mod)
    {
    }

    auto ofp_message() const noexcept
      -> ofp_type const&
    {
      return table_mod_;
    }

  private:
    ofp_type table_mod_;
  };

} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_TABLE_MOD_HPP
