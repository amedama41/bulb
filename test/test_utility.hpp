#ifndef CANARD_NET_OFP_TEST_UTILITY_HPP
#define CANARD_NET_OFP_TEST_UTILITY_HPP

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <random>
#include <vector>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>
#include <boost/container/vector.hpp>
#include <boost/utility/string_ref.hpp>
#include <canard/mac_address.hpp>

inline auto operator"" _bin(char const* const str, std::size_t const size)
    -> std::vector<std::uint8_t>
{
    return std::vector<std::uint8_t>(str, str + size);
}

inline auto operator"" _bbin(char const* const str, std::size_t const size)
    -> boost::container::vector<std::uint8_t>
{
    return boost::container::vector<std::uint8_t>(str, str + size);
}

inline auto operator"" _mac(char const* const str, std::size_t const size)
    -> canard::mac_address
{
    if (size != 6) {
        throw std::runtime_error{"invalid size mac address"};
    }
    auto const bytes = reinterpret_cast<unsigned char const*>(str);
    return canard::mac_address{
        {{bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]}}
    };
}

inline auto operator"" _ipv4(char const* const ipv4, std::size_t)
    -> boost::asio::ip::address_v4
{
    return boost::asio::ip::address_v4::from_string(ipv4);
}

inline auto operator"" _ipv6(char const* const ipv6, std::size_t)
    -> boost::asio::ip::address_v6
{
    return boost::asio::ip::address_v6::from_string(ipv6);
}

constexpr auto operator ""_sr(
    char const* const str, std::size_t const len) noexcept
  -> boost::string_ref
{
  return boost::string_ref(str, len);
}

template <class T = void>
struct rand_holder
{
  static std::mt19937 rnd;
};

template <class T>
std::mt19937 rand_holder<T>::rnd(std::time(nullptr));

template <class T>
inline auto random()
  -> T
{
  return rand_holder<>::rnd();
}

template <class T>
auto add_const(T const& t) noexcept
  -> T const&
{
  return t;
}

#endif // CANARD_NET_OFP_TEST_UTILITY_HPP
