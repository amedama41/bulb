#ifndef CANARD_NET_OFP_V13_INSTRUCTION_ID_HPP
#define CANARD_NET_OFP_V13_INSTRUCTION_ID_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>
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
    public:
        using raw_ofp_type = v13_detail::ofp_instruction;

        explicit instruction_id(std::uint16_t const type) noexcept
            : type_{type}
        {
        }

        auto type() const noexcept
            -> protocol::ofp_instruction_type
        {
            return protocol::ofp_instruction_type(type_);
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
        }

        static void validate_instruction_header(
                v13_detail::ofp_instruction const& instruction)
        {
            if (instruction.type == protocol::OFPIT_EXPERIMENTER) {
                throw std::runtime_error{"invalid instruction type"};
            }
            if (instruction.len != length()) {
                throw std::runtime_error{"instruction id length must be 4"};
            }
        }

    private:
        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(
                    container, raw_ofp_type{std::uint16_t(type()), length()});
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> instruction_id
        {
            auto const id = detail::decode<raw_ofp_type>(first, last);
            return instruction_id{id.type};
        }

        auto equal_impl(instruction_id const& rhs) const noexcept
            -> bool
        {
            return type_ == rhs.type_;
        }

        auto equivalent_impl(instruction_id const& rhs) const noexcept
            -> bool
        {
            return type_ == rhs.type_;
        }

    private:
        std::uint16_t type_;
    };


    class instruction_experimenter_id
        : public detail::basic_protocol_type<instruction_experimenter_id>
    {
    public:
        using raw_ofp_type = v13_detail::ofp_instruction_experimenter;
        using data_type = std::vector<unsigned char>;

        explicit instruction_experimenter_id(
                std::uint32_t const experimenter)
            : experimenter_(experimenter)
            , data_{}
        {
        }

        instruction_experimenter_id(
                std::uint32_t const experimenter, data_type data)
            : experimenter_(experimenter)
            , data_(std::move(data))
        {
        }

        static constexpr auto type() noexcept
            -> protocol::ofp_instruction_type
        {
            return protocol::OFPIT_EXPERIMENTER;
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

        static void validate_instruction_header(
                v13_detail::ofp_instruction const& instruction)
        {
            if (instruction.type != protocol::OFPIT_EXPERIMENTER) {
                throw std::runtime_error{"invalid instruction type"};
            }
            if (instruction.len < sizeof(raw_ofp_type)) {
                throw std::runtime_error{
                    "instruction experimenter id length is too small"
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
            -> instruction_experimenter_id
        {
            auto const exp_header = detail::decode<raw_ofp_type>(first, last);

            last = std::next(first, exp_header.len - sizeof(raw_ofp_type));
            auto data = data_type(first, last);
            first = last;
            return instruction_experimenter_id{
                exp_header.experimenter, std::move(data)
            };
        }

        auto equal_impl(instruction_experimenter_id const& rhs) const noexcept
            -> bool
        {
            return experimenter_ == rhs.experimenter_
                && data_ == rhs.data_;
        }

        auto equivalent_impl(
                instruction_experimenter_id const& rhs) const noexcept
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

#endif // CANARD_NET_OFP_V13_INSTRUCTION_ID_HPP
