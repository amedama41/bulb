#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v13/common/packet_queue.hpp>
#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <vector>

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace protocol = v13::protocol;
namespace queue_props = v13::queue_properties;

namespace {

auto operator ""_bin(char const* const str, std::size_t const size)
    -> std::vector<std::uint8_t>
{
    return std::vector<std::uint8_t>(str, str + size);
}

struct properties_fixture
{
    v13::packet_queue::properties_type properties = {
          v13::any_queue_property{queue_props::min_rate{0x1234}}
        , v13::any_queue_property{queue_props::max_rate{0x5678}}
    }; // 16 + 16 = 32
};

struct packet_queue_fixture : properties_fixture
{
    v13::packet_queue sut{0xf1f2f3f4, 0xf5f6f7f8, properties};
    std::vector<std::uint8_t> binary
        = "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8""\x00\x30\x00\x00\x00\x00\x00\x00"
          "\x00\x01\x00\x10\x00\x00\x00\x00""\x12\x34\x00\x00\x00\x00\x00\x00"
          "\x00\x02\x00\x10\x00\x00\x00\x00""\x56\x78\x00\x00\x00\x00\x00\x00"
          ""_bin
        ;
};

}

BOOST_AUTO_TEST_SUITE(packet_queue_test)

    BOOST_FIXTURE_TEST_CASE(construct_from_queue_id_test, properties_fixture)
    {
        auto const queue_id = of::queue_id{32, 43};

        auto const sut = v13::packet_queue{queue_id, properties};

        BOOST_TEST(sut.queue_id() == queue_id.queue());
        BOOST_TEST(sut.port_no() == queue_id.port());
        BOOST_TEST(sut.length() == sizeof(protocol::ofp_packet_queue) + 32);
        BOOST_TEST(sut.properties().size() == properties.size());
        BOOST_TEST(sut.properties()[0].type() == properties[0].type());
        BOOST_TEST(sut.properties()[0].length() == properties[0].length());
        BOOST_TEST(sut.properties()[1].type() == properties[1].type());
        BOOST_TEST(sut.properties()[1].length() == properties[1].length());
    }

    BOOST_FIXTURE_TEST_CASE(copy_construct_test, packet_queue_fixture)
    {
        auto const copy = sut;

        BOOST_TEST(copy.queue_id() == sut.queue_id());
        BOOST_TEST(copy.port_no() == sut.port_no());
        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST(copy.properties().size() == sut.properties().size());
    }

    BOOST_FIXTURE_TEST_CASE(move_construct_test, packet_queue_fixture)
    {
        auto src = sut;

        auto const copy = std::move(src);

        BOOST_TEST(copy.queue_id() == sut.queue_id());
        BOOST_TEST(copy.port_no() == sut.port_no());
        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST(copy.properties().size() == sut.properties().size());
        BOOST_TEST(src.length() == sizeof(protocol::ofp_packet_queue));
        BOOST_TEST(src.properties().size() == 0);
    }

    BOOST_FIXTURE_TEST_CASE(copy_assignment_test, packet_queue_fixture)
    {
        auto copy = v13::packet_queue{0, 0, {}};
        copy = sut;

        BOOST_TEST(copy.queue_id() == sut.queue_id());
        BOOST_TEST(copy.port_no() == sut.port_no());
        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST(copy.properties().size() == sut.properties().size());
    }

    BOOST_FIXTURE_TEST_CASE(move_assignment_test, packet_queue_fixture)
    {
        auto copy = v13::packet_queue{0, 0, {}};
        auto src = sut;

        copy = std::move(src);

        BOOST_TEST(copy.queue_id() == sut.queue_id());
        BOOST_TEST(copy.port_no() == sut.port_no());
        BOOST_TEST(copy.length() == sut.length());
        BOOST_TEST(copy.properties().size() == sut.properties().size());
        BOOST_TEST(src.length() == sizeof(protocol::ofp_packet_queue));
        BOOST_TEST(src.properties().size() == 0);
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = v13::packet_queue{0, 1, {
            v13::any_queue_property{queue_props::min_rate{0}}
          , v13::any_queue_property{queue_props::max_rate{0}}
        }};

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_queue_id_and_properties_are_equal)
      {
        auto const queue_id = of::queue_id{1, 2};
        auto const properties = {
            v13::any_queue_property{queue_props::min_rate{1}}
          , v13::any_queue_property{queue_props::max_rate{2}}
        };

        BOOST_TEST(
            (v13::packet_queue{queue_id, properties}
             == v13::packet_queue{queue_id, properties}));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_properties_are_empty)
      {
        auto const queue_id = of::queue_id{1, 2};

        BOOST_TEST(
            (v13::packet_queue{queue_id, {}} == v13::packet_queue{queue_id, {}}));
      }
      BOOST_AUTO_TEST_CASE(false_if_queue_id_is_not_equal)
      {
        auto const port_no = std::uint32_t{32};
        auto const properties = {
            v13::any_queue_property{queue_props::min_rate{2}}
          , v13::any_queue_property{queue_props::max_rate{3}}
        };

        BOOST_TEST(
            (v13::packet_queue{1, port_no, properties}
             != v13::packet_queue{2, port_no, properties}));
      }
      BOOST_AUTO_TEST_CASE(false_if_port_no_is_not_equal)
      {
        auto const queue_id = std::uint32_t{2};
        auto const properties = {
            v13::any_queue_property{queue_props::min_rate{2}}
          , v13::any_queue_property{queue_props::max_rate{3}}
        };

        BOOST_TEST(
            (v13::packet_queue{queue_id, 1, properties}
             != v13::packet_queue{queue_id, 2, properties}));
      }
      BOOST_AUTO_TEST_CASE(false_if_properties_value_is_not_equal)
      {
        auto const queue_id = of::queue_id{1, 2};
        auto const min_rate
          = v13::any_queue_property{queue_props::min_rate{2}};
        auto const max_rate1
          = v13::any_queue_property{queue_props::max_rate{1}};
        auto const max_rate2
          = v13::any_queue_property{queue_props::max_rate{2}};

        BOOST_TEST(
            (v13::packet_queue{queue_id, { min_rate, max_rate1 }}
             != v13::packet_queue{queue_id, { min_rate, max_rate2 }}));
      }
      BOOST_AUTO_TEST_CASE(false_if_properties_size_is_not_equal)
      {
        auto const queue_id = of::queue_id{1, 2};
        auto const min_rate
          = v13::any_queue_property{queue_props::min_rate{2}};
        auto const max_rate
          = v13::any_queue_property{queue_props::max_rate{3}};

        BOOST_TEST(
            (v13::packet_queue{queue_id, { min_rate, max_rate }}
             != v13::packet_queue{queue_id, { min_rate }}));
      }
      BOOST_AUTO_TEST_CASE(false_if_properties_is_not_equal_but_equivalent)
      {
        auto const queue_id = of::queue_id{1, 2};
        auto const zero_pad_min_rate
          = v13::any_queue_property{queue_props::min_rate{0x234}};
        auto const binary
          = "\x00\x01\x00\x10\x00\x00\x00\x00""\x02\x34\x00\x00\x00\x00\x00\x01"
            ""_bin;
        auto it = binary.begin();
        auto const nonzero_pad_min_rate = v13::any_queue_property{
          queue_props::min_rate::decode(it, binary.end())
        };

        BOOST_TEST(
            (v13::packet_queue{queue_id, { zero_pad_min_rate }}
             != v13::packet_queue{queue_id, { nonzero_pad_min_rate }}));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = v13::packet_queue{0, 1, {
            v13::any_queue_property{queue_props::min_rate{0}}
          , v13::any_queue_property{queue_props::max_rate{0}}
        }};

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_queue_id_and_properties_are_equal)
      {
        auto const queue_id = of::queue_id{1, 2};
        auto const properties = {
            v13::any_queue_property{queue_props::min_rate{1}}
          , v13::any_queue_property{queue_props::max_rate{2}}
        };

        BOOST_TEST(
            equivalent(
                v13::packet_queue{queue_id, properties}
              , v13::packet_queue{queue_id, properties}));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_properties_are_empty)
      {
        auto const queue_id = of::queue_id{1, 2};

        BOOST_TEST(
            equivalent(
              v13::packet_queue{queue_id, {}}, v13::packet_queue{queue_id, {}}));
      }
      BOOST_AUTO_TEST_CASE(false_if_queue_id_is_not_equal)
      {
        auto const port_no = std::uint32_t{32};
        auto const properties = {
            v13::any_queue_property{queue_props::min_rate{2}}
          , v13::any_queue_property{queue_props::max_rate{3}}
        };

        BOOST_TEST(
            !equivalent(
                v13::packet_queue{1, port_no, properties}
              , v13::packet_queue{2, port_no, properties}));
      }
      BOOST_AUTO_TEST_CASE(false_if_port_no_is_not_equal)
      {
        auto const queue_id = std::uint32_t{2};
        auto const properties = {
            v13::any_queue_property{queue_props::min_rate{2}}
          , v13::any_queue_property{queue_props::max_rate{3}}
        };

        BOOST_TEST(
            !equivalent(
                v13::packet_queue{queue_id, 1, properties}
              , v13::packet_queue{queue_id, 2, properties}));
      }
      BOOST_AUTO_TEST_CASE(false_if_properties_value_is_not_equal)
      {
        auto const queue_id = of::queue_id{1, 2};
        auto const min_rate
          = v13::any_queue_property{queue_props::min_rate{2}};
        auto const max_rate1
          = v13::any_queue_property{queue_props::max_rate{1}};
        auto const max_rate2
          = v13::any_queue_property{queue_props::max_rate{2}};

        BOOST_TEST(
            !equivalent(
                v13::packet_queue{queue_id, { min_rate, max_rate1 }}
              , v13::packet_queue{queue_id, { min_rate, max_rate2 }}));
      }
      BOOST_AUTO_TEST_CASE(false_if_properties_size_is_not_equal)
      {
        auto const queue_id = of::queue_id{1, 2};
        auto const min_rate
          = v13::any_queue_property{queue_props::min_rate{2}};
        auto const max_rate
          = v13::any_queue_property{queue_props::max_rate{3}};

        BOOST_TEST(
            !equivalent(
                v13::packet_queue{queue_id, { min_rate, max_rate }}
              , v13::packet_queue{queue_id, { min_rate }}));
      }
      BOOST_AUTO_TEST_CASE(true_if_properties_is_not_equal_but_equivalent)
      {
        auto const queue_id = of::queue_id{1, 2};
        auto const zero_pad_min_rate
          = v13::any_queue_property{queue_props::min_rate{0x234}};
        auto const binary
          = "\x00\x01\x00\x10\x00\x00\x00\x00""\x02\x34\x00\x00\x00\x00\x00\x01"
            ""_bin;
        auto it = binary.begin();
        auto const nonzero_pad_min_rate = v13::any_queue_property{
          queue_props::min_rate::decode(it, binary.end())
        };

        BOOST_TEST(
            equivalent(
                v13::packet_queue{queue_id, { zero_pad_min_rate }}
              , v13::packet_queue{queue_id, { nonzero_pad_min_rate }}));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, packet_queue_fixture)
    {
        auto buffer = std::vector<std::uint8_t>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == sut.length());
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, packet_queue_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const packet_queue = v13::packet_queue::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST(packet_queue.queue_id() == sut.queue_id());
        BOOST_TEST(packet_queue.port_no() == sut.port_no());
        BOOST_TEST(packet_queue.length() == sut.length());
        BOOST_TEST(packet_queue.properties().size() == sut.properties().size());
    }

BOOST_AUTO_TEST_SUITE_END() // packet_queue_test
