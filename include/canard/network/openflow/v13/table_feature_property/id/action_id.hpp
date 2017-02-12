#ifndef CANARD_NET_OFP_V13_ACTION_ID_HPP
#define CANARD_NET_OFP_V13_ACTION_ID_HPP

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
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

    class action_id
        : public detail::basic_protocol_type<action_id>
    {
    public:
        using raw_ofp_type = protocol::ofp_action_header;

        explicit action_id(std::uint16_t const type) noexcept
            : type_(type)
        {
        }

        auto type() const noexcept
            -> protocol::ofp_action_type
        {
            return protocol::ofp_action_type(type_);
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return offsetof(raw_ofp_type, pad);
        }

        static void validate_action_header(
                protocol::ofp_action_header const& header)
        {
            if (header.type == protocol::OFPAT_EXPERIMENTER) {
                throw std::runtime_error{"invalid action type"};
            }
            if (header.len != offsetof(protocol::ofp_action_header, pad)) {
                throw std::runtime_error{"action id length must be 4"};
            }
        }

    private:
        friend basic_protocol_type;

        friend constexpr auto get_min_length(
                detail::basic_protocol_type_tag<action_id>) noexcept
            -> std::uint16_t
        {
            return action_id::length();
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(
                      container
                    , raw_ofp_type{std::uint16_t(type()), length()}
                    , std::integral_constant<std::size_t, length()>{});
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> action_id
        {
            auto const action_header = detail::decode<raw_ofp_type>(
                      first, last
                    , std::integral_constant<std::size_t, length()>{});
            return action_id{action_header.type};
        }

        auto equal_impl(action_id const& rhs) const noexcept
            -> bool
        {
            return type_ == rhs.type_;
        }

        auto equivalent_impl(action_id const& rhs) const noexcept
            -> bool
        {
            return type_ == rhs.type_;
        }

    private:
        std::uint16_t type_;
    };


    class action_experimenter_id
        : public detail::basic_protocol_type<action_experimenter_id>
    {
    public:
        using raw_ofp_type = protocol::ofp_action_experimenter_header;
        using data_type = ofp::data_type;

        explicit action_experimenter_id(std::uint32_t const experimenter)
            : experimenter_(experimenter)
            , data_{}
        {
        }

        action_experimenter_id(std::uint32_t const experimenter, data_type data)
            : experimenter_(experimenter)
            , data_(std::move(data))
        {
        }

        static constexpr auto type() noexcept
            -> protocol::ofp_action_type
        {
            return protocol::OFPAT_EXPERIMENTER;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type) + data_.size();
        }

        auto experimenter() const noexcept
            -> std::uint32_t
        {
            return experimenter_;
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
            return data;
        }

        static void validate_action_header(
                protocol::ofp_action_header const& header)
        {
            if (header.type != protocol::OFPAT_EXPERIMENTER) {
                throw std::runtime_error{"invalid action type"};
            }
            if (header.len < min_length()) {
                throw std::runtime_error{
                    "experimenter action id length is too small"
                };
            }
        }

    private:
        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            auto const exp_header = raw_ofp_type{
                std::uint16_t(type()), length(), experimenter()
            };
            detail::encode(container, exp_header);
            detail::encode_byte_array(container, data_.data(), data_.size());
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> action_experimenter_id
        {
            auto const exp_header = detail::decode<raw_ofp_type>(first, last);

            auto data = ofp::decode_data(
                    first, exp_header.len - sizeof(raw_ofp_type));

            return action_experimenter_id{
                exp_header.experimenter, std::move(data)
            };
        }

        auto equal_impl(action_experimenter_id const& rhs) const noexcept
            -> bool
        {
            return experimenter_ == rhs.experimenter_
                && data_ == rhs.data_;
        }

        auto equivalent_impl(action_experimenter_id const& rhs) const noexcept
            -> bool
        {
            return experimenter_ == rhs.experimenter_
                && data_ == rhs.data_;
        }

    private:
        std::uint32_t experimenter_;
        data_type data_;
    };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_ACTION_ID_HPP
