#ifndef CANARD_NET_OFP_V13_INSTRUCTIONS_HPP
#define CANARD_NET_OFP_V13_INSTRUCTIONS_HPP

#include <tuple>
#include <canard/network/openflow/v13/instruction/apply_actions.hpp>
#include <canard/network/openflow/v13/instruction/clear_actions.hpp>
#include <canard/network/openflow/v13/instruction/goto_table.hpp>
#include <canard/network/openflow/v13/instruction/meter.hpp>
#include <canard/network/openflow/v13/instruction/write_actions.hpp>
#include <canard/network/openflow/v13/instruction/write_metadata.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    using default_instruction_list = std::tuple<
          instructions::goto_table
        , instructions::write_metadata
        , instructions::write_actions
        , instructions::apply_actions
        , instructions::clear_actions
        , instructions::meter
    >;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_INSTRUCTIONS_HPP
