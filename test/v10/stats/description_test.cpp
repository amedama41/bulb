#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v10/message/stats/description.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace msg = v10::messages;
namespace stats = msg::statistics;
namespace protocol = v10::protocol;

namespace {
struct description_request_fixture {
  stats::description_request sut{0x12345678};
  std::vector<unsigned char> bin
    = "\x01\x10\x00\x0c\x12\x34\x056\x78""\x00\x00\x00\x00"_bin;
};
struct description_reply_parameters {
    std::string mfr_desc = "manufacture description";
    std::string hw_desc = "hardware description";
    std::string sw_desc = "software description";
    std::string serial_num = "12345678-0000-87654321";
    std::string dp_desc = "datapath description";
    std::uint32_t xid = 0x12345678;
};
struct description_reply_fixture : description_reply_parameters {
  description_reply_fixture()
  {
    auto stats_size = sizeof(protocol::ofp_stats_reply);
    bin.reserve(stats_size + sizeof(protocol::ofp_desc_stats));

    bin.insert(bin.end(), mfr_desc.begin(), mfr_desc.end());
    stats_size += protocol::DESC_STR_LEN;
    bin.resize(stats_size, '\x00');

    bin.insert(bin.end(), hw_desc.begin(), hw_desc.end());
    stats_size += protocol::DESC_STR_LEN;
    bin.resize(stats_size, '\x00');

    bin.insert(bin.end(), sw_desc.begin(), sw_desc.end());
    stats_size += protocol::DESC_STR_LEN;
    bin.resize(stats_size, '\x00');

    bin.insert(bin.end(), serial_num.begin(), serial_num.end());
    stats_size += protocol::SERIAL_NUM_LEN;
    bin.resize(stats_size, '\x00');

    bin.insert(bin.end(), dp_desc.begin(), dp_desc.end());
    stats_size += protocol::DESC_STR_LEN;
    bin.resize(stats_size, '\x00');
  }

  stats::description_reply sut{
    mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid
  };
  std::vector<unsigned char> bin
    = "\x01\x11\x04\x2c\x12\x34\x056\x78""\x00\x00\x00\x00"_bin;
};
}

BOOST_AUTO_TEST_SUITE(stats_test)

BOOST_AUTO_TEST_SUITE(description_request)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_default_constructible)
    {
      stats::description_request sut{};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request));
      BOOST_TEST(sut.flags() == 0);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_xid)
    {
      auto const xid = 1234;

      stats::description_request sut{xid};

      BOOST_TEST(sut.length() == sizeof(protocol::ofp_stats_request));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.flags() == 0);
    }
    BOOST_AUTO_TEST_CASE(is_copy_constructible_from_non_const_lvalue)
    {
      stats::description_request sut{0x12345678};

      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_AUTO_TEST_CASE(is_copy_constructible_from_const_lvalue)
    {
      stats::description_request const sut{0x12345678};

      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::description_request{0x12345678};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_xid_is_equal)
    {
      auto const xid = 0x01020304;

      BOOST_TEST(
          (stats::description_request{xid} == stats::description_request{xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::description_request{1} != stats::description_request{2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, description_request_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, description_request_fixture)
    BOOST_AUTO_TEST_CASE(constructs_description_request_from_binary)
    {
      auto it = bin.begin();

      auto const description_request
        = stats::description_request::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((description_request == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // description_request


BOOST_AUTO_TEST_SUITE(description_reply)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(is_constructible_from_all_parameters)
    {
      auto const mfr_desc = "manufacture description";
      auto const hw_desc = std::string{"hardware description"};
      auto const sw_desc = std::string{"software description"};
      auto const serial_num = "123456789";
      auto const dp_desc = "bulb test description";
      auto const xid = 0x12345678;

      stats::description_reply sut{
        mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_stats_reply) + sizeof(protocol::ofp_desc_stats));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.manufacture_desc() == mfr_desc);
      BOOST_TEST(sut.hardware_desc() == hw_desc);
      BOOST_TEST(sut.software_desc() == sw_desc);
      BOOST_TEST(sut.serial_number() == serial_num);
      BOOST_TEST(sut.datapath_desc() == dp_desc);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_all_parameters_without_xid)
    {
      auto const mfr_desc = "manufacture description without xid";
      auto const hw_desc = "hardware description without xid";
      auto const sw_desc = "software description without xid";
      auto const serial_num = std::string(protocol::SERIAL_NUM_LEN - 1, '0');
      auto const dp_desc = std::string(protocol::DESC_STR_LEN - 1, 'X');

      stats::description_reply sut{
        mfr_desc, hw_desc, sw_desc, serial_num, dp_desc
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_stats_reply) + sizeof(protocol::ofp_desc_stats));
      BOOST_TEST(sut.manufacture_desc() == mfr_desc);
      BOOST_TEST(sut.hardware_desc() == hw_desc);
      BOOST_TEST(sut.software_desc() == sw_desc);
      BOOST_TEST(sut.serial_number() == serial_num);
      BOOST_TEST(sut.datapath_desc() == dp_desc);
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_too_large_size_parameters)
    {
      auto const mfr_desc = std::string(protocol::DESC_STR_LEN, 'A');
      auto const hw_desc = std::string(protocol::DESC_STR_LEN + 1, 'B');
      auto const sw_desc = std::string(protocol::DESC_STR_LEN, 'C');
      auto const serial_num = std::string(protocol::DESC_STR_LEN, 'D');
      auto const dp_desc = std::string(protocol::DESC_STR_LEN, 'E');
      auto const xid = 0x87654321;

      stats::description_reply sut{
        mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid
      };

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_stats_reply) + sizeof(protocol::ofp_desc_stats));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.manufacture_desc().size() == protocol::DESC_STR_LEN - 1);
      auto const manufacture_desc = sut.manufacture_desc();
      BOOST_TEST(manufacture_desc.size() == protocol::DESC_STR_LEN - 1);
      BOOST_TEST(
          manufacture_desc
       == boost::string_ref(mfr_desc.data(), manufacture_desc.size()));
      auto const hardware_desc = sut.hardware_desc();
      BOOST_TEST(hardware_desc.size() == protocol::DESC_STR_LEN - 1);
      BOOST_TEST(
          hardware_desc
       == boost::string_ref(hw_desc.data(), hardware_desc.size()));
      auto const software_desc = sut.software_desc();
      BOOST_TEST(software_desc.size() == protocol::DESC_STR_LEN - 1);
      BOOST_TEST(
          software_desc
       == boost::string_ref(sw_desc.data(), software_desc.size()));
      auto const serial_number = sut.serial_number();
      BOOST_TEST(serial_number.size() == protocol::SERIAL_NUM_LEN - 1);
      BOOST_TEST(
          serial_number
       == boost::string_ref(serial_num.data(), serial_number.size()));
      auto const datapath_desc = sut.datapath_desc();
      BOOST_TEST(datapath_desc.size() == protocol::DESC_STR_LEN - 1);
      BOOST_TEST(
          datapath_desc
       == boost::string_ref(dp_desc.data(), datapath_desc.size()));
    }
    BOOST_AUTO_TEST_CASE(is_constructible_from_empty_strings)
    {
      auto const xid = 0x01020304;

      stats::description_reply sut{"", "", "", "", "", xid};

      BOOST_TEST(
          sut.length()
       == sizeof(protocol::ofp_stats_reply) + sizeof(protocol::ofp_desc_stats));
      BOOST_TEST(sut.xid() == xid);
      BOOST_TEST(sut.manufacture_desc() == "");
      BOOST_TEST(sut.hardware_desc() == "");
      BOOST_TEST(sut.software_desc() == "");
      BOOST_TEST(sut.serial_number() == "");
      BOOST_TEST(sut.datapath_desc() == "");
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_non_const_lvalue, description_reply_fixture)
    {
      auto const copy = sut;

      BOOST_TEST((copy == sut));
    }
    BOOST_FIXTURE_TEST_CASE(
        is_copy_constructible_from_const_lvalue, description_reply_fixture)
    {
      auto const& const_description_reply = sut;

      auto const copy = const_description_reply;

      BOOST_TEST((copy == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_FIXTURE_TEST_SUITE(equality, description_reply_parameters)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = stats::description_reply{
        mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid
      };

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_strings_and_xid_are_equal)
    {
      BOOST_TEST(
          (stats::description_reply{
              mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid}
        == stats::description_reply{
              mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_mfr_desc_is_not_equal)
    {
      BOOST_TEST(
          (stats::description_reply{
              "mfr_desc1", hw_desc, sw_desc, serial_num, dp_desc, xid}
        != stats::description_reply{
              "mfr_desc2", hw_desc, sw_desc, serial_num, dp_desc, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_hw_desc_is_not_equal)
    {
      BOOST_TEST(
          (stats::description_reply{
              mfr_desc, "hw_desc1", sw_desc, serial_num, dp_desc, xid}
        != stats::description_reply{
              mfr_desc, "hw_desc2", sw_desc, serial_num, dp_desc, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_sw_desc_is_not_equal)
    {
      BOOST_TEST(
          (stats::description_reply{
              mfr_desc, hw_desc, "sw_desc1", serial_num, dp_desc, xid}
        != stats::description_reply{
              mfr_desc, hw_desc, "sw_desc2", serial_num, dp_desc, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_serial_num_is_not_equal)
    {
      BOOST_TEST(
          (stats::description_reply{
              mfr_desc, hw_desc, sw_desc, "serial_num1", dp_desc, xid}
        != stats::description_reply{
              mfr_desc, hw_desc, sw_desc, "serial_num2", dp_desc, xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_dp_desc_is_not_equal)
    {
      BOOST_TEST(
          (stats::description_reply{
              mfr_desc, hw_desc, sw_desc, serial_num, "dp_desc1", xid}
        != stats::description_reply{
              mfr_desc, hw_desc, sw_desc, serial_num, "dp_desc2", xid}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_xid_is_not_equal)
    {
      BOOST_TEST(
          (stats::description_reply{
              mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, 1}
        != stats::description_reply{
              mfr_desc, hw_desc, sw_desc, serial_num, dp_desc, 2}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_FIXTURE_TEST_SUITE(encode, description_reply_fixture)
    BOOST_AUTO_TEST_CASE(generates_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == bin, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, description_reply_fixture)
    BOOST_AUTO_TEST_CASE(constructs_description_reply_from_binary)
    {
      auto it = bin.begin();

      auto const description_reply
        = stats::description_reply::decode(it, bin.end());

      BOOST_TEST((it == bin.end()));
      BOOST_TEST((description_reply == sut));
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // description_reply

BOOST_AUTO_TEST_SUITE_END() // stats_test
