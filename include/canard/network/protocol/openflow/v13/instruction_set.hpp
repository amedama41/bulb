#ifndef CANARD_NETWORK_OPENFLOW_V13_INSTRUCTION_SET_HPP
#define CANARD_NETWORK_OPENFLOW_V13_INSTRUCTION_SET_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <boost/operators.hpp>
#include <boost/optional/optional.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/iterator.hpp>
#include <canard/network/protocol/openflow/detail/is_related.hpp>
#include <canard/network/protocol/openflow/v13/decoder/instruction_decoder.hpp>
#include <canard/network/protocol/openflow/v13/any_instruction.hpp>
#include <canard/network/protocol/openflow/v13/instruction_order.hpp>
#include <canard/network/protocol/openflow/v13/openflow.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {

    class instruction_set
        : private boost::equality_comparable<instruction_set>
    {
        using container_type = std::map<std::uint64_t, any_instruction>;

    public:
        using key_type = std::uint64_t;
        using value_type = container_type::mapped_type;
        using reference = value_type const&;
        using const_reference = value_type const&;
        using iterator = boost::range_iterator<
            boost::select_second_const_range<container_type>
        >::type;
        using const_iterator = iterator;
        using difference_type = container_type::difference_type;
        using size_type = container_type::size_type;

        template <
              class... Instructions
            , class = typename std::enable_if<
                    !detail::is_related<instruction_set, Instructions...>::value
              >::type
        >
        instruction_set(Instructions&&... instructions)
            : instructions_{to_pair(instructions)...}
        {
        }

        CANARD_NET_OFP_DECL auto begin() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto end() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto cbegin() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto cend() const noexcept
            -> const_iterator;

        CANARD_NET_OFP_DECL auto empty() const noexcept
            -> bool;

        CANARD_NET_OFP_DECL auto size() const noexcept
            -> size_type;

        CANARD_NET_OFP_DECL auto max_size() const noexcept
            -> size_type;

        template <class Instruction>
        auto get() const
            -> Instruction const&
        {
            constexpr auto order = instruction_order<Instruction>::value;
            auto const it = instructions_.find(order);
            return v13::any_cast<Instruction>(it->second);
        }

        CANARD_NET_OFP_DECL auto at(key_type const) const
            -> const_reference;

        template <class Instruction>
        auto at() const
            -> Instruction const&
        {
            constexpr auto order = instruction_order<Instruction>::value;
            return v13::any_cast<Instruction>(at(order));
        }

        template <class Instruction>
        auto insert(Instruction&& instruction)
            -> std::pair<const_iterator, bool>
        {
            auto const order = get_order(instruction);
            auto const result = instructions_.emplace(
                    order, std::forward<Instruction>(instruction));
            return make_result(result.first, result.second);
        }

        template <class Instruction>
        auto assign(Instruction&& instruction)
            -> std::pair<const_iterator, bool>
        {
            auto const it = instructions_.find(get_order(instruction));
            if (it == instructions_.end()) {
                return make_result(instructions_.end(), false);
            }
            it->second = std::forward<Instruction>(instruction);
            return make_result(it, true);
        }

        template <class Instruction>
        auto insert_or_assign(Instruction&& instruction)
            -> std::pair<const_iterator, bool>
        {
            auto const order = get_order(instruction);
            auto const it = instructions_.lower_bound(order);
            if (it != instructions_.end()
                    && !instructions_.key_comp()(order, it->first)) {
                it->second = std::forward<Instruction>(instruction);
                return make_result(it, false);
            }
            else {
                auto const result = instructions_.emplace_hint(
                        it, order, std::forward<Instruction>(instruction));
                return make_result(result, true);
            }
        }

        template <class Instruction>
        auto erase()
            -> size_type
        {
            constexpr auto order = instruction_order<Instruction>::value;
            return erase(order);
        }

        CANARD_NET_OFP_DECL auto erase(key_type const)
            -> size_type;

        CANARD_NET_OFP_DECL auto erase(const_iterator)
            -> const_iterator;

        CANARD_NET_OFP_DECL void swap(instruction_set&);

        CANARD_NET_OFP_DECL void clear() noexcept;

        CANARD_NET_OFP_DECL auto find(key_type const) const
            -> const_iterator;

        template <class Instruction>
        auto find() const
            -> boost::optional<Instruction const&>
        {
            constexpr auto order = instruction_order<Instruction>::value;
            auto const it = instructions_.find(order);
            if (it == instructions_.end()) {
                return boost::none;
            }
            return v13::any_cast<Instruction>(it->second);
        }

        CANARD_NET_OFP_DECL auto length() const noexcept
            -> std::size_t;

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            boost::for_each(*this, [&](const_reference instruction) {
                instruction.encode(container);
            });
            return container;
        }

        template <class Iterator>
        static auto decode(Iterator& first, Iterator last)
            -> instruction_set
        {
            auto instructions = container_type{};
            while (std::distance(first, last)
                    >= sizeof(v13_detail::ofp_instruction)) {
                instruction_decoder::decode<void>(
                        first, last, instruction_inserter{instructions});
            }
            if (first != last) {
                throw std::runtime_error{"invalid instruction set length"};
            }
            return instruction_set{std::move(instructions)};
        }

        friend auto operator==(
                instruction_set const& lhs, instruction_set const& rhs)
            -> bool
        {
            return lhs.equal_impl(rhs);
        }

        friend auto equivalent(
                instruction_set const& lhs, instruction_set const& rhs) noexcept
            -> bool
        {
            return lhs.equivalent_impl(rhs);
        }

    private:
        CANARD_NET_OFP_DECL explicit instruction_set(container_type&&);

        CANARD_NET_OFP_DECL auto equal_impl(instruction_set const&) const
            -> bool;

        CANARD_NET_OFP_DECL auto equivalent_impl(
                instruction_set const&) const noexcept
            -> bool;

        template <class Instruction>
        static auto to_pair(Instruction&& instruction)
            -> container_type::value_type
        {
            auto const instruction_order = get_order(instruction);
            return container_type::value_type{
                instruction_order, std::forward<Instruction>(instruction)
            };
        }

        CANARD_NET_OFP_DECL static auto make_result(
                container_type::iterator it, bool const result)
            -> std::pair<const_iterator, bool>;

        struct instruction_inserter
        {
            template <class Instruction>
            void operator()(Instruction&& instruction) const
            {
                auto const order = get_order(instruction);
                auto const result = instructions.emplace(
                        order, std::forward<Instruction>(instruction));
                if (!result.second) {
                    throw std::runtime_error{"duplicated instruction type"};
                }
            }

            container_type& instructions;
        };

    private:
        container_type instructions_;
    };

} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#if defined(CANARD_NET_OFP_HEADER_ONLY)
# include <canard/network/protocol/openflow/v13/impl/instruction_set.ipp>
#endif

#endif // CANARD_NETWORK_OPENFLOW_V13_INSTRUCTION_SET_HPP
