cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-C++11-01)

if(${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. "
  "Use an out of source build area: \"cmake the/path/to/cpp-concurrency\"")
endif()

# Add our CMake module paths and find the modules we need
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../../cmake/modules;${CMAKE_MODULE_PATH}")
include(cpp-concurrency)

enable_testing()

# Find the system's threading libraries
find_package(Threads REQUIRED)

## Add each executable we build
# If you add a new source file and executable target, then add it to this list

# Simple thread invocations
simple_thread_exe(start-threads)
simple_thread_exe(start-threads-vec)
simple_thread_exe(thread-arguments)
simple_thread_exe(thread-arguments-problems-fixed)
simple_thread_exe(thread-arguments-problems)
simple_thread_exe(thread-arguments-problems2-fixed)
simple_thread_exe(thread-arguments-problems2)
simple_thread_exe(hw-concurrency)


## Tests
add_test(hw-concurrency hw-concurrency)
add_test(start-threads start-threads)
add_test(start-threads-vec start-threads-vec)
add_test(thread-arguments thread-arguments)
add_test(thread-arguments-problems-fixed thread-arguments-problems-fixed)
add_test(thread-arguments-problems2-fixed thread-arguments-problems2-fixed)
