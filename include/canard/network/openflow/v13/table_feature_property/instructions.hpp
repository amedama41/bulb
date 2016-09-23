#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_INSTRUCTIONS_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_INSTRUCTIONS_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include <boost/range/numeric.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/is_related.hpp>
#include <canard/network/openflow/detail/padding.hpp>
#include <canard/network/openflow/v13/any_instruction_id.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace table_feature_properties {

    template <class T>
    class basic_prop_instructions
        : public detail::basic_protocol_type<basic_prop_instructions<T>>
    {
    public:
        using raw_ofp_type = v13_detail::ofp_table_feature_prop_instructions;
        using instruction_id_container = std::vector<any_instruction_id>;
        using iterator = instruction_id_container::iterator;
        using const_iterator = instruction_id_container::const_iterator;

        static constexpr protocol::ofp_table_feature_prop_type prop_type
            = T::prop_type;

        explicit basic_prop_instructions(
                instruction_id_container&& instruction_ids)
            : table_feature_prop_instructions_{
                  prop_type
                , std::uint16_t(calc_length(instruction_ids))
              }
            , instruction_ids_(std::move(instruction_ids))
        {
        }

        template <
              class... InstructionIDs
            , class = typename std::enable_if<
                !detail::is_related<
                    InstructionIDs..., basic_prop_instructions>::value
              >::type
        >
        explicit basic_prop_instructions(InstructionIDs&&... instruction_ids)
            : basic_prop_instructions{
                instruction_id_container{
                    std::forward<InstructionIDs>(instruction_ids)...
                }
              }
        {
        }

        basic_prop_instructions(basic_prop_instructions const&) = default;

        basic_prop_instructions(basic_prop_instructions&& other)
            : table_feature_prop_instructions_(
                    other.table_feature_prop_instructions_)
            , instruction_ids_(other.extract_instruction_ids())
        {
        }

        auto operator=(basic_prop_instructions const& other)
            -> basic_prop_instructions&
        {
            return operator=(basic_prop_instructions{other});
        }

        auto operator=(basic_prop_instructions&& other)
            -> basic_prop_instructions&
        {
            auto tmp = std::move(other);
            std::swap(table_feature_prop_instructions_
                    , tmp.table_feature_prop_instructions_);
            instruction_ids_.swap(tmp.instruction_ids_);
            return *this;
        }

        static constexpr auto type() noexcept
            -> protocol::ofp_table_feature_prop_type
        {
            return prop_type;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return table_feature_prop_instructions_.length;
        }

        auto instruction_ids() const noexcept
            -> instruction_id_container const&
        {
            return instruction_ids_;
        }

        auto extract_instruction_ids()
            -> instruction_id_container
        {
            auto instruction_ids = instruction_id_container{};
            instruction_ids.swap(instruction_ids_);
            table_feature_prop_instructions_.length = sizeof(raw_ofp_type);
            return instruction_ids;
        }

        auto begin() noexcept
            -> iterator
        {
            return instruction_ids_.begin();
        }

        auto begin() const noexcept
            -> const_iterator
        {
            return instruction_ids_.begin();
        }

        auto end() noexcept
            -> iterator
        {
            return instruction_ids_.end();
        }

        auto end() const noexcept
            -> const_iterator
        {
            return instruction_ids_.end();
        }

    private:
        basic_prop_instructions(
                  raw_ofp_type const& table_feature_prop_instructions
                , instruction_id_container&& instruction_ids)
            : table_feature_prop_instructions_(table_feature_prop_instructions)
            , instruction_ids_(std::move(instruction_ids))
        {
        }

        static auto calc_length(
                instruction_id_container const& instruction_ids) noexcept
            -> std::size_t
        {
            return boost::accumulate(
                      instruction_ids, sizeof(raw_ofp_type)
                    , [](std::size_t const length, any_instruction_id const& id)
                      { return length + id.length(); });
        }

        friend detail::basic_protocol_type<basic_prop_instructions>;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, table_feature_prop_instructions_);
            boost::for_each(
                    instruction_ids_, [&](any_instruction_id const& id) {
                id.encode(container);
            });
            detail::encode_byte_array(
                      container
                    , detail::padding
                    , v13_detail::padding_length(length()));
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> basic_prop_instructions
        {
            auto const property = detail::decode<raw_ofp_type>(first, last);

            auto const ids_last
                = std::next(first, property.length - sizeof(raw_ofp_type));
            constexpr auto base_id_size = sizeof(v13_detail::ofp_instruction);
            auto ids = instruction_id_container{};
            ids.reserve(std::distance(first, ids_last) / base_id_size);
            while (std::distance(first, ids_last) >= base_id_size) {
                ids.push_back(any_instruction_id::decode(first, ids_last));
            }
            if (first != ids_last) {
                throw std::runtime_error{
                    "invalid table_feature_prop_instructions length"
                };
            }

            std::advance(first, v13_detail::padding_length(property.length));

            return basic_prop_instructions{property, std::move(ids)};
        }

        auto equal_impl(basic_prop_instructions const& rhs) const noexcept
            -> bool
        {
            return instruction_ids_ == rhs.instruction_ids_;
        }

        auto equivalent_impl(basic_prop_instructions const& rhs) const noexcept
            -> bool
        {
            if (instruction_ids_.size() != rhs.instruction_ids_.size()) {
                return false;
            }
            auto const rhs_ids_end = rhs.instruction_ids_.end();
            for (auto const& lhs_id : instruction_ids_) {
                using const_reference
                    = instruction_id_container::const_reference;
                if (boost::find_if(
                              rhs.instruction_ids_
                            , [&lhs_id](const_reference rhs_id)
                              { return equivalent(lhs_id, rhs_id); })
                        == rhs_ids_end) {
                    return false;
                }
            }
            return true;
        }

    private:
        raw_ofp_type table_feature_prop_instructions_;
        instruction_id_container instruction_ids_;
    };

    namespace instructions_detail {

        struct instructions
        {
            static constexpr protocol::ofp_table_feature_prop_type prop_type
                = protocol::OFPTFPT_INSTRUCTIONS;
        };

        struct instructions_miss
        {
            static constexpr protocol::ofp_table_feature_prop_type prop_type
                = protocol::OFPTFPT_INSTRUCTIONS_MISS;
        };

    } // namespace instructions_detail

    using instructions
        = basic_prop_instructions<instructions_detail::instructions>;
    using instructions_miss
        = basic_prop_instructions<instructions_detail::instructions_miss>;

} // namespace table_feature_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_INSTRUCTIONS_HPP

