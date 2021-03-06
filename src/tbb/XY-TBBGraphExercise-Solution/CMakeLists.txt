cmake_minimum_required (VERSION 3.6)

project(CppConcurrency-TBBtasks-GraphExerciseSolution)

# Setup our necessary module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../../cmake/modules;${CMAKE_SOURCE_DIR}/../../../cmake/FindTBB;${CMAKE_MODULE_PATH}")

include(cpp-concurrency)

find_package(Boost)
find_package(TBB)
find_package(Threads)

# Define a function that wraps the setting of the correct libraries
# and build options for this exercise
function(tbb_graph_exe TARGET)
	set(CXX_STANDARD_REQUIRED ON)
	add_executable(${TARGET} "${TARGET}.cc")
	target_link_libraries(${TARGET} fdet ${CMAKE_THREAD_LIBS_INIT} tbb)
	set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 17)
endfunction(tbb_graph_exe)

## Build the detector description library
add_library(fdet fdet.cc)
target_link_libraries(fdet ${CMAKE_THREAD_LIBS_INIT} tbb)
set_property(TARGET fdet PROPERTY CXX_STANDARD 17)

# Test detector classes
tbb_graph_exe(fdet-test)

# Data generator
tbb_graph_exe(fdet-write)

# Data reader
tbb_graph_exe(fdet-read)

# Solution
tbb_graph_exe(solution)
