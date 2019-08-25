// Test program showing how to get TBB to write to 
// cout from many threads

#include <iostream>
#include "tbb/tbb.h"

int main() {
    const int size = 100;

    // This code is a good way to parallelise some simple workload across
    // TBB's worker thread pool (contrast with startinig a bunch of C++11
    // threads as shown earlier in the tutorial).
    tbb::parallel_for(0, size, 1, [=](int i){std::cout << "Hello from iteration " << i << std::endl;});

    return 0;
}