#ifndef CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_ACTIONS_HPP
#define CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_ACTIONS_HPP

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/detail/memcmp.hpp>
#include <canard/net/ofp/type_traits/is_all_constructible.hpp>
#include <canard/net/ofp/v13/action_list.hpp>
#include <canard/net/ofp/v13/detail/basic_instruction.hpp>
#include <canard/net/ofp/v13/detail/byteorder.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {
namespace v13 {

  template <class T>
  class basic_instruction_actions
    : public basic_instruction<T>
  {
    using base_t = basic_instruction<T>;

  public:
    using ofp_type = ofp::v13::protocol::ofp_instruction_actions;

    auto length() const noexcept
      -> std::uint16_t
    {
      return instruction_actions_.len;
    }

    auto actions() const noexcept
      -> ofp::v13::action_list const&
    {
      return actions_;
    }

    auto extract_actions()
      -> ofp::v13::action_list
    {
      auto actions = ofp::v13::action_list{};
      actions.swap(actions_);
      instruction_actions_.len = sizeof(ofp_type);
      return actions;
    }

  protected:
    template <class... Actions>
    using enable_if_is_all_constructible_t = typename std::enable_if<
      type_traits::is_all_constructible<
        ofp::v13::action_list::value_type, Actions...
      >::value
    >::type;

    explicit basic_instruction_actions(ofp::v13::action_list&& actions)
      : instruction_actions_{
            T::instruction_type
          , actions.calc_ofp_length(sizeof(ofp_type))
          , { 0, 0, 0, 0 }
        }
      , actions_(std::move(actions))
    {
    }

    basic_instruction_actions(
          ofp_type const& instruction_actions
        , ofp::v13::action_list&& actions)
      : instruction_actions_(instruction_actions)
      , actions_(std::move(actions))
    {
    }

    basic_instruction_actions(basic_instruction_actions const&) = default;

    basic_instruction_actions(basic_instruction_actions&& other)
      : instruction_actions_(other.instruction_actions_)
      , actions_(other.extract_actions())
    {
    }

    auto operator=(basic_instruction_actions const& other)
      -> basic_instruction_actions&
    {
      return operator=(basic_instruction_actions{other});
    }

    auto operator=(basic_instruction_actions&& other)
      -> basic_instruction_actions&
    {
      auto tmp = std::move(other);
      std::swap(instruction_actions_, tmp.instruction_actions_);
      actions_.swap(tmp.actions_);
      return *this;
    }

  private:
    friend base_t;

    static constexpr bool is_fixed_length_instruction = false;

    friend typename base_t::basic_protocol_type;

    template <class Validator>
    void validate_impl(Validator&& validator) const
    {
      static_cast<T const*>(this)->validate_instruction(
          std::forward<Validator>(validator));
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, instruction_actions_);
      actions_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> T
    {
      auto const instruction_actions
        = detail::decode<ofp_type>(first, last);
      last = std::next(first, instruction_actions.len - sizeof(ofp_type));
      auto actions = ofp::v13::action_list::decode(first, last);
      return T{instruction_actions, std::move(actions)};
    }

    auto equal_impl(T const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(instruction_actions_, rhs.instruction_actions_)
          && actions_ == rhs.actions_;
    }

    auto equivalent_impl(T const& rhs) const noexcept
      -> bool
    {
      return static_cast<T const*>(this)->is_equivalent_instruction(rhs);
    }

  private:
    ofp_type instruction_actions_;
    ofp::v13::action_list actions_;
  };

} // namespace v13
} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_V13_BASIC_INSTRUCTION_ACTIONS_HPP
