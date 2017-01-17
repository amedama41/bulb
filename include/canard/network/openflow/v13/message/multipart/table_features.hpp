#ifndef CANARD_NET_OFP_V13_MESSAGES_MULTIPART_TABLE_FEATURES_HPP
#define CANARD_NET_OFP_V13_MESSAGES_MULTIPART_TABLE_FEATURES_HPP

#include <cstdint>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/utility/string_ref.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/v13/detail/basic_multipart.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/utility/table_feature_property_set.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {
namespace multipart {

    class table_features
        : public detail::basic_protocol_type<table_features>
    {
    public:
        using raw_ofp_type = protocol::ofp_table_features;
        using properties_type = ofp::list<any_table_feature_property>;

        table_features(
                  std::uint8_t const table_id
                , boost::string_ref const name
                , std::uint64_t const metadata_match
                , std::uint64_t const metadata_write
                , std::uint32_t const config
                , std::uint32_t const max_entries
                , properties_type properties)
            : table_features_{
                  properties.calc_ofp_length(sizeof(raw_ofp_type))
                , table_id
                , { 0, 0, 0, 0, 0 }
                , ""
                , metadata_match
                , metadata_write
                , config
                , max_entries
              }
            , properties_(std::move(properties))
        {
            auto const name_size
                = std::min(name.size(), sizeof(table_features_.name) - 1);
            using boost::adaptors::sliced;
            boost::copy(name | sliced(0, name_size), table_features_.name);
        }

        table_features(
                  std::uint8_t const table_id
                , boost::string_ref const name
                , std::uint64_t const metadata_match
                , std::uint64_t const metadata_write
                , std::uint32_t const config
                , std::uint32_t const max_entries
                , table_feature_property_set properties)
            : table_features{
                  table_id, name
                , metadata_match, metadata_write, config, max_entries
                , std::move(properties).to_list()
              }
        {
        }

        table_features(table_features const&) = default;

        table_features(table_features&& other)
            : table_features_(other.table_features_)
            , properties_(other.extract_properties())
        {
        }

        auto operator=(table_features const& other)
            -> table_features&
        {
            return operator=(table_features{other});
        }

        auto operator=(table_features&& other)
            -> table_features&
        {
            auto tmp = std::move(other);
            std::swap(table_features_, tmp.table_features_);
            properties_.swap(properties_);
            return *this;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return table_features_.length;
        }

        auto table_id() const noexcept
            -> std::uint8_t
        {
            return table_features_.table_id;
        }

        auto name() const
            -> boost::string_ref
        {
            return table_features_.name;
        }

        auto metadata_match() const noexcept
            -> std::uint64_t
        {
            return table_features_.metadata_match;
        }

        auto metadata_write() const noexcept
            -> std::uint64_t
        {
            return table_features_.metadata_write;
        }

        auto config() const noexcept
            -> std::uint32_t
        {
            return table_features_.config;
        }

        auto max_entries() const noexcept
            -> std::uint32_t
        {
            return table_features_.max_entries;
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
            table_features_.length = min_length();
            return properties;
        }

    private:
        table_features(
                  raw_ofp_type const& features
                , properties_type&& properties)
            : table_features_(features)
            , properties_(std::move(properties))
        {
        }

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            detail::encode(container, table_features_);
            properties_.encode(container);
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> table_features
        {
            auto const features = detail::decode<raw_ofp_type>(first, last);
            if (features.length < min_length()) {
                throw std::runtime_error{"table_features length is too small"};
            }
            auto const prop_length = features.length - sizeof(raw_ofp_type);
            if (std::distance(first, last) < prop_length) {
                throw std::runtime_error{"table_features length is too big"};
            }

            last = std::next(first, prop_length);
            auto properties = properties_type::decode(first, last);
            return table_features{features, std::move(properties)};
        }

    private:
        raw_ofp_type table_features_;
        properties_type properties_;
    };


    class table_features_request
        : public multipart_detail::basic_multipart_request<
                table_features_request, table_features[]
          >
    {
    public:
        static constexpr protocol::ofp_multipart_type multipart_type_value
            = protocol::OFPMP_TABLE_FEATURES;

        explicit table_features_request(std::uint32_t const xid = get_xid())
            : basic_multipart_request{0, {}, xid}
        {
        }

        table_features_request(
                  body_type table_features
                , std::uint16_t const flags = 0
                , std::uint32_t const xid = get_xid())
            : basic_multipart_request{flags, std::move(table_features), xid}
        {
        }

    private:
        friend basic_multipart_request::base_type;

        table_features_request(
                  protocol::ofp_multipart_request const& multipart_request
                , body_type&& table_features)
            : basic_multipart_request{
                multipart_request, std::move(table_features)
              }
        {
        }
    };


    class table_features_reply
        : public multipart_detail::basic_multipart_reply<
                table_features_reply, table_features[]
          >
    {
    public:
        static constexpr protocol::ofp_multipart_type multipart_type_value
            = protocol::OFPMP_TABLE_FEATURES;

        table_features_reply(
                  body_type table_features
                , std::uint16_t const flags = 0
                , std::uint32_t const xid = get_xid())
            : basic_multipart_reply{flags, std::move(table_features), xid }
        {
        }

    private:
        friend basic_multipart_reply::base_type;

        table_features_reply(
                  protocol::ofp_multipart_reply const& multipart_reply
                , body_type&& table_features)
            : basic_multipart_reply{multipart_reply, std::move(table_features)}
        {
        }
    };

} // namespace multipart
} // namespace messages
} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_MESSAGES_MULTIPART_TABLE_FEATURES_HPP
