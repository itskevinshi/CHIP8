add_executable(chip8_test chip8_test.cpp)
target_include_directories(chip8_test PRIVATE
        "${SDL3_DIR}/../../../include"
)
target_link_libraries(chip8_test PRIVATE
        lib
        GTest::GTest
        GTest::Main
)