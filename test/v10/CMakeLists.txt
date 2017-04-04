cmake_minimum_required(VERSION 3.5)

project(v10_bulb_test LANGUAGES CXX)

find_package(Boost 1.59 REQUIRED COMPONENTS unit_test_framework)

set(action_srcs
    action/output_test.cpp
    action/set_field_test.cpp
    action/strip_vlan_test.cpp
    action/enqueue_test.cpp
    action/validate_header_test.cpp)
set(common_type_srcs
    common_type/match_fields_test.cpp
    common_type/match_set_test.cpp
    common_type/packet_queue_test.cpp
    common_type/port_test.cpp)
set(decoder_srcs
    decoder/flow_mod_decoder_test.cpp)
set(io_srcs
    io/openflow_test.cpp)
set(message_srcs
    message/barrier_test.cpp
    message/echo_test.cpp
    message/error_test.cpp
    message/flow_add_test.cpp
    message/flow_delete_test.cpp
    message/flow_modify_test.cpp
    message/flow_removed_test.cpp
    message/packet_in_test.cpp
    message/packet_out_test.cpp
    message/port_mod_test.cpp
    message/port_status_test.cpp
    message/queue_config_test.cpp
    message/switch_config_test.cpp
    message/switch_features_test.cpp
    message/validate_header_test.cpp)
set(queue_property_srcs
    queue_property/min_rate_test.cpp
    queue_property/validate_header_test.cpp)
set(stats_srcs
    stats/aggregate_stats_test.cpp
    stats/description_test.cpp
    stats/flow_stats_test.cpp
    stats/port_stats_test.cpp
    stats/queue_stats_test.cpp
    stats/table_stats_test.cpp
    stats/validate_header_test.cpp)
set(utility_srcs
    utility/action_list_test.cpp
    utility/any_action_test.cpp
    utility/any_queue_property_test.cpp
    utility/flow_entry_test.cpp)

set(test_modules
    action common_type decoder io message queue_property stats utility)

add_executable(v10_test $<TARGET_OBJECTS:bulb::driver>)
target_link_libraries(v10_test PRIVATE bulb_base)
add_executable(bulb::v10::test ALIAS v10_test)

foreach(module IN LISTS test_modules)
    add_library(v10_${module} SHARED ${${module}_srcs})
    target_link_libraries(v10_${module}
        PUBLIC Boost::unit_test_framework bulb::libbulb
        PRIVATE bulb_base)
    add_library(bulb::v10::${module} ALIAS v10_${module})

    target_link_libraries(v10_test PRIVATE bulb::v10::${module})

    add_executable(v10_${module}_test
        EXCLUDE_FROM_ALL $<TARGET_OBJECTS:bulb::driver>)
    target_link_libraries(v10_${module}_test PRIVATE bulb::v10::${module})
endforeach()

