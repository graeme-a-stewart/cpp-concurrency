cmake_minimum_required (VERSION 2.8)

project(CppConcurrency-C++11)

# Enable c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. "
  "Use an out of source build area: \"cmake the/path/to/cpp-concurrency/src/cpp11\"")
endif()

# Add each executable we build
# If you add a new source file and executable target, then add it to this list
add_executable(async-tasks-multi async-tasks-multi.cc)
add_executable(async-tasks async-tasks.cc)
add_executable(atomic-lock-free atomic-lock-free.cc)
add_executable(deadlock-fixed-using-lock-adoption deadlock-fixed-using-lock-adoption.cc)
add_executable(deadlock-fixed deadlock-fixed.cc)
add_executable(deadlock deadlock.cc)
add_executable(hw-concurrency hw-concurrency.cc)
add_executable(multithread-calo-atomic-broken multithread-calo-atomic-broken.cc)
add_executable(multithread-calo-fixed multithread-calo-fixed.cc)
add_executable(multithread-calo multithread-calo.cc)
add_executable(multithread-sum-fixed-atomic-efficient multithread-sum-fixed-atomic-efficient.cc)
add_executable(multithread-sum-fixed-atomic multithread-sum-fixed-atomic.cc)
add_executable(multithread-sum-fixed-mutex-efficient multithread-sum-fixed-mutex-efficient.cc)
add_executable(multithread-sum-fixed-mutex multithread-sum-fixed-mutex.cc)
add_executable(multithread-sum multithread-sum.cc)
add_executable(start_threads start_threads.cc)
add_executable(start_threads_vec start_threads_vec.cc)
add_executable(thread-arguments-mutex thread-arguments-mutex.cc)
add_executable(thread-arguments-problems-fixed thread-arguments-problems-fixed.cc)
add_executable(thread-arguments-problems thread-arguments-problems.cc)
add_executable(thread-arguments-problems2-fixed thread-arguments-problems2-fixed.cc)
add_executable(thread-arguments-problems2 thread-arguments-problems2.cc)
add_executable(thread-arguments thread-arguments.cc)


