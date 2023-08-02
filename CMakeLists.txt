cmake_minimum_required(VERSION 3.22.1)
project(zj_fhe_lib)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_RPATH "${LIBINSTALL}")

# Options
option(ENABLE_SANITIZE_ADDR "Enable address sanitize." ON)
option(ENABLE_SANITIZE_UNDEF "Enable undefined sanitize." ON)
option(ENABLE_SANITIZE_LEAK "Enable leak sanitize (Gcc/Clang only)." ON)
option(ENABLE_SANITIZE_THREAD "Enable thread sanitize (Gcc/Clang only)." OFF)

option(ENABLE_COVERAGE "Enable a Code Coverage build." OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
include(Tools)

add_cmake_format_target()
add_clang_format_target()

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

if(ENABLE_SANITIZE_ADDR OR ENABLE_SANITIZE_UNDEF)
    message(STATUS "Sanitizers enabled")
    include(Sanitizer)
    add_sanitizer_flags()
endif()

if (ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

add_subdirectory(src bin)

add_executable(zj_fhe_lib main.cpp)
target_link_libraries(zj_fhe_lib PRIVATE MathFunctions)

# Get third libraries: just googletest & googlebenchmark for now.
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

# Disable the Google Benchmark requirement on Google Test
set(BENCHMARK_ENABLE_TESTING NO)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1)
FetchContent_MakeAvailable(googlebenchmark)

# If the option does not change after we changing, we can run `cmake -S . -B build -UFHE_BUILD_TEST to force cmake updating the option
option(FHE_BUILD_TEST "IF BUILD TEST TARGET" ON)
message(STATUS "FHE_BUILD_TEST: ${FHE_BUILD_TEST}")

if(FHE_BUILD_TEST)
    add_subdirectory(test)
endif()

option(FHE_BUILD_BENCHMARK "IF BUILD BENCHMARK TARGET" ON)
message(STATUS "FHE_BUILD_BENCHMARK: ${FHE_BUILD_BENCHMARK}")

if(FHE_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()
