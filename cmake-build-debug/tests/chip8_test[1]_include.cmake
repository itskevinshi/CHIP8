if(EXISTS "C:/Users/kevin/Documents/UVA/Coding Projects/CHIP8/cmake-build-debug/tests/chip8_test[1]_tests.cmake")
  include("C:/Users/kevin/Documents/UVA/Coding Projects/CHIP8/cmake-build-debug/tests/chip8_test[1]_tests.cmake")
else()
  add_test(chip8_test_NOT_BUILT chip8_test_NOT_BUILT)
endif()
