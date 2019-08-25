cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-TBBtasks-GraphExerciseSolution)

# Setup our necessary module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../../cmake/modules;${CMAKE_SOURCE_DIR}/../../../cmake/FindTBB;${CMAKE_MODULE_PATH}")

include(cpp-concurrency)

find_package(Boost)
find_package(TBB)
find_package(Threads)

## Build the detector description library
add_library(fdet fdet.cc)
target_link_libraries(fdet ${CMAKE_THREAD_LIBS_INIT} tbb)
set_property(TARGET fdet PROPERTY CXX_STANDARD 17)

# Test detector classes
add_executable(fdet-test fdet-test.cc)
target_link_libraries(fdet-test fdet ${CMAKE_THREAD_LIBS_INIT} tbb)
set_property(TARGET fdet-test PROPERTY CXX_STANDARD 17)

# Data generator
add_executable(fdet-write fdet-write.cc)
target_link_libraries(fdet-write fdet ${CMAKE_THREAD_LIBS_INIT} tbb)
set_property(TARGET fdet-write PROPERTY CXX_STANDARD 17)
