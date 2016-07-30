#ifndef CANARD_NETWORK_OPENFLOW_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP
#define CANARD_NETWORK_OPENFLOW_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <boost/range/algorithm/fill.hpp>
#include <canard/network/protocol/openflow/detail/is_related.hpp>
#include <canard/network/protocol/openflow/v13/action_list.hpp>
#include <canard/network/protocol/openflow/v13/action_set.hpp>
#include <canard/network/protocol/openflow/v13/detail/basic_instruction_actions.hpp>
#include <canard/network/protocol/openflow/v13/openflow.hpp>

namespace canard {
namespace network {
namespace openflow {
namespace v13 {
namespace instructions {

    class write_actions
        : public detail::v13::basic_instruction_actions<write_actions>
    {
    public:
        static constexpr protocol::ofp_instruction_type instruction_type
            = protocol::OFPIT_WRITE_ACTIONS;

        explicit write_actions(action_list actions)
            : basic_instruction_actions{std::move(actions)}
        {
        }

        explicit write_actions(action_set actions)
            : write_actions{std::move(actions).to_list()}
        {
        }

        template <
              class... Actions
            , typename std::enable_if<
                !detail::is_related<write_actions, Actions...>::value
              >::type* = nullptr
        >
        explicit write_actions(Actions&&... actions)
            : write_actions{action_set{std::forward<Actions>(actions)...}}
        {
        }

    private:
        friend basic_instruction_actions;

        write_actions(
                raw_ofp_type const& instruction_actions, action_list&& actions)
            : basic_instruction_actions{instruction_actions, std::move(actions)}
        {
        }

        static void validate_impl(write_actions const& write_actions)
        {
            if (!action_set::is_action_set(write_actions.actions())) {
                throw std::runtime_error{"duplicated action type"};
            }
        }
    };

    namespace write_actions_detail {

        class action_set_info
        {
            struct action_info
            {
                action_list::const_iterator pos;
                std::size_t num_actions;
            };

        public:
            action_set_info(action_list const& actions) noexcept
                : it_end_(actions.end())
            {
                boost::fill(info_map_, action_info{it_end_, 0});
                for (auto it = actions.begin(); it != it_end_; ++it) {
                    auto& action_info = info_map_[it->index()];
                    if (action_info.num_actions++ == 0) {
                        action_info.pos = it;
                    }
                }
            }

            auto has_equivalent_action(
                    action_list::const_reference action) const noexcept
                -> bool
            {
                auto const& info = info_map_[action.index()];
                if (info.pos == it_end_) {
                    return false;
                }
                if (!equivalent(action, *info.pos)) {
                    return false;
                }
                return true;
            }

            void set_next_pos(action_list::const_reference action) noexcept
            {
                auto const index = action.index();
                auto& info = info_map_[index];
                if (info.num_actions-- > 0) {
                    info.pos = std::find_if(
                              std::next(info.pos), it_end_
                            , [index](action_list::const_reference action)
                              { return action.index() == index; });
                }
                else {
                    info.pos = it_end_;
                }
            }

        public:
            std::array<
                action_info, action_list::value_type::number_of_types
            > info_map_;
            action_list::const_iterator it_end_;
        };

    } // namespace write_actions_detail

    inline auto equivalent(
            write_actions const& lhs, write_actions const& rhs) noexcept
        -> bool
    {
        if (lhs.actions().size() != rhs.actions().size()) {
            return false;
        }

        auto rhs_action_set_info
            = write_actions_detail::action_set_info{rhs.actions()};
        for (auto const& lhs_action : lhs.actions()) {
            if (!rhs_action_set_info.has_equivalent_action(lhs_action)) {
                return false;
            }
            rhs_action_set_info.set_next_pos(lhs_action);
        }
        return true;
    }

} // namespace instructions
} // namespace v13
} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_V13_INSTRUCTIONS_WRITE_ACTIONS_HPP
