cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-C++11)

if(${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. "
  "Use an out of source build area: \"cmake the/path/to/cpp-concurrency/src/cpp11\"")
endif()

# Add our CMake module paths and find the modules we need
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules;${CMAKE_MODULE_PATH}")
include(cpp-concurrency)

enable_testing()

add_subdirectory("01-BasicThreadManagement")
add_subdirectory("02-MutexAndAtomics")
add_subdirectory("03-AsyncFuture")
