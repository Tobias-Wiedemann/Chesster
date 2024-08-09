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

