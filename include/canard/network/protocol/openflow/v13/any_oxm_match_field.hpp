#ifndef CANARD_NETWORK_OPENFLOW_V13_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NETWORK_OPENFLOW_V13_ANY_OXM_MATCH_FIELD_HPP

#include <canard/network/protocol/openflow/detail/config.hpp>

#include <canard/network/protocol/openflow/detail/any_oxm_match_field.hpp>
#include <canard/network/protocol/openflow/v13/decoder/oxm_match_field_decoder.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {

    using any_oxm_match_field
        = detail::any_oxm_match_field<oxm_match_field_decoder>;

    template <class T>
    auto any_cast(any_oxm_match_field const& field)
        -> T const&
    {
        return detail::any_cast<T>(field);
    }

    template <class T>
    auto any_cast(any_oxm_match_field const* field)
        -> T const*
    {
        return detail::any_cast<T>(field);
    }

} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)

namespace canard {
namespace network {
namespace openflow {
namespace detail {

    extern template class any_oxm_match_field<openflow::v13::oxm_match_field_decoder>;

} // namespace detail
} // namespace openflow
} // namespace network
} // namespace canard

# endif
#endif

#endif // CANARD_NETWORK_OPENFLOW_V13_ANY_OXM_MATCH_FIELD_HPP
