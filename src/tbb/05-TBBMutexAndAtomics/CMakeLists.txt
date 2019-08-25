cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-TBB-05)

if(${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. "
  "Use an out of source build area: \"cmake the/path/to/cpp-concurrency/src/tbb\"")
endif()

# Add our CMake module paths and find the modules we need
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules;${CMAKE_SOURCE_DIR}/../../cmake/FindTBB;${CMAKE_MODULE_PATH}")
include(cpp-concurrency)
find_package(TBB REQUIRED)
find_package(Threads REQUIRED)

# Mutexes and Atoomics
simple_tbb_exe(parallel-cout)
simple_tbb_exe(parallel-cout-mutex)
simple_tbb_exe(multithread-tbb-sum)
simple_tbb_exe(multithread-tbb-sum-mutex)
simple_tbb_exe(multithread-tbb-sum-atomic)
simple_tbb_exe(multithread-tbb-sum-mutex-efficient)
simple_tbb_exe(multithread-tbb-sum-atomic-efficient)
simple_tbb_exe(deadlock-tbb)
simple_tbb_exe(deadlock-tbb-fixed)

## Now add tests
add_test(parallel-cout parallel-cout)
add_test(parallel-cout-mutex parallel-cout-mutex)
add_test(multithread-tbb-sum multithread-tbb-sum)
add_test(multithread-tbb-sum-mutex multithread-tbb-sum-mutex)
add_test(multithread-tbb-sum-atomic multithread-tbb-sum-atomic)
add_test(multithread-tbb-sum-mutex-efficient multithread-tbb-sum-mutex-efficient)
add_test(multithread-tbb-sum-atomic-efficient multithread-tbb-sum-atomic-efficient)
add_test(deadlock-tbb-fixed deadlock-tbb-fixed)
