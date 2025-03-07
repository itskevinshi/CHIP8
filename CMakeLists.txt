cmake_minimum_required(VERSION 3.30)
project(CHIP8)
include(CTest)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 23)

# Set SDL3_DIR to point to the directory containing SDL3Config.cmake
set(SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3-3.2.8/cmake")

# Find SDL3 package
find_package(SDL3 REQUIRED)

add_library(lib src/chip8.h src/chip8.cpp)

add_executable(CHIP8 src/main.cpp
        src/graphics.cpp
        src/graphics.h)

target_link_libraries(CHIP8 PRIVATE lib SDL3::SDL3-shared)

if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()
