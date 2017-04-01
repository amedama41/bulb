#ifndef CANARD_NET_OFP_V13_HELLO_ELEMENTS_HPP
#define CANARD_NET_OFP_V13_HELLO_ELEMENTS_HPP

#include <tuple>
#include <canard/net/ofp/v13/hello_element/unknown_element.hpp>
#include <canard/net/ofp/v13/hello_element/versionbitmap.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    using default_hello_element_list = std::tuple<
        hello_elements::versionbitmap
    >;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_HELLO_ELEMENTS_HPP
