cmake_minimum_required (VERSION 3.6)

project(CppConcurrency)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules;${CMAKE_SOURCE_DIR}/cmake/FindTBB;${CMAKE_MODULE_PATH}")

message(INFO "Module path: ${CMAKE_MODULE_PATH}")

add_subdirectory("src/cpp11")
add_subdirectory("src/tbb")
add_subdirectory("src/tbb/graph")
add_subdirectory("src/tbb/tasks")
