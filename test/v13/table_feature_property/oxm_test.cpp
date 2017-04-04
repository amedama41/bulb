#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/table_feature_property/oxm.hpp>
#include <boost/test/unit_test.hpp>

#include <canard/net/ofp/v13/io/openflow.hpp>
#include <canard/net/ofp/v13/table_feature_property/id/oxm_id.hpp>

#include <type_traits>
#include "../oxm_headers.hpp"
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace table_feature_properties = v13::table_feature_properties;
namespace protocol = v13::protocol;

namespace {
struct match_fixture
{
    table_feature_properties::match sut{
          v13::oxm_id{OXM_OF_IN_PORT}
        , v13::oxm_id{OXM_OF_ETH_SRC_W}
        , v13::oxm_id{OXM_OF_ETH_TYPE}
        , v13::oxm_id{OXM_OF_IPV4_SRC_W}
        , v13::oxm_id{OXM_OF_IP_PROTO}
        , v13::oxm_id{OXM_OF_TCP_DST}
        , v13::oxm_id{0x12, true, 0x34, 0x12345678}
    };
    std::vector<unsigned char> binary
        = "\x00\x08\x00\x24\x80\x00\x00\x04""\x80\x00\x09\x0c\x80\x00\x0a\x02"
          "\x80\x00\x17\x08\x80\x00\x14\x01""\x80\x00\x1c\x02\xff\xff\x25\x34"
          "\x12\x34\x56\x78\x00\x00\x00\x00"
          ""_bin;
};
}

BOOST_AUTO_TEST_SUITE(table_feature_property_test)

BOOST_AUTO_TEST_SUITE(match_test)

    BOOST_AUTO_TEST_SUITE(type_definition_test)
      BOOST_AUTO_TEST_CASE(type)
      {
        using sut = table_feature_properties::match;

        using type = std::integral_constant<std::uint16_t, sut::type()>;

        BOOST_TEST(type::value == protocol::OFPTFPT_MATCH);
      }
      BOOST_AUTO_TEST_CASE(min_length)
      {
        using sut = table_feature_properties::match;

        using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

        BOOST_TEST(min_length::value == 4);
      }
      BOOST_AUTO_TEST_CASE(min_byte_length)
      {
        using sut = table_feature_properties::match;

        using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

        BOOST_TEST(min_byte_length::value == 8);
      }
    BOOST_AUTO_TEST_SUITE_END() // type_definition_test

    BOOST_AUTO_TEST_CASE(default_construct_test)
    {
        auto const sut = table_feature_properties::match{};

        BOOST_TEST(sut.length() == sizeof(protocol::ofp_table_feature_prop_oxm));
        BOOST_TEST(sut.oxm_ids().empty());
    }

    BOOST_AUTO_TEST_CASE(construct_from_single_id_test)
    {
        auto const in_port = v13::oxm_id{OXM_OF_IN_PORT};

        auto const sut = table_feature_properties::match{in_port};

        BOOST_TEST_REQUIRE(in_port.length() == 4);
        BOOST_TEST(sut.length()
                == sizeof(protocol::ofp_table_feature_prop_oxm) + 4);
        BOOST_TEST_REQUIRE(sut.oxm_ids().size() == 1);
        BOOST_TEST((sut.oxm_ids()[0] == in_port));
    }

    BOOST_AUTO_TEST_CASE(construct_from_multiple_ids_test)
    {
        auto const in_port = v13::oxm_id{OXM_OF_IN_PORT};
        auto const eth_dst = v13::oxm_id{OXM_OF_ETH_DST_W};
        auto const ip_proto = v13::oxm_id{OXM_OF_IP_PROTO};
        auto const tcp_src = v13::oxm_id{OXM_OF_TCP_SRC};
        auto const experimenter = v13::oxm_id{0x12, true, 0x04, 0x12345678};

        auto const sut = table_feature_properties::match{
            in_port, eth_dst, ip_proto, tcp_src, experimenter
        };

        BOOST_TEST_REQUIRE(experimenter.length() == 8);
        BOOST_TEST(sut.length()
                == sizeof(protocol::ofp_table_feature_prop_oxm) + 4 * 4 + 8);
        BOOST_TEST_REQUIRE(sut.oxm_ids().size() == 5);
        BOOST_TEST((sut.oxm_ids()[0] == in_port));
        BOOST_TEST((sut.oxm_ids()[1] == eth_dst));
        BOOST_TEST((sut.oxm_ids()[2] == ip_proto));
        BOOST_TEST((sut.oxm_ids()[3] == tcp_src));
        BOOST_TEST((sut.oxm_ids()[4] == experimenter));
    }

    BOOST_FIXTURE_TEST_CASE(copy_construct_test, match_fixture)
    {
        auto const copy = sut;

        BOOST_TEST((copy == sut));
    }

    BOOST_FIXTURE_TEST_CASE(move_construct_test, match_fixture)
    {
        auto src = sut;

        auto const copy = std::move(src);

        BOOST_TEST((copy == sut));
        BOOST_TEST(src.length() == sizeof(protocol::ofp_table_feature_prop_oxm));
        BOOST_TEST(src.oxm_ids().empty());
    }

    BOOST_FIXTURE_TEST_CASE(copy_assignment_test, match_fixture)
    {
        auto copy = table_feature_properties::match{};

        copy = sut;

        BOOST_TEST((copy == sut));
    }

    BOOST_FIXTURE_TEST_CASE(move_assignment_test, match_fixture)
    {
        auto src = sut;
        auto copy = table_feature_properties::match{};

        copy = std::move(src);

        BOOST_TEST((copy == sut));
        BOOST_TEST(src.length() == sizeof(protocol::ofp_table_feature_prop_oxm));
        BOOST_TEST(src.oxm_ids().empty());
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = table_feature_properties::match{
            v13::oxm_id{OXM_OF_IN_PORT}
          , v13::oxm_id{OXM_OF_ETH_SRC_W}
          , v13::oxm_id{0x12, true, 0x04, 0x12345678}
        };

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_oxm_ids_are_equal)
      {
        BOOST_TEST(
            (table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC_W}
               , v13::oxm_id{0x12, true, 0x04, 0x12345678}
             }
             == table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC_W}
               , v13::oxm_id{0x12, true, 0x04, 0x12345678}
             }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_oxm_ids_are_emtpy)
      {
        BOOST_TEST(
            (table_feature_properties::match{}
             == table_feature_properties::match{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_oxm_id_value_is_not_equal)
      {
        BOOST_TEST(
            (table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC_W}
               , v13::oxm_id{0x12, true, 0x04, 0x12345678}
             }
             != table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC_W}
               , v13::oxm_id{0x12, true, 0x04, 0x87654321}
             }));
      }
      BOOST_AUTO_TEST_CASE(false_if_oxm_id_type_is_not_equal)
      {
        BOOST_TEST(
            (table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC_W}
               , v13::oxm_id{0x12, true, 0x04, 0x12345678}
             }
             != table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC}
               , v13::oxm_id{0x12, true, 0x04, 0x12345678}
             }));
      }
      BOOST_AUTO_TEST_CASE(false_if_oxm_id_order_is_not_equal)
      {
        BOOST_TEST(
            (table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC_W}
               , v13::oxm_id{0x12, true, 0x04, 0x12345678}
             }
             != table_feature_properties::match{
                 v13::oxm_id{OXM_OF_ETH_SRC_W}
               , v13::oxm_id{0x12, true, 0x04, 0x12345678}
               , v13::oxm_id{OXM_OF_IN_PORT}
             }));
      }
      BOOST_AUTO_TEST_CASE(false_if_oxm_id_number_is_not_equal)
      {
        BOOST_TEST(
            (table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC_W}
               , v13::oxm_id{0x12, true, 0x04, 0x12345678}
             }
             != table_feature_properties::match{
                 v13::oxm_id{OXM_OF_IN_PORT}
               , v13::oxm_id{OXM_OF_ETH_SRC_W}
             }));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = table_feature_properties::match{
            v13::oxm_id{OXM_OF_IN_PORT}
          , v13::oxm_id{OXM_OF_ETH_SRC_W}
          , v13::oxm_id{0x12, true, 0x04, 0x12345678}
        };

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_oxm_ids_are_equal)
      {
        BOOST_TEST(
            equivalent(
                table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC_W}
                  , v13::oxm_id{0x12, true, 0x04, 0x12345678}
                }
              , table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC_W}
                  , v13::oxm_id{0x12, true, 0x04, 0x12345678}
                }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_oxm_ids_are_emtpy)
      {
        BOOST_TEST(
            equivalent(
                table_feature_properties::match{}
              , table_feature_properties::match{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_oxm_id_value_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC_W}
                  , v13::oxm_id{0x12, true, 0x04, 0x12345678}
                }
              , table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC_W}
                  , v13::oxm_id{0x12, true, 0x04, 0x87654321}
                }));
      }
      BOOST_AUTO_TEST_CASE(false_if_oxm_id_type_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC_W}
                  , v13::oxm_id{0x12, true, 0x04, 0x12345678}
                }
              , table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC}
                  , v13::oxm_id{0x12, true, 0x04, 0x12345678}
                }));
      }
      BOOST_AUTO_TEST_CASE(true_if_oxm_id_order_is_not_equal)
      {
        BOOST_TEST(
            equivalent(
                table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC_W}
                  , v13::oxm_id{0x12, true, 0x04, 0x12345678}
                }
              , table_feature_properties::match{
                    v13::oxm_id{OXM_OF_ETH_SRC_W}
                  , v13::oxm_id{0x12, true, 0x04, 0x12345678}
                  , v13::oxm_id{OXM_OF_IN_PORT}
                }));
      }
      BOOST_AUTO_TEST_CASE(false_if_oxm_id_number_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC_W}
                  , v13::oxm_id{0x12, true, 0x04, 0x12345678}
                }
              , table_feature_properties::match{
                    v13::oxm_id{OXM_OF_IN_PORT}
                  , v13::oxm_id{OXM_OF_ETH_SRC_W}
                }));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, match_fixture)
    {
        auto buffer = std::vector<unsigned char>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == (sut.length() + 7) / 8 * 8);
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, match_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const match = table_feature_properties::match::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((match == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // match_test

BOOST_AUTO_TEST_SUITE(wildcards_test)

    BOOST_AUTO_TEST_SUITE(type_definition_test)
      BOOST_AUTO_TEST_CASE(type)
      {
        using sut = table_feature_properties::wildcards;

        using type = std::integral_constant<std::uint16_t, sut::type()>;

        BOOST_TEST(type::value == protocol::OFPTFPT_WILDCARDS);
      }
      BOOST_AUTO_TEST_CASE(min_length)
      {
        using sut = table_feature_properties::wildcards;

        using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

        BOOST_TEST(min_length::value == 4);
      }
      BOOST_AUTO_TEST_CASE(min_byte_length)
      {
        using sut = table_feature_properties::wildcards;

        using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

        BOOST_TEST(min_byte_length::value == 8);
      }
    BOOST_AUTO_TEST_SUITE_END() // type_definition_test

BOOST_AUTO_TEST_SUITE_END() // wildcards_test

BOOST_AUTO_TEST_SUITE(write_setfield_test)

    BOOST_AUTO_TEST_SUITE(type_definition_test)
      BOOST_AUTO_TEST_CASE(type)
      {
        using sut = table_feature_properties::write_setfield;

        using type = std::integral_constant<std::uint16_t, sut::type()>;

        BOOST_TEST(type::value == protocol::OFPTFPT_WRITE_SETFIELD);
      }
      BOOST_AUTO_TEST_CASE(min_length)
      {
        using sut = table_feature_properties::write_setfield;

        using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

        BOOST_TEST(min_length::value == 4);
      }
      BOOST_AUTO_TEST_CASE(min_byte_length)
      {
        using sut = table_feature_properties::write_setfield;

        using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

        BOOST_TEST(min_byte_length::value == 8);
      }
    BOOST_AUTO_TEST_SUITE_END() // type_definition_test

BOOST_AUTO_TEST_SUITE_END() // write_setfield_test

BOOST_AUTO_TEST_SUITE(write_setfield_miss_test)

    BOOST_AUTO_TEST_SUITE(type_definition_test)
      BOOST_AUTO_TEST_CASE(type)
      {
        using sut = table_feature_properties::write_setfield_miss;

        using type = std::integral_constant<std::uint16_t, sut::type()>;

        BOOST_TEST(type::value == protocol::OFPTFPT_WRITE_SETFIELD_MISS);
      }
      BOOST_AUTO_TEST_CASE(min_length)
      {
        using sut = table_feature_properties::write_setfield_miss;

        using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

        BOOST_TEST(min_length::value == 4);
      }
      BOOST_AUTO_TEST_CASE(min_byte_length)
      {
        using sut = table_feature_properties::write_setfield_miss;

        using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

        BOOST_TEST(min_byte_length::value == 8);
      }
    BOOST_AUTO_TEST_SUITE_END() // type_definition_test

BOOST_AUTO_TEST_SUITE_END() // write_setfield_miss_test

BOOST_AUTO_TEST_SUITE(apply_setfield_test)

    BOOST_AUTO_TEST_SUITE(type_definition_test)
      BOOST_AUTO_TEST_CASE(type)
      {
        using sut = table_feature_properties::apply_setfield;

        using type = std::integral_constant<std::uint16_t, sut::type()>;

        BOOST_TEST(type::value == protocol::OFPTFPT_APPLY_SETFIELD);
      }
      BOOST_AUTO_TEST_CASE(min_length)
      {
        using sut = table_feature_properties::apply_setfield;

        using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

        BOOST_TEST(min_length::value == 4);
      }
      BOOST_AUTO_TEST_CASE(min_byte_length)
      {
        using sut = table_feature_properties::apply_setfield;

        using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

        BOOST_TEST(min_byte_length::value == 8);
      }
    BOOST_AUTO_TEST_SUITE_END() // type_definition_test

BOOST_AUTO_TEST_SUITE_END() // apply_setfield_test

BOOST_AUTO_TEST_SUITE(apply_setfield_miss_test)

    BOOST_AUTO_TEST_SUITE(type_definition_test)
      BOOST_AUTO_TEST_CASE(type)
      {
        using sut = table_feature_properties::apply_setfield_miss;

        using type = std::integral_constant<std::uint16_t, sut::type()>;

        BOOST_TEST(type::value == protocol::OFPTFPT_APPLY_SETFIELD_MISS);
      }
      BOOST_AUTO_TEST_CASE(min_length)
      {
        using sut = table_feature_properties::apply_setfield_miss;

        using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

        BOOST_TEST(min_length::value == 4);
      }
      BOOST_AUTO_TEST_CASE(min_byte_length)
      {
        using sut = table_feature_properties::apply_setfield_miss;

        using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

        BOOST_TEST(min_byte_length::value == 8);
      }
    BOOST_AUTO_TEST_SUITE_END() // type_definition_test

BOOST_AUTO_TEST_SUITE_END() // apply_setfield_miss_test

BOOST_AUTO_TEST_SUITE_END() // table_feature_property_test
