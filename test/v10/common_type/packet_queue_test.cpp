#define BOOST_TEST_DYN_LINK
#include <canard/network/openflow/v10/packet_queue.hpp>
#include <boost/test/unit_test.hpp>

#include "../../test_utility.hpp"

namespace ofp = canard::net::ofp;
namespace v10 = ofp::v10;
namespace queue_props = v10::queue_properties;
namespace v10_detail = v10::v10_detail;

using properties_t = v10::packet_queue::properties_type;

namespace {
struct packet_queue_fixture
{
  v10::packet_queue sut{
      0x12345678, properties_t{
        v10::any_queue_property{queue_props::min_rate{0x1234}}
      }
  };
  std::vector<unsigned char> binary
    = "\x12\x34\x56\x78\x00\x18\x00\x00"
      "\x00\x01\x00\x10\x00\x00\x00\x00""\x12\x34\x00\x00\x00\x00\x00\x00"_bin;

  v10::packet_queue multiple_queue_props_packet_queue{
      0x12345678
    , properties_t{
          v10::any_queue_property{queue_props::min_rate{0x1234}}
        , v10::any_queue_property{queue_props::min_rate{0x5678}}
      }
  };
  std::vector<unsigned char> multiple_queue_props_binary
    = "\x12\x34\x56\x78\x00\x28\x00\x00"
      "\x00\x01\x00\x10\x00\x00\x00\x00""\x12\x34\x00\x00\x00\x00\x00\x00"
      "\x00\x01\x00\x10\x00\x00\x00\x00""\x56\x78\x00\x00\x00\x00\x00\x00"_bin;

  v10::packet_queue empty_queue_props_packet_queue{0x12345678, properties_t{}};
  std::vector<unsigned char> empty_queue_props_binary
    = "\x12\x34\x56\x78\x00\x08\x00\x00"_bin;
};
}

BOOST_AUTO_TEST_SUITE(common_type_test)
BOOST_AUTO_TEST_SUITE(packet_queue_test)

  BOOST_AUTO_TEST_SUITE(constructor)
    BOOST_AUTO_TEST_CASE(constructible_from_empty_properties)
    {
      auto const queue_id = std::uint32_t{1};
      auto const empty_props = properties_t{};

      auto const sut = v10::packet_queue{queue_id, empty_props};

      BOOST_TEST(sut.queue_id() == queue_id);
      BOOST_TEST(sut.length() == sizeof(v10_detail::ofp_packet_queue));
      BOOST_TEST((sut.properties() == empty_props));
    }
  BOOST_AUTO_TEST_SUITE_END() // constructor

  BOOST_AUTO_TEST_SUITE(equality)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = v10::packet_queue{0x12345678, {
        v10::any_queue_property{queue_props::min_rate{0x1234}}
      }};

      BOOST_TEST((sut == sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_queue_id_and_properties_are_equal)
    {
      auto const queue_id = std::uint32_t{1};
      auto const properties = {
        v10::any_queue_property{queue_props::min_rate{1000}}
      };

      BOOST_TEST(
          (v10::packet_queue{queue_id, properties}
        == v10::packet_queue{queue_id, properties}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_queue_id_is_equal_and_property_set_is_empty)
    {
      auto const queue_id = std::uint32_t{2};

      BOOST_TEST(
          (v10::packet_queue{queue_id, {}}
        == v10::packet_queue{queue_id, {}}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_queue_id_is_not_equal)
    {
      auto const properties = {
        v10::any_queue_property{queue_props::min_rate{1000}}
      };

      BOOST_TEST(
          (v10::packet_queue{1, properties}
        != v10::packet_queue{2, properties}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_properties_value_is_not_equal)
    {
      auto const queue_id = std::uint32_t{1};

      BOOST_TEST(
          (v10::packet_queue{
             queue_id, { v10::any_queue_property{queue_props::min_rate{1}} }}
        != v10::packet_queue{
             queue_id, { v10::any_queue_property{queue_props::min_rate{2}} }}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_properties_value_is_not_equal_but_equivalent)
    {
      auto const queue_id = std::uint32_t{1};

      BOOST_TEST(
          (v10::packet_queue{
               queue_id
             , { v10::any_queue_property{queue_props::min_rate{1001}} }}
        != v10::packet_queue{
               queue_id
             , { v10::any_queue_property{queue_props::min_rate{1002}} }}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_property_set_size_is_not_equal)
    {
      auto const queue_id = std::uint32_t{1};

      BOOST_TEST(
          (v10::packet_queue{
               queue_id
             , { v10::any_queue_property{queue_props::min_rate{1}} }}
        != v10::packet_queue{
               queue_id
             , { v10::any_queue_property{queue_props::min_rate{1}}
               , v10::any_queue_property{queue_props::min_rate{1}} }}));
    }
  BOOST_AUTO_TEST_SUITE_END() // equality

  BOOST_AUTO_TEST_SUITE(function_equivalent)
    BOOST_AUTO_TEST_CASE(is_true_if_object_is_same)
    {
      auto const sut = v10::packet_queue{0x12345678, {
        v10::any_queue_property{queue_props::min_rate{0x1234}}
      }};

      BOOST_TEST(equivalent(sut, sut));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_queue_id_and_properties_are_equal)
    {
      auto const queue_id = std::uint32_t{1};
      auto const properties = {
        v10::any_queue_property{queue_props::min_rate{1000}}
      };

      BOOST_TEST(
          equivalent(
              v10::packet_queue{queue_id, properties}
            , v10::packet_queue{queue_id, properties}));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_queue_id_is_equal_and_property_set_is_empty)
    {
      auto const queue_id = std::uint32_t{2};

      BOOST_TEST(
          equivalent(
              v10::packet_queue{queue_id, {}}
            , v10::packet_queue{queue_id, {}}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_queue_id_is_not_equal)
    {
      auto const properties = {
        v10::any_queue_property{queue_props::min_rate{1000}}
      };

      BOOST_TEST(
          !equivalent(
              v10::packet_queue{1, properties}
            , v10::packet_queue{2, properties}));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_properties_value_is_not_equal)
    {
      auto const queue_id = std::uint32_t{1};

      BOOST_TEST(
          !equivalent(
              v10::packet_queue{
                queue_id, { v10::any_queue_property{queue_props::min_rate{1}} }
              }
            , v10::packet_queue{
                queue_id, { v10::any_queue_property{queue_props::min_rate{2}} }
              }));
    }
    BOOST_AUTO_TEST_CASE(is_true_if_properties_value_is_not_equal_but_equivalent)
    {
      auto const queue_id = std::uint32_t{1};

      BOOST_TEST(
          equivalent(
              v10::packet_queue{
                  queue_id
                , { v10::any_queue_property{queue_props::min_rate{1001}} }
              }
            , v10::packet_queue{
                  queue_id
                , { v10::any_queue_property{queue_props::min_rate{1002}} }
              }));
    }
    BOOST_AUTO_TEST_CASE(is_false_if_property_set_size_is_not_equal)
    {
      auto const queue_id = std::uint32_t{1};

      BOOST_TEST(
          !equivalent(
              v10::packet_queue{
                  queue_id
                , { v10::any_queue_property{queue_props::min_rate{1}} }
              }
            , v10::packet_queue{
                  queue_id
                , { v10::any_queue_property{queue_props::min_rate{1}}
                  , v10::any_queue_property{queue_props::min_rate{1}} }
              }));
    }
  BOOST_AUTO_TEST_SUITE_END() // function_equivalent

  BOOST_FIXTURE_TEST_SUITE(encode, packet_queue_fixture)
    BOOST_AUTO_TEST_CASE(generate_binary)
    {
      auto buf = std::vector<unsigned char>{};

      sut.encode(buf);

      BOOST_TEST(buf.size() == sut.length());
      BOOST_TEST(buf == binary, boost::test_tools::per_element{});
    }
  BOOST_AUTO_TEST_SUITE_END() // encode

  BOOST_FIXTURE_TEST_SUITE(decode, packet_queue_fixture)
    BOOST_AUTO_TEST_CASE(constructible_from_binary)
    {
      auto it = binary.begin();

      auto const packet_queue
        = v10::packet_queue::decode(it, binary.end());

      BOOST_TEST((it == binary.end()));
      BOOST_TEST((packet_queue == sut));
    }
    BOOST_AUTO_TEST_CASE(
        constructible_from_binary_containing_multiple_properties)
    {
      auto it = multiple_queue_props_binary.begin();

      auto const packet_queue
        = v10::packet_queue::decode(it, multiple_queue_props_binary.end());

      BOOST_TEST((it == multiple_queue_props_binary.end()));
      BOOST_TEST((packet_queue == multiple_queue_props_packet_queue));
    }
    BOOST_AUTO_TEST_CASE(constructible_from_binary_containing_no_property)
    {
      auto it = empty_queue_props_binary.begin();

      auto const packet_queue
        = v10::packet_queue::decode(it, empty_queue_props_binary.end());

      BOOST_TEST((it == empty_queue_props_binary.end()));
      BOOST_TEST((packet_queue == empty_queue_props_packet_queue));
    }
    BOOST_AUTO_TEST_CASE(
        throw_exception_if_binary_size_is_invalid)
    {
      binary.resize(
            sizeof(v10_detail::ofp_packet_queue)
          + (sizeof(v10_detail::ofp_queue_prop_header) - 1));
      auto it = binary.begin();

      BOOST_CHECK_THROW(
            v10::packet_queue::decode(it, binary.end())
          , std::runtime_error);
    }
    BOOST_AUTO_TEST_CASE(
        throw_exception_if_length_field_is_smaller_than_minmal)
    {
      binary[5] = 0;
      auto it = binary.begin();

      BOOST_CHECK_THROW(
            v10::packet_queue::decode(it, std::prev(binary.end()))
          , std::runtime_error);
    }
    BOOST_AUTO_TEST_CASE(
        throw_exception_if_length_field_is_larger_than_binary_size)
    {
      binary[5] = binary.size() + 1;
      auto it = binary.begin();

      BOOST_CHECK_THROW(
            v10::packet_queue::decode(it, std::prev(binary.end()))
          , std::runtime_error);
    }
  BOOST_AUTO_TEST_SUITE_END() // decode

BOOST_AUTO_TEST_SUITE_END() // packet_queue_test
BOOST_AUTO_TEST_SUITE_END() // common_type_test
