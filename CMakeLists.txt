cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_executable(search_engine main.cpp)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

add_custom_command(TARGET search_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/config.json
        $<TARGET_FILE_DIR:search_engine>
        )

add_custom_command(TARGET search_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/requests.json
        $<TARGET_FILE_DIR:search_engine>
        )