#ifndef CANARD_NET_OFP_V13_HELLO_HPP
#define CANARD_NET_OFP_V13_HELLO_HPP

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <boost/optional/optional.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <canard/net/ofp/detail/basic_protocol_type.hpp>
#include <canard/net/ofp/detail/decode.hpp>
#include <canard/net/ofp/detail/encode.hpp>
#include <canard/net/ofp/get_xid.hpp>
#include <canard/net/ofp/list.hpp>
#include <canard/net/ofp/v13/any_hello_element.hpp>
#include <canard/net/ofp/v13/hello_element/versionbitmap.hpp>
#include <canard/net/ofp/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {
namespace messages {

  namespace hello_detail {

    template <class HelloElement>
    auto find(ofp::list<any_hello_element> const& elements)
      -> boost::optional<HelloElement const&>
    {
      auto const it = boost::find_if(
            elements
          , [](any_hello_element const& element)
            { return element.type() == HelloElement::hello_element_type; });
      if (it == elements.end()) {
        return boost::none;
      }
      return any_cast<HelloElement>(*it);
    }

    inline auto get_version(ofp::list<any_hello_element> const& elements)
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

    using raw_ofp_type = protocol::ofp_hello;
    using hello_elements_type = ofp::list<any_hello_element>;

    explicit hello(
          std::uint8_t const version = protocol::OFP_VERSION
        , std::uint32_t const xid = get_xid())
      : hello_{
          protocol::ofp_header{version, message_type, sizeof(raw_ofp_type), xid}
        }
      , elements_{}
    {
    }

    hello(std::uint8_t const version
        , hello_elements_type elements
        , std::uint32_t const xid = get_xid())
      : hello_{
          protocol::ofp_header{
              version
            , message_type
            , elements.calc_ofp_length(sizeof(raw_ofp_type))
            , xid
          }
        }
      , elements_(std::move(elements))
    {
    }

    explicit hello(
        hello_elements_type elements, std::uint32_t const xid = get_xid())
      : hello{hello_detail::get_version(elements), std::move(elements), xid}
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
      -> protocol::ofp_header const&
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
      if (auto const versionbitmap = find<hello_elements::versionbitmap>()) {
        return versionbitmap->support(version);
      }
      return this->version() >= version;
    }

    auto max_support_version() const
      -> std::uint8_t
    {
      if (auto const versionbitmap = find<hello_elements::versionbitmap>()) {
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

    friend basic_protocol_type;

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, hello_);
      elements_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> hello
    {
      auto const h = detail::decode<raw_ofp_type>(first, last);
      auto const hello_elements_length = h.header.length - sizeof(raw_ofp_type);
      last = std::next(first, hello_elements_length);

      auto elements = hello_elements_type::decode(first, last);

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
