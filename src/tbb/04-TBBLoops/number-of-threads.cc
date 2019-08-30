/*
illustration of control of number of threads and object-based TBB tasks (as
opposed to function-based tasks)
*/

#include <vector>
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>

#include "tbb/tbb.h"
#include "tutorialutils.h"

std::mutex mtx;

struct mytask {

    mytask(size_t n): _n(n){
    }

    double operator()(){
        return burn(5000000);
    }

    size_t _n;

};

int main(int argn, char* argv[]){
    int threads = tbb::task_scheduler_init::default_num_threads();
    if (argn==2) {
        threads = std::atoi(argv[1]);
    }

    tbb::task_scheduler_init init(threads);
    std::cout << "Initialised TBB with " << threads << " threads" << std::endl;

    std::vector<mytask> tasks;
    for (int i=0; i < 1000;++i){
        tasks.push_back(mytask(i));
    }

    tbb::tick_count t0 = tbb::tick_count::now();

    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, tasks.size()),
        [&tasks](const tbb::blocked_range<size_t>& r){
            double total=0.0;
            for (size_t i = r.begin(); i < r.end(); ++i) {
                total += tasks[i]();
            }
        }
    );
    tbb::tick_count t1 = tbb::tick_count::now();
    auto tick_interval = t1-t0;
    std::cout << "Execution took "
      << tick_interval.seconds()
      << "s"
      << std::endl;


    std::cerr << std::endl;
    return 0;

}
