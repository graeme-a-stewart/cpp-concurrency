cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-TBBtasks)

# Setup our necessary module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../../cmake/modules;${CMAKE_SOURCE_DIR}/../../../cmake/FindTBB;${CMAKE_MODULE_PATH}")

include(cpp-concurrency)

find_package(Boost)
find_package(TBB)
find_package(Threads)

simple_tbb_exe(fib-example)
simple_tbb_exe(fib-group)
simple_tbb_exe(maze)
simple_tbb_exe(cancel-sleep)
