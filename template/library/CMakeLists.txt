cmake_minimum_required(VERSION 3.20)

# Set CPP Version
set(CMAKE_CXX_STANDARD 23)

# Tell CMake to generate compile_comands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(_PROJECT_ VERSION 0.0.1)

if(BUILD_SHARED_LIBS) 
    message("Building in shared library mode")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(_PROJECT__EXPORTING ON)
endif()

# Create library
add_library(_PROJECT_ src/_PROJECT_.cpp)

# Set public include library dir
target_include_directories(_PROJECT_ PUBLIC include)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/include/_PROJECT_/config.h"
)

# Symlink compile_commands.json back to project root on configure
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
