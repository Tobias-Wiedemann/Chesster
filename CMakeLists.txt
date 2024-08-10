# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Chesster VERSION 0.1 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Add executable target
add_executable(chesster ${SOURCES})

# Add the test executable (assuming you have a test file)
add_executable(tests tests/perft_tests.cpp)
add_executable(perft_pos1 tests/perft_pos1.cpp)
add_executable(perft_pos2 tests/perft_pos2.cpp)
add_executable(perft_pos3 tests/perft_pos3.cpp)
add_executable(perft_pos4 tests/perft_pos4.cpp)

# Enable testing with CTest
enable_testing()

# Register the test with CTest
add_test(NAME perft_tests COMMAND tests)
add_test(NAME perft_pos1 COMMAND perft_pos1)
add_test(NAME perft_pos2 COMMAND perft_pos2)
add_test(NAME perft_pos3 COMMAND perft_pos3)
add_test(NAME perft_pos4 COMMAND perft_pos4)
