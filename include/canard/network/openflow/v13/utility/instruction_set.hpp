#ifndef CANARD_NET_OFP_V13_INSTRUCTION_SET_HPP
#define CANARD_NET_OFP_V13_INSTRUCTION_SET_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstdint>
#include <canard/network/openflow/detail/set_base.hpp>
#include <canard/network/openflow/v13/any_instruction.hpp>
#include <canard/network/openflow/v13/instruction_order.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class instruction_set
    : public detail::set_base<instruction_set, std::uint64_t, any_instruction>
  {
  public:
    using set_base::set_base;

  private:
    friend set_base;

    using set_info = set_base::default_set_info;

    template <class Instruction>
    static constexpr auto key_impl() noexcept
      -> key_type
    {
      return instruction_order<Instruction>::value;
    };

    template <class Instruction>
    static auto get_key_impl(Instruction const& instruction) noexcept
      -> key_type
    {
      return get_order(instruction);
    }

    template <class T>
    static auto cast_impl(const_reference instruction)
      -> T const&
    {
      return v13::any_cast<T>(instruction);
    }
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY) && defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
namespace canard {
namespace net {
namespace ofp {

  extern template class detail::set_base<
    v13::instruction_set, std::uint64_t, v13::any_instruction
  >;

} // namespace ofp
} // namespace net
} // namespace canard
#else
# include <canard/network/openflow/v13/impl/instruction_set.ipp>
#endif

#endif // CANARD_NET_OFP_V13_INSTRUCTION_SET_HPP
