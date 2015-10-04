cmake_minimum_required (VERSION 2.8)

project(CppConcurrency-C++11)

# Enable c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(${PROJECT_BINARY_DIR} EQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. Use \"cmake some/path/to/src/cpp11\"")
endif()

## This method automatically adds a binary for each source file in the source directory...
file(GLOB simple_sources RELATIVE ${CMAKE_SOURCE_DIR} *.cc)

foreach(source ${simple_sources})
  string(REPLACE ".cc" "" binary ${source})
  message(STATUS "Will compile ${source} to ${binary}")
  add_executable(${binary} ${source})
endforeach()

## ...alternatively, add executables by hand for each source file
#add_executable(start_threads start_threads.cc) 
#add_executable(start_threads_vec start_threads_vec.cc) 
