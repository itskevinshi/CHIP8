cmake_minimum_required(VERSION 3.30)
project(CHIP8)
include(CTest)
include(GoogleTest)

set(CMAKE_CXX_STANDARD 23)

add_library(lib src/chip8.h src/chip8.cpp)

add_executable(CHIP8 src/main.cpp)

target_link_libraries(CHIP8 lib)

if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()
