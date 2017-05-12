cmake_minimum_required(VERSION 3.5)

project(v13_bulb_test LANGUAGES CXX)

find_package(Boost 1.59 REQUIRED COMPONENTS unit_test_framework)

set(action_srcs
    action/output_test.cpp
    action/copy_ttl_out_test.cpp
    action/copy_ttl_in_test.cpp
    action/set_mpls_ttl_test.cpp
    action/decrement_mpls_ttl_test.cpp
    action/push_vlan_test.cpp
    action/pop_vlan_test.cpp
    action/push_mpls_test.cpp
    action/pop_mpls_test.cpp
    action/set_queue_test.cpp
    action/group_test.cpp
    action/set_nw_ttl_test.cpp
    action/decrement_nw_ttl_test.cpp
    action/set_field_test.cpp
    action/push_pbb_test.cpp
    action/pop_pbb_test.cpp
    action/validate_header_test.cpp)
set(common_type_srcs
    common_type/port_test.cpp
    common_type/oxm_header_test.cpp
    common_type/oxm_match_test.cpp
    common_type/bucket_test.cpp
    common_type/packet_queue_test.cpp)
set(decoder_srcs
    decoder/meter_band_decoder_test.cpp)
set(hello_element_srcs
    hello_element/versionbitmap_test.cpp
    hello_element/unknown_element_test.cpp
    hello_element/validate_header_test.cpp)
set(instruction_srcs
    instruction/goto_table_test.cpp
    instruction/write_metadata_test.cpp
    instruction/write_actions_test.cpp
    instruction/apply_actions_test.cpp
    instruction/clear_actions_test.cpp
    instruction/meter_test.cpp
    instruction/validate_header_test.cpp)
set(message_srcs
    message/hello_test.cpp
    message/error_test.cpp
    message/echo_test.cpp
    message/switch_features_test.cpp
    message/switch_config_test.cpp
    message/table_mod_test.cpp
    message/packet_in_test.cpp
    message/flow_removed_test.cpp
    message/port_status_test.cpp
    message/packet_out_test.cpp
    message/flow_add_test.cpp
    message/flow_modify_test.cpp
    message/flow_delete_test.cpp
    message/group_mod_test.cpp
    message/meter_add_test.cpp
    message/meter_delete_test.cpp
    message/port_mod.cpp
    message/barrier_test.cpp
    message/queue_config_test.cpp
    message/role_test.cpp
    message/async_config_test.cpp
    message/validate_header_test.cpp)
set(meter_band_srcs
    meter_band/drop_test.cpp
    meter_band/dscp_remark_test.cpp
    meter_band/validate_header_test.cpp)
set(multipart_srcs
    multipart/description_test.cpp
    multipart/flow_stats_test.cpp
    multipart/aggregate_stats_test.cpp
    multipart/table_stats_test.cpp
    multipart/port_stats_test.cpp
    multipart/queue_stats_test.cpp
    multipart/group_stats_test.cpp
    multipart/group_description_test.cpp
    multipart/group_features_test.cpp
    multipart/table_features_test.cpp
    multipart/port_description_test.cpp
    multipart/meter_features_test.cpp
    multipart/validate_header_test.cpp)
set(oxm_match_srcs
    oxm_match/match_field_test.cpp
    oxm_match/in_port_test.cpp
    oxm_match/in_phy_port_test.cpp
    oxm_match/eth_dst_test.cpp
    oxm_match/vlan_vid_test.cpp
    oxm_match/vlan_pcp_test.cpp
    oxm_match/ip_dscp_test.cpp
    oxm_match/ip_ecn_test.cpp
    oxm_match/ipv4_src_test.cpp
    oxm_match/ipv6_src_test.cpp
    oxm_match/ipv6_flabel_test.cpp
    oxm_match/mpls_label_test.cpp
    oxm_match/mpls_tc_test.cpp
    oxm_match/mpls_bos_test.cpp
    oxm_match/pbb_isid_test.cpp
    oxm_match/ipv6_exthdr_test.cpp
    oxm_match/validate_header_test.cpp)
set(queue_property_srcs
    queue_property/min_rate_test.cpp
    queue_property/max_rate_test.cpp
    queue_property/validate_header_test.cpp)
set(table_feature_property_srcs
    table_feature_property/action_id_test.cpp
    table_feature_property/instruction_id_test.cpp
    table_feature_property/oxm_id_test.cpp
    table_feature_property/instructions_test.cpp
    table_feature_property/next_tables_test.cpp
    table_feature_property/actions_test.cpp
    table_feature_property/oxm_test.cpp
    table_feature_property/validate_header_test.cpp)
set(utility_srcs
    utility/any_oxm_match_field_test.cpp
    utility/any_action_test.cpp
    utility/action_list_test.cpp
    utility/action_set_test.cpp
    utility/any_instruction_test.cpp
    utility/instruction_set_test.cpp
    utility/any_queue_property_test.cpp
    utility/any_hello_element_test.cpp
    utility/oxm_match_field_set_test.cpp
    utility/any_meter_band_test.cpp
    utility/meter_band_set_test.cpp)

set(test_modules
    oxm_match hello_element queue_property meter_band
    action common_type decoder instruction
    table_feature_property message multipart utility)

add_executable(v13_test $<TARGET_OBJECTS:bulb::driver>)
target_link_libraries(v13_test PRIVATE bulb_base)
add_executable(bulb::v13::test ALIAS v13_test)

foreach(module IN LISTS test_modules)
    add_library(v13_${module} SHARED ${${module}_srcs})
    target_link_libraries(v13_${module}
        PUBLIC Boost::unit_test_framework bulb::libbulb
        PRIVATE bulb_base)
    add_library(bulb::v13::${module} ALIAS v13_${module})

    target_link_libraries(v13_test PRIVATE bulb::v13::${module})

    add_executable(v13_${module}_test
        EXCLUDE_FROM_ALL $<TARGET_OBJECTS:bulb::driver>)
    target_link_libraries(v13_${module}_test PRIVATE bulb::v13::${module})
endforeach()

