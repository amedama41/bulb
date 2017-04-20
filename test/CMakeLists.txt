cmake_minimum_required(VERSION 3.5)

project(bulb_test LANGUAGES CXX)

enable_testing()

add_library(driver OBJECT driver.cpp)
target_compile_definitions(driver PRIVATE BOOST_TEST_MODULE=bulb_test)
target_compile_options(driver
    PRIVATE $<TARGET_PROPERTY:bulb_base,INTERFACE_COMPILE_OPTIONS>)
add_library(bulb::driver ALIAS driver)

add_subdirectory(v10)
add_subdirectory(v13)

add_test(NAME v10_test COMMAND bulb::v10::test -l message)
add_test(NAME v13_test COMMAND bulb::v13::test -l message)

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS bulb::v10::test bulb::v13::test)

