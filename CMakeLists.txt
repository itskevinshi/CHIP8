cmake_minimum_required(VERSION 3.30)
project(CHIP8)
include(CTest)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 23)

# SDL3 Configuration ----------------------------------------------------------
set(SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3-3.2.8/x86_64-w64-mingw32/lib/cmake/SDL3")
find_package(SDL3 REQUIRED)
message(STATUS "SDL3 found: ${SDL3_VERSION}")

# Library Target --------------------------------------------------------------
add_library(lib
        src/chip8.cpp
        src/chip8.h
)

target_include_directories(lib PRIVATE
        "${SDL3_DIR}/../../../include"  # Base include path
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Executable Target -----------------------------------------------------------
add_executable(CHIP8
        src/main.cpp
        src/graphics.cpp
        src/graphics.h
)

target_include_directories(CHIP8 PRIVATE
        "${SDL3_DIR}/../../../include/SDL3"  # SDL3 headers
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(CHIP8 PRIVATE
        lib
        SDL3::SDL3-shared
)

# Testing Configuration -------------------------------------------------------
if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()