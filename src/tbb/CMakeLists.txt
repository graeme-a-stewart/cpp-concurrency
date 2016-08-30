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

# Add a CMake target for each of our examples
simple_tbb_exe(concurrent_vector)
simple_tbb_exe(concurrent_vector_grow_by)
simple_tbb_exe(concurrent_vector_stdthread)
simple_tbb_exe(gen_input)
simple_tbb_exe(generate_and_filter)
simple_tbb_exe(maths_pipeline)
simple_tbb_exe(number_of_threads)
simple_tbb_exe(parallel_for_basic)
simple_tbb_exe(parallel_for_lambda)
simple_tbb_exe(parallel_mandel)
simple_tbb_exe(parallel_mandel_vector)
simple_tbb_exe(parallel_reduce)
simple_tbb_exe(parallel_reduce_pi_estimation)
simple_tbb_exe(version)

