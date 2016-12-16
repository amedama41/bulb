#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/message/meter_mod.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v13 = ofp::v13;
namespace msg = v13::messages;
namespace meter_bands = v13::meter_bands;

namespace {
  using bands_type = msg::meter_add::bands_type;
  using v13::protocol;

  struct meter_bands_fixture
  {
    meter_bands::drop drop{0x00010001, 0x55667788}; // 16
    meter_bands::dscp_remark dscp_remark1{0xab, 0x00020001, 0x87654321}; // 16
    meter_bands::dscp_remark dscp_remark2{0xcd, 0x00020002, 0x08070605}; // 16
  };
  struct parameters : meter_bands_fixture
  {
    std::uint32_t meter_id = 0x01020304;
    std::uint16_t flags
      = protocol::OFPMF_PKTPS | protocol::OFPMF_BURST | protocol::OFPMF_STATS;
    bands_type bands{ drop, dscp_remark1, dscp_remark2 };
    std::uint32_t xid = 0x1a2b3c4d;
  };
  struct meter_add_fixture : parameters
  {
    msg::meter_add sut{meter_id, flags, bands, xid};
    std::vector<unsigned char> bin
      = "\x04\x1d\x00\x40\x1a\x2b\x3c\x4d""\x00\x00\x00\x0e\x01\x02\x03\x04"
        "\x00\x01\x00\x10\x00\x01\x00\x01""\x55\x66\x77\x88\x00\x00\x00\x00"
        "\x00\x02\x00\x10\x00\x02\x00\x01""\x87\x65\x43\x21\xab\x00\x00\x00"
        "\x00\x02\x00\x10\x00\x02\x00\x02""\x08\x07\x06\x05\xcd\x00\x00\x00"
        ""_bin;
  };
}

BOOST_AUTO_TEST_SUITE(message_test)
BOOST_AUTO_TEST_SUITE(meter_add)

  BOOST_FIXTURE_TEST_SUITE(constructor, meter_bands_fixture)
    BOOST_AUTO_TEST_CASE(is_constructible_with_xid)
    {
      auto const meter_id = std::uint32_t{32};
      auto const flags = protocol::OFPMF_KBPS | protocol::OFPMF_BURST;
      auto const bands = bands_type{ drop, dscp_remark1 };
      auto const xid = std::uint32_t{12345};

      msg::meter_add const sut{meter_id, flags, bands, xid};

      BOOST_TEST(sut.length() == sizeof(v13::v13_detail::ofp_meter_mod) + 32);
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.meter_id() == meter_id);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.bands() == bands));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_without_xid)
    {
      auto const meter_id = std::uint32_t{12};
      auto const flags = protocol::OFPMF_BURST;
      auto const bands = bands_type{ drop };

      msg::meter_add const sut{meter_id, flags, bands};

      BOOST_TEST(sut.length() == sizeof(v13::v13_detail::ofp_meter_mod) + 16);
      BOOST_TEST(sut.meter_id() == meter_id);
      BOOST_TEST(sut.flags() == flags);
      BOOST_TEST((sut.bands() == bands));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, meter_add_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, meter_add_fixture)
    {
      auto const& const_meter_add = sut;

      auto const copy = const_meter_add;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(is_move_constructible, meter_add_fixture)
    {
      auto moved = sut;

      auto const copy = std::move(moved);

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(extract_bands)
    BOOST_FIXTURE_TEST_CASE(make_meter_adds_bands_empty, meter_add_fixture)
    {
      auto const prev_bands = sut.bands();

      auto const extracted_bands = sut.extract_bands();

      BOOST_TEST((extracted_bands == prev_bands));
      BOOST_TEST(sut.length() == sizeof(v13::v13_detail::ofp_meter_mod));
      BOOST_TEST(sut.bands().empty());
    }
  BOOST_AUTO_TEST_SUITE_END() // extract_bands

  BOOST_FIXTURE_TEST_SUITE(equality, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_same_object)
    {
      auto const sut = msg::meter_add{meter_id, flags, bands, xid};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_auto_values_are_equal)
    {
      BOOST_TEST(
          (msg::meter_add{meter_id, flags, bands, xid}
        == msg::meter_add{meter_id, flags, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_meter_id_is_not_equal)
    {
      BOOST_TEST(
          (msg::meter_add{0x01, flags, bands, xid}
        != msg::meter_add{0x02, flags, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          (msg::meter_add{meter_id, protocol::OFPMF_KBPS, bands, xid}
        != msg::meter_add{meter_id, protocol::OFPMF_PKTPS, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_kbps_bit_is_not_equal_with_pktps_bit)
    {
      using p = protocol;

      BOOST_TEST(
          (msg::meter_add{meter_id, p::OFPMF_KBPS | p::OFPMF_PKTPS, bands, xid}
        != msg::meter_add{meter_id, p::OFPMF_PKTPS, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_kbps_bit_is_not_equal_without_pktps_bit)
    {
      using p = protocol;

      BOOST_TEST(
          (msg::meter_add{meter_id, p::OFPMF_KBPS | p::OFPMF_BURST, bands, xid}
        != msg::meter_add{meter_id, p::OFPMF_BURST, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_bands_is_not_equal)
    {
      BOOST_TEST(
          (msg::meter_add{meter_id, flags, { dscp_remark1 }, xid}
        != msg::meter_add{meter_id, flags, { dscp_remark2 }, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_bands_order_is_not_equal)
    {
      BOOST_TEST(
          (msg::meter_add{meter_id, flags, { dscp_remark1, drop }, xid}
        != msg::meter_add{meter_id, flags, { drop, dscp_remark1 }, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (msg::meter_add{meter_id, flags, bands, 1}
        != msg::meter_add{meter_id, flags, bands, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(function_equivalent, parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_same_object)
    {
      auto const sut = msg::meter_add{meter_id, flags, bands, xid};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_auto_values_are_equal)
    {
      BOOST_TEST(
          equivalent(
              msg::meter_add{meter_id, flags, bands, xid}
            , msg::meter_add{meter_id, flags, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_meter_id_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              msg::meter_add{0x01, flags, bands, xid}
            , msg::meter_add{0x02, flags, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_flags_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              msg::meter_add{meter_id, protocol::OFPMF_KBPS, bands, xid}
            , msg::meter_add{meter_id, protocol::OFPMF_PKTPS, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_kbps_bit_is_not_equal_with_pktps_bit)
    {
      using p = protocol;

      BOOST_TEST(
          equivalent(
              msg::meter_add{
                meter_id, p::OFPMF_KBPS | p::OFPMF_PKTPS, bands, xid}
            , msg::meter_add{
                meter_id, p::OFPMF_PKTPS, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_kbps_bit_is_not_equal_without_pktps_bit)
    {
      using p = protocol;

      BOOST_TEST(
          equivalent(
              msg::meter_add{
                meter_id, p::OFPMF_KBPS | p::OFPMF_BURST, bands, xid}
            , msg::meter_add{
                meter_id, p::OFPMF_BURST, bands, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_bands_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              msg::meter_add{meter_id, flags, { dscp_remark1 }, xid}
            , msg::meter_add{meter_id, flags, { dscp_remark2 }, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_bands_order_is_not_equal)
    {
      BOOST_TEST(
          equivalent(
              msg::meter_add{meter_id, flags, { dscp_remark1, drop }, xid}
            , msg::meter_add{meter_id, flags, { drop, dscp_remark1 }, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          !equivalent(
              msg::meter_add{meter_id, flags, bands, 1}
            , msg::meter_add{meter_id, flags, bands, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_AUTO_TEST_SUITE(encode)
    BOOST_FIXTURE_TEST_CASE(generate_binary, meter_add_fixture)
    {
      auto buffer = std::vector<unsigned char>{};

      sut.encode(buffer);

      BOOST_TEST(buffer.size() == sut.byte_length());
      BOOST_TEST(buffer == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_AUTO_TEST_SUITE(decode)
    BOOST_FIXTURE_TEST_CASE(construct_from_binary, meter_add_fixture)
    {
      auto it = bin.begin();

      auto const meter_add = msg::meter_add::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((meter_add == sut));
    }
    BOOST_FIXTURE_TEST_CASE(construct_from_too_long_binary, meter_add_fixture)
    {
      auto const meter_add_length = bin.size();
      bin.resize(meter_add_length + 1);
      auto it = bin.begin();

      auto const meter_add = msg::meter_add::decode(it, bin.end());

      BOOST_TEST((it == std::next(bin.begin(), meter_add_length)));
      BOOST_TEST((meter_add == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // meter_add
BOOST_AUTO_TEST_SUITE_END() // message_test
