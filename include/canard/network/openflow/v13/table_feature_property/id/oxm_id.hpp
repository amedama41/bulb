#ifndef CANARD_NET_OFP_V13_OXM_ID_HPP
#define CANARD_NET_OFP_V13_OXM_ID_HPP

#include <cstdint>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/v13/any_oxm_id.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    class oxm_id
        : public detail::basic_protocol_type<oxm_id>
    {
    public:
        using oxm_header_type = std::uint32_t;
        using raw_ofp_type = oxm_header_type;

        explicit oxm_id(oxm_header_type const oxm_header) noexcept
            : oxm_header_{oxm_header}
        {
        }

        oxm_id(protocol::ofp_oxm_class const oxm_class
             , std::uint8_t const field
             , bool const hasmask
             , std::uint8_t const length) noexcept
            : oxm_header_{
                  (oxm_header_type(oxm_class) << 16)
                | (oxm_header_type{field} << 9)
                | (oxm_header_type{hasmask} << 8)
                | length
              }
        {
        }

        auto oxm_class() const noexcept
            -> protocol::ofp_oxm_class
        {
            return protocol::ofp_oxm_class(oxm_header_ >> 16);
        }

        auto oxm_field() const noexcept
            -> std::uint8_t
        {
            return (oxm_header_ >> 9) & 0x7f;
        }

        auto oxm_type() const noexcept
            -> std::uint32_t
        {
            return (oxm_header_ >> 9);
        }

        auto oxm_hasmask() const noexcept
            -> bool
        {
            return oxm_header_ & 0x100;
        }

        auto oxm_length() const noexcept
            -> std::uint8_t
        {
            return oxm_header_ & 0xff;
        }

        auto oxm_header() const noexcept
            -> oxm_header_type
        {
            return oxm_header_;
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(oxm_header_type);
        };

    private:
        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, oxm_header_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> oxm_id
        {
            return oxm_id{detail::decode<oxm_header_type>(first, last)};
        }

        auto equal_impl(oxm_id const& rhs) const noexcept
            -> bool
        {
            return oxm_header_ == rhs.oxm_header_;
        }

        auto equivalent_impl(oxm_id const& rhs) const noexcept
            -> bool
        {
            return oxm_header_ == rhs.oxm_header_;
        }

    private:
        oxm_header_type oxm_header_;
    };


    class oxm_experimenter_id
        : public detail::basic_protocol_type<oxm_experimenter_id>
    {
    public:
        using oxm_header_type = std::uint32_t;
        using raw_ofp_type = protocol::ofp_oxm_experimenter_header;

        oxm_experimenter_id(
                  std::uint8_t const oxm_field
                , bool const oxm_hasmask
                , std::uint8_t const oxm_length
                , std::uint32_t const experimenter) noexcept
            : oxm_experimenter_header_{
                  (oxm_header_type(protocol::OFPXMC_EXPERIMENTER) << 16)
                | (oxm_header_type(oxm_field) << 9)
                | (oxm_header_type(oxm_hasmask) << 8)
                | (oxm_header_type(oxm_length))
                , experimenter
              }
        {
        }

        static constexpr auto oxm_class() noexcept
            -> protocol::ofp_oxm_class
        {
            return protocol::OFPXMC_EXPERIMENTER;
        }

        auto oxm_field() const noexcept
            -> std::uint8_t
        {
            return (oxm_experimenter_header_.oxm_header >> 9) & 0x7f;
        }

        auto oxm_type() const noexcept
            -> std::uint32_t
        {
            return (oxm_experimenter_header_.oxm_header >> 9);
        }

        auto oxm_hasmask() const noexcept
            -> bool
        {
            return oxm_experimenter_header_.oxm_header & 0x100;
        }

        auto oxm_length() const noexcept
            -> std::uint8_t
        {
            return oxm_experimenter_header_.oxm_header & 0xff;
        }

        auto oxm_header() const noexcept
            -> oxm_header_type
        {
            return oxm_experimenter_header_.oxm_header;
        }

        auto experimenter() const noexcept
            -> std::uint32_t
        {
            return oxm_experimenter_header_.experimenter;
        }

        static constexpr auto length() noexcept
            -> std::uint16_t
        {
            return sizeof(raw_ofp_type);
        }

    private:
        explicit oxm_experimenter_id(
                raw_ofp_type const& oxm_experimenter_header) noexcept
            : oxm_experimenter_header_(oxm_experimenter_header)
        {
        }

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, oxm_experimenter_header_);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> oxm_experimenter_id
        {
            return oxm_experimenter_id{
                detail::decode<raw_ofp_type>(first, last)
            };
        }

        auto equal_impl(oxm_experimenter_id const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(
                    oxm_experimenter_header_, rhs.oxm_experimenter_header_);
        }

        auto equivalent_impl(oxm_experimenter_id const& rhs) const noexcept
            -> bool
        {
            return oxm_header() == rhs.oxm_header()
                && experimenter() == rhs.experimenter();
        }

    private:
        raw_ofp_type oxm_experimenter_header_;
    };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OXM_ID_HPP
