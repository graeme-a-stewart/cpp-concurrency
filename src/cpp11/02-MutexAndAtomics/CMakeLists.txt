cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-C++11-02)

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

# Deadlocks
simple_thread_exe(deadlock)
simple_thread_exe(deadlock-fixed)
simple_thread_exe(deadlock-fixed-using-lock-adoption)

# Summing across threads, mutexes and atomics
simple_thread_exe(thread-arguments-mutex)
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


## Tests
add_test(thread-arguments-mutex thread-arguments-mutex)
add_test(deadlock-fixed deadlock-fixed)
add_test(deadlock-fixed-using-lock-adoption deadlock-fixed-using-lock-adoption)

add_test(multithread-sum-fixed-atomic multithread-sum-fixed-atomic)
add_test(multithread-sum-fixed-atomic-efficient multithread-sum-fixed-atomic-efficient)
add_test(multithread-sum-fixed-mutex multithread-sum-fixed-mutex)
add_test(multithread-sum-fixed-mutex-efficient multithread-sum-fixed-mutex-efficient)

add_test(multithread-calo-fixed multithread-calo-fixed)
