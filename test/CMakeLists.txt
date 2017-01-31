cmake_minimum_required(VERSION 3.5)

project(bulb_test LANGUAGES CXX)

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
target_compile_options(driver
    PRIVATE $<TARGET_PROPERTY:bulb_test,INTERFACE_COMPILE_OPTIONS>)
add_library(bulb::driver ALIAS driver)

add_subdirectory(v10)
add_subdirectory(v13)

add_test(NAME v10_test COMMAND bulb::v10::test -l message)
add_test(NAME v13_test COMMAND bulb::v13::test -l message)

