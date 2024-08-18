# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Chesster VERSION 0.1 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)
# Include directories
include_directories(include)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Add executable target
add_executable(chesster ${SOURCES})

# Add the test executable (assuming you have a test file)
add_executable(perft_pos1_d1 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos1_d1.cpp)
add_executable(perft_pos1_d2 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos1_d2.cpp)
add_executable(perft_pos1_d3 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos1_d3.cpp)
add_executable(perft_pos1_d4 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos1_d4.cpp)
add_executable(perft_pos1_d5 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos1_d5.cpp)

add_executable(qperft_pos1_d5 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos1_d5.cpp)
add_executable(qperft_pos1_d6 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos1_d6.cpp)
add_executable(qperft_pos1_d7 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos1_d7.cpp)

add_executable(perft_pos2_d1 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos2_d1.cpp)
add_executable(perft_pos2_d2 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos2_d2.cpp)
add_executable(perft_pos2_d3 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos2_d3.cpp)
add_executable(perft_pos2_d4 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos2_d4.cpp)

add_executable(qperft_pos2_d5 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos2_d5.cpp)

add_executable(perft_pos3_d1 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos3_d1.cpp)
add_executable(perft_pos3_d2 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos3_d2.cpp)
add_executable(perft_pos3_d3 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos3_d3.cpp)
add_executable(perft_pos3_d4 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos3_d4.cpp)
add_executable(perft_pos3_d5 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos3_d5.cpp)

add_executable(perft_pos4_d1 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos4_d1.cpp)
add_executable(perft_pos4_d2 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos4_d2.cpp)
add_executable(perft_pos4_d3 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos4_d3.cpp)
add_executable(perft_pos4_d4 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos4_d4.cpp)
add_executable(perft_pos4_d5 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/perft_pos4_d5.cpp)

add_executable(qperft_pos5_d1 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos5_d1.cpp)
add_executable(qperft_pos5_d2 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos5_d2.cpp)
add_executable(qperft_pos5_d3 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos5_d3.cpp)
add_executable(qperft_pos5_d4 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos5_d4.cpp)
add_executable(qperft_pos5_d5 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos5_d5.cpp)

add_executable(qperft_pos6_d1 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos6_d1.cpp)
add_executable(qperft_pos6_d2 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos6_d2.cpp)
add_executable(qperft_pos6_d3 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos6_d3.cpp)
add_executable(qperft_pos6_d4 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos6_d4.cpp)
add_executable(qperft_pos6_d5 src/board.cpp src/move.cpp src/perft.cpp src/movegen.cpp src/attack_masks.cpp tests/qperft_pos6_d5.cpp)


# Enable testing with CTest
enable_testing()

# Register the test with CTest
add_test(NAME perft_pos1_d1 COMMAND perft_pos1_d1)
add_test(NAME perft_pos1_d2 COMMAND perft_pos1_d2)
add_test(NAME perft_pos1_d3 COMMAND perft_pos1_d3)
add_test(NAME perft_pos1_d4 COMMAND perft_pos1_d4)
add_test(NAME perft_pos1_d5 COMMAND perft_pos1_d5)

add_test(NAME qperft_pos1_d5 COMMAND qperft_pos1_d5)
add_test(NAME qperft_pos1_d6 COMMAND qperft_pos1_d6)
add_test(NAME qperft_pos1_d7 COMMAND qperft_pos1_d7)

add_test(NAME perft_pos2_d1 COMMAND perft_pos2_d1)
add_test(NAME perft_pos2_d2 COMMAND perft_pos2_d2)
add_test(NAME perft_pos2_d3 COMMAND perft_pos2_d3)
add_test(NAME perft_pos2_d4 COMMAND perft_pos2_d4)

add_test(NAME qperft_pos2_d5 COMMAND qperft_pos2_d5)

add_test(NAME perft_pos3_d1 COMMAND perft_pos3_d1)
add_test(NAME perft_pos3_d2 COMMAND perft_pos3_d2)
add_test(NAME perft_pos3_d3 COMMAND perft_pos3_d3)
add_test(NAME perft_pos3_d4 COMMAND perft_pos3_d4)
add_test(NAME perft_pos3_d5 COMMAND perft_pos3_d5)

add_test(NAME perft_pos4_d1 COMMAND perft_pos4_d1)
add_test(NAME perft_pos4_d2 COMMAND perft_pos4_d2)
add_test(NAME perft_pos4_d3 COMMAND perft_pos4_d3)
add_test(NAME perft_pos4_d4 COMMAND perft_pos4_d4)
add_test(NAME perft_pos4_d5 COMMAND perft_pos4_d5)

add_test(NAME qperft_pos5_d1 COMMAND qperft_pos5_d1)
add_test(NAME qperft_pos5_d2 COMMAND qperft_pos5_d2)
add_test(NAME qperft_pos5_d3 COMMAND qperft_pos5_d3)
add_test(NAME qperft_pos5_d4 COMMAND qperft_pos5_d4)
add_test(NAME qperft_pos5_d5 COMMAND qperft_pos5_d5)

add_test(NAME qperft_pos6_d1 COMMAND qperft_pos6_d1)
add_test(NAME qperft_pos6_d2 COMMAND qperft_pos6_d2)
add_test(NAME qperft_pos6_d3 COMMAND qperft_pos6_d3)
add_test(NAME qperft_pos6_d4 COMMAND qperft_pos6_d4)
add_test(NAME qperft_pos6_d5 COMMAND qperft_pos6_d5)

