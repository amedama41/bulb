#ifndef CANARD_NET_OFP_V13_IMPL_OXM_MATCH_SET_IPP
#define CANARD_NET_OFP_V13_IMPL_OXM_MATCH_SET_IPP

#include <canard/network/openflow/detail/config.hpp>
#include <canard/network/openflow/v13/oxm_match_set.hpp>

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/numeric.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  auto oxm_match_set::length() const noexcept
    -> std::uint16_t
  {
    using boost::adaptors::transformed;
    using pair = container_type::value_type;
    return boost::accumulate(
          oxm_tlvs_
        | transformed([](pair const& p) { return p.second.length(); })
        , std::uint16_t{base_length});
  }

  auto oxm_match_set::begin() const noexcept
    -> const_iterator
  {
    return const_iterator{oxm_tlvs_.begin()};
  }

  auto oxm_match_set::end() const noexcept
    -> const_iterator
  {
    return const_iterator{oxm_tlvs_.end()};
  }

  auto oxm_match_set::cbegin() const noexcept
    -> const_iterator
  {
    return const_iterator{oxm_tlvs_.cbegin()};
  }

  auto oxm_match_set::cend() const noexcept
    -> const_iterator
  {
    return const_iterator{oxm_tlvs_.cend()};
  }

  auto oxm_match_set::empty() const noexcept
    -> bool
  {
    return oxm_tlvs_.empty();
  }

  auto oxm_match_set::size() const noexcept
    -> size_type
  {
    return oxm_tlvs_.size();
  }

  auto oxm_match_set::max_size() const noexcept
    -> size_type
  {
    return oxm_tlvs_.max_size();
  }

  auto oxm_match_set::at(key_type const oxm_type) const
    -> const_reference
  {
    return oxm_tlvs_.at(oxm_type);
  }

  auto oxm_match_set::erase(const_iterator it)
    -> const_iterator
  {
    return const_iterator{oxm_tlvs_.erase(it.base())};
  }

  void oxm_match_set::swap(oxm_match_set& other)
  {
    oxm_tlvs_.swap(other.oxm_tlvs_);
  }

  void oxm_match_set::clear() noexcept
  {
    oxm_tlvs_.clear();
  }

  auto oxm_match_set::find(key_type const oxm_type) const
    -> const_iterator
  {
    return const_iterator{oxm_tlvs_.find(oxm_type)};
  }

  void oxm_match_set::validate_ofp_match(v13_detail::ofp_match const& match)
  {
    if (match.type != protocol::OFPMT_OXM) {
      throw std::runtime_error{"match_type is not OFPMT_OXM"};
    }
    if (match.length < base_length) {
      throw std::runtime_error{"oxm_match length is too short"};
    }
  }

  oxm_match_set::oxm_match_set(container_type&& oxm_tlvs)
    : oxm_tlvs_(std::move(oxm_tlvs))
  {
  }

  auto oxm_match_set::equal_impl(oxm_match_set const& rhs) const
    -> bool
  {
    return oxm_tlvs_ == rhs.oxm_tlvs_;
  }

  auto oxm_match_set::equivalent_impl(oxm_match_set const& rhs) const noexcept
    -> bool
  {
    auto const filter = [](const_reference v) { return !v.is_wildcard(); };
    using boost::adaptors::filtered;
    return boost::equal(
          *this | filtered(filter), rhs | filtered(filter)
        , [](const_reference lhs_tlv, const_reference rhs_tlv)
          { return equivalent(lhs_tlv, rhs_tlv); });
  }

  auto oxm_match_set::make_result(
      container_type::iterator it, bool const result)
    -> std::pair<const_iterator, bool>
  {
    return std::make_pair(const_iterator{it}, result);
  }

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_IMPL_OXM_MATCH_SET_IPP
