#ifndef CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_FEATURES_HPP
#define CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_FEATURES_HPP

#include <cstdint>
#include <array>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/v13/detail/basic_multipart.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

  class group_features_request
    : public multipart_detail::basic_multipart_request<
        group_features_request, void
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_GROUP_FEATURES;

    explicit group_features_request(std::uint32_t const xid = get_xid())
      : basic_multipart_request{0, xid}
    {
    }

  private:
    friend basic_multipart_request::base_type;

    explicit group_features_request(ofp_type const& multipart_request) noexcept
      : basic_multipart_request{multipart_request}
    {
    }
  };

  class group_features_reply
    : public multipart_detail::basic_multipart_reply<
        group_features_reply, protocol::ofp_group_features
      >
  {
  public:
    static constexpr protocol::ofp_multipart_type multipart_type_value
      = protocol::OFPMP_GROUP_FEATURES;

    using max_groups_type = std::array<std::uint32_t, 4>;
    using actions_type = std::array<std::uint32_t, 4>;

    group_features_reply(
          std::uint32_t const types
        , std::uint32_t const capabilities
        , max_groups_type const& max_groups
        , actions_type const& actions
        , std::uint32_t const xid = get_xid())
      : basic_multipart_reply{
            0
          , body_type{
                types
              , capabilities
              , { max_groups[0], max_groups[1], max_groups[2], max_groups[3] }
              , { actions[0], actions[1], actions[2], actions[3] }
            }
          , xid
        }
    {
    }

    auto types() const noexcept
      -> std::uint32_t
    {
      return body().types;
    }

    auto capabilities() const noexcept
      -> std::uint32_t
    {
      return body().capabilities;
    }

    auto max_groups() const noexcept
      -> max_groups_type
    {
      auto const& max_groups = body().max_groups;
      return {{ max_groups[0], max_groups[1], max_groups[2], max_groups[3] }};
    }

    auto actions() const noexcept
      -> actions_type
    {
      auto const& actions = body().actions;
      return {{ actions[0], actions[1], actions[2], actions[3] }};
    }

  private:
    friend basic_multipart_reply::base_type;

    group_features_reply(
          ofp_type const& multipart_reply
        , body_type const& group_features) noexcept
      : basic_multipart_reply{multipart_reply, group_features}
    {
    }
  };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGE_MULTIPART_GROUP_FEATURES_HPP
