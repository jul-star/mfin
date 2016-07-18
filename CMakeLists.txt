cmake_minimum_required (VERSION 2.8)
project (final)

# target_include_directories({$CMAKE_CURRENT_SOURCE_DIR})
SET(GCC_COVERAGE_COMPILE_FLAGS "-fopenmp -g")

add_executable(final main.cpp)

