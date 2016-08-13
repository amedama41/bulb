#ifndef CANARD_NET_OFP_V13_IMPL_INSTRUCTION_SET_IPP
#define CANARD_NET_OFP_V13_IMPL_INSTRUCTION_SET_IPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/instruction_set.hpp>

#include <cstddef>
#include <utility>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/numeric.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  auto instruction_set::begin() const noexcept
    -> const_iterator
  {
    return const_iterator{instructions_.begin()};
  }

  auto instruction_set::end() const noexcept
    -> const_iterator
  {
    return const_iterator{instructions_.end()};
  }

  auto instruction_set::cbegin() const noexcept
    -> const_iterator
  {
    return const_iterator{instructions_.begin()};
  }

  auto instruction_set::cend() const noexcept
    -> const_iterator
  {
    return const_iterator{instructions_.end()};
  }

  auto instruction_set::empty() const noexcept
    -> bool
  {
    return instructions_.empty();
  }

  auto instruction_set::size() const noexcept
    -> size_type
  {
    return instructions_.size();
  }

  auto instruction_set::max_size() const noexcept
    -> size_type
  {
    return instructions_.max_size();
  }

  auto instruction_set::at(key_type const instruction_order) const
    -> const_reference
  {
    return instructions_.at(instruction_order);
  }

  auto instruction_set::erase(key_type const instruction_order)
    -> size_type
  {
    return instructions_.erase(instruction_order);
  }

  auto instruction_set::erase(const_iterator it)
    -> const_iterator
  {
    return const_iterator{instructions_.erase(it.base())};
  }

  void instruction_set::swap(instruction_set& other)
  {
    instructions_.swap(other.instructions_);
  }

  void instruction_set::clear() noexcept
  {
    instructions_.clear();
  }

  auto instruction_set::find(key_type const instruction_order) const
    -> const_iterator
  {
    return const_iterator{instructions_.find(instruction_order)};
  }

  auto instruction_set::length() const noexcept
    -> std::size_t
  {
    using boost::adaptors::transformed;
    return boost::accumulate(
          *this
          | transformed([](const_reference e) { return e.length(); })
        , std::size_t{0});
  }

  instruction_set::instruction_set(container_type&& instructions)
    : instructions_(std::move(instructions))
  {
  }

  auto instruction_set::equal_impl(instruction_set const& rhs) const
    -> bool
  {
    return instructions_ == rhs.instructions_;
  }

  auto instruction_set::equivalent_impl(
      instruction_set const& rhs) const noexcept
    -> bool
  {
    return boost::equal(
          *this, rhs
        , [](const_reference lhs_inst, const_reference rhs_inst)
          { return equivalent(lhs_inst, rhs_inst); });
  }

  auto instruction_set::make_result(
      container_type::iterator it, bool const result)
    -> std::pair<const_iterator, bool>
  {
    return std::make_pair(const_iterator{it}, result);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_IMPL_INSTRUCTION_SET_IPP
