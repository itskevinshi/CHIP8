add_executable(chip8_test chip8_test.cpp)
target_link_libraries(chip8_test lib GTest::GTest GTest::Main)
gtest_discover_tests(chip8_test)