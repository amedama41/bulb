#ifndef CANARD_NET_OFP_V13_PACKET_QUEUE_HPP
#define CANARD_NET_OFP_V13_PACKET_QUEUE_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <boost/range/algorithm/equal.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/queue_id.hpp>
#include <canard/network/openflow/v13/any_queue_property.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    class packet_queue
        : public detail::basic_protocol_type<packet_queue>
    {
    public:
        using raw_ofp_type = v13_detail::ofp_packet_queue;
        using properties_type = list<any_queue_property>;
        using iterator = properties_type::const_iterator;
        using const_iterator = properties_type::const_iterator;

        packet_queue(std::uint32_t const queue_id
                   , std::uint32_t const port_no
                   , properties_type properties)
            : packet_queue_{
                  queue_id
                , port_no
                , properties.calc_ofp_length(sizeof(raw_ofp_type))
                , { 0, 0, 0, 0, 0, 0 }
              }
            , properties_(std::move(properties))
        {
        }

        packet_queue(queue_id const& id, properties_type properties)
            : packet_queue{id.queue(), id.port(), std::move(properties)}
        {
        }

        packet_queue(packet_queue const&) = default;

        packet_queue(packet_queue&& other)
            : packet_queue_(other.packet_queue_)
            , properties_(other.extract_properties())
        {
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

        auto port_no() const noexcept
            -> std::uint32_t
        {
            return packet_queue_.port;
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

        auto extract_properties()
            -> properties_type
        {
            auto properties = properties_type{};
            properties.swap(properties_);
            packet_queue_.len = sizeof(raw_ofp_type);
            return properties;
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
        packet_queue(raw_ofp_type const& pkt_queue
                   , properties_type&& properties)
            : packet_queue_(pkt_queue)
            , properties_(std::move(properties))
        {
        }

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, packet_queue_);
            properties_.encode(container);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> packet_queue
        {
            auto const pkt_queue = detail::decode<raw_ofp_type>(first, last);
            if (pkt_queue.len < sizeof(raw_ofp_type)) {
                throw std::runtime_error{"packet_queue length is too small"};
            }
            auto const properties_length = pkt_queue.len - sizeof(raw_ofp_type);
            if (std::distance(first, last) < properties_length) {
                throw std::runtime_error{"packet_queue length is too big"};
            }
            last = std::next(first, properties_length);

            auto properties = properties_type::decode(first, last);

            return packet_queue{pkt_queue, std::move(properties)};
        }

        auto equal_impl(packet_queue const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(packet_queue_, rhs.packet_queue_)
                && properties() == rhs.properties();
        }

        auto equivalent_impl(packet_queue const& rhs) const noexcept
            -> bool
        {
            using const_reference = properties_type::const_reference;
            return queue_id() == rhs.queue_id()
                && port_no() == rhs.port_no()
                && boost::equal(
                          properties_, rhs.properties_
                        , [](const_reference lhs_prop, const_reference rhs_prop)
                          { return equivalent(lhs_prop, rhs_prop); });
        }

    private:
        raw_ofp_type packet_queue_;
        properties_type properties_;
    };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_PACKET_QUEUE_HPP
