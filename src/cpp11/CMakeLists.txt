cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-C++11)

if(${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. "
  "Use an out of source build area: \"cmake the/path/to/cpp-concurrency/src/cpp11\"")
endif()

# Add our CMake module paths and find the modules we need
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules;${CMAKE_MODULE_PATH}")
include(cpp-concurrency)

# Find the system's threading libraries
find_package(Threads REQUIRED)

# Add each executable we build
# If you add a new source file and executable target, then add it to this list
simple_thread_exe(async-tasks-multi)
simple_thread_exe(async-tasks)
simple_thread_exe(packaged_task)
simple_thread_exe(atomic-lock-free)
simple_thread_exe(deadlock-fixed-using-lock-adoption)
simple_thread_exe(deadlock-fixed)
simple_thread_exe(deadlock)
simple_thread_exe(hw-concurrency)
simple_thread_exe(multithread-calo-atomic-broken)
simple_thread_exe(multithread-calo-fixed)
simple_thread_exe(multithread-calo)
simple_thread_exe(multithread-sum-fixed-atomic-efficient)
simple_thread_exe(multithread-sum-fixed-atomic)
simple_thread_exe(multithread-sum-fixed-mutex-efficient)
simple_thread_exe(multithread-sum-fixed-mutex)
simple_thread_exe(multithread-sum)
simple_thread_exe(start_threads)
simple_thread_exe(start_threads_vec)
simple_thread_exe(thread-arguments-mutex)
simple_thread_exe(thread-arguments-problems-fixed)
simple_thread_exe(thread-arguments-problems)
simple_thread_exe(thread-arguments-problems2-fixed)
simple_thread_exe(thread-arguments-problems2)
simple_thread_exe(thread-arguments)
