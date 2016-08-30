# Helper functions for CPP Concurrency CMake setup

# Define the function used to build a simple C++11 threaded executable,
# adding the correct thread library
function(simple_thread_exe TARGET)
	set(CXX_STANDARD_REQUIRED ON)
	add_executable(${TARGET} "${TARGET}.cc")
	target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT})
	set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 11)
endfunction(simple_thread_exe)

# Define the function used to build a simple TBB executable,
# adding the correct thread and TBB libraries 
function(simple_tbb_exe TARGET)
	set(CXX_STANDARD_REQUIRED ON)
	add_executable(${TARGET} "${TARGET}.cc")
	target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT} tbb)
	set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 11)
endfunction(simple_tbb_exe)
