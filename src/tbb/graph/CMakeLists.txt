cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-TBBgraph)

# Setup our necessary module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../../cmake/modules;${CMAKE_SOURCE_DIR}/../../../cmake/FindTBB;${CMAKE_MODULE_PATH}")

include(cpp-concurrency)

find_package(Boost)
find_package(TBB)
find_package(Threads)

# Simple TBB stand alone graph examples
simple_tbb_exe(data_flow)
simple_tbb_exe(data_flow_basic)
simple_tbb_exe(data_flow_ptr)
simple_tbb_exe(data_flow_sharedptr)

simple_tbb_exe(single_node)

simple_tbb_exe(multi_node)
simple_tbb_exe(multi_node_reject)

simple_tbb_exe(dep_graph)
simple_tbb_exe(file_process)

# Utility program to generate simulated fooble data
add_executable(det_rand_dump det_rand_dump.cc)
set_property(TARGET det_rand_dump PROPERTY CXX_STANDARD 11)

# Fooble detector code
simple_tbb_exe(det_data_proc)
