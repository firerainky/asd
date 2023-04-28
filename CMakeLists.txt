cmake_minimum_required(VERSION 3.22.1)
project(zj_fhe_lib)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_RPATH "${LIBINSTALL}")

file (GLOB_RECURSE CORE_SRC_FILES CONFIGURE_DEPENDS src/*.c src/*.cpp lib/utils/*.cpp)

add_subdirectory(src/math)

include_directories(src/util)

add_executable(zj_fhe_lib main.cpp)
target_link_libraries(zj_fhe_lib MathFunctions)

# Get third libraries: just googletest & googlebenchmark for now.
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

# Disable the Google Benchmark requirement on Google Test
set(BENCHMARK_ENABLE_TESTING NO)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.7.1
)
FetchContent_MakeAvailable(googlebenchmark)

# Add test sub directory
add_subdirectory(test)
add_subdirectory(benchmark)