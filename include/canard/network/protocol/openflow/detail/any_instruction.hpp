#ifndef CANARD_NET_OFP_DETAIL_ANY_INSTRUCTION_HPP
#define CANARD_NET_OFP_DETAIL_ANY_INSTRUCTION_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/size.hpp>
#include <boost/operators.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <canard/mpl/adapted/std_tuple.hpp>
#include <canard/network/protocol/openflow/detail/construct.hpp>
#include <canard/network/protocol/openflow/detail/is_related.hpp>
#include <canard/network/protocol/openflow/detail/visitors.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

    template <class InstructionDecoder>
    class any_instruction
        : private boost::equality_comparable<
            any_instruction<InstructionDecoder>
          >
    {
        using ofp_instruction_type
            = typename InstructionDecoder::ofp_instruction_type;
        using instruction_type_list
            = typename InstructionDecoder::instruction_type_list;
        using instruction_variant
            = typename boost::make_variant_over<instruction_type_list>::type;

        template <class Instruction>
        using containable_if_t = typename std::enable_if<
            boost::mpl::contains<instruction_type_list, Instruction>::value
        >::type;

    public:
        static constexpr std::size_t number_of_types
            = boost::mpl::size<instruction_type_list>::type::value;

        template <
              class Instruction
            , typename std::enable_if<
                !detail::is_related<any_instruction, Instruction>::value
              >::type* = nullptr
        >
        any_instruction(Instruction&& instruction)
            : variant_(std::forward<Instruction>(instruction))
        {
        }

        template <
              class Instruction
            , typename std::enable_if<
                !detail::is_related<any_instruction, Instruction>::value
              >::type* = nullptr
        >
        auto operator=(Instruction&& instruction)
            -> any_instruction&
        {
            variant_ = std::forward<Instruction>(instruction);
            return *this;
        }

        CANARD_NET_OFP_DECL auto type() const noexcept
            -> ofp_instruction_type;

        CANARD_NET_OFP_DECL auto length() const noexcept
            -> std::uint16_t;

        CANARD_NET_OFP_DECL auto index() const noexcept
            -> std::size_t;

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            auto visitor = detail::encoding_visitor<Container>{container};
            return boost::apply_visitor(visitor, variant_);
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> any_instruction
        {
            return InstructionDecoder::template decode<any_instruction>(
                    first, last, construct<any_instruction>{});
        }

        template <class Visitor>
        auto visit(Visitor&& visitor)
            -> typename std::remove_reference<Visitor>::type::result_type
        {
            return boost::apply_visitor(
                    std::forward<Visitor>(visitor), variant_);
        }

        template <class Visitor>
        auto visit(Visitor&& visitor) const
            -> typename std::remove_reference<Visitor>::type::result_type
        {
            return boost::apply_visitor(
                    std::forward<Visitor>(visitor), variant_);
        }

        friend auto operator==(
                any_instruction const& lhs, any_instruction const& rhs)
            -> bool
        {
            return lhs.equal_impl(rhs);
        }

        template <class Instruction, class = containable_if_t<Instruction>>
        friend auto operator==(
                any_instruction const& lhs, Instruction const& rhs)
            -> bool
        {
            if (auto const instruction
                    = boost::get<Instruction>(std::addressof(lhs.variant_))) {
                return *instruction == rhs;
            }
            return false;
        }

        template <class Instruction, class = containable_if_t<Instruction>>
        friend auto operator==(
                Instruction const& lhs, any_instruction const& rhs)
            -> bool
        {
            if (auto const instruction
                    = boost::get<Instruction>(std::addressof(rhs.variant_))) {
                return lhs == *instruction;
            }
            return false;
        }

        template <class Instruction, class = containable_if_t<Instruction>>
        friend auto operator!=(
                any_instruction const& lhs, Instruction const& rhs)
            -> bool
        {
            return !(lhs == rhs);
        }

        template <class Instruction, class = containable_if_t<Instruction>>
        friend auto operator!=(
                Instruction const& lhs, any_instruction const& rhs)
            -> bool
        {
            return !(lhs == rhs);
        }

        friend auto equivalent(
                any_instruction const& lhs, any_instruction const& rhs) noexcept
            -> bool
        {
            return lhs.equivalent_impl(rhs);
        }

        template <class Instruction, class = containable_if_t<Instruction>>
        friend auto equivalent(
                any_instruction const& lhs, Instruction const& rhs) noexcept
            -> bool
        {
            if (auto const instruction
                    = boost::get<Instruction>(std::addressof(lhs.variant_))) {
                return equivalent(*instruction, rhs);
            }
            return false;
        }

        template <class Instruction, class = containable_if_t<Instruction>>
        friend auto equivalent(
                Instruction const& lhs, any_instruction const& rhs) noexcept
            -> bool
        {
            if (auto const instruction
                    = boost::get<Instruction>(std::addressof(rhs.variant_))) {
                return equivalent(lhs, *instruction);
            }
            return false;
        }

        template <class T, class Decoder>
        friend auto any_cast(any_instruction<Decoder> const&)
            -> T const&;

        template <class T, class Decoder>
        friend auto any_cast(any_instruction<Decoder> const*)
            -> T const*;

    private:
        CANARD_NET_OFP_DECL auto equal_impl(any_instruction const& rhs) const
            -> bool;

        CANARD_NET_OFP_DECL auto equivalent_impl(
                any_instruction const& rhs) const noexcept
            -> bool;

        instruction_variant variant_;
    };

    template <class T, class InstructionDecoder>
    auto any_cast(any_instruction<InstructionDecoder> const& instruction)
        -> T const&
    {
        return boost::get<T>(instruction.variant_);
    }

    template <class T, class InstructionDecoder>
    auto any_cast(any_instruction<InstructionDecoder> const* const instruction)
        -> T const*
    {
        return boost::get<T>(std::addressof(instruction->variant_));
    }

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY) || !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   include <canard/network/protocol/openflow/detail/impl/any_instruction.hpp>
#endif

#endif // CANARD_NET_OFP_DETAIL_ANY_INSTRUCTION_HPP
