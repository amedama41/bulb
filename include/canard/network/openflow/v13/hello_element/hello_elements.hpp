#ifndef CANARD_NET_OFP_V13_HELLO_ELEM_VERSIONBITMAP_HPP
#define CANARD_NET_OFP_V13_HELLO_ELEM_VERSIONBITMAP_HPP

#include <tuple>
#include <canard/network/openflow/v13/hello_element/unknown_element.hpp>
#include <canard/network/openflow/v13/hello_element/versionbitmap.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    using hello_element_list = std::tuple<
          hello_elements::versionbitmap
        , hello_elements::unknown_element
    >;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_HELLO_ELEM_VERSIONBITMAP_HPP
