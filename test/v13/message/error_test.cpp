#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/message/error.hpp>
#include <boost/test/unit_test.hpp>
#include <canard/net/ofp/v13/message/switch_config.hpp>
#include <cstdint>
#include <vector>
#include <boost/endian/conversion.hpp>

#include <canard/net/ofp/v13/io/openflow.hpp>
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;

constexpr auto error_size = sizeof(protocol::ofp_error_msg);

BOOST_AUTO_TEST_SUITE(message_test)

BOOST_AUTO_TEST_SUITE(error_test)

    BOOST_AUTO_TEST_CASE(construct_from_binary_data_test)
    {
        auto const etype = protocol::OFPET_HELLO_FAILED;
        auto const ecode = protocol::OFPHFC_INCOMPATIBLE;
        auto const edata = "incompatible version"_bbin;

        auto const sut
            = v13::messages::error{etype, ecode, edata};

        BOOST_TEST(sut.version() == protocol::OFP_VERSION);
        BOOST_TEST(sut.type() == protocol::OFPT_ERROR);
        BOOST_TEST(sut.length() == error_size + edata.size());
        BOOST_TEST(sut.error_type() == etype);
        BOOST_TEST(sut.error_code() == ecode);
        BOOST_TEST(sut.data_length() == edata.size());
        BOOST_TEST(sut.data() == edata, boost::test_tools::per_element{});
    }

    BOOST_AUTO_TEST_CASE(construct_from_message_test)
    {
        auto const etype = protocol::OFPET_SWITCH_CONFIG_FAILED;
        auto const ecode = protocol::OFPSCFC_BAD_FLAGS;
        auto const msg = v13::messages::set_config{0xffff, 0xffff};

        auto const sut = v13::messages::error{etype, ecode, msg};

        BOOST_TEST(sut.version() == protocol::OFP_VERSION);
        BOOST_TEST(sut.type() == protocol::OFPT_ERROR);
        BOOST_TEST(sut.length() == error_size + msg.length());
        BOOST_TEST(sut.xid() == msg.xid());
        BOOST_TEST(sut.error_type() == etype);
        BOOST_TEST(sut.error_code() == ecode);
        BOOST_TEST(sut.data_length() == msg.length());
        auto buffer = std::vector<std::uint8_t>{};
        BOOST_TEST(sut.data() == msg.encode(buffer), boost::test_tools::per_element{});
    }

    struct error_fixture
    {
        v13::messages::set_config const msg{0xff01, 0xfffe, 0x12345678};
        v13::messages::error sut = v13::messages::error{
              protocol::OFPET_SWITCH_CONFIG_FAILED
            , protocol::OFPSCFC_BAD_FLAGS, msg
        };
        std::vector<std::uint8_t> bin_error
            = "\x04\x01\x00\x18\x12\x34\x56\x78"
              "\x00\x0a\x00\x00\x04\x09\x00\x0c"
              "\x12\x34\x56\x78\xff\x01\xff\xfe"_bin;
    };

    BOOST_FIXTURE_TEST_CASE(copy_construct_test, error_fixture)
    {
        auto const copy = sut;

        BOOST_TEST(copy.version() == sut.version());
        BOOST_TEST(copy.type() == sut.type());
        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST(copy.xid() == sut.xid());
        BOOST_TEST(copy.error_type() == sut.error_type());
        BOOST_TEST(copy.error_code() == sut.error_code());
        BOOST_TEST(copy.data_length() == sut.data_length());
        BOOST_TEST((copy.data() == sut.data()));
    }

    BOOST_FIXTURE_TEST_CASE(move_construct_test, error_fixture)
    {
        auto src = sut;

        auto const copy = std::move(src);

        BOOST_TEST(copy.version() == sut.version());
        BOOST_TEST(copy.type() == sut.type());
        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST(copy.xid() == sut.xid());
        BOOST_TEST(copy.error_type() == sut.error_type());
        BOOST_TEST(copy.error_code() == sut.error_code());
        BOOST_TEST(copy.data_length() == sut.data_length());
        BOOST_TEST((copy.data() == sut.data()));

        BOOST_TEST(src.length() == error_size);
        BOOST_TEST(src.data_length() == 0);
        BOOST_TEST(src.data().empty());
    }

    BOOST_FIXTURE_TEST_CASE(copy_assignment_test, error_fixture)
    {
        auto copy = v13::messages::error{
              protocol::OFPET_FLOW_MOD_FAILED
            , protocol::OFPFMFC_TABLE_FULL
            , ""_bbin
        };

        copy = sut;

        BOOST_TEST(copy.version() == sut.version());
        BOOST_TEST(copy.type() == sut.type());
        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST(copy.xid() == sut.xid());
        BOOST_TEST(copy.error_type() == sut.error_type());
        BOOST_TEST(copy.error_code() == sut.error_code());
        BOOST_TEST(copy.data_length() == sut.data_length());
        BOOST_TEST((copy.data() == sut.data()));
    }

    BOOST_FIXTURE_TEST_CASE(move_assignment_test, error_fixture)
    {
        auto copy = v13::messages::error{
              protocol::OFPET_FLOW_MOD_FAILED
            , protocol::OFPFMFC_TABLE_FULL
            , ""_bbin
        };
        auto src = sut;

        copy = std::move(src);

        BOOST_TEST(copy.version() == sut.version());
        BOOST_TEST(copy.type() == sut.type());
        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST(copy.xid() == sut.xid());
        BOOST_TEST(copy.error_type() == sut.error_type());
        BOOST_TEST(copy.error_code() == sut.error_code());
        BOOST_TEST(copy.data_length() == sut.data_length());
        BOOST_TEST((copy.data() == sut.data()));

        BOOST_TEST(src.length() == error_size);
        BOOST_TEST(src.data_length() == 0);
        BOOST_TEST(src.data().empty());
    }


    BOOST_FIXTURE_TEST_CASE(extract_data_test, error_fixture)
    {
        auto const data = sut.extract_data();

        BOOST_TEST(sut.length() == error_size);
        BOOST_TEST(sut.data_length() == 0);
        BOOST_TEST(sut.data().empty());

        auto data_range = boost::make_iterator_range(data);
        auto buffer = std::vector<std::uint8_t>{};
        BOOST_TEST(data_range == msg.encode(buffer), boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(failed_request_header_test, error_fixture)
    {
        auto const header = sut.failed_request_header();

        BOOST_TEST(header.version == msg.version());
        BOOST_TEST(header.type == msg.type());
        BOOST_TEST(header.length == msg.length());
        BOOST_TEST(header.xid == msg.xid());
    }

    BOOST_FIXTURE_TEST_CASE(encode_test, error_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == bin_error);
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, error_fixture)
    {
        auto it = bin_error.begin();
        auto const it_end = bin_error.end();

        auto error_msg = v13::messages::error::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST(error_msg.version() == sut.version());
        BOOST_TEST(error_msg.type() == sut.type());
        BOOST_TEST(error_msg.length() == sut.length());
        BOOST_TEST(error_msg.xid() == sut.xid());
        BOOST_TEST(error_msg.error_type() == sut.error_type());
        BOOST_TEST(error_msg.error_code() == sut.error_code());
        BOOST_TEST(error_msg.data_length() == sut.data_length());
        BOOST_TEST((error_msg.data() == sut.data()));
    }

BOOST_AUTO_TEST_SUITE_END() // error_test

BOOST_AUTO_TEST_SUITE_END() // message_test

