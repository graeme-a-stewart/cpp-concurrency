cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-TBB)

if(${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. "
  "Use an out of source build area: \"cmake the/path/to/cpp-concurrency/src/tbb\"")
endif()

# Add our CMake module paths and find the modules we need
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules;${CMAKE_SOURCE_DIR}/../../cmake/FindTBB;${CMAKE_MODULE_PATH}")
include(cpp-concurrency)
find_package(TBB REQUIRED)
find_package(Threads REQUIRED)

# Build the tutorial utilities library and the cpuburn calbrator
add_library(tutorialutils tutorialutils.cc)
set_property(TARGET tutorialutils PROPERTY CXX_STANDARD 14)

add_executable(burn burn.cc)
target_link_libraries(burn tutorialutils)
set_property(TARGET burn PROPERTY CXX_STANDARD 14)


# Add a CMake target for each of our examples
simple_tbb_exe(concurrent-vector)
simple_tbb_exe(concurrent-vector-stl)
simple_tbb_exe(concurrent-vector-grow-by)
simple_tbb_exe(concurrent-vector-stdthread)
simple_tbb_exe(gen-input)
simple_tbb_exe(generate-and-filter)
simple_tbb_exe(maths-pipeline)
simple_tbb_exe(number-of-threads)

utils_tbb_exe(parallel-for-basic)
utils_tbb_exe(parallel-for-lambda)
simple_tbb_exe(parallel-mandel)
simple_tbb_exe(parallel-mandel-vector)
utils_tbb_exe(parallel-reduce)
simple_tbb_exe(parallel-reduce-pi-estimation)
simple_tbb_exe(version)

simple_tbb_exe(parallel-for-mutex)
