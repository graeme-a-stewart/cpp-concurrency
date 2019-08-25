cmake_minimum_required (VERSION 3.6)

project(CppConcurrency)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules;${CMAKE_SOURCE_DIR}/cmake/FindTBB;${CMAKE_MODULE_PATH}")

enable_testing()

add_subdirectory("src/cpp11/01-BasicThreadManagement")
add_subdirectory("src/cpp11/02-MutexAndAtomics")
add_subdirectory("src/cpp11/03-AsyncFuture")
add_subdirectory("src/tbb/04-TBBLoops")
add_subdirectory("src/tbb/05-TBBMutexAndAtomics")
add_subdirectory("src/tbb/06-TBBContainers")
add_subdirectory("src/tbb/07-TBBPipelines")
add_subdirectory("src/tbb/08-TBBGraphs")
add_subdirectory("src/tbb/09-TBBTasks")

add_subdirectory("src/tbb/XY-TBBGraphExercise-Solution")

# Note that the docs subdirectory is not added to the master CMakeFile as it
# also requires pandoc to operate correctly, which may not be available in
# all environments. Simply set this up as a separate project.
