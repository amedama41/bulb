cmake_minimum_required(VERSION 3.5)

project(v10_bulb_test LANGUAGES CXX)

find_package(Boost 1.59 REQUIRED COMPONENTS system)

enable_testing()

add_library(bulb_base INTERFACE)
target_compile_options(bulb_base INTERFACE "-Wall" "-pedantic" "-std=c++11")
message(STATUS "target compiler is \"${CMAKE_CXX_COMPILER_ID}\"")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(bulb_base INTERFACE "-stdlib=libc++")
    target_link_libraries(bulb_base INTERFACE "-stdlib=libc++")
endif()

set(libbulb_srcs
    include/canard/net/ofp/v10/impl/any_action.ipp
    include/canard/net/ofp/v10/impl/action_list.ipp
    include/canard/net/ofp/v10/impl/any_queue_property.ipp
    include/canard/net/ofp/v13/impl/any_oxm_match_field.ipp
    include/canard/net/ofp/v13/impl/oxm_match_field_set.ipp
    include/canard/net/ofp/v13/impl/any_action.ipp
    include/canard/net/ofp/v13/impl/action_list.ipp
    include/canard/net/ofp/v13/impl/action_set.ipp
    include/canard/net/ofp/v13/impl/any_instruction.ipp
    include/canard/net/ofp/v13/impl/instruction_set.ipp
    include/canard/net/ofp/v13/impl/any_queue_property.ipp
    include/canard/net/ofp/v13/impl/any_hello_element.ipp
    include/canard/net/ofp/v13/impl/any_meter_band.ipp
    include/canard/net/ofp/v13/impl/meter_band_set.ipp
    include/canard/net/ofp/v13/impl/any_table_feature_property.ipp
    include/canard/net/ofp/v13/impl/table_feature_property_set.ipp)
set_source_files_properties(${libbulb_srcs} PROPERTIES LANGUAGE CXX)

add_library(bulb SHARED ${libbulb_srcs})
set_target_properties(bulb PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(bulb PUBLIC "include")
target_compile_options(bulb PRIVATE "-x" "c++")
target_compile_definitions(bulb
    PUBLIC CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
target_link_libraries(bulb PUBLIC Boost::system PRIVATE bulb_base)
add_library(bulb::libbulb ALIAS bulb)

add_subdirectory(test EXCLUDE_FROM_ALL)

