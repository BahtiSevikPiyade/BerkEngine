cmake_minimum_required(VERSION 3.15)
project(BerkEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# BerkEngine is a header-only library
add_library(BerkEngine INTERFACE)
target_include_directories(BerkEngine INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Tests
enable_testing()
add_subdirectory(tests)
