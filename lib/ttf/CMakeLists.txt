cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(libttf src/ttf.cpp)

add_subdirectory(../util util)

target_include_directories(libttf PUBLIC include)
target_link_libraries(libttf PRIVATE libutil)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
