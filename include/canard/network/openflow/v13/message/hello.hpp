#ifndef CANARD_NET_OFP_V13_HELLO_HPP
#define CANARD_NET_OFP_V13_HELLO_HPP

#include <cstdint>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>
#include <boost/optional/optional.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/numeric.hpp>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/get_xid.hpp>
#include <canard/network/openflow/v13/any_hello_element.hpp>
#include <canard/network/openflow/v13/detail/length_utility.hpp>
#include <canard/network/openflow/v13/hello_element/versionbitmap.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

    namespace hello_detail {

        template <class HelloElement>
        auto find(std::vector<any_hello_element> const& elements)
            -> boost::optional<HelloElement const&>
        {
            auto const it = boost::find_if(
                    elements, [](any_hello_element const& element) {
                return element.type() == HelloElement::hello_element_type;
            });
            if (it == elements.end()) {
                return boost::none;
            }
            return any_cast<HelloElement>(*it);
        }

        auto get_version(std::vector<any_hello_element> const& elements)
            -> std::uint8_t
        {
            if (auto const versionbitmap
                    = find<hello_elements::versionbitmap>(elements)) {
                return versionbitmap->max_support_version();
            }
            throw std::runtime_error{"no versionbitmap"};
        }

    } // namespace hello_detail


    class hello
        : public detail::basic_protocol_type<hello>
    {
    public:
        static constexpr protocol::ofp_type message_type = protocol::OFPT_HELLO;

        using raw_ofp_type = v13_detail::ofp_hello;
        using hello_elements_type = std::vector<any_hello_element>;

        explicit hello(
                  std::uint8_t const version = protocol::OFP_VERSION
                , std::uint32_t const xid = get_xid())
            : hello_{
                v13_detail::ofp_header{
                      version
                    , message_type
                    , sizeof(raw_ofp_type)
                    , xid
                }
              }
            , elements_{}
        {
        }

        hello(std::uint8_t const version
            , hello_elements_type elements
            , std::uint32_t const xid = get_xid())
            : hello_{
                v13_detail::ofp_header{
                      version
                    , message_type
                    , calc_length(elements)
                    , xid
                }
              }
            , elements_(std::move(elements))
        {
        }

        explicit hello(
                  hello_elements_type elements
                , std::uint32_t const xid = get_xid())
            : hello{
                hello_detail::get_version(elements), std::move(elements), xid
              }
        {
        }

        explicit hello(
                  hello_elements::versionbitmap versionbitmap
                , std::uint32_t const xid = get_xid())
            : hello{
                  versionbitmap.max_support_version()
                , {std::move(versionbitmap)}, xid
              }
        {
        }

        hello(hello const&) = default;

        hello(hello&& other)
            : hello_(other.hello_)
            , elements_(other.extract_elements())
        {
        }

        auto operator=(hello const& other)
            -> hello&
        {
            return operator=(hello{other});
        }

        auto operator=(hello&& other)
            -> hello&
        {
            auto tmp = std::move(other);
            swap(tmp);
            return *this;
        }

        void swap(hello& other) noexcept
        {
            std::swap(hello_, other.hello_);
            elements_.swap(other.elements_);
        }

        auto header() const noexcept
            -> v13_detail::ofp_header const&
        {
            return hello_.header;
        }

        auto version() const noexcept
            -> std::uint8_t
        {
            return header().version;
        }

        static constexpr auto type() noexcept
            -> std::uint8_t
        {
            return message_type;
        }

        auto length() const noexcept
            -> std::uint16_t
        {
            return header().length;
        }

        auto xid() const noexcept
            -> std::uint32_t
        {
            return header().xid;
        }

        auto elements() const noexcept
            -> hello_elements_type const&
        {
            return elements_;
        }

        auto extract_elements()
            -> hello_elements_type
        {
            auto elements = hello_elements_type{};
            elements.swap(elements_);
            hello_.header.length = sizeof(raw_ofp_type);
            return elements;
        }

        auto support(std::uint8_t const version) const
            -> bool
        {
            if (auto const versionbitmap
                    = find<hello_elements::versionbitmap>()) {
                return versionbitmap->support(version);
            }
            return this->version() >= version;
        }

        auto max_support_version() const
            -> std::uint8_t
        {
            if (auto const versionbitmap
                    = find<hello_elements::versionbitmap>()) {
                return versionbitmap->max_support_version();
            }
            return version();
        }

        template <class HelloElement>
        auto find() const
            -> boost::optional<HelloElement const&>
        {
            return hello_detail::find<HelloElement>(elements_);
        }

    private:
        hello(raw_ofp_type const& hello, hello_elements_type&& elements)
            : hello_(hello)
            , elements_(std::move(elements))
        {
        }

        static auto calc_length(hello_elements_type const& elements)
            -> std::uint16_t
        {
            using const_reference = hello_elements_type::const_reference;
            return boost::accumulate(
                      elements
                    , std::uint16_t{sizeof(raw_ofp_type)}
                    , [](std::uint16_t const len, const_reference e)
                      { return len + v13_detail::exact_length(e.length()); });
        }

        friend basic_protocol_type;

        template <class Container>
        void encode_impl(Container& container) const
        {
            using const_reference = hello_elements_type::const_reference;
            detail::encode(container, hello_);
            boost::for_each(
                    elements_, [&](const_reference e) { e.encode(container); });
        }

        template <class Iterator>
        static auto decode_impl(Iterator& first, Iterator last)
            -> hello
        {
            auto const h = detail::decode<raw_ofp_type>(first, last);
            auto const hello_elements_length
                = h.header.length - sizeof(raw_ofp_type);
            if (std::distance(first, last) < hello_elements_length) {
                throw std::runtime_error{"too large hello length"};
            }
            last = std::next(first, hello_elements_length);

            auto elements = hello_elements_type{};
            using value_type = hello_elements_type::value_type;
            while (std::distance(first, last)
                    >= sizeof(value_type::header_type)) {
                elements.push_back(value_type::decode(first, last));
            }
            if (first != last) {
                throw std::runtime_error{"invalid hello length"};
            }
            return hello{h, std::move(elements)};
        }

        auto equal_impl(hello const& rhs) const noexcept
            -> bool
        {
            return detail::memcmp(hello_, rhs.hello_)
                && elements_ == rhs.elements_;
        }

    private:
        raw_ofp_type hello_;
        hello_elements_type elements_;
    };

} // namespace messages

using messages::hello;

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_HELLO_HPP
