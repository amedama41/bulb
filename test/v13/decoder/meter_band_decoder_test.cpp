#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/decoder/meter_band_decoder.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/optional/optional.hpp>
#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace bands = v13::meter_bands;

namespace {
  struct meter_band_decoder_fixture
  {
    bands::drop drop{0x01020304, 0x05060708};
    std::vector<unsigned char> drop_bin
      = "\x00\x01\x00\x10\x01\x02\x03\x04""\x05\x06\x07\x08\x00\x00\x00\x00"
        ""_bin;
    bands::dscp_remark dscp_remark{0xab, 0x01020304, 0x05060708};
    std::vector<unsigned char> dscp_remark_bin
      = "\x00\x02\x00\x10\x01\x02\x03\x04""\x05\x06\x07\x08\xab\x00\x00\x00"
        ""_bin;
  };

  struct discard
  {
    template <class T>
    void operator()(T&&) const {}
  };

  template <class T = char>
  struct to_optional
  {
    using result_type = boost::optional<T>;
    auto operator()(T const& t) const -> result_type
    { return result_type{t}; }
    template <class U>
    auto operator()(U const&) const -> result_type
    { return boost::none; }
  };
}

BOOST_AUTO_TEST_SUITE(decoder_test)
BOOST_AUTO_TEST_SUITE(meter_band_decoder)
  BOOST_AUTO_TEST_SUITE(type_definition_test)
    BOOST_AUTO_TEST_CASE(header_type)
    {
      using sut = v13::meter_band_decoder;

      using is_same = std::is_same<
        sut::header_type, v13::protocol::ofp_meter_band_header
      >;

      BOOST_TEST(is_same::value);
    }
    BOOST_AUTO_TEST_CASE(type_id)
    {
      using sut = v13::meter_band_decoder;

      using is_same = std::is_same<sut::type_id, std::uint16_t>;

      BOOST_TEST(is_same::value);
    }
  BOOST_AUTO_TEST_SUITE_END() // type_definition_test

  BOOST_FIXTURE_TEST_SUITE(decode, meter_band_decoder_fixture)
    BOOST_AUTO_TEST_CASE(construct_drop_if_binary_represents_drop)
    {
      auto it = drop_bin.begin();
      using f = to_optional<bands::drop>;

      auto const band = v13::meter_band_decoder::decode<f::result_type>(
          it, drop_bin.end(), f{});

      BOOST_TEST(bool(band));
      BOOST_TEST((*band == drop));
    }
    BOOST_AUTO_TEST_CASE(construct_dscp_remark_if_binary_represents_dscp_remark)
    {
      auto it = dscp_remark_bin.begin();
      using f = to_optional<bands::dscp_remark>;

      auto const band = v13::meter_band_decoder::decode<f::result_type>(
          it, dscp_remark_bin.end(), f{});

      BOOST_TEST(bool(band));
      BOOST_TEST((*band == dscp_remark));
    }
    BOOST_AUTO_TEST_CASE(throw_exception_if_binary_size_is_smaller_than_length)
    {
      drop_bin.resize(drop_bin.size() - 1);
      auto it = drop_bin.begin();

      BOOST_CHECK_EXCEPTION(
            v13::meter_band_decoder::decode<void>(it, drop_bin.end(), discard{})
          , v13::exception
          , [](v13::exception const& e) {
              return e.error_type() == v13::protocol::error_type::bad_request
                  && e.error_code() == v13::protocol::bad_request_code::bad_len;
            });
    }
    BOOST_AUTO_TEST_CASE(throw_exception_if_meter_band_type_is_unknown)
    {
      drop_bin[0] = 0x12;
      auto it = drop_bin.begin();

      BOOST_CHECK_EXCEPTION(
            v13::meter_band_decoder::decode<void>(it, drop_bin.end(), discard{})
          , v13::exception
          , [](v13::exception const& e) {
              return e.error_type() == v13::protocol::error_type::meter_mod_failed
                  && e.error_code()
                    == v13::protocol::meter_mod_failed_code::unknown_meter;
            });
    }
  BOOST_AUTO_TEST_SUITE_END() // decode
BOOST_AUTO_TEST_SUITE_END() // meter_band_decoder
BOOST_AUTO_TEST_SUITE_END() // decoder_test
