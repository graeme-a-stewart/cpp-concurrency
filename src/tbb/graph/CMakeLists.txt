cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-TBBgraph)

# Setup our necessary module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../../cmake/modules;${CMAKE_SOURCE_DIR}/../../../cmake/FindTBB;${CMAKE_MODULE_PATH}")

include(cpp-concurrency)

find_package(Boost)
find_package(TBB)
find_package(Threads)

# Simple TBB stand alone graph examples
simple_tbb_exe(data-flow)
simple_tbb_exe(data-flow-basic)
simple_tbb_exe(data-flow-ptr)
simple_tbb_exe(data-flow-sharedptr)

simple_tbb_exe(single-node)

simple_tbb_exe(multi-node)
simple_tbb_exe(multi-node-reject)

simple_tbb_exe(dep-graph)
simple_tbb_exe(dep-graph-broadcast)
simple_tbb_exe(file-process)

# Utility program to generate simulated fooble data
add_executable(det-rand-dump det-rand-dump.cc)
set_property(TARGET det-rand-dump PROPERTY CXX_STANDARD 11)

# Fooble detector code
simple_tbb_exe(det-data-proc)
