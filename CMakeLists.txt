cmake_minimum_required(VERSION 3.24)
project(zj_fhe_lib)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_RPATH "${LIBINSTALL}")

file (GLOB_RECURSE CORE_SRC_FILES CONFIGURE_DEPENDS src/*.c src/*.cpp lib/utils/*.cpp)

add_subdirectory(src/math)

include_directories(src/util)

add_executable(zj_fhe_lib main.cpp)
target_link_libraries(zj_fhe_lib MathFunctions)