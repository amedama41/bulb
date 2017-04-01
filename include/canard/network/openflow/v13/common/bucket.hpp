#ifndef CANARD_NET_OFP_V13_BUCKET_HPP
#define CANARD_NET_OFP_V13_BUCKET_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/v13/action_list.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/utility/action_set.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    class bucket
        : public detail::basic_protocol_type<bucket>
    {
    public:
        using raw_ofp_type = protocol::ofp_bucket;

        bucket(std::uint16_t const weight
             , std::uint32_t const watch_port
             , std::uint32_t const watch_group
             , action_set actions)
            : bucket_{
                  std::uint16_t(sizeof(bucket_) + actions.length())
                , weight
                , watch_port
                , watch_group
                , { 0, 0, 0, 0 }
              }
            , actions_(std::move(actions).to_list())
        {
        }

        explicit bucket(action_set actions)
            : bucket{
                0, protocol::OFPP_ANY, protocol::OFPG_ANY, std::move(actions)
              }
        {
        }

        bucket(std::uint16_t const weight, action_set actions)
            : bucket{
                  weight
                , protocol::OFPP_ANY, protocol::OFPG_ANY
                , std::move(actions)
              }
        {
        }

        bucket(std::uint32_t const watch_port
             , std::uint32_t const watch_group
             , action_set actions)
            : bucket{0, watch_port, watch_group, std::move(actions)}
        {
        }

        bucket(std::uint16_t const weight
             , std::uint32_t const watch_port
             , std::uint32_t const watch_group
             , action_list actions)
            : bucket_{
                  actions.calc_ofp_length(sizeof(bucket_))
                , weight
                , watch_port
                , watch_group
                , { 0, 0, 0, 0 }
              }
            , actions_(std::move(actions))
        {
        }

        bucket(bucket const&) = default;

        bucket(bucket&& other)
            : bucket_(other.bucket_)
            , actions_(other.extract_actions())
        {
        }

        auto operator=(bucket const& other)
            -> bucket&
        {
            return operator=(bucket{other});
        }

        auto operator=(bucket&& other)
            -> bucket&
        {
            auto tmp = std::move(other);
            std::swap(bucket_, tmp.bucket_);
            actions_.swap(tmp.actions_);
            return *this;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return bucket_.len;
        }

        auto weight() const noexcept
            -> std::uint16_t
        {
            return bucket_.weight;
        }

        auto watch_port() const noexcept
            -> std::uint32_t
        {
            return bucket_.watch_port;
        }

        auto watch_group() const noexcept
            -> std::uint32_t
        {
            return bucket_.watch_group;
        }

        auto actions() const noexcept
            -> action_list const&
        {
            return actions_;
        }

        auto extract_actions()
            -> action_list
        {
            auto actions = action_list{};
            actions.swap(actions_);
            bucket_.len = sizeof(raw_ofp_type);
            return actions;
        }

        static auto all(action_set actions)
            -> bucket
        {
            return bucket{std::move(actions)};
        }

        static auto select(std::uint16_t const weight, action_set actions)
            -> bucket
        {
            return bucket{weight, std::move(actions)};
        }

        static auto indirect(action_set actions)
            -> bucket
        {
            return bucket{std::move(actions)};
        }

        static auto failover(
                  std::uint32_t const watch_port
                , std::uint32_t const watch_group
                , action_set actions)
            -> bucket
        {
            return bucket{watch_port, watch_group, std::move(actions)};
        }

    private:
        bucket(raw_ofp_type const& bkt, action_list&& actions)
            : bucket_(bkt)
            , actions_(std::move(actions))
        {
        }

        friend basic_protocol_type;

        template <class Validator>
        void validate_impl(Validator validator) const
        {
            if (!action_set::is_valid_set(actions_)) {
                throw std::runtime_error{"duplicated action type"};
            }
            validator(actions_);
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, bucket_);
            actions_.encode(container);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> bucket
        {
            auto const bkt = detail::decode<raw_ofp_type>(first, last);

            if (bkt.len < sizeof(raw_ofp_type)) {
                throw exception{
                      exception::ex_error_type::bad_bucket
                    , exception::ex_error_code::bad_length
                    , "too small bucket length"
                } << CANARD_NET_OFP_ERROR_INFO();
            }
            auto const actions_length = bkt.len - sizeof(raw_ofp_type);
            if (std::distance(first, last) < actions_length) {
                throw exception{
                      protocol::bad_request_code::bad_len
                    , "too small data size for bucket"
                } << CANARD_NET_OFP_ERROR_INFO();
            }

            last = std::next(first, actions_length);

            auto actions = action_list::decode(first, last);

            return bucket{bkt, std::move(actions)};
        }

        auto equal_impl(bucket const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(bucket_, rhs.bucket_)
                && actions_ == rhs.actions_;
        }

        auto equivalent_impl(bucket const& rhs) const noexcept
            -> bool
        {
            return length() == rhs.length()
                && weight() == rhs.weight()
                && watch_port() == rhs.watch_port()
                && watch_group() == rhs.watch_group()
                && action_set::equivalent_as_set(actions_, rhs.actions_);
        }

    private:
        raw_ofp_type bucket_;
        action_list actions_;
    };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_BUCKET_HPP
