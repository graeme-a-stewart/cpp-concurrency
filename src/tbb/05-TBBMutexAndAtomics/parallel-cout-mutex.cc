// Test program showing how to get TBB to write to 
// cout from many threads

#include <iostream>
#include <mutex>

#include "tbb/tbb.h"

int main() {
    const int size = 100;
    std::mutex mtx;

    // Note that the mutex is captured by reference
    tbb::parallel_for(0, size, 1, [=, &mtx](int i){
        std::lock_guard<std::mutex> lck(mtx);
        std::cout << "Hello from iteration " << i << std::endl;
        });

    return 0;
}
