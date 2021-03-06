#define BOOST_TEST_DYN_LINK
#include <canard/net/ofp/v13/table_feature_property/instructions.hpp>
#include <boost/test/unit_test.hpp>

#include <canard/net/ofp/v13/io/openflow.hpp>
#include <canard/net/ofp/v13/table_feature_property/id/instruction_id.hpp>

#include <type_traits>
#include "../../test_utility.hpp"

namespace of = canard::net::ofp;
namespace v13 = of::v13;
namespace table_feature_properties = v13::table_feature_properties;
namespace protocol = v13::protocol;

namespace {
struct instructions_fixture
{
    table_feature_properties::instructions sut{
          v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
        , v13::instruction_id{protocol::OFPIT_WRITE_METADATA}
        , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
        , v13::instruction_id{protocol::OFPIT_APPLY_ACTIONS}
        , v13::instruction_id{protocol::OFPIT_CLEAR_ACTIONS}
        , v13::instruction_id{protocol::OFPIT_METER}
        , v13::instruction_id{0x12345678, {'A', 'B'}}
    };
    std::vector<unsigned char> binary
        = "\x00\x00\x00\x26\x00\x01\x00\x04""\x00\x02\x00\x04\x00\x03\x00\x04"
          "\x00\x04\x00\x04\x00\x05\x00\x04""\x00\x06\x00\x04\xff\xff\x00\x0a"
          "\x12\x34\x56\x78""A""B""\x00\x00"_bin;
};
}

BOOST_AUTO_TEST_SUITE(table_feature_property_test)

BOOST_AUTO_TEST_SUITE(instructions_test)

    BOOST_AUTO_TEST_SUITE(type_definition_test)
      BOOST_AUTO_TEST_CASE(type)
      {
        using sut = table_feature_properties::instructions;

        using type = std::integral_constant<std::uint16_t, sut::type()>;

        BOOST_TEST(type::value == protocol::OFPTFPT_INSTRUCTIONS);
      }
      BOOST_AUTO_TEST_CASE(min_length)
      {
        using sut = table_feature_properties::instructions;

        using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

        BOOST_TEST(min_length::value == 4);
      }
      BOOST_AUTO_TEST_CASE(min_byte_length)
      {
        using sut = table_feature_properties::instructions;

        using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

        BOOST_TEST(min_byte_length::value == 8);
      }
    BOOST_AUTO_TEST_SUITE_END() // type_definition_test

    BOOST_AUTO_TEST_CASE(default_construct_test)
    {
        auto const sut = table_feature_properties::instructions{};

        BOOST_TEST(sut.length()
                == sizeof(protocol::ofp_table_feature_prop_instructions));
        BOOST_TEST(sut.instruction_ids().empty());
    }

    BOOST_AUTO_TEST_CASE(construct_from_single_id_test)
    {
        auto const goto_table = v13::instruction_id{protocol::OFPIT_GOTO_TABLE};

        auto const sut = table_feature_properties::instructions{goto_table};

        BOOST_TEST_REQUIRE(goto_table.length() == 4);
        BOOST_TEST(sut.length()
               ==  sizeof(protocol::ofp_table_feature_prop_instructions) + 4);
        BOOST_TEST(sut.instruction_ids().size() == 1);
        BOOST_TEST((sut.instruction_ids().front() == goto_table));
    }

    BOOST_AUTO_TEST_CASE(construct_from_multiple_ids_test)
    {
        auto const goto_table = v13::instruction_id{protocol::OFPIT_GOTO_TABLE};
        auto const write_metadata
            = v13::instruction_id{protocol::OFPIT_WRITE_METADATA};
        auto const write_actions
            = v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS};
        auto const apply_actions
            = v13::instruction_id{protocol::OFPIT_APPLY_ACTIONS};
        auto const clear_actions
            = v13::instruction_id{protocol::OFPIT_CLEAR_ACTIONS};
        auto const meter = v13::instruction_id{protocol::OFPIT_METER};
        auto const experimenter = v13::instruction_id{32, {}};

        auto const sut = table_feature_properties::instructions{
              goto_table, write_metadata
            , write_actions, apply_actions, clear_actions, meter
            , experimenter
        };

        BOOST_TEST(sut.length()
               ==  sizeof(protocol::ofp_table_feature_prop_instructions) + 4 * 6 + 8);
        BOOST_TEST_REQUIRE(sut.instruction_ids().size() == 7);
        auto it = sut.instruction_ids().begin();
        BOOST_TEST((*it++ == goto_table));
        BOOST_TEST((*it++ == write_metadata));
        BOOST_TEST((*it++ == write_actions));
        BOOST_TEST((*it++ == apply_actions));
        BOOST_TEST((*it++ == clear_actions));
        BOOST_TEST((*it++ == meter));
        BOOST_TEST((*it++ == experimenter));
        BOOST_TEST((it == sut.instruction_ids().end()));
    }

    BOOST_FIXTURE_TEST_CASE(copy_construct_test, instructions_fixture)
    {
        auto const copy = sut;

        BOOST_TEST((copy == sut));
    }

    BOOST_FIXTURE_TEST_CASE(move_construct_test, instructions_fixture)
    {
        auto src = sut;

        auto const copy = std::move(src);

        BOOST_TEST((copy == sut));
        BOOST_TEST(src.length()
                == sizeof(protocol::ofp_table_feature_prop_instructions));
        BOOST_TEST(src.instruction_ids().empty());
    }

    BOOST_FIXTURE_TEST_CASE(copy_assignment_test, instructions_fixture)
    {
        auto copy = table_feature_properties::instructions{};

        copy = sut;

        BOOST_TEST((copy == sut));
    }

    BOOST_FIXTURE_TEST_CASE(move_assignment_test, instructions_fixture)
    {
        auto src = sut;
        auto copy = table_feature_properties::instructions{};

        copy = std::move(src);

        BOOST_TEST((copy == sut));
        BOOST_TEST(src.length()
                == sizeof(protocol::ofp_table_feature_prop_instructions));
        BOOST_TEST(src.instruction_ids().empty());
    }

    BOOST_AUTO_TEST_SUITE(equality)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = table_feature_properties::instructions{
            v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
          , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
          , v13::instruction_id{0x12345678, {}}
        };

        BOOST_TEST((sut == sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_instruction_ids_are_equal)
      {
        BOOST_TEST(
            (table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
              , v13::instruction_id{0x12345678, {}}
             }
             == table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
              , v13::instruction_id{0x12345678, {}}
             }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_instruction_ids_are_emtpy)
      {
        BOOST_TEST(
            (table_feature_properties::instructions{}
             == table_feature_properties::instructions{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_instruction_id_value_is_not_equal)
      {
        BOOST_TEST(
            (table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
              , v13::instruction_id{0x12345678, {}}
             }
             != table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
              , v13::instruction_id{0x87654321, {}}
             }));
      }
      BOOST_AUTO_TEST_CASE(false_if_instruction_id_type_is_not_equal)
      {
        BOOST_TEST(
            (table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
              , v13::instruction_id{0x12345678, {}}
            }
            != table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_APPLY_ACTIONS}
              , v13::instruction_id{0x12345678, {}}
            }));
      }
      BOOST_AUTO_TEST_CASE(false_if_instruction_id_order_is_not_equal)
      {
        BOOST_TEST(
           (table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
              , v13::instruction_id{0x12345678, {}}
            }
            != table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
              , v13::instruction_id{0x12345678, {}}
              , v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
            }));
      }
      BOOST_AUTO_TEST_CASE(false_if_instruction_id_number_is_not_equal)
      {
        BOOST_TEST(
            (table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
              , v13::instruction_id{0x12345678, {}}
            }
            != table_feature_properties::instructions{
                v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
              , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
            }));
      }
    BOOST_AUTO_TEST_SUITE_END() // equality

    BOOST_AUTO_TEST_SUITE(function_equivalent)
      BOOST_AUTO_TEST_CASE(true_if_same_object)
      {
        auto const sut = table_feature_properties::instructions{
            v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
          , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
          , v13::instruction_id{0x12345678, {}}
        };

        BOOST_TEST(equivalent(sut, sut));
      }
      BOOST_AUTO_TEST_CASE(true_if_instruction_ids_are_equal)
      {
        BOOST_TEST(
            equivalent(
                table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                  , v13::instruction_id{0x12345678, {}}
                }
              , table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                  , v13::instruction_id{0x12345678, {}}
                }));
      }
      BOOST_AUTO_TEST_CASE(true_if_both_instruction_ids_are_emtpy)
      {
        BOOST_TEST(
            equivalent(
                table_feature_properties::instructions{}
              , table_feature_properties::instructions{}));
      }
      BOOST_AUTO_TEST_CASE(false_if_instruction_id_value_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                  , v13::instruction_id{0x12345678, {}}
                }
              , table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                  , v13::instruction_id{0x87654321, {}}
                }));
      }
      BOOST_AUTO_TEST_CASE(false_if_instruction_id_type_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                  , v13::instruction_id{0x12345678, {}}
                }
              , table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_APPLY_ACTIONS}
                  , v13::instruction_id{0x12345678, {}}
                }));
      }
      BOOST_AUTO_TEST_CASE(true_if_instruction_id_order_is_not_equal)
      {
        BOOST_TEST(
            equivalent(
                table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                  , v13::instruction_id{0x12345678, {}}
                }
              , table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                  , v13::instruction_id{0x12345678, {}}
                  , v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                }));
      }
      BOOST_AUTO_TEST_CASE(false_if_instruction_id_number_is_not_equal)
      {
        BOOST_TEST(
            !equivalent(
                table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                  , v13::instruction_id{0x12345678, {}}
                }
              , table_feature_properties::instructions{
                    v13::instruction_id{protocol::OFPIT_GOTO_TABLE}
                  , v13::instruction_id{protocol::OFPIT_WRITE_ACTIONS}
                }));
      }
    BOOST_AUTO_TEST_SUITE_END() // function_equivalent

    BOOST_FIXTURE_TEST_CASE(encode_test, instructions_fixture)
    {
        auto buffer = std::vector<unsigned char>{};

        sut.encode(buffer);

        BOOST_TEST(buffer.size() == (sut.length() + 7) / 8 * 8);
        BOOST_TEST(buffer == binary, boost::test_tools::per_element{});
    }

    BOOST_FIXTURE_TEST_CASE(decode_test, instructions_fixture)
    {
        auto it = binary.begin();
        auto const it_end = binary.end();

        auto const instructions
            = table_feature_properties::instructions::decode(it, it_end);

        BOOST_TEST((it == it_end));
        BOOST_TEST((instructions == sut));
    }

BOOST_AUTO_TEST_SUITE_END() // instructions_test

BOOST_AUTO_TEST_SUITE(instructions_miss_test)

    BOOST_AUTO_TEST_SUITE(type_definition_test)
      BOOST_AUTO_TEST_CASE(type)
      {
        using sut = table_feature_properties::instructions_miss;

        using type = std::integral_constant<std::uint16_t, sut::type()>;

        BOOST_TEST(type::value == protocol::OFPTFPT_INSTRUCTIONS_MISS);
      }
      BOOST_AUTO_TEST_CASE(min_length)
      {
        using sut = table_feature_properties::instructions_miss;

        using min_length
          = std::integral_constant<std::uint16_t, sut::min_length()>;

        BOOST_TEST(min_length::value == 4);
      }
      BOOST_AUTO_TEST_CASE(min_byte_length)
      {
        using sut = table_feature_properties::instructions_miss;

        using min_byte_length
          = std::integral_constant<std::uint16_t, sut::min_byte_length()>;

        BOOST_TEST(min_byte_length::value == 8);
      }
    BOOST_AUTO_TEST_SUITE_END() // type_definition_test

BOOST_AUTO_TEST_SUITE_END() // instructions_miss_test

BOOST_AUTO_TEST_SUITE_END() // table_feature_property_test
