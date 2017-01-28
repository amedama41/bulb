cmake_minimum_required(VERSION 3.5)

project(bulb_test LANGUAGES CXX)

find_package(Boost 1.59 REQUIRED COMPONENTS system unit_test_framework)

set(v10_libbulb_srcs
    ../include/canard/network/openflow/v10/impl/action_list.ipp
    ../include/canard/network/openflow/v10/impl/any_action.ipp
    ../include/canard/network/openflow/v10/impl/any_queue_property.ipp)
set(v10_action_srcs
    v10/action/output_test.cpp
    v10/action/set_field_test.cpp
    v10/action/strip_vlan_test.cpp
    v10/action/enqueue_test.cpp
    v10/action/validate_header_test.cpp)
set(v10_common_type_srcs
    v10/common_type/match_fields_test.cpp
    v10/common_type/match_set_test.cpp
    v10/common_type/packet_queue_test.cpp
    v10/common_type/port_test.cpp)
set(v10_io_srcs
    v10/io/openflow_test.cpp)
set(v10_message_srcs
    v10/message/barrier_test.cpp
    v10/message/echo_test.cpp
    v10/message/error_test.cpp
    v10/message/flow_add_test.cpp
    v10/message/flow_delete_test.cpp
    v10/message/flow_modify_test.cpp
    v10/message/flow_removed_test.cpp
    v10/message/packet_in_test.cpp
    v10/message/packet_out_test.cpp
    v10/message/port_mod_test.cpp
    v10/message/port_status_test.cpp
    v10/message/queue_config_test.cpp
    v10/message/switch_config_test.cpp
    v10/message/switch_features_test.cpp
    v10/message/validate_header_test.cpp)
set(v10_queue_property_srcs
    v10/queue_property/min_rate_test.cpp
    v10/queue_property/validate_header_test.cpp)
set(v10_stats_srcs
    v10/stats/aggregate_stats_test.cpp
    v10/stats/description_test.cpp
    v10/stats/flow_stats_test.cpp
    v10/stats/port_stats_test.cpp
    v10/stats/queue_stats_test.cpp
    v10/stats/table_stats_test.cpp
    v10/stats/validate_header_test.cpp)
set(v10_utility_srcs
    v10/utility/action_list_test.cpp
    v10/utility/any_action_test.cpp
    v10/utility/any_queue_property_test.cpp
    v10/utility/flow_entry_test.cpp)
set(v10_test_modules
    v10_action v10_common_type v10_io
    v10_message v10_queue_property v10_stats v10_utility)

set(v13_libbulb_srcs
    ../include/canard/network/openflow/v13/impl/any_action.ipp
    ../include/canard/network/openflow/v13/impl/any_instruction.ipp
    ../include/canard/network/openflow/v13/impl/any_oxm_match_field.ipp
    ../include/canard/network/openflow/v13/impl/any_queue_property.ipp
    ../include/canard/network/openflow/v13/impl/any_hello_element.ipp
    ../include/canard/network/openflow/v13/impl/any_table_feature_property.ipp
    ../include/canard/network/openflow/v13/impl/any_meter_band.ipp
    ../include/canard/network/openflow/v13/impl/action_list.ipp
    ../include/canard/network/openflow/v13/impl/action_set.ipp
    ../include/canard/network/openflow/v13/impl/instruction_set.ipp
    ../include/canard/network/openflow/v13/impl/oxm_match_field_set.ipp
    ../include/canard/network/openflow/v13/impl/meter_band_set.ipp
    ../include/canard/network/openflow/v13/impl/table_feature_property_set.ipp)
set(v13_action_srcs
    v13/action/output_test.cpp
    v13/action/copy_ttl_out_test.cpp
    v13/action/copy_ttl_in_test.cpp
    v13/action/set_mpls_ttl_test.cpp
    v13/action/decrement_mpls_ttl_test.cpp
    v13/action/push_vlan_test.cpp
    v13/action/pop_vlan_test.cpp
    v13/action/push_mpls_test.cpp
    v13/action/pop_mpls_test.cpp
    v13/action/set_queue_test.cpp
    v13/action/group_test.cpp
    v13/action/set_nw_ttl_test.cpp
    v13/action/decrement_nw_ttl_test.cpp
    v13/action/set_field_test.cpp
    v13/action/push_pbb_test.cpp
    v13/action/pop_pbb_test.cpp)
set(v13_common_type_srcs
    v13/common_type/port_test.cpp
    v13/common_type/oxm_match_test.cpp
    v13/common_type/bucket_test.cpp
    v13/common_type/packet_queue_test.cpp)
set(v13_decoder_srcs
    v13/decoder/meter_band_decoder_test.cpp)
set(v13_hello_element_srcs
    v13/hello_element/versionbitmap_test.cpp
    v13/hello_element/unknown_element_test.cpp)
set(v13_instruction_srcs
    v13/instruction/goto_table_test.cpp
    v13/instruction/write_metadata_test.cpp
    v13/instruction/write_actions_test.cpp
    v13/instruction/apply_actions_test.cpp
    v13/instruction/clear_actions_test.cpp
    v13/instruction/meter_test.cpp)
set(v13_message_srcs
    v13/message/hello_test.cpp
    v13/message/error_test.cpp
    v13/message/echo_test.cpp
    v13/message/switch_features_test.cpp
    v13/message/switch_config_test.cpp
    v13/message/table_mod_test.cpp
    v13/message/packet_in_test.cpp
    v13/message/flow_removed_test.cpp
    v13/message/port_status_test.cpp
    v13/message/packet_out_test.cpp
    v13/message/flow_add_test.cpp
    v13/message/flow_modify_test.cpp
    v13/message/flow_delete_test.cpp
    v13/message/group_mod_test.cpp
    v13/message/meter_add_test.cpp
    v13/message/meter_delete_test.cpp
    v13/message/port_mod.cpp
    v13/message/barrier_test.cpp
    v13/message/queue_config_test.cpp)
set(v13_meter_band_srcs
    v13/meter_band/drop_test.cpp
    v13/meter_band/dscp_remark_test.cpp)
set(v13_multipart_srcs
    v13/multipart/description_test.cpp
    v13/multipart/flow_stats_test.cpp
    v13/multipart/aggregate_stats_test.cpp
    v13/multipart/table_stats_test.cpp
    v13/multipart/port_stats_test.cpp
    v13/multipart/queue_stats_test.cpp
    v13/multipart/table_features_test.cpp
    v13/multipart/port_description_test.cpp)
set(v13_oxm_match_srcs
    v13/oxm_match/match_field_test.cpp
    v13/oxm_match/in_port_test.cpp
    v13/oxm_match/in_phy_port_test.cpp
    v13/oxm_match/eth_dst_test.cpp
    v13/oxm_match/vlan_vid_test.cpp
    v13/oxm_match/vlan_pcp_test.cpp
    v13/oxm_match/ip_dscp_test.cpp
    v13/oxm_match/ip_ecn_test.cpp
    v13/oxm_match/ipv4_src_test.cpp
    v13/oxm_match/ipv6_src_test.cpp
    v13/oxm_match/ipv6_flabel_test.cpp
    v13/oxm_match/mpls_label_test.cpp
    v13/oxm_match/mpls_tc_test.cpp
    v13/oxm_match/mpls_bos_test.cpp
    v13/oxm_match/pbb_isid_test.cpp
    v13/oxm_match/ipv6_exthdr_test.cpp)
set(v13_queue_property_srcs
    v13/queue_property/min_rate_test.cpp
    v13/queue_property/max_rate_test.cpp)
set(v13_table_feature_property_srcs
    v13/table_feature_property/instructions_test.cpp
    v13/table_feature_property/next_tables_test.cpp
    v13/table_feature_property/actions_test.cpp
    v13/table_feature_property/oxm_test.cpp)
set(v13_utility_srcs
    v13/utility/any_oxm_match_field_test.cpp
    v13/utility/any_action_test.cpp
    v13/utility/action_list_test.cpp
    v13/utility/action_set_test.cpp
    v13/utility/any_instruction_test.cpp
    v13/utility/instruction_set_test.cpp
    v13/utility/any_queue_property_test.cpp
    v13/utility/any_hello_element_test.cpp
    v13/utility/oxm_match_field_set_test.cpp
    v13/utility/any_meter_band_test.cpp
    v13/utility/meter_band_set_test.cpp)
set(v13_test_modules
    v13_action v13_common_type v13_decoder v13_hello_element
    v13_instruction v13_message v13_meter_band v13_multipart
    v13_oxm_match v13_queue_property v13_table_feature_property
    v13_utility)
set(ofp_versions 10 13)

enable_testing()

add_library(bulb_test INTERFACE)
target_compile_options(bulb_test INTERFACE "-Wall" "-pedantic" "-std=c++11")
message(STATUS "target compiler is \"${CMAKE_CXX_COMPILER_ID}\"")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(bulb_test INTERFACE "-stdlib=libc++")
    target_link_libraries(bulb_test INTERFACE "-stdlib=libc++")
endif()

add_library(driver OBJECT driver.cpp)
target_compile_definitions(driver PRIVATE BOOST_TEST_MODULE=bulb_test)
target_compile_options(driver PRIVATE $<TARGET_PROPERTY:bulb_test,INTERFACE_COMPILE_OPTIONS>)
add_library(bulb::driver ALIAS driver)

foreach(version IN LISTS ofp_versions)
    set(libbulb v${version}_libbulb)
    set_source_files_properties(${${libbulb}_srcs} PROPERTIES LANGUAGE CXX)
    add_library(${libbulb} SHARED ${${libbulb}_srcs})
    set_target_properties(${libbulb} PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(${libbulb} PUBLIC "../include")
    target_compile_options(${libbulb} PRIVATE "-x" "c++")
    target_compile_definitions(${libbulb}
        PUBLIC CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
    target_link_libraries(${libbulb} PUBLIC Boost::system PRIVATE bulb_test)
    add_library(bulb::${libbulb} ALIAS ${libbulb})

    foreach(module IN LISTS v${version}_test_modules)
        add_library(${module} SHARED ${${module}_srcs})
        target_link_libraries(${module}
            PRIVATE Boost::unit_test_framework bulb::${libbulb} bulb_test)
        add_library(bulb::${module} ALIAS ${module})
    endforeach()

    set(ofp_all_test v${version}_all_test)
    add_executable(${ofp_all_test} $<TARGET_OBJECTS:bulb::driver>)
    target_link_libraries(${ofp_all_test}
        PRIVATE Boost::unit_test_framework ${v${version}_test_modules} bulb_test)
    add_executable(bulb::${ofp_all_test} ALIAS ${ofp_all_test})
    add_test(NAME ${ofp_all_test} COMMAND ${ofp_all_test} -l message)
endforeach()

