cmake_minimum_required (VERSION 2.8)
project (final)

# target_include_directories({$CMAKE_CURRENT_SOURCE_DIR})
SET(GCC_COVERAGE_COMPILE_FLAGS "-fopenmp -g")
SET(GCC_COVERAGE_LINK_FLAGS "-fopenmp -g")

# add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

add_executable(final main.cpp)

