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

# Find the system's threading libraries
find_package(Threads REQUIRED)

## Add each executable we build
# If you add a new source file and executable target, then add it to this list

# Simple thread invocations
simple_thread_exe(start-threads)
simple_thread_exe(start-threads-vec)
simple_thread_exe(thread-arguments)
simple_thread_exe(thread-arguments-mutex)
simple_thread_exe(thread-arguments-problems-fixed)
simple_thread_exe(thread-arguments-problems)
simple_thread_exe(thread-arguments-problems2-fixed)
simple_thread_exe(thread-arguments-problems2)
simple_thread_exe(hw-concurrency)

# Deadlocks
simple_thread_exe(deadlock)
simple_thread_exe(deadlock-fixed)
simple_thread_exe(deadlock-fixed-using-lock-adoption)

# Summing across threads, mutexes and atomics
simple_thread_exe(multithread-sum)
simple_thread_exe(multithread-sum-fixed-atomic)
simple_thread_exe(multithread-sum-fixed-atomic-efficient)
simple_thread_exe(multithread-sum-fixed-mutex)
simple_thread_exe(multithread-sum-fixed-mutex-efficient)
simple_thread_exe(atomic-lock-free)

# Calorimiter parallel example
simple_thread_exe(multithread-calo)
simple_thread_exe(multithread-calo-fixed)
simple_thread_exe(multithread-calo-atomic-broken)

# Async and packaged tasks
simple_thread_exe(async-tasks-multi)
simple_thread_exe(async-tasks)
simple_thread_exe(packaged-task)


## Tests
add_test(hw-concurrency hw-concurrency)
add_test(start-threads start-threads)
add_test(start-threads-vec start-threads-vec)
add_test(thread-arguments thread-arguments)
add_test(thread-arguments-mutex thread-arguments-mutex)
add_test(thread-arguments-problems-fixed thread-arguments-problems-fixed)
add_test(thread-arguments-problems2-fixed thread-arguments-problems2-fixed)

add_test(deadlock-fixed deadlock-fixed)
add_test(deadlock-fixed-using-lock-adoption deadlock-fixed-using-lock-adoption)

add_test(multithread-sum-fixed-atomic multithread-sum-fixed-atomic)
add_test(multithread-sum-fixed-atomic-efficient multithread-sum-fixed-atomic-efficient)
add_test(multithread-sum-fixed-mutex multithread-sum-fixed-mutex)
add_test(multithread-sum-fixed-mutex-efficient multithread-sum-fixed-mutex-efficient)

add_test(multithread-calo-fixed multithread-calo-fixed)

add_test(async-tasks async-tasks)
add_test(async-tasks-multi async-tasks-multi)
add_test(packaged-task packaged-task)
