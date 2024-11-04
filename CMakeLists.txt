cmake_minimum_required(VERSION 3.22)
project(JsonParser)

set(CMAKE_CXX_STANDARD 20)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

include_directories(${SRC_DIR})

file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)

file(GLOB_RECURSE TEST_FILES ${TEST_DIR}/*.cpp)

add_library(JsonParser ${SRC_FILES})

add_executable(json_eval main.cpp)
target_link_libraries(json_eval JsonParser)

find_package(GTest REQUIRED)

add_executable(json_test ${TEST_FILES})
target_link_libraries(json_test GTest::gtest GTest::gtest_main pthread JsonParser)
