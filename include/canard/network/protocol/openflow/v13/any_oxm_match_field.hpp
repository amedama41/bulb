#ifndef CANARD_NETWORK_OPENFLOW_V13_ANY_OXM_MATCH_FIELD_HPP
#define CANARD_NETWORK_OPENFLOW_V13_ANY_OXM_MATCH_FIELD_HPP

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

namespace detail {

#if defined(CANARD_NET_OFP_SUPPRESS_IMPLICIT_INSTANTIATION)

    extern template class any_oxm_match_field<
        openflow::v13::oxm_match_field_decoder
    >;

#elif defined(CANARD_NET_OFP_IMPOSE_EXPLICIT_INSTANTIATION)

    template class any_oxm_match_field<openflow::v13::oxm_match_field_decoder>;

#endif

} // namespace detail

} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_V13_ANY_OXM_MATCH_FIELD_HPP
