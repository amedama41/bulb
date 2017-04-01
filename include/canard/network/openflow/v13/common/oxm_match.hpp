#ifndef CANARD_NET_OFP_V13_OXM_MATCH_HPP
#define CANARD_NET_OFP_V13_OXM_MATCH_HPP

#include <canard/network/openflow/detail/config.hpp>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <canard/network/openflow/detail/basic_protocol_type.hpp>
#include <canard/network/openflow/detail/decode.hpp>
#include <canard/network/openflow/detail/encode.hpp>
#include <canard/network/openflow/detail/memcmp.hpp>
#include <canard/network/openflow/list.hpp>
#include <canard/network/openflow/type_traits/is_all_constructible.hpp>
#include <canard/network/openflow/v13/any_oxm_match_field.hpp>
#include <canard/network/openflow/v13/detail/byteorder.hpp>
#include <canard/network/openflow/v13/exception.hpp>
#include <canard/network/openflow/v13/openflow.hpp>
#include <canard/network/openflow/v13/utility/oxm_match_field_set.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

  class oxm_match
    : public detail::basic_protocol_type<oxm_match>
  {
    static constexpr protocol::ofp_match_type match_type = protocol::OFPMT_OXM;

  public:
    using raw_ofp_type = protocol::ofp_match;
    using oxm_fields_type = ofp::list<any_oxm_match_field>;

  private:
    template <class... Ts>
    using enable_if_is_all_constructible_t = typename std::enable_if<
         sizeof...(Ts)
      && type_traits::is_all_constructible<any_oxm_match_field, Ts...>::value
    >::type;

  public:
    oxm_match()
      : oxm_match{oxm_fields_type{}}
    {
    }

    explicit oxm_match(oxm_match_field_set field_set)
      : oxm_match{std::move(field_set).to_list()}
    {
    }

    explicit oxm_match(oxm_fields_type oxm_fields)
      : match_{
            match_type
          , oxm_fields.calc_ofp_length(base_size)
          , { 0, 0, 0, 0 }
        }
      , oxm_fields_(std::move(oxm_fields))
    {
    }

    template <
        class... OXMMatchFields
      , class = enable_if_is_all_constructible_t<OXMMatchFields...>
    >
    explicit oxm_match(OXMMatchFields&&... fields)
      : oxm_match{oxm_fields_type{std::forward<OXMMatchFields>(fields)...}}
    {
    }

    oxm_match(oxm_match const&) = default;

    oxm_match(oxm_match&& other)
      : match_(other.match_)
      , oxm_fields_(other.extract_oxm_fields())
    {
    }

    auto operator=(oxm_match const& other)
      -> oxm_match&
    {
      return operator=(oxm_match{other});
    }

    auto operator=(oxm_match&& other)
      -> oxm_match&
    {
      auto tmp = std::move(other);
      swap(tmp);
      return *this;
    }

    void swap(oxm_match& other) noexcept
    {
      std::swap(match_, other.match_);
      oxm_fields_.swap(other.oxm_fields_);
    }

    static constexpr auto type() noexcept
      -> std::uint16_t
    {
      return match_type;
    }

    auto length() const noexcept
      -> std::uint16_t
    {
      return match_.length;
    }

    auto oxm_fields() const noexcept
      -> oxm_fields_type const&
    {
      return oxm_fields_;
    }

    auto extract_oxm_fields()
      -> oxm_fields_type
    {
      auto oxm_fields = oxm_fields_type{};
      oxm_fields.swap(oxm_fields_);
      match_.length = base_size;
      return oxm_fields;
    }

    auto calc_ofp_length(std::uint16_t const base_length) const
      -> std::uint16_t
    {
      auto const length = byte_length();
      if (length > std::numeric_limits<std::uint16_t>::max() - base_length) {
        throw std::runtime_error{"too large oxm_match length"};
      }
      return base_length + length;
    }

    static void validate_header(protocol::ofp_match const& match)
    {
      if (match.type != match_type) {
        throw exception{
          protocol::bad_match_code::bad_type, "invalid match type"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
      if (match.length < base_size) {
        throw exception{
          protocol::bad_match_code::bad_len, "too small match length"
        } << CANARD_NET_OFP_ERROR_INFO();
      }
    }

  private:
    oxm_match(raw_ofp_type const& match, oxm_fields_type&& oxm_fields)
      : match_(match)
      , oxm_fields_(std::move(oxm_fields))
    {
    }

    friend basic_protocol_type;

    static constexpr std::uint16_t base_size
      = offsetof(oxm_match::raw_ofp_type, pad);

    friend constexpr auto get_min_length(
        detail::basic_protocol_type_tag<oxm_match>) noexcept
      -> std::uint16_t
    {
      return base_size;
    }

    friend constexpr auto exclude_padding(
        detail::basic_protocol_type_tag<oxm_match>) noexcept
      -> bool
    {
      return true;
    }

    template <class Container>
    void encode_impl(Container& container) const
    {
      detail::encode(container, match_, detail::copy_size<base_size>{});
      oxm_fields_.encode(container);
    }

    template <class Iterator>
    static auto decode_impl(Iterator& first, Iterator last)
      -> oxm_match
    {
      auto const match = detail::decode<raw_ofp_type>(
          first, last, detail::copy_size<base_size>{});
      last = std::next(first, match.length - base_size);
      auto oxm_fields = oxm_fields_type::decode(first, last);
      return oxm_match{match, std::move(oxm_fields)};
    }

    auto equal_impl(oxm_match const& rhs) const noexcept
      -> bool
    {
      return detail::memcmp(match_, rhs.match_)
          && oxm_fields_ == rhs.oxm_fields_;
    }

    auto equivalent_impl(oxm_match const& rhs) const
      -> bool
    {
      return oxm_match_field_set::equivalent_as_set(
          oxm_fields_, rhs.oxm_fields_);
    }

  private:
    raw_ofp_type match_;
    oxm_fields_type oxm_fields_;
  };

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_OXM_MATCH_HPP
