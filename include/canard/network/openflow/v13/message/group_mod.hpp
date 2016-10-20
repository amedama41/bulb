#ifndef CANARD_NET_OFP_V13_GROUP_MOD_HPP
#define CANARD_NET_OFP_V13_GROUP_MOD_HPP

#include <cstdint>
#include <iterator>
#include <utility>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/v13/bucket.hpp>
#include <canard/network/openflow/v13/detail/basic_openflow_message.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    namespace group_mod_detail {

        template <class GroupMod>
        class basic_group_mod
            : public detail::v13::basic_openflow_message<GroupMod>
        {
            using base_t = detail::v13::basic_openflow_message<GroupMod>;

        public:
            static protocol::ofp_type const message_type
                = protocol::OFPT_GROUP_MOD;

            using raw_ofp_type = v13_detail::ofp_group_mod;
            using buckets_type = ofp::list<bucket>;

            auto header() const noexcept
                -> v13_detail::ofp_header const&
            {
                return group_mod_.header;
            }

            static constexpr auto command() noexcept
                -> protocol::ofp_group_mod_command
            {
                return GroupMod::command_type;
            }

            auto group_id() const noexcept
                -> std::uint32_t
            {
                return group_mod_.group_id;
            }

            auto group_type() const noexcept
                -> std::uint8_t
            {
                return group_mod_.type;
            }

            auto buckets() const noexcept
                -> buckets_type const&
            {
                return buckets_;
            }

            auto extract_buckets()
                -> buckets_type
            {
                auto buckets = buckets_type{};
                buckets.swap(buckets_);
                group_mod_.header.length = sizeof(raw_ofp_type);
                return buckets;
            }

        protected:
            basic_group_mod(
                      std::uint32_t const group_id
                    , std::uint8_t const group_type
                    , buckets_type&& buckets
                    , std::uint32_t const xid)
                : group_mod_{
                      v13_detail::ofp_header{
                          base_t::version()
                        , base_t::type()
                        , buckets.calc_ofp_length(sizeof(raw_ofp_type))
                        , xid
                      }
                    , command()
                    , group_type
                    , 0
                    , group_id
                  }
                , buckets_(std::move(buckets))
            {
            }

            basic_group_mod(
                    std::uint32_t const group_id, std::uint32_t const xid)
                : group_mod_{
                      v13_detail::ofp_header{
                          base_t::version()
                        , base_t::type()
                        , sizeof(raw_ofp_type)
                        , xid
                      }
                    , command()
                    , 0
                    , 0
                    , group_id
                  }
                , buckets_{}
            {
            }

            basic_group_mod(
                    raw_ofp_type const& group_mod, buckets_type&& buckets)
                : group_mod_(group_mod)
                , buckets_(std::move(buckets))
            {
            }

            basic_group_mod(basic_group_mod const&) = default;

            basic_group_mod(basic_group_mod&& other)
                : group_mod_(other.group_mod_)
                , buckets_(other.extract_buckets())
            {
            }

            auto operator=(basic_group_mod const& other)
                -> basic_group_mod&
            {
                return operator=(basic_group_mod{other});
            }

            auto operator=(basic_group_mod&& other)
                -> basic_group_mod&
            {
                auto tmp = std::move(other);
                std::swap(group_mod_, tmp.group_mod_);
                buckets_.swap(tmp.buckets_);
                return *this;
            }

        private:
            friend typename base_t::basic_protocol_type;

            template <class Container>
            void encode_impl(Container& container) const
            {
                detail::encode(container, group_mod_);
                buckets_.encode(container);
            }

            template <class Iterator>
            static auto decode_impl(Iterator& first, Iterator last)
                -> GroupMod
            {
                auto const group_mod
                    = detail::decode<raw_ofp_type>(first, last);
                auto const buckets_length
                    = group_mod.header.length - sizeof(raw_ofp_type);
                last = std::next(first, buckets_length);

                auto buckets = buckets_type::decode(first, last);

                return GroupMod{group_mod, std::move(buckets)};
            }

        private:
            raw_ofp_type group_mod_;
            buckets_type buckets_;
        };

    } // namespace group_mod_detail


    class group_add
        : public group_mod_detail::basic_group_mod<group_add>
    {
    public:
        static protocol::ofp_group_mod_command const command_type
            = protocol::OFPGC_ADD;

        group_add(std::uint32_t const group_id
                , std::uint8_t const group_type
                , buckets_type buckets
                , std::uint32_t const xid = get_xid())
            : basic_group_mod{group_id, group_type, std::move(buckets), xid}
        {
        }

    private:
        friend basic_group_mod;

        group_add(raw_ofp_type const& group_mod, buckets_type&& buckets)
            : basic_group_mod{group_mod, std::move(buckets)}
        {
        }
    };

    class group_modify
        : public group_mod_detail::basic_group_mod<group_modify>
    {
    public:
        static protocol::ofp_group_mod_command const command_type
            = protocol::OFPGC_MODIFY;

        group_modify(
                  std::uint32_t const group_id
                , std::uint8_t const group_type
                , buckets_type buckets
                , std::uint32_t const xid = get_xid())
            : basic_group_mod{group_id, group_type, std::move(buckets), xid}
        {
        }

    private:
        friend basic_group_mod;

        group_modify(raw_ofp_type const& group_mod, buckets_type&& buckets)
            : basic_group_mod{group_mod, std::move(buckets)}
        {
        }
    };

    class group_delete
        : public group_mod_detail::basic_group_mod<group_delete>
    {
    public:
        static protocol::ofp_group_mod_command const command_type
            = protocol::OFPGC_DELETE;

        explicit group_delete(
                  std::uint32_t const group_id
                , std::uint32_t const xid = get_xid())
            : basic_group_mod{group_id, xid}
        {
        }

    private:
        friend basic_group_mod;

        group_delete(raw_ofp_type const& group_mod, buckets_type&& buckets)
            : basic_group_mod{group_mod, std::move(buckets)}
        {
        }
    };

} // namespace messages

using messages::group_add;
using messages::group_modify;
using messages::group_delete;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_GROUP_MOD_HPP
