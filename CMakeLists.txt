cmake_minimum_required(VERSION 3.20)
project(32_5_2)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_executable(32_5_2 main.cpp)

target_link_libraries(32_5_2 PRIVATE nlohmann_json::nlohmann_json)