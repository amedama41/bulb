#ifndef CANARD_NET_OFP_DATA_TYPE_HPP
#define CANARD_NET_OFP_DATA_TYPE_HPP

#include <cstdint>
#include <algorithm>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <boost/container/container_fwd.hpp>
#include <boost/container/vector.hpp>

namespace canard {
namespace net {
namespace ofp {

  using data_type = boost::container::vector<std::uint8_t>;

  namespace data_type_detail {

    template <class Iterator>
    auto decode_data(Iterator& first, std::size_t const data_length)
      -> data_type
    {
      auto data = data_type{data_length, boost::container::default_init};
      auto const last = std::next(first, data_length);
      std::copy(first, last, data.data());
      first = last;
      return data;
    }

  } // namespace data_type_detail

  using data_type_detail::decode_data;

  inline auto calc_ofp_length(
      data_type const& data, std::uint16_t const base_length)
    -> std::uint16_t
  {
    auto const max_length
      = std::uint16_t(std::numeric_limits<std::uint16_t>::max() - base_length);
    if (data.size() > max_length) {
      throw std::runtime_error{"too large data length"};
    }
    return base_length + data.size();
  }

} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DATA_TYPE_HPP
