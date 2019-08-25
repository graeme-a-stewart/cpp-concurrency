cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-TBB-06)

if(${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. "
  "Use an out of source build area: \"cmake the/path/to/cpp-concurrency/src/tbb\"")
endif()

# Add our CMake module paths and find the modules we need
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules;${CMAKE_SOURCE_DIR}/../../cmake/FindTBB;${CMAKE_MODULE_PATH}")
include(cpp-concurrency)
find_package(TBB REQUIRED)
find_package(Threads REQUIRED)

# Concurrent containers
simple_tbb_exe(concurrent-vector)
simple_tbb_exe(concurrent-vector-grow-by)
simple_tbb_exe(concurrent-vector-stdthread)
simple_tbb_exe(generate-and-filter)

## Now add tests
# Concurrent containers, N.B. stl and stdthread versions will fail by design 
add_test(concurrent-vector concurrent-vector)
add_test(concurrent-vector-stdthread concurrent-vector-stdthread)
add_test(concurrent-vector-grow-by concurrent-vector-grow-by)
add_test(generate-and-filter generate-and-filter)
