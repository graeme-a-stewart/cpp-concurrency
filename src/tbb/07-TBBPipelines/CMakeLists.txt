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

# Pipelines
simple_tbb_exe(gen-input)
simple_tbb_exe(maths-pipeline)
