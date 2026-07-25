cmake_minimum_required(VERSION 3.20)

# Set CPP Version
set(CMAKE_CXX_STANDARD 23)

# Tell CMake to generate compile_comands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(_PROJECT_ VERSION 0.0.1)

# Create library
add_executable(_PROJECT_ src/_PROJECT_.cpp)

# Symlink compile_commands.json back to project root on configure
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
