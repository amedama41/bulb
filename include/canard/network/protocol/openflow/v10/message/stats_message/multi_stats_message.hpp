#ifndef CANARD_NET_OFP_V10_MESSAGES_MULTI_STATS_MESSAGE_HPP
#define CANARD_NET_OFP_V10_MESSAGES_MULTI_STATS_MESSAGE_HPP

namespace canard {
namespace net {
namespace ofp {
namespace v10 {
namespace messages {

    template <class StatsMessage>
    class multi_stats_message
    {
    public:
        auto begin();
        auto end();

        void push_back(StatsMessage&& msg)
        {
            auto const is_empty = stats_messages_.empty();
            stats_messages_.push_back(msg);
            if (!is_empty) {
                stats_messages_.back().more(true);
            }
            stats_messages_.back().more(false);
        }

        template <class Container>
        auto encode(Container& container) const
            -> Container&
        {
            boost::for_each(stats_messages_, [&](StatsMessage const& msg) {
                msg.encode(stats);
            });
            return container;
        }

    private:
        std::vector<StatsMessage> stats_messages_;
    };

} // namespace messages
} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_MESSAGES_MULTI_STATS_MESSAGE_HPP
