#ifndef CANARD_NET_OFP_V10_PACKET_QUEUE_HPP
#define CANARD_NET_OFP_V10_PACKET_QUEUE_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/numeric.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/detail/min_base_size_element.hpp>
#include <canard/network/openflow/queue_id.hpp>
#include <canard/network/openflow/v10/any_queue_property.hpp>
#include <canard/network/openflow/v10/detail/byteorder.hpp>
#include <canard/network/openflow/v10/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v10 {

    class packet_queue
        : public detail::basic_protocol_type<packet_queue>
    {
    public:
        using raw_ofp_type = v10_detail::ofp_packet_queue;
        using properties_type = std::vector<any_queue_property>;
        using iterator = properties_type::const_iterator;
        using const_iterator = properties_type::const_iterator;

        packet_queue(std::uint32_t const queue_id
                   , properties_type properties)
            : packet_queue_{
                  queue_id
                , std::uint16_t(
                          sizeof(raw_ofp_type)
                        + calc_propertis_length(properties))
                , { 0, 0 }
              }
            , properties_(std::move(properties))
        {
        }

        packet_queue(queue_id const& id, properties_type properties)
            : packet_queue{id.queue(), std::move(properties)}
        {
        }

        packet_queue(packet_queue const&) = default;

        packet_queue(packet_queue&& other)
            : packet_queue_(other.packet_queue_)
            , properties_(std::move(other).properties_)
        {
            other.packet_queue_.len = sizeof(raw_ofp_type);
        }

        auto operator=(packet_queue const& other)
            -> packet_queue&
        {
            return operator=(packet_queue{other});
        }

        auto operator=(packet_queue&& other)
            -> packet_queue&
        {
            auto tmp = std::move(other);
            std::swap(packet_queue_, tmp.packet_queue_);
            properties_.swap(tmp.properties_);
            return *this;
        }

        auto queue_id() const noexcept
            -> std::uint32_t
        {
            return packet_queue_.queue_id;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return packet_queue_.len;
        }

        auto properties() const noexcept
            -> properties_type const&
        {
            return properties_;
        }

        auto begin() const noexcept
            -> const_iterator
        {
            return properties_.begin();
        }

        auto end() const noexcept
            -> const_iterator
        {
            return properties_.end();
        }

    private:
        friend basic_protocol_type;

        packet_queue(raw_ofp_type const& pkt_queue
                   , properties_type&& properties)
            : packet_queue_(pkt_queue)
            , properties_(std::move(properties))
        {
        }

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, packet_queue_);
            boost::for_each(properties_, [&](any_queue_property const& e) {
                e.encode(container);
            });
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> packet_queue
        {
            auto const pkt_queue = detail::decode<raw_ofp_type>(first, last);
            if (pkt_queue.len < sizeof(raw_ofp_type)) {
                throw std::runtime_error{"packet_queue length is too small"};
            }
            if (std::distance(first, last)
                    < pkt_queue.len - sizeof(raw_ofp_type)) {
                throw std::runtime_error{"packet_queue length is too big"};
            }
            auto const properties_length = pkt_queue.len - sizeof(raw_ofp_type);
            last = std::next(first, properties_length);

            auto properties = properties_type{};
            constexpr auto min_base_size = detail::min_base_size_element<
                any_queue_property::type_list
            >::value;
            properties.reserve(properties_length / min_base_size);
            while (std::distance(first, last)
                    >= sizeof(v10_detail::ofp_queue_prop_header)) {
                properties.push_back(any_queue_property::decode(first, last));
            }
            if (first != last) {
                throw std::runtime_error{"invalid packet_queue length"};
            }

            return packet_queue{pkt_queue, std::move(properties)};
        }

        auto equal_impl(packet_queue const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(packet_queue_, rhs.packet_queue_)
                && properties_ == rhs.properties_;
        }

        auto equivalent_impl(packet_queue const& rhs) const noexcept
            -> bool
        {
            using cref = packet_queue::properties_type::const_reference;
            return queue_id() == rhs.queue_id()
                && boost::equal(
                          properties(), rhs.properties()
                        , [](cref lhs_prop, cref rhs_prop)
                          { return equivalent(lhs_prop, rhs_prop); });
        }

        static auto calc_propertis_length(properties_type const& properties)
            -> std::size_t
        {
            auto const properties_length = boost::accumulate(
                      properties, std::size_t{0}
                    , [](std::size_t const sum, any_queue_property const& e) {
                            return sum + e.length();
                      });
            if (properties_length + sizeof(raw_ofp_type)
                    > std::numeric_limits<std::uint16_t>::max()) {
                throw std::runtime_error{"properties length is too big"};
            }
            return properties_length;
        }

    private:
        raw_ofp_type packet_queue_;
        properties_type properties_;
    };

} // namespace v10
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V10_PACKET_QUEUE_HPP