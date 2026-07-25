cmake_minimum_required(VERSION 3.20)

# Set CPP Version
set(CMAKE_CXX_STANDARD 23)

# Tell CMake to generate compile_comands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(myproject VERSION 0.0.1)

# Create library
add_library(mylib src/myfile.cpp)

# Set public include library dir
target_include_directories(mylib PUBLIC mylib)

# Symlink compile_commands.json back to project root on configure
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
