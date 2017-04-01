#ifndef CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_INSTRUCTIONS_HPP
#define CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_INSTRUCTIONS_HPP

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/type_traits/is_all_constructible.hpp>
#include <canard/net/ofp/v13/detail/basic_table_feature_property.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/openflow.hpp>
#include <canard/net/ofp/v13/table_feature_property/id/instruction_id.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace table_feature_properties {

  namespace instructions_detail {

    template <class T>
    class basic_prop_instructions
      : public detail::v13::basic_table_feature_property<T>
    {
      using base_t = detail::v13::basic_table_feature_property<T>;

    public:
      using raw_ofp_type = protocol::ofp_table_feature_prop_instructions;
      using instruction_ids_type = ofp::list<instruction_id>;
      using iterator = instruction_ids_type::iterator;
      using const_iterator = instruction_ids_type::const_iterator;

    private:
      template <class... InstructionIDs>
      using enable_if_is_all_constructible_t = typename std::enable_if<
        type_traits::is_all_constructible<
          instruction_ids_type::value_type, InstructionIDs...
        >::value
      >::type;

    public:
      explicit basic_prop_instructions(instruction_ids_type&& instruction_ids)
        : table_feature_prop_instructions_{
              base_t::type()
            , instruction_ids.calc_ofp_length(sizeof(raw_ofp_type))
          }
        , instruction_ids_(std::move(instruction_ids))
      {
      }

      template <
          class... InstructionIDs
        , class = enable_if_is_all_constructible_t<InstructionIDs...>
      >
      explicit basic_prop_instructions(InstructionIDs&&... instruction_ids)
        : basic_prop_instructions{
            instruction_ids_type{
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
        std::swap(
              table_feature_prop_instructions_
            , tmp.table_feature_prop_instructions_);
        instruction_ids_.swap(tmp.instruction_ids_);
        return *this;
      }

      auto length() const noexcept
        -> std::uint16_t
      {
        return table_feature_prop_instructions_.length;
      }

      auto instruction_ids() const noexcept
        -> instruction_ids_type const&
      {
        return instruction_ids_;
      }

      auto extract_instruction_ids()
        -> instruction_ids_type
      {
        auto instruction_ids = instruction_ids_type{};
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

    protected:
      basic_prop_instructions(
            raw_ofp_type const& table_feature_prop_instructions
          , instruction_ids_type&& instruction_ids)
        : table_feature_prop_instructions_(table_feature_prop_instructions)
        , instruction_ids_(std::move(instruction_ids))
      {
      }

    private:
      friend typename base_t::basic_protocol_type;

      friend constexpr auto exclude_padding(
          detail::basic_protocol_type_tag<T>) noexcept
        -> bool
      {
        return true;
      }

      template <class Container>
      void encode_impl(Container& container) const
      {
        detail::encode(container, table_feature_prop_instructions_);
        boost::for_each(
              instruction_ids_
            , [&](instruction_id const& id) { id.encode(container); });
      }

      template <class Iterator>
      static auto decode_impl(Iterator& first, Iterator last)
        -> T
      {
        auto const property = detail::decode<raw_ofp_type>(first, last);

        last = std::next(first, property.length - sizeof(raw_ofp_type));
        auto ids = instruction_ids_type::decode(first, last);

        return T{property, std::move(ids)};
      }

      auto equal_impl(T const& rhs) const noexcept
        -> bool
      {
        return instruction_ids_ == rhs.instruction_ids_;
      }

      auto equivalent_impl(T const& rhs) const noexcept
        -> bool
      {
        if (instruction_ids_.size() != rhs.instruction_ids_.size()) {
          return false;
        }
        auto const rhs_ids_end = rhs.instruction_ids_.end();
        for (auto const& lhs_id : instruction_ids_) {
          using const_reference = instruction_ids_type::const_reference;
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
      instruction_ids_type instruction_ids_;
    };

  } // namespace instructions_detail


  class instructions
    : public instructions_detail::basic_prop_instructions<instructions>
  {
  public:
    static constexpr protocol::ofp_table_feature_prop_type prop_type
      = protocol::OFPTFPT_INSTRUCTIONS;

    using basic_prop_instructions::basic_prop_instructions;

    friend basic_prop_instructions;
  };

  class instructions_miss
    : public instructions_detail::basic_prop_instructions<instructions_miss>
  {
  public:
    static constexpr protocol::ofp_table_feature_prop_type prop_type
      = protocol::OFPTFPT_INSTRUCTIONS_MISS;

    using basic_prop_instructions::basic_prop_instructions;

    friend basic_prop_instructions;
  };

} // namespace table_feature_properties
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_TABLE_FEATURE_PROPERTIES_INSTRUCTIONS_HPP
