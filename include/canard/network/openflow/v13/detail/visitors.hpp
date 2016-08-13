#ifndef CANARD_NET_OFP_V13_VISITORS_HPP
#define CANARD_NET_OFP_V13_VISITORS_HPP

#include <cstdint>
#include <boost/variant/static_visitor.hpp>
#include <canard/network/openflow/detail/visitors.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    namespace v13_detail {

        class calculating_exact_length_visitor
            : public boost::static_visitor<std::uint16_t>
        {
        public:
            template <class T>
            auto operator()(T const& t) const
                -> std::uint16_t
            {
                return v13_detail::exact_length(t.length());
            }
        };

        using detail::oxm_type_visitor;
        using detail::oxm_header_visitor;
        using detail::oxm_has_mask_visitor;
        using detail::oxm_length_visitor;

    } // namespace v13_detail

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_VISITORS_HPP
