cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(search_engine main.cpp ConverterJSON.h ConverterJSON.cpp InvertedIndex.h InvertedIndex.cpp SearchServer.h SearchServer.cpp)

add_custom_command(TARGET search_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/config.json
        ${CMAKE_CURRENT_BINARY_DIR}/config.json
        )

add_custom_command(TARGET search_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/requests.json
        ${CMAKE_CURRENT_BINARY_DIR}/requests.json
        )

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
file(COPY resources_test DESTINATION ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_all tests/ConverterJSON_unittest.cpp ConverterJSON.h ConverterJSON.cpp InvertedIndex.h InvertedIndex.cpp tests/InvertedIndex_unittest.cpp SearchServer.h SearchServer.cpp tests/SearchServerUnittest.cpp)
target_link_libraries(test_all PRIVATE gtest_main)

add_custom_command(TARGET test_all POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/config.json
        ${CMAKE_CURRENT_BINARY_DIR}/config.json
        )

add_custom_command(TARGET test_all POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/requests.json
        ${CMAKE_CURRENT_BINARY_DIR}/requests.json
        )

include(GoogleTest)
gtest_discover_tests(test_all)