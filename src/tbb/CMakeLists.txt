cmake_minimum_required (VERSION 2.8)

project(CppConcurrency-TBB)

# Enable c++11. This is not very nice - more up to date CMake would use 
# target_compile_features, but we need to ensure this works with the old
# CMake supplied by CentOS7, 2.8.11.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(${PROJECT_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "This project cannot build in the source directory. "
  "Use an out of source build area: \"cmake the/path/to/cpp-concurrency/src/cpp11\"")
endif()

# Find the system's threading libraries
find_package(Threads REQUIRED)

# Define the function used to build a simple TBB executable,
# adding the correct thread and TBB libraries 
function(simple_tbb_exe TARGET)
	add_executable(${TARGET} "${TARGET}.cc")

	# Would be much better to use an up to date CMake with a proper find_tbb 
    # module, but this does the job for now...
    # (Also add maths library)
	target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT} tbb m)

endfunction(simple_tbb_exe)

# Add a CMake target for each of our examples
simple_tbb_exe(concurrent_vector)
simple_tbb_exe(concurrent_vector_grow_by)
simple_tbb_exe(concurrent_vector_stdthread)
simple_tbb_exe(gen_input)
simple_tbb_exe(generate_and_filter)
simple_tbb_exe(maths_pipeline)
simple_tbb_exe(number_of_threads)
simple_tbb_exe(parallel_for_basic)
simple_tbb_exe(parallel_for_lambda)
simple_tbb_exe(parallel_mandel)
simple_tbb_exe(parallel_mandel_vector)
simple_tbb_exe(parallel_reduce)
simple_tbb_exe(parallel_reduce_pi_estimation)
simple_tbb_exe(version)

