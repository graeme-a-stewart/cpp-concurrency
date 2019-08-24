cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-C++11-03)

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

# Async and packaged tasks
simple_thread_exe(async-tasks-multi)
simple_thread_exe(async-tasks)
simple_thread_exe(packaged-task)


## Tests
add_test(async-tasks async-tasks)
add_test(async-tasks-multi async-tasks-multi)
add_test(packaged-task packaged-task)
